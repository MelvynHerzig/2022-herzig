#ifndef XPERTQUERYIMPORT_H
#define XPERTQUERYIMPORT_H

#include "tucuquery/queryimport.h"

#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/query/xpertadministrativedata.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class extends the tucucore queryimporter in order to import administrative data
///         and the custom requests "requestXpert" when loading an xml query.
/// \date 23/04/2022
/// \author Herzig Melvyn
class XpertQueryImport : public Query::QueryImport
{
public:

    /// \brief XpertQueryImport empty constructor.
    XpertQueryImport();

    /// \brief Destructor.
    virtual ~XpertQueryImport();

    /// \brief Imports a query based on a file name. This function is reentrant.
    /// \param _query A reference to a query pointer that will be allocated within the function.
    /// \param _fileName The name of the file in which the drug model is stored.
    /// \return Status::Ok if the import went well, another Status otherwise.
    Status importFromFile(std::unique_ptr<XpertQueryData>& _query, const std::string& _fileName);

    /// \brief Imports a query based on an xml string. This function is reentrant.
    /// \param _query A reference to a query pointer that will be allocated within the function
    /// \param _xml A string in which the drug model is stored
    /// \return Status::Ok if the import went well, another Status otherwise.
    Status importFromString(std::unique_ptr<XpertQueryData>& _query, const std::string& _xml);

protected:

    /// \brief Imports a query from an xml document. This function is reentrant.
    /// \param _query A reference to a query pointer that will be allocated within the function
    /// \param _document An XmlDocument in which the query is stored
    /// \return Status::Ok if the import went well, another Status otherwise.
    Status importDocument(std::unique_ptr<XpertQueryData>& _query, Common::XmlDocument& _document);

    // Methods to separate the creation of an AdministrativeData. Those methods were first created in
    // Benallal TB from 2018.
    /// \brief Creates administrative data from the xml document.
    /// \return Unique pointer on administrative data. Nullpointer if node doesn't exists.
    std::unique_ptr<AdministrativeData> createAdministrativeData(Common::XmlDocument& _document);

    /// \brief Creates a person from a mandator/patient node.
    /// \param _personRootIterator Mendator/patient node to start from.
    /// \return Unique pointer on person data. Nullpointer if node doesn't exists.
    std::unique_ptr<Person> createPerson(Common::XmlNodeIterator& _personRootIterator);

    /// \brief Creates a person contact object.
    /// \param _personalContactRootIterator Person node iterator to start from.
    /// \return Unique pointer on person contact. Nullpointer if node doesn't exists.
    std::unique_ptr<PersonalContact> createPersonalContact(Common::XmlNodeIterator& _personalContactRootIterator);

    /// \brief Creates an institut contact object.
    /// \param _instituteContactRootIterator Institut node iterator to start from.
    /// \return Unique pointer on institut contact. Nullpointer if node doesn't exists.
    std::unique_ptr<InstituteContact> createInstituteContact(Common::XmlNodeIterator& _instituteContactRootIterator);

    /// \brief Creates an address object.
    /// \param _addressRootIterator Addresse node iterator to start from.
    /// \return Unique pointer on address. Nullpointer if node doesn't exists.
    std::unique_ptr<Address> createAddress(Common::XmlNodeIterator& _addressRootIterator);

    /// \brief Creates a phone number object.
    /// \param _phoneRootIterator Phone node iterator to start from.
    /// \return Unique pointer on phone number. Nullpointer if node doesn't exists.
    std::unique_ptr<Phone> createPhone(Common::XmlNodeIterator& _phoneRootIterator);

    /// \brief Creates an email object.
    /// \param _emailRootIterator Email node iterator to start from.
    /// \return Unique pointer on email. Nullpointer if node doesn't exists.
    std::unique_ptr<Email> createEmail(Common::XmlNodeIterator& _emailRootIterator);

    /// \brief Creates clinical data from ClinicalData node.
    /// \param _clinicalDataRootIterator ClinicalData node to start from.
    /// \return Unique pointer on clinical data. Nullpointer if node doesn't exists.
    std::unique_ptr<ClinicalData> createClinicalData(Common::XmlNodeIterator& _clinicalDataRootIterator);

    // Methods to separate the creation of the requests
    /// \brief Extracts a custom Xpert Request from the xml.
    /// \param _requestXpertRootIterator XpertRequest node to start from.
    /// \return Unique pointer on request xpert data.
    std::unique_ptr<XpertRequestData> createRequestXpert(Common::XmlNodeIterator& _requestXpertRootIterator);

    /// \brief Extracts a child string value with optionnal value if not present. Does not set any import error.
    /// \param _rootIterator XpertRequest root iterator.
    /// \param _childName Node name of options.
    /// \param _default Default value to return if child name is not present.
    /// \return Returns the value from child if present otherwise _default.
    std::string getChildStringOptional(Common::XmlNodeIterator _rootIterator, const std::string& _childName, const std::string& _default);

    /// \brief Parses the document to get the loadingOption choice optionally. Does not set any import error.
    /// \param _rootIterator XpertRequest root iterator.
    /// \param _childName Node name of options.
    /// \param _default Default value to return if child name is not present.
    /// \return If found the loading option set otherwise Unspecified.
    LoadingOption getChildLoadingOptionEnumOptional(Common::XmlNodeIterator _rootIterator, const std::string& _childName, LoadingOption _default);

    /// \brief Parse the document to get the restPeriodOtion choice optionally. Does not set any import error.
    /// \param _rootIterator XpertRequest root iterator.
    /// \param _childName Node name of options.
    /// \param _default Default value to return if child name is not present.
    /// \return If found the rest period option set otherwise Unspecified.
    RestPeriodOption getChildRestPeriodTargetOptionEnumOptional(Common::XmlNodeIterator _rootIterator, const std::string& _childName, RestPeriodOption _default);

    /// \brief Parses the document to get the targetExtractionOption choice optionally. Does not set any import error.
    /// \param _rootIterator XpertRequest root iterator.
    /// \param _childName Node name of options.
    /// \param _default Default value to return if child name is not present.
    /// \return If found the target extraction option set otherwise definitionIfNoIndividualTarget.
    Core::TargetExtractionOption getChildTargetExtractionOptionEnumOptional(Common::XmlNodeIterator _rootIterator, const std::string& _childName, Core::TargetExtractionOption _default);

    /// \brief Parses the document to get the formulationAndRouteSelectionOpton choice optionally. Does not set any import error.
    /// \param _rootIterator XpertRequest root iterator.
    /// \param _childName Node name of options.
    /// \param _default Default value to return if child name is not present.
    /// \return If found the formulation and route selection option set otherwise lastFormulationAndRoute.
    Core::FormulationAndRouteSelectionOption getChildFormulationAndRouteSelectionOptionEnumOptional(Common::XmlNodeIterator _rootIterator, const std::string& _childName, Core::FormulationAndRouteSelectionOption _default);

    /// \brief Parses the document to get the adjustment date optionally. Does not set any import error.
    /// \param _rootIterator XpertRequest root iterator.
    /// \param _childName Node name of adjustment date.
    /// \return If found the adjustment date set otherwhise UndefinedDateTime.
    Common::DateTime getChildDateTime(Common::XmlNodeIterator _rootIterator, const std::string& _childName);
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTQUERYIMPORT_H
