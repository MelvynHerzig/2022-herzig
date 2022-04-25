#include "xpertqueryimport.h"

#include "tucuquery/parametersdata.h"

using namespace std;

namespace Tucuxi {
namespace XpertQuery {

XpertQueryImport::XpertQueryImport() = default;


XpertQueryImport::~XpertQueryImport() = default;


Common::IImport::Status XpertQueryImport::importFromFile(std::unique_ptr<XpertQueryData> &_query, const std::string &_fileName)
{
    // Ensure the function is reentrant
    std::lock_guard<std::mutex> lock(m_mutex);

    setStatus(Status::Ok);
    _query = nullptr;

    Tucuxi::Common::XmlDocument document;
    if (!document.open(_fileName)) {
        setStatus(Status::CantCreateXmlDocument, "file could not be opened");
        return Status::CantOpenFile;
    }

    return importDocument(_query, document);
}

Common::IImport::Status XpertQueryImport::importFromString(std::unique_ptr<XpertQueryData> &_query, const std::string &_xml)
{
    // Ensure the function is reentrant
    std::lock_guard<std::mutex> lock(m_mutex);

    setStatus(Status::Ok);
    _query = nullptr;

    Tucuxi::Common::XmlDocument document;

    if (!document.fromString(_xml)) {
        setStatus(Status::CantCreateXmlDocument, "xml document could not be created. The tags must be controlled");
        return Status::CantCreateXmlDocument;
    }

    return importDocument(_query, document);
}

Common::IImport::Status XpertQueryImport::importDocument(std::unique_ptr<XpertQuery::XpertQueryData>& _query, Common::XmlDocument& _document)
{
    static const string QUERY_ID_NODE_NAME = "queryId";
    static const string CLIENT_ID_NODE_NAME = "clientId";
    static const string DATE_NODE_NAME = "date";
    static const string LANGUAGE_NODE_NAME = "language";
    static const string REQUESTS_NODE_NAME = "requests";
    static const string REQUEST_REQUESTS_NODE_NAME = "request";
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

    unique_ptr<Query::AdministrativeData> pAdministrativeData = createAdministrativeData(_document);

    unique_ptr<Query::DrugTreatmentData> pParametersData = createDrugTreatmentData(_document);

    Common::XmlNodeIterator requestsRootIterator = root.getChildren(REQUESTS_NODE_NAME);
    checkNodeIterator(requestsRootIterator, REQUESTS_NODE_NAME);
    Common::XmlNodeIterator requestsIterator = requestsRootIterator->getChildren(REQUEST_REQUESTS_NODE_NAME);
    checkNodeIterator(requestsIterator, REQUEST_REQUESTS_NODE_NAME);

    vector<unique_ptr<Query::RequestData> > requests;
    while (requestsIterator != Common::XmlNodeIterator::none()) {
        requests.emplace_back(createRequest(requestsIterator));
        requestsIterator++;
    }

    Common::XmlNodeIterator requestsXpertIterator = requestsRootIterator->getChildren(REQUEST_XPERT_REQUESTS_NODE_NAME);
    checkNodeIterator(requestsIterator, REQUEST_REQUESTS_NODE_NAME);

    vector<unique_ptr<XpertRequestData> > requestsXpert;
    while (requestsIterator != Common::XmlNodeIterator::none()) {
        //requests.emplace_back(createRequest(requestsIterator));
        requestsIterator++;
    }

    _query = std::make_unique<XpertQueryData>(queryId, clientId, date, language,  move(pAdministrativeData), move(pParametersData), requests, requestsXpert);

    return getStatus();
}

std::unique_ptr<Query::AdministrativeData> XpertQueryImport::createAdministrativeData(Common::XmlDocument& _document)
{
    static const string ADMIN_NODE_NAME = "admin";
    static const string MANDATOR_NODE_NAME = "mandator";
    static const string PATIENT_NODE_NAME = "patient";
    static const string CLINICALDATA_NODE_NAME = "clinicalData";

    Common::XmlNode root = _document.getRoot();
    Common::XmlNodeIterator adminRootIterator = root.getChildren(ADMIN_NODE_NAME);

    // If the node is not present
    if (!adminRootIterator->isValid()) {
        return nullptr;
    }

    Common::XmlNodeIterator mandatorRootIterator = adminRootIterator->getChildren(MANDATOR_NODE_NAME);
    Common::XmlNodeIterator patientRootIterator  = adminRootIterator->getChildren(PATIENT_NODE_NAME);
    Common::XmlNodeIterator clinicalDataRootIterator  = adminRootIterator->getChildren(CLINICALDATA_NODE_NAME);

    unique_ptr<Query::Person> pMandator = createPerson(mandatorRootIterator);
    unique_ptr<Query::Person> pPatient = createPerson(patientRootIterator);
    unique_ptr<Query::ClinicalData> pClinicalData = createClinicalData(clinicalDataRootIterator);

    return make_unique<Query::AdministrativeData>(
                move(pMandator),
                move(pPatient),
                move(pClinicalData)
                );
}

std::unique_ptr<Query::Person> XpertQueryImport::createPerson(Common::XmlNodeIterator& _personRootIterator)
{
    if (!_personRootIterator->isValid()) {
        return nullptr;
    }

    static const string PERSON_NODE_NAME = "person";
    static const string INSTITUTE_NODE_NAME = "institute";

    Common::XmlNodeIterator iterator = _personRootIterator->getChildren(PERSON_NODE_NAME);
    unique_ptr<Query::PersonalContact> pPersonalContact = createPersonalContact(iterator);
    iterator = _personRootIterator->getChildren(INSTITUTE_NODE_NAME);
    unique_ptr<Query::InstituteContact> pInstituteContact = createInstituteContact(iterator);

    return make_unique<Query::Person>(
                move(pPersonalContact),
                move(pInstituteContact)
                );
}

std::unique_ptr<Query::PersonalContact> XpertQueryImport::createPersonalContact(Common::XmlNodeIterator& _personalContactRootIterator)
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

