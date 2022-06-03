#include "xpertqueryimport.h"

#include "xpertadministrativedata.h"

using namespace std;

namespace Tucuxi {
namespace XpertQuery {

XpertQueryImport::XpertQueryImport() = default;


XpertQueryImport::~XpertQueryImport() = default;


Common::IImport::Status XpertQueryImport::importFromFile(unique_ptr<XpertQueryData>& _query, const string &_fileName)
{
    // Ensure the function is reentrant
    lock_guard<mutex> lock(m_mutex);

    setStatus(Status::Ok);
    _query = nullptr;

    Tucuxi::Common::XmlDocument document;
    if (!document.open(_fileName)) {
        setStatus(Status::CantCreateXmlDocument, "file could not be opened");
        return Status::CantOpenFile;
    }

    return importDocument(_query, document);
}

Common::IImport::Status XpertQueryImport::importFromString(unique_ptr<XpertQueryData> &_query, const string &_xml)
{
    // Ensure the function is reentrant
    lock_guard<mutex> lock(m_mutex);

    setStatus(Status::Ok);
    _query = nullptr;

    Tucuxi::Common::XmlDocument document;

    if (!document.fromString(_xml)) {
        setStatus(Status::CantCreateXmlDocument, "xml document could not be created. The tags must be controlled");
        return Status::CantCreateXmlDocument;
    }

    return importDocument(_query, document);
}

Common::IImport::Status XpertQueryImport::importDocument(unique_ptr<XpertQueryData>& _query,Common::XmlDocument& _document)
{
    static const string QUERY_ID_NODE_NAME = "queryId";
    static const string CLIENT_ID_NODE_NAME = "clientId";
    static const string DATE_NODE_NAME = "date";
    static const string LANGUAGE_NODE_NAME = "language";
    static const string REQUESTS_NODE_NAME = "requests";
    static const string REQUEST_XPERT_REQUESTS_NODE_NAME = "requestXpert";

    Common::XmlNode root = _document.getRoot();

    string queryId = root.getChildren(QUERY_ID_NODE_NAME)->getValue();
    string clientId = root.getChildren(CLIENT_ID_NODE_NAME)->getValue();

    Common::XmlNodeIterator dateIterator = root.getChildren(DATE_NODE_NAME);
    Common::DateTime date;
    if (!checkNodeIterator(dateIterator, DATE_NODE_NAME).empty()) {
        date = extractDateTime(dateIterator);
    }

    string language = root.getChildren(LANGUAGE_NODE_NAME)->getValue();

    unique_ptr<AdministrativeData> pAdministrativeData = createAdministrativeData(_document);

    unique_ptr<Query::DrugTreatmentData> pParametersData = createDrugTreatmentData(_document);

    Common::XmlNodeIterator requestsRootIterator = root.getChildren(REQUESTS_NODE_NAME);
    checkNodeIterator(requestsRootIterator, REQUESTS_NODE_NAME);

    vector<unique_ptr<Query::RequestData> > requests;


    requestsRootIterator = root.getChildren(REQUESTS_NODE_NAME);
    Common::XmlNodeIterator requestsXpertIterator = requestsRootIterator->getChildren(REQUEST_XPERT_REQUESTS_NODE_NAME);
    checkNodeIterator(requestsXpertIterator, REQUEST_XPERT_REQUESTS_NODE_NAME);

    vector<unique_ptr<XpertRequestData> > requestsXpert;
    while (requestsXpertIterator != Common::XmlNodeIterator::none()) {
        requestsXpert.emplace_back(createRequestXpert(requestsXpertIterator));
        requestsXpertIterator++;
    }

    if (requestsXpert.size() == 0) {
        setStatus(Status::Error, "No requestXpert found");
    }

    _query = make_unique<XpertQueryData>(queryId, clientId, date, language,  move(pAdministrativeData), move(pParametersData), requests, requestsXpert);

    return getStatus();
}

unique_ptr<AdministrativeData> XpertQueryImport::createAdministrativeData(Common::XmlDocument& _document)
{
    static const string ADMIN_NODE_NAME = "admin";
    static const string MANDATOR_NODE_NAME = "mandator";
    static const string PATIENT_NODE_NAME = "patient";
    static const string CLINICALDATA_NODE_NAME = "clinicalData";

    Common::XmlNode root = _document.getRoot();
    Common::XmlNodeIterator adminRootIterator = root.getChildren(ADMIN_NODE_NAME);

    // If the node is not present.
    if (!adminRootIterator->isValid()) {
        return nullptr;
    }

    Common::XmlNodeIterator mandatorRootIterator = adminRootIterator->getChildren(MANDATOR_NODE_NAME);
    Common::XmlNodeIterator patientRootIterator  = adminRootIterator->getChildren(PATIENT_NODE_NAME);
    Common::XmlNodeIterator clinicalDataRootIterator  = adminRootIterator->getChildren(CLINICALDATA_NODE_NAME);

    unique_ptr<Person> pMandator = createPerson(mandatorRootIterator);
    unique_ptr<Person> pPatient = createPerson(patientRootIterator);
    unique_ptr<ClinicalData> pClinicalData = createClinicalData(clinicalDataRootIterator);

    return make_unique<AdministrativeData>(
                move(pMandator),
                move(pPatient),
                move(pClinicalData)
                );
}

unique_ptr<Person> XpertQueryImport::createPerson(Common::XmlNodeIterator& _personRootIterator)
{
    if (!_personRootIterator->isValid()) {
        return nullptr;
    }

    static const string PERSON_NODE_NAME = "person";
    static const string INSTITUTE_NODE_NAME = "institute";

    Common::XmlNodeIterator iterator = _personRootIterator->getChildren(PERSON_NODE_NAME);
    unique_ptr<PersonalContact> pPersonalContact = createPersonalContact(iterator);
    iterator = _personRootIterator->getChildren(INSTITUTE_NODE_NAME);
    unique_ptr<InstituteContact> pInstituteContact = createInstituteContact(iterator);

    return make_unique<Person>(
                move(pPersonalContact),
                move(pInstituteContact)
                );
}

unique_ptr<PersonalContact> XpertQueryImport::createPersonalContact(Common::XmlNodeIterator& _personalContactRootIterator)
{
    if (!_personalContactRootIterator->isValid()) {
        return nullptr;
    }

    static const string ID_NODE_NAME = "id";
    static const string TITLE_NODE_NAME = "title";
    static const string FIRSTNAME_NODE_NAME = "firstName";
    static const string LASTNAME_NODE_NAME = "lastName";
    static const string ADDRESS_NODE_NAME = "address";
    static const string PHONE_NODE_NAME = "phone";
    static const string EMAIL_NODE_NAME = "email";

    string id = getChildStringOptional(_personalContactRootIterator, ID_NODE_NAME, "");
    string title = getChildStringOptional(_personalContactRootIterator, TITLE_NODE_NAME, "");
    string firstname = getChildString(_personalContactRootIterator, FIRSTNAME_NODE_NAME);
    string lastname = getChildString(_personalContactRootIterator, LASTNAME_NODE_NAME);
    Common::XmlNodeIterator iterator = _personalContactRootIterator->getChildren(ADDRESS_NODE_NAME);
    unique_ptr<Address> pAddress = createAddress(iterator);
    iterator = _personalContactRootIterator->getChildren(PHONE_NODE_NAME);
    unique_ptr<Phone> pPhone = createPhone(iterator);
    iterator = _personalContactRootIterator->getChildren(EMAIL_NODE_NAME);
    unique_ptr<Email> pEmail = createEmail(iterator);

    return make_unique<PersonalContact>(
                id,
                title,
                firstname,
                lastname,
                move(pAddress),
                move(pPhone),
                move(pEmail)
                );
}

unique_ptr<InstituteContact> XpertQueryImport::createInstituteContact(Common::XmlNodeIterator& _instituteContactRootIterator)
{
    if (!_instituteContactRootIterator->isValid()) {
        return nullptr;
    }

    static const string ID_NODE_NAME = "id";
    static const string NAME_NODE_NAME = "name";
    static const string ADDRESS_NODE_NAME = "address";
    static const string PHONE_NODE_NAME = "phone";
    static const string EMAIL_NODE_NAME = "email";

    string id = getChildStringOptional(_instituteContactRootIterator, ID_NODE_NAME, "");
    string name = getChildString(_instituteContactRootIterator, NAME_NODE_NAME);
    Common::XmlNodeIterator iterator = _instituteContactRootIterator->getChildren(ADDRESS_NODE_NAME);
    unique_ptr<Address> pAddress = createAddress(iterator);
    iterator = _instituteContactRootIterator->getChildren(PHONE_NODE_NAME);
    unique_ptr<Phone> pPhone = createPhone(iterator);
    iterator = _instituteContactRootIterator->getChildren(EMAIL_NODE_NAME);
    unique_ptr<Email> pEmail = createEmail(iterator);

    return make_unique<InstituteContact>(
                id,
                name,
                move(pAddress),
                move(pPhone),
                move(pEmail)
                );
}

unique_ptr<Address> XpertQueryImport::createAddress(Common::XmlNodeIterator& _addressRootIterator)
{
    if (!_addressRootIterator->isValid()) {
        return nullptr;
    }

    static const string STREET_NODE_NAME = "street";
    static const string POSTALCODE_NODE_NAME = "postCode";
    static const string CITY_NODE_NAME = "city";
    static const string STATE_NODE_NAME = "state";
    static const string COUNTRY_NODE_NAME = "country";

    string street = getChildString(_addressRootIterator, STREET_NODE_NAME);
    int postalCode = getChildInt(_addressRootIterator, POSTALCODE_NODE_NAME);
    string city = getChildString(_addressRootIterator, CITY_NODE_NAME);
    string state = getChildStringOptional(_addressRootIterator, STATE_NODE_NAME, "");
    string country = getChildStringOptional(_addressRootIterator, COUNTRY_NODE_NAME, "");

    return make_unique<Address>(
                street,
                postalCode,
                city,
                state,
                country
                );
}

unique_ptr<Phone> XpertQueryImport::createPhone(Common::XmlNodeIterator& _phoneRootIterator)
{
    if (!_phoneRootIterator->isValid()) {
        return nullptr;
    }

    static const string NUMBER_NODE_NAME = "number";
    static const string TYPE_NODE_NAME = "type";

    string number = getChildString(_phoneRootIterator, NUMBER_NODE_NAME);
    string type = getChildStringOptional(_phoneRootIterator, TYPE_NODE_NAME, "");

    return make_unique<Phone>(number, type);
}
unique_ptr<Email> XpertQueryImport::createEmail(Common::XmlNodeIterator& _emailRootIterator)
{
    if (!_emailRootIterator->isValid()) {
        return nullptr;
    }

    static const string ADDRESS_NODE_NAME = "address";
    static const string TYPE_NODE_NAME = "type";

    string address = getChildString(_emailRootIterator, ADDRESS_NODE_NAME);
    string type = getChildStringOptional(_emailRootIterator, TYPE_NODE_NAME, "");

    return make_unique<Email>(address, type);
}

unique_ptr<ClinicalData> XpertQueryImport::createClinicalData(Common::XmlNodeIterator& _clinicalDataRootIterator)
{
    if (!_clinicalDataRootIterator->isValid()) {
        return nullptr;
    }

    static const string CLINICAL_DATA_ENTRY_NODE_NAME = "clinicalDataEntry";

    map<string, string> data;

    Common::XmlNodeIterator xmlNodeIterator = _clinicalDataRootIterator->getChildren(CLINICAL_DATA_ENTRY_NODE_NAME);
    while(xmlNodeIterator != xmlNodeIterator.none()) {
        data[xmlNodeIterator->getAttribute("key").getValue()] = xmlNodeIterator->getValue();
        xmlNodeIterator++;
    }

    return make_unique<ClinicalData>(data);
}

unique_ptr<XpertRequestData> XpertQueryImport::createRequestXpert(Common::XmlNodeIterator& _requestXpertRootIterator)
{
    static const string DRUG_ID_NODE_NAME = "drugId";
    static const string OUTPUT_NODE_NAME = "output";
    static const string DATE_ADJUSTMENT_TIME_NODE_NAME = "adjustmentDate";
    static const string OPTIONS_NODE_NAME = "options";

    string drugId = getChildString(_requestXpertRootIterator, DRUG_ID_NODE_NAME);

    Common::XmlNodeIterator outputRootIterator = _requestXpertRootIterator->getChildren(OUTPUT_NODE_NAME);

    static const string FORMAT_NODE_NAME = "format";
    static const string LANGUAGE_NODE_NAME = "language";

    string formatStr = getChildString(outputRootIterator, FORMAT_NODE_NAME);
    OutputFormat format;

    if (formatStr == "xml") {
        format = OutputFormat::XML;
    } else if (formatStr == "html") {
        format = OutputFormat::HTML;
    } else if (formatStr == "pdf") {
        format = OutputFormat::PDF;
    } else {
        setStatus(Status::Error, "Unknown output format");
    }

    string languageStr = getChildString(outputRootIterator, LANGUAGE_NODE_NAME);

    OutputLang language;

    if (languageStr == "en") {
        language = OutputLang::ENGLISH;
    } else if (languageStr == "fr") {
        language = OutputLang::FRENCH;
    } else {
        setStatus(Status::Error, "Unknown output language");
    }

    Common::DateTime adjustmentTime =
            getChildDateTime(_requestXpertRootIterator, DATE_ADJUSTMENT_TIME_NODE_NAME);

    LoadingOption loadingOption =
            getChildLoadingOptionEnumOptional(_requestXpertRootIterator, OPTIONS_NODE_NAME, LoadingOption::Unspecified);

    RestPeriodOption restPeriodOption =
            getChildRestPeriodTargetOptionEnumOptional(_requestXpertRootIterator, OPTIONS_NODE_NAME, RestPeriodOption::Unspecified);

    Tucuxi::Core::TargetExtractionOption targetExtractionOption =
            getChildTargetExtractionOptionEnumOptional(_requestXpertRootIterator, OPTIONS_NODE_NAME, Core::TargetExtractionOption::DefinitionIfNoIndividualTarget);

    Tucuxi::Core::FormulationAndRouteSelectionOption formulationAndRouteSelectionOption =
            getChildFormulationAndRouteSelectionOptionEnumOptional(_requestXpertRootIterator, OPTIONS_NODE_NAME, Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute);


    return make_unique<XpertRequestData>(drugId,
                                         format,
                                         language,
                                         adjustmentTime,
                                         loadingOption,
                                         restPeriodOption,
                                         targetExtractionOption,
                                         formulationAndRouteSelectionOption);
}

string XpertQueryImport::getChildStringOptional(
        Common::XmlNodeIterator _rootIterator,
        const string& _childName,
        const string& _default)
{
    auto child = _rootIterator->getChildren(_childName);

    if (child == Common::XmlNodeIterator::none()) {
        return _default;
    }
    return extractString(child);
}

LoadingOption XpertQueryImport::getChildLoadingOptionEnumOptional(
        Common::XmlNodeIterator _rootIterator, const string& _childName, LoadingOption _default)
{
    static const string LOADING_OPTION_NODE = "loadingOption";

    Common::XmlNodeIterator optionsRootIterator = _rootIterator->getChildren(_childName);

    Common::XmlNodeIterator loadingOptionRootIterator = optionsRootIterator->getChildren(LOADING_OPTION_NODE);


    static map<string, LoadingOption> m = {
            {"noLoadingDose", LoadingOption::NoLoadingDose},
            {"loadingDoseAllowed", LoadingOption::LoadingDoseAllowed}};

    string value = loadingOptionRootIterator->getValue();
    auto it = m.find(value);
    if (it != m.end()) {
        return it->second;
    }

    return _default;
}

RestPeriodOption XpertQueryImport::getChildRestPeriodTargetOptionEnumOptional(
        Common::XmlNodeIterator _rootIterator, const string& _childName, RestPeriodOption _default)
{

    static const string REST_PERIOD_OPTION_NODE = "restPeriodOption";

    Common::XmlNodeIterator optionsRootIterator = _rootIterator->getChildren(_childName);

    Common::XmlNodeIterator restPeriodOptionRootIterator = optionsRootIterator->getChildren(REST_PERIOD_OPTION_NODE);


    static map<string, RestPeriodOption> m = {
            {"noRestPeriod", RestPeriodOption::NoRestPeriod},
            {"restPeriodAllowed", RestPeriodOption::RestPeriodAllowed}};

    string value = restPeriodOptionRootIterator->getValue();
    auto it = m.find(value);
    if (it != m.end()) {
        return it->second;
    }

    return _default;
}

Core::TargetExtractionOption XpertQueryImport::getChildTargetExtractionOptionEnumOptional(
        Common::XmlNodeIterator _rootIterator, const string& _childName, Core::TargetExtractionOption _default)
{

    static const string TARGET_EXTRACTION_OPTION_NODE = "targetExtractionOption";

    Common::XmlNodeIterator optionsRootIterator = _rootIterator->getChildren(_childName);

    Common::XmlNodeIterator targetExtractionOptionRootIterator =
            optionsRootIterator->getChildren(TARGET_EXTRACTION_OPTION_NODE);


    static map<string, Tucuxi::Core::TargetExtractionOption> m = {
            {"populationValues", Tucuxi::Core::TargetExtractionOption::PopulationValues},
            {"aprioriValues", Tucuxi::Core::TargetExtractionOption::AprioriValues},
            {"individualTargets", Tucuxi::Core::TargetExtractionOption::IndividualTargets},
            {"individualTargetsIfDefinitionExists",
             Tucuxi::Core::TargetExtractionOption::IndividualTargetsIfDefinitionExists},
            {"definitionIfNoIndividualTarget", Tucuxi::Core::TargetExtractionOption::DefinitionIfNoIndividualTarget},
            {"individualTargetsIfDefinitionExistsAndDefinitionIfNoIndividualTarget",
             Tucuxi::Core::TargetExtractionOption::IndividualTargetsIfDefinitionExistsAndDefinitionIfNoIndividualTarget}

    };

    string value = targetExtractionOptionRootIterator->getValue();
    auto it = m.find(value);
    if (it != m.end()) {
        return it->second;
    }

    return _default;
}

Core::FormulationAndRouteSelectionOption XpertQueryImport::getChildFormulationAndRouteSelectionOptionEnumOptional(
        Common::XmlNodeIterator _rootIterator, const string& _childName, Core::FormulationAndRouteSelectionOption _default)
{

    static const string FORMULATION_AND_ROUTE_SELECTION_OPTION_NODE = "formulationAndRouteSelectionOption";

    Common::XmlNodeIterator optionsRootIterator = _rootIterator->getChildren(_childName);

    Common::XmlNodeIterator formulationAndRouteSelectionOptioneRootIterator =
            optionsRootIterator->getChildren(FORMULATION_AND_ROUTE_SELECTION_OPTION_NODE);


    static map<string, Tucuxi::Core::FormulationAndRouteSelectionOption> m = {
            {"allFormulationAndRoutes", Tucuxi::Core::FormulationAndRouteSelectionOption::AllFormulationAndRoutes},
            {"lastFormulationAndRoute", Tucuxi::Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute},
            {"defaultFormulationAndRoute",
             Tucuxi::Core::FormulationAndRouteSelectionOption::DefaultFormulationAndRoute}};

    string value = formulationAndRouteSelectionOptioneRootIterator->getValue();
    auto it = m.find(value);
    if (it != m.end()) {
        return it->second;
    }

    return _default;
}

Common::DateTime XpertQueryImport::getChildDateTime(Common::XmlNodeIterator _rootIterator, const string& _childName)
{
    Common::XmlNodeIterator child = _rootIterator->getChildren(_childName);

    if (not child->isValid()) {
        return DateTime::undefinedDateTime();
    }
    return extractDateTime(child);
}

} // namespace XpertQuery
} // namespace Tucuxi
