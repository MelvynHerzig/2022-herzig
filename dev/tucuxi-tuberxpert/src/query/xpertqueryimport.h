#ifndef XPERTQUERYIMPORT_H
#define XPERTQUERYIMPORT_H

#include "tucuquery/queryimport.h"

#include "xpertquerydata.h"
#include "xpertadministrativedata.h"

namespace Tucuxi {
namespace XpertQuery {

/// \brief This class extends the tucucore queryimporter in order to load administrative information
/// and the custom request "requestXpert"
/// \date 23/04/2022
/// \author Herzig Melvyn
class XpertQueryImport : public Query::QueryImport
{
public:

    /// \brief Xpert query import empty constructor.
    XpertQueryImport();

    /// \brief Destructor.
    virtual ~XpertQueryImport();

    /// \brief Import a query based on a document name. This function is reentrant.
    /// \param _query A reference to a query pointer that will be allocated within the function
    /// \param _fileName The name of the file in which the drug model is stored
    /// \return Result::Ok if the import went well, another Result else.
    Status importFromFile(std::unique_ptr<XpertQueryData>& _query, const std::string& _fileName);

    /// \brief Import a query based on an xml string. This function is reentrant.
    /// \param _query A reference to a query pointer that will be allocated within the function
    /// \param _xml A string in which the drug model is stored
    /// \return Result::Ok if the import went well, another Result else.
    Status importFromString(std::unique_ptr<XpertQueryData>& _query, const std::string& _xml);

protected:

    /// \brief Import a query from an xml document. This function is reentrant.
    /// \param _query A reference to a query pointer that will be allocated within the function
    /// \param _document An XmlDocument in which the query is stored
    /// \return Result::Ok if the import went well, another Result else.
    Status importDocument(std::unique_ptr<XpertQueryData>& _query, Tucuxi::Common::XmlDocument& _document);

    // Methods to separate the creation of an AdministrativeData. Those methods were first creater in
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
    /// \param _instituteContactRootIterator Phone node iterator to start from.
    /// \return Unique pointer on phone number. Nullpointer if node doesn't exists.
    std::unique_ptr<Phone> createPhone(Common::XmlNodeIterator& _phoneRootIterator);

    /// \brief Creates an email object.
    /// \param _instituteContactRootIterator Email node iterator to start from.
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

    /// \brief getChildStringOptional Extract a child string value with optionnal value if not present.
    /// \param _rootIterator Iterator to get child string value.
    /// \param _childName Child name of iterator to extract string.
    /// \param _defaultValue Default value to return if child name is not present.
    /// \return  Return the value from child if present otherwise _defaultValue.
    std::string getChildStringOptional(Common::XmlNodeIterator _rootIterator, const std::string& _childName, std::string _defaultValue);
};

} // namespace XpertQuery
} // namespace Tucuxi

#endif // XPERTQUERYIMPORT_H