    string id = getChildString(_personalContactRootIterator, ID_NODE_NAME);
    string title = getChildString(_personalContactRootIterator, TITLE_NODE_NAME);
    string firstname = getChildString(_personalContactRootIterator, FIRSTNAME_NODE_NAME);
    string lastname = getChildString(_personalContactRootIterator, LASTNAME_NODE_NAME);
    Common::XmlNodeIterator iterator = _personalContactRootIterator->getChildren(ADDRESS_NODE_NAME);
    unique_ptr<Query::Address> pAddress = createAddress(iterator);
    iterator = _personalContactRootIterator->getChildren(PHONE_NODE_NAME);
    unique_ptr<Query::Phone> pPhone = createPhone(iterator);
    iterator = _personalContactRootIterator->getChildren(EMAIL_NODE_NAME);
    unique_ptr<Query::Email> pEmail = createEmail(iterator);

    return make_unique<Query::PersonalContact>(
                id,
                title,
                firstname,
                lastname,
                move(pAddress),
                move(pPhone),
                move(pEmail)
                );
}

std::unique_ptr<Query::InstituteContact> XpertQueryImport::createInstituteContact(Common::XmlNodeIterator& _instituteContactRootIterator)
{
    if (!_instituteContactRootIterator->isValid()) {
        return nullptr;
    }

    static const string ID_NODE_NAME = "id";
    static const string NAME_NODE_NAME = "name";
    static const string ADDRESS_NODE_NAME = "address";
    static const string PHONE_NODE_NAME = "phone";
    static const string EMAIL_NODE_NAME = "email";

    string id = getChildString(_instituteContactRootIterator, ID_NODE_NAME);
    string name = getChildString(_instituteContactRootIterator, NAME_NODE_NAME);
    Common::XmlNodeIterator iterator = _instituteContactRootIterator->getChildren(ADDRESS_NODE_NAME);
    unique_ptr<Query::Address> pAddress = createAddress(iterator);
    iterator = _instituteContactRootIterator->getChildren(PHONE_NODE_NAME);
    unique_ptr<Query::Phone> pPhone = createPhone(iterator);
    iterator = _instituteContactRootIterator->getChildren(EMAIL_NODE_NAME);
    unique_ptr<Query::Email> pEmail = createEmail(iterator);

    return make_unique<Query::InstituteContact>(
                id,
                name,
                move(pAddress),
                move(pPhone),
                move(pEmail)
                );
}

std::unique_ptr<Query::Address> XpertQueryImport::createAddress(Common::XmlNodeIterator& _addressRootIterator)
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
    string state = getChildString(_addressRootIterator, STATE_NODE_NAME);
    string country = getChildString(_addressRootIterator, COUNTRY_NODE_NAME);

    return make_unique<Query::Address>(
                street,
                postalCode,
                city,
                state,
                country
                );
}

std::unique_ptr<Query::Phone> XpertQueryImport::createPhone(Common::XmlNodeIterator& _phoneRootIterator)
{
    if (!_phoneRootIterator->isValid()) {
        return nullptr;
    }

    static const string NUMBER_NODE_NAME = "address";
    static const string TYPE_NODE_NAME = "type";

    string number = getChildString(_phoneRootIterator, NUMBER_NODE_NAME);
    string type = getChildString(_phoneRootIterator, TYPE_NODE_NAME);

    return make_unique<Query::Phone>(number, type);
}

std::unique_ptr<Query::Email> XpertQueryImport::createEmail(Common::XmlNodeIterator& _emailRootIterator)
{
    if (!_emailRootIterator->isValid()) {
        return nullptr;
    }

    static const string ADDRESS_NODE_NAME = "address";
    static const string TYPE_NODE_NAME = "type";

    string address = getChildString(_emailRootIterator, ADDRESS_NODE_NAME);
    string type = getChildString(_emailRootIterator, TYPE_NODE_NAME);

    return make_unique<Query::Email>(address, type);
}

