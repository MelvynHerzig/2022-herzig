#ifndef XPERTQUERYIMPORT_H
#define XPERTQUERYIMPORT_H

#include "tucuquery/queryimport.h"

#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/query/admindata.h"

namespace Tucuxi {
namespace Xpert {

/// \brief This class extends the Tucuxi computation core query importer to import admin data
///        and the custom requests "xpertRequest" when loading the XML query.
/// \date 23/04/2022
/// \author Herzig Melvyn
class XpertQueryImport : public Query::QueryImport
{
public:

    /// \brief Empty constructor.
    XpertQueryImport();

    /// \brief Import a query based on a file name. This function is reentrant.
    /// \param _query A reference to an xpert query pointer that will be allocated in the function.
    /// \param _fileName The name of the xml file that contains the query.
    /// \return Status::Ok if the import went well, otherwise another Status.
    Status importFromFile(std::unique_ptr<XpertQueryData>& _query, const std::string& _fileName);

    /// \brief Import a query based on an XML string. This function is reentrant.
    /// \param _query A reference to an xpert query pointer that will be allocated in the function.
    /// \param _xml An XML string that represents the query.
    /// \return Status::Ok if the import went well, otherwise another Status.
    Status importFromString(std::unique_ptr<XpertQueryData>& _query, const std::string& _xml);

protected:

    // -------------- Admin data --------------

    /// \brief Import an xpert query from an xml document. This function is reentrant.
    /// \param _query A reference to an xpert query pointer that will be allocated in the function.
    /// \param _document An xml document of the xpert query.
    /// \return Status::Ok if the import went well, otherwise another Status.
    Status importDocument(std::unique_ptr<XpertQueryData>& _query, Common::XmlDocument& _document);

    /// \brief Create admin data from the xml document.
    /// \param _document The xml document containing the admin element.
    /// \return A unique pointer on the created admin data. If the element doesn't exist, nullptr.
    std::unique_ptr<AdminData> createAdminData(Common::XmlDocument& _document);

    /// \brief Create a full person data from a mandator/patient element iterator.
    /// \param _mandatorPatientRootIterator Mendator/patient node iterator.
    /// \return A unique pointer on the mandator/patient data. If the iterator is not valid, nullptr.
    std::unique_ptr<FullPersonData> createFullPersonData(Common::XmlNodeIterator& _mandatorPatientRootIterator);

    /// \brief Create a person data object from a person element iterator.
    /// \param _personDataRootIterator Person element root iterator.
    /// \return A unique pointer on the person data. If the iterator is not valid, nullptr.
    std::unique_ptr<PersonData> createPersonData(Common::XmlNodeIterator& _personDataRootIterator);

    /// \brief Create an institute data object from an institute element iterator.
    /// \param _instituteRootIterator Institute node iterator.
    /// \return A unique pointer on the institute data. If the iterator is not valid, nullptr.
    std::unique_ptr<InstituteData> createInstituteData(Common::XmlNodeIterator& _instituteRootIterator);

    /// \brief Create an address data object from an address element iterator.
    /// \param _addressRootIterator Addresse node iterator.
    /// \return A unique pointer on the address data. If the iterator is not valid, nullptr.
    std::unique_ptr<AddressData> createAddressData(Common::XmlNodeIterator& _addressRootIterator);

    /// \brief Creates a phone data object from a phone element iterator.
    /// \param _phoneRootIterator Phone node iterator.
    /// \return A unique pointer on the phone data. If the iterator is not valid, nullptr.
    std::unique_ptr<PhoneData> createPhoneData(Common::XmlNodeIterator& _phoneRootIterator);

    /// \brief Creates an email data object from an email element iterator.
    /// \param _emailRootIterator Email node iterator.
    /// \return A unique pointer on the email data. If the iterator is not valid, nullptr.
    std::unique_ptr<EmailData> createEmailData(Common::XmlNodeIterator& _emailRootIterator);

    /// \brief Create the clinical datas object from the clinicalDatas iterator.
    /// \param _clinicalDatasRootIterator Clinical datas node iterator.
    /// \return A unique pointer on the clinical datas. If the iterator is not valid, nullptr.
    std::unique_ptr<ClinicalDatas> createClinicalDatas(Common::XmlNodeIterator& _clinicalDatasRootIterator);

