#ifndef XPERTADMINISTRATIVEDATA_H
#define XPERTADMINISTRATIVEDATA_H

/**************************************************
 |      Basically this class is a copypasta       |
 |    of Tucuquery::Query::AdministrativeData,    |
 |    but with better practices, const refs and   |
 |                  optionals                     |
 **************************************************/

#include <string>
#include <optional>
#include <memory>
#include <map>


namespace Tucuxi {
namespace XpertQuery {

/// \brief Represent an address.
/// \date 28/04/2022
class Address
{
public:
    // Constructors

    /// \brief Default constructor is not supported.
    Address() = delete;

    /// \brief Address constructor.
    /// \param _street Street name.
    /// \param _postCode Post code of the address.
    /// \param _city City of the address.
    /// \param _state State of the address.
    /// \param _country Country of the address.
    Address(const std::string& _street, int _postCode, const std::string& _city, const std::string& _state, const std::string& _country);

    // Getters
    /// \brief Gets the street.
    /// \return Street string
    const std::string& getStreet() const;

    /// \brief Get the address post code.
    /// \return Post code integer.
    int getPostCode() const;

    /// \brief Gets the city.
    /// \return City name string.
    const std::string& getCity() const;

    /// \brief Gets the state.
    /// \return State name string
    const std::string& getState() const;

    /// \brief Gets the country
    /// \return Country name string.
    const std::string& getCountry() const;

protected:
    /// street of the address.
    const std::string m_street;

    /// Post code of the address.
    const int m_postCode;

    /// City of the address.
    const std::string m_city;

    /// State of the address.
    const std::string m_state;

    /// Country of the address.
    const std::string m_country;
};

/// \brief Represent phone number
/// \date 28/04/2022
class Phone
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    Phone() = delete;

    /// \brief Phone constructor.
    /// \param _number Phone number.
    /// \param _type Phone type.
    Phone(const std::string& _number, const std::string& _type);

    // Getters
    const std::string& getNumber() const;
    const std::string& getType() const;

protected:
    const std::string m_number;
    const std::string m_type;
};

class Email
{
public:
    // Constructors
    Email() = delete;

    Email(const std::string& _address, const std::string& _type);

    Email(Email& _other) = delete;

    // Getters;
    const std::string& getAddress() const;
    const std::string& getType() const;

protected:
    const std::string m_address;
    const std::string m_type;
};

class PersonalContact
{
public:
    // Constructors
    PersonalContact() = delete;

    PersonalContact(
            const std::string& _id,
            const std::string& _title,
            const std::string& _firstName,
            const std::string& _lastName,
            std::unique_ptr<Address> _pAddress,
            std::unique_ptr<Phone> _pPhone,
            std::unique_ptr<Email> _pEmail);

    PersonalContact(PersonalContact& _other) = delete;

    // Getters
    const std::string& getId() const;
    const std::string& getTitle() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    std::optional<std::reference_wrapper<const Address>> getpAddress() const;
    std::optional<std::reference_wrapper<const Phone>> getpPhone() const;
    std::optional<std::reference_wrapper<const Email>> getpEmail() const;

protected:
    const std::string m_id;
    const std::string m_title;
    const std::string m_firstName;
    const std::string m_lastName;
    std::unique_ptr<Address> m_pAddress;
    std::unique_ptr<Phone> m_pPhone;
    std::unique_ptr<Email> m_pEmail;
};

class InstituteContact
{
public:
    // Constructors
    InstituteContact() = delete;

    InstituteContact(
            const std::string& _id,
            const std::string& _name,
            std::unique_ptr<Address> _pAddress,
            std::unique_ptr<Phone> _pPhone,
            std::unique_ptr<Email> _pEmail);

    InstituteContact(InstituteContact& _other) = delete;

    // Getters
    const std::string& getId() const;
    const std::string& getName() const;
    std::optional<std::reference_wrapper<const Address>> getpAddress() const;
    std::optional<std::reference_wrapper<const Phone>> getpPhone() const;
    std::optional<std::reference_wrapper<const Email>> getpEmail() const;

protected:
    const std::string m_id;
    const std::string m_name;
    std::unique_ptr<Address> m_pAddress;
    std::unique_ptr<Phone> m_pPhone;
    std::unique_ptr<Email> m_pEmail;
};

class Person
{
public:
    // Constructors
    Person() = delete;

    Person(std::unique_ptr<PersonalContact> _pPerson, std::unique_ptr<InstituteContact> _pInstitute);

    Person(Person& _other) = delete;

    // Getters
    std::optional<std::reference_wrapper<const PersonalContact>> getpPerson() const;
    std::optional<std::reference_wrapper<const InstituteContact>> getpInstitute() const;

protected:
    std::unique_ptr<PersonalContact> m_pPerson;
    std::unique_ptr<InstituteContact> m_pInstitute;
};

class ClinicalData
{
public:
    // Constructors
    ClinicalData() = delete;

    ClinicalData(const std::map<std::string, std::string>& _data);

    ClinicalData(ClinicalData& _other) = delete;

    // Getters
    const std::map<std::string, std::string>& getData() const;

protected:
    const std::map<std::string, std::string> m_data;
};

/// \brief This class extends the query AdministrativeData to provide better getter.
/// Since most information are supposed optional, returning a reference on nullptr is a bad practice.
/// Instead this class provide optional object.
/// \date 28/04/2022
/// \author Herzig Melvyn
class AdministrativeData
{
public:
    // Constructors

    /// \brief Default constructor is not supported.
    AdministrativeData() = delete;

    /// \brief Constructor of administrative data.
    /// \param _pMandator Mandator information pointer. May be nullptr.
    /// \param _pPatient Client information pointer. May be nullptr.
    /// \param _pClinicalData Client clinical data information pointer. May be nullptr.
    AdministrativeData(
            std::unique_ptr<Person> _pMandator,
            std::unique_ptr<Person> _pPatient,
            std::unique_ptr<ClinicalData> _pClinicalData);

    /// \brief Copy constructor is not supported.
    AdministrativeData(AdministrativeData& _other) = delete;

    // Getters
    std::optional<std::reference_wrapper<const Person>> getpMandator() const;
    std::optional<std::reference_wrapper<const Person>> getpPatient() const;
    std::optional<std::reference_wrapper<const ClinicalData>> getpClinicalData() const;

protected:
    std::unique_ptr<Person> m_pMandator;
    std::unique_ptr<Person> m_pPatient;
    std::unique_ptr<ClinicalData> m_pClinicalData;
};

} // namespace XpertQuery
} // namespace Tucuxi

#endif // XPERTADMINISTRATIVEDATA_H