std::unique_ptr<Query::ClinicalData> XpertQueryImport::createClinicalData(Common::XmlNodeIterator& _clinicalDataRootIterator)
{
    if (!_clinicalDataRootIterator->isValid()) {
        return nullptr;
    }

    map<string, string> data;

    Common::XmlNodeIterator xmlNodeIterator = _clinicalDataRootIterator->getChildren();
    while(xmlNodeIterator != xmlNodeIterator.none()) {
        data[xmlNodeIterator->getName()] = xmlNodeIterator->getValue();
        xmlNodeIterator++;
    }

    return make_unique<Query::ClinicalData>(data);
}

std::unique_ptr<XpertRequestData> XpertQueryImport::createRequest(Common::XmlNodeIterator &_requestXpertRootIterator)
{
    static const string DRUG_ID_NODE_NAME = "drugId";
    static const string LOCAL_COMPUTATION_NODE_NAME = "localComputation";
    static const string OUTPUT_NODE_NAME = "output";
    static const string DATE_ADJUSTMENT_TIME_NODE_NAME = "adjustmentDate";
    static const string OPTIONS_NODE_NAME = "options";

    return nullptr;
}
//unique_ptr<RequestData> QueryImport::createRequest(Tucuxi::Common::XmlNodeIterator& _requestRootIterator)
//{
//    static const string REQUEST_ID_NODE_NAME = "requestId";
//    static const string DRUG_ID_NODE_NAME = "drugId";
//    static const string DRUGMODEL_ID_NODE_NAME = "drugModelId";
//    static const string COMPUTING_TRAIT_ADJUSTMENT_NAME = "adjustmentTraits";
//    static const string COMPUTING_TRAIT_CONCENTRATION_NAME = "predictionTraits";
//    static const string COMPUTING_TRAIT_PERCENTILES_NAME = "percentilesTraits";
//    static const string COMPUTING_TRAIT_SINGLE_POINT_NAME = "predictionAtTimesTraits";
//    static const string COMPUTING_TRAIT_AT_MESURE_NAME = "predictionAtSampleTimesTraits";

//    string requestId = getChildString(_requestRootIterator, REQUEST_ID_NODE_NAME);
//    string drugId = getChildString(_requestRootIterator, DRUG_ID_NODE_NAME);
//    string drugModelId = getChildString(_requestRootIterator, DRUGMODEL_ID_NODE_NAME);

//    unique_ptr<Tucuxi::Core::ComputingTrait> computingTrait;
//    Common::XmlNodeIterator computingTraitAdjustmentRootIterator =
//            _requestRootIterator->getChildren(COMPUTING_TRAIT_ADJUSTMENT_NAME);

//    if (computingTraitAdjustmentRootIterator != Common::XmlNodeIterator::none()) {
//        computingTrait = getChildComputingTraitAdjustment(computingTraitAdjustmentRootIterator, requestId);
//    }

//    Common::XmlNodeIterator computingTraitConcentrationRootIterator =
//            _requestRootIterator->getChildren(COMPUTING_TRAIT_CONCENTRATION_NAME);

//    if (computingTraitConcentrationRootIterator != Common::XmlNodeIterator::none()) {
//        computingTrait = getChildComputingTraitConcentration(computingTraitConcentrationRootIterator, requestId);
//    }

//    Common::XmlNodeIterator computingTraitPercentilesRootIterator =
//            _requestRootIterator->getChildren(COMPUTING_TRAIT_PERCENTILES_NAME);

//    if (computingTraitPercentilesRootIterator != Common::XmlNodeIterator::none()) {
//        computingTrait = getChildComputingTraitPercentiles(computingTraitPercentilesRootIterator, requestId);
//    }

//    Common::XmlNodeIterator computingTraitSinglePointRootIterator =
//            _requestRootIterator->getChildren(COMPUTING_TRAIT_SINGLE_POINT_NAME);

//    if (computingTraitSinglePointRootIterator != Common::XmlNodeIterator::none()) {
//        computingTrait = getChildComputingTraitSinglePoints(computingTraitSinglePointRootIterator, requestId);
//    }

//    Common::XmlNodeIterator computingTraitAtMeasuresRootIterator =
//            _requestRootIterator->getChildren(COMPUTING_TRAIT_AT_MESURE_NAME);

//    if (computingTraitAtMeasuresRootIterator != Common::XmlNodeIterator::none()) {
//        computingTrait = getChildComputingTraitAtMeasures(computingTraitAtMeasuresRootIterator, requestId);
//    }

//    if (computingTrait == nullptr) {
//        setNodeError(computingTraitAdjustmentRootIterator);
//    }



//    return make_unique<RequestData>(requestId, drugId, drugModelId, move(computingTrait));
//}

} // namespace XpertQuery
} // namespace Tucuxi