    // -------------- Xpert request --------------

    /// \brief Extract an xpertRequest from an xpertRequest element iterator.
    /// \param _xpertRequestRootIterator XpertRequest node iterator.
    /// \return A unique pointer on the xpertRequest data. If the iterator is not valid, nullptr.
    std::unique_ptr<XpertRequestData> createXpertRequesData(Common::XmlNodeIterator& _xpertRequestRootIterator);

    /// \brief Extract a string from a child element of the given iterator. Does not set any import error.
    ///        If the child does not exist, return the given default value.
    /// \param _rootIterator Root iterator to look for the child element.
    /// \param _childName Name of the child element to look for.
    /// \param _default Default value to return if the child element does not exist.
    /// \return The value from child if it exists, otherwise _default.
    std::string getChildStringOptional(Common::XmlNodeIterator _rootIterator,
                                       const std::string& _childName,
                                       const std::string& _default);

    /// \brief Extract the loadingOption choice from an xpertRequest iterator. Does not set any import error.
    ///        If the element does not exist, return the given default value.
    /// \param _xpertRequestRootIterator XpertRequest root iterator.
    /// \param _childName Node name of the node containing the loadingOption.
    /// \param _default Default value to return if the child element or the loadingOption does not exist.
    /// \return The value from the loadingOption if it exists, otherwise _default.
    LoadingOption getChildLoadingOptionEnumOptional(Common::XmlNodeIterator _xpertRequestRootIterator,
                                                    const std::string& _childName,
                                                    LoadingOption _default);

    /// \brief Extract the restPeriodOption choice from an xpertRequest iterator. Does not set any import error.
    ///        If the element does not exist, return the given default value.
    /// \param _xpertRequestRootIterator XpertRequest root iterator.
    /// \param _childName Node name of the node containing the restPeriodOption.
    /// \param _default Default value to return if the child element or the restPeriodOption does not exist.
    /// \return The value from the restPeriodOption if it exists, otherwise _default.
    RestPeriodOption getChildRestPeriodOptionEnumOptional(Common::XmlNodeIterator _xpertRequestRootIterator,
                                                                const std::string& _childName,
                                                                RestPeriodOption _default);

    /// \brief Extract the targetExtractionOption choice from an xpertRequest iterator.
    ///        Does not set any import error. If the element does not exist, return the given default value.
    /// \param _xpertRequestRootIterator XpertRequest root iterator.
    /// \param _childName Node name of the node containing the targetExtractionOption.
    /// \param _default Default value to return if the child element or the targetExtractionOption does not exist.
    /// \return The value from the targetExtractionOption if it exists, otherwise _default.
    Core::TargetExtractionOption getChildTargetExtractionOptionEnumOptional(Common::XmlNodeIterator _xpertRequestRootIterator,
                                                                            const std::string& _childName,
                                                                            Core::TargetExtractionOption _default);

    /// \brief Extract the formulationAndRouteSelectionOption choice from an xpertRequest iterator.
    ///        Does not set any import error. If the element does not exist, return the given default value.
    /// \param _xpertRequestRootIterator XpertRequest root iterator.
    /// \param _childName Node name of the node containing the formulationAndRouteSelectionOption.
    /// \param _default Default value to return if the child element or the formulationAndRouteSelectionOption does not exist.
    /// \return The value from the formulationAndRouteSelectionOption if it exists, otherwise _default.
    Core::FormulationAndRouteSelectionOption getChildFormulationAndRouteSelectionOptionEnumOptional(Common::XmlNodeIterator _xpertRequestRootIterator,
                                                                                                    const std::string& _childName,
                                                                                                    Core::FormulationAndRouteSelectionOption _default);

    /// \brief For a given node iterator, extract the date from a child node.
    ///        Does not set any import error.
    /// \param _rootIterator Root iterator that has the child node.
    /// \param _childName Name of the child node that contains the date.
    /// \return If the child element exists and contains a valid dateTime, then it returns the contained dateTime.
    ///         Otherwise, it returns an undefined dateTime.
    Common::DateTime getChildDateTime(Common::XmlNodeIterator _rootIterator, const std::string& _childName);
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTQUERYIMPORT_H
