#ifndef XPERTADMINISTRATIVEDATA_H
#define XPERTADMINISTRATIVEDATA_H

/**************************************************
 |      Basically this class is a copypasta       |
 |    of Tucuquery::Query::AdministrativeData,    |
 |       but with const refs, and comments.       |
 **************************************************/

#include <string>
#include <optional>
#include <memory>
#include <map>

namespace Tucuxi {
namespace Xpert {

/// \brief Class representing an address of an institute or a person.
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
    /// \return Street name as string.
    const std::string& getStreet() const;

    /// \brief Gets the address post code.
    /// \return Post code integer.
    int getPostCode() const;

    /// \brief Gets the city.
    /// \return City name string.
    const std::string& getCity() const;

    /// \brief Gets the state.
    /// \return State name string.
    const std::string& getState() const;

    /// \brief Gets the country.
    /// \return Country name string.
    const std::string& getCountry() const;

protected:
    /// \brief Street of the address.
    const std::string m_street;

    /// \brief Post code of the address.
    const int m_postCode;

    /// \brief City of the address.
    const std::string m_city;

    /// \brief State of the address.
    const std::string m_state;

    /// \brief Country of the address.
    const std::string m_country;
};

/// \brief Class represent a phone number of an institute or a person.
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
    /// \brief Gets the phone number.
    /// \return Phone number string.
    const std::string& getNumber() const;

    /// \brief Gets the phone number type.
    /// \return Phone number type string.
    const std::string& getType() const;

protected:
    /// \brief Phone number.
    const std::string m_number;

    /// \brief Phone number type.
    const std::string m_type;
};

/// \brief Class represent a email address of an institute or a person.
/// \date 28/04/2022
class Email
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    Email() = delete;

    /// \brief Email constructor.
    /// \param _address Email address.
    /// \param _type Email type.
    Email(const std::string& _address, const std::string& _type);

    // Getters;
    /// \brief Gets the email address.
    /// \return Email address string.
    const std::string& getAddress() const;

    /// \brief Gets the email address type.
    /// \return Email address type string.
    const std::string& getType() const;

protected:
    /// \brief Email address.
    const std::string m_address;

    /// \brief Email address type.
    const std::string m_type;
};

/// \brief Class representing a person and his contact coordinates.
/// \date 28/04/2022
class PersonalContact
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    PersonalContact() = delete;

    /// \brief Personal contact constructor.
    /// \param _id Person's identifier.
    /// \param _title Person's title.
    /// \param _firstName Perosn's firstname.
    /// \param _lastName Person's lastname.
    /// \param _pAddress Person's address.
    /// \param _pPhone Person's phone.
    /// \param _pEmail Person's email.
    PersonalContact(
            const std::string& _id,
            const std::string& _title,
            const std::string& _firstName,
            const std::string& _lastName,
            std::unique_ptr<Address> _pAddress,
            std::unique_ptr<Phone> _pPhone,
            std::unique_ptr<Email> _pEmail);

    /// \brief Copy constructor is not supported because of unique pointers.
    PersonalContact(PersonalContact& _other) = delete;

    // Getters
    /// \brief Gets the person's identifier.
    /// \return Person's identifier string.
    const std::string& getId() const;

    /// \brief Gets the person's title.
    /// \return Person's title string.
    const std::string& getTitle() const;

    /// \brief Gets the person's firstname.
    /// \return Person's firstname string.
    const std::string& getFirstName() const;

    /// \brief Gets the person's lastname.
    /// \return Person's lastname string.
    const std::string& getLastName() const;

    /// \brief Gets the person's address.
    /// \return Person's address optional. May be nullptr.
    const std::unique_ptr<Address>& getpAddress() const;

    /// \brief Gets the person's phone number.
    /// \return Person's phone number optional. May be nullptr.
    const std::unique_ptr<Phone>& getpPhone() const;

    /// \brief Gets the person's email.
    /// \return Person's email optional. May be nullptr.
    const std::unique_ptr<Email>& getpEmail() const;

protected:
    /// \brief Perosn's identifier.
    const std::string m_id;

    /// \brief Person's title.
    const std::string m_title;

    /// \brief Person's firstname.
    const std::string m_firstName;

    /// \brief Person's lastname.
    const std::string m_lastName;

    /// \brief Person's address. May be nullptr.
    std::unique_ptr<Address> m_pAddress;

    /// \brief Person's phone number. May be nullptr.
    std::unique_ptr<Phone> m_pPhone;

    /// \brief Person's email. May be nullptr.
    std::unique_ptr<Email> m_pEmail;
};

/// \brief Class representing an institute and its contact coordinates.
/// \date 28/04/2022
class InstituteContact
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    InstituteContact() = delete;

    /// \brief Institute contact constructor.
    /// \param _id Identifier of the institute.
    /// \param _name Name of the institute.
    /// \param _pAddress Address of the institute.
    /// \param _pPhone Phone of the institute.
    /// \param _pEmail Email of the institute.
    InstituteContact(
            const std::string& _id,
            const std::string& _name,
            std::unique_ptr<Address> _pAddress,
            std::unique_ptr<Phone> _pPhone,
            std::unique_ptr<Email> _pEmail);

    /// \brief Copy constructor is not supported because of unique pointers.
    InstituteContact(InstituteContact& _other) = delete;

    // Getters
    /// \brief Gets the identifier of the institute.
    /// \return String identifier of the institute.
    const std::string& getId() const;

    /// \brief Gets the name of the institute.
    /// \return String name of the institute.
    const std::string& getName() const;

    /// \brief Gets the address of the institute.
    /// \return Optional address of the institute. May be nullptr.
    const std::unique_ptr<Address>& getpAddress() const;

    /// \brief Gets the phone of the institute.
    /// \return Optional phone of the institute. May be nullptr.
    const std::unique_ptr<Phone>& getpPhone() const;

    /// \brief Gets the email of the institute.
    /// \return Optional email of the institute. May be nullptr.
    const std::unique_ptr<Email>& getpEmail() const;

protected:
    /// \brief Identifier of the institute.
    const std::string m_id;

    /// \brief Name of the institute.
    const std::string m_name;

    /// \brief Address of the institute. May be nullptr.
    std::unique_ptr<Address> m_pAddress;

    /// \brief Phone of the institute. May be nullptr.
    std::unique_ptr<Phone> m_pPhone;

    /// \brief Email of the institute. May be nullptr.
    std::unique_ptr<Email> m_pEmail;
};

/// \brief A person (mandator or patient) has contact coordinates and may be affiliated to an institute.
/// \date 28/04/2022
class Person
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    Person() = delete;

    /// \brief Person constructor.
    /// \param _pPerson Person coordinates.
    /// \param _pInstitute Institute coordinates
    Person(std::unique_ptr<PersonalContact> _pPerson, std::unique_ptr<InstituteContact> _pInstitute);

    /// \brief Copy constructor is not supported because of unique pointers.
    Person(Person& _other) = delete;

    // Getters
    /// \brief Gets the person's coordinates.
    /// \return Person's coordinates reference.
    const PersonalContact& getpPerson() const;

    /// \brief Gets the person's institute.
    /// \return Person's optional institute.
    const std::unique_ptr<InstituteContact>& getpInstitute() const;

protected:
    /// \brief Person's coordinates.
    std::unique_ptr<PersonalContact> m_pPerson;

    /// \brief Peson's institute.
    std::unique_ptr<InstituteContact> m_pInstitute;
};

/// \brief Class for storing any information about a patient.
/// \date 28/04/2022
class ClinicalData
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    ClinicalData() = delete;

    /// \brief Clinical data constructor.
    /// \param _data Map of any data.
    ClinicalData(const std::map<std::string, std::string>& _data);

    // Getters
    /// \brief Gets the clinical data.
    /// \return Map containing clinical data.
    const std::map<std::string, std::string>& getData() const;

protected:
    /// \brief Clinical data.
    std::map<std::string, std::string> m_data;
};

/// \brief Wrapper for all administrative data of TuberXpert query.
/// \date 28/04/2022
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

    /// \brief Copy constructor is not supported because of unique pointers.
    AdministrativeData(AdministrativeData& _other) = delete;

    // Getters
    /// \brief Gets the mandator person.
    /// \return Optional mandator person. may be nullptr.
    const std::unique_ptr<Person>& getpMandator() const;

    /// \brief Gets the patient person.
    /// \return Optional patient person. May be nullptr.
    const std::unique_ptr<Person>& getpPatient() const;

    /// \brief Gets the clinical data.
    /// \return Optional clinical data. May be nullptr.
    const std::unique_ptr<ClinicalData>& getpClinicalData() const;

protected:
    /// \brief Mandator. May be nullptr.
    std::unique_ptr<Person> m_pMandator;

    /// \brief Patient. May be nullptr.
    std::unique_ptr<Person> m_pPatient;

    /// \brief Clinical data. May be nullptr.
    std::unique_ptr<ClinicalData> m_pClinicalData;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // XPERTADMINISTRATIVEDATA_H
