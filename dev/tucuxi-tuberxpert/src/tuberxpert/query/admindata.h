#ifndef ADMINDATA_H
#define ADMINDATA_H

/**************************************************
 |      Basically this class is a copypasta       |
 |    of Tucuquery::Query::AdministrativeData,    |
 |       but with const refs, and comments.       |
 |            and more consistant naming          |
 **************************************************/

#include <string>
#include <optional>
#include <memory>
#include <map>

namespace Tucuxi {
namespace Xpert {

/// \brief Class representing an address of an institute or a person.
/// \date 28/04/2022
class AddressData
{
public:
    // Constructors

    /// \brief Default constructor is not supported.
    AddressData() = delete;

    /// \brief Address constructor.
    /// \param _street Street name.
    /// \param _postCode Post code of the address.
    /// \param _city City of the address.
    /// \param _state State of the address.
    /// \param _country Country of the address.
    AddressData(const std::string& _street, int _postCode, const std::string& _city, const std::string& _state, const std::string& _country);

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
class PhoneData
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    PhoneData() = delete;

    /// \brief Phone constructor.
    /// \param _number Phone number.
    /// \param _type Phone type.
    PhoneData(const std::string& _number, const std::string& _type);

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

/// \brief Class represent an email address of an institute or a person.
/// \date 28/04/2022
class EmailData
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    EmailData() = delete;

    /// \brief Email constructor.
    /// \param _address Email address.
    /// \param _type Email type.
    EmailData(const std::string& _address, const std::string& _type);

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
class PersonData
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    PersonData() = delete;

    /// \brief Personal contact constructor.
    /// \param _id Person's identifier.
    /// \param _title Person's title.
    /// \param _firstName Perosn's firstname.
    /// \param _lastName Person's lastname.
    /// \param _address Person's address.
    /// \param _phone Person's phone.
    /// \param _email Person's email.
    PersonData(
            const std::string& _id,
            const std::string& _title,
            const std::string& _firstName,
            const std::string& _lastName,
            std::unique_ptr<AddressData> _address,
            std::unique_ptr<PhoneData> _phone,
            std::unique_ptr<EmailData> _email);

    /// \brief Copy constructor is not supported because of unique pointers.
    PersonData(PersonData& _other) = delete;

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
    const std::unique_ptr<AddressData>& getAddress() const;

    /// \brief Gets the person's phone number.
    /// \return Person's phone number optional. May be nullptr.
    const std::unique_ptr<PhoneData>& getPhone() const;

    /// \brief Gets the person's email.
    /// \return Person's email optional. May be nullptr.
    const std::unique_ptr<EmailData>& getEmail() const;

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
    std::unique_ptr<AddressData> m_address;

    /// \brief Person's phone number. May be nullptr.
    std::unique_ptr<PhoneData> m_phone;

    /// \brief Person's email. May be nullptr.
    std::unique_ptr<EmailData> m_email;
};

/// \brief Class representing an institute and its contact coordinates.
/// \date 28/04/2022
class InstituteData
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    InstituteData() = delete;

    /// \brief Institute contact constructor.
    /// \param _id Identifier of the institute.
    /// \param _name Name of the institute.
    /// \param _address Address of the institute.
    /// \param _phone Phone of the institute.
    /// \param _email Email of the institute.
    InstituteData(
            const std::string& _id,
            const std::string& _name,
            std::unique_ptr<AddressData> _address,
            std::unique_ptr<PhoneData> _phone,
            std::unique_ptr<EmailData> _email);

    /// \brief Copy constructor is not supported because of unique pointers.
    InstituteData(InstituteData& _other) = delete;

    // Getters
    /// \brief Gets the identifier of the institute.
    /// \return String identifier of the institute.
    const std::string& getId() const;

    /// \brief Gets the name of the institute.
    /// \return String name of the institute.
    const std::string& getName() const;

    /// \brief Gets the address of the institute.
    /// \return Optional address of the institute. May be nullptr.
    const std::unique_ptr<AddressData>& getAddress() const;

    /// \brief Gets the phone of the institute.
    /// \return Optional phone of the institute. May be nullptr.
    const std::unique_ptr<PhoneData>& getPhone() const;

    /// \brief Gets the email of the institute.
    /// \return Optional email of the institute. May be nullptr.
    const std::unique_ptr<EmailData>& getEmail() const;

protected:
    /// \brief Identifier of the institute.
    const std::string m_id;

    /// \brief Name of the institute.
    const std::string m_name;

    /// \brief Address of the institute. May be nullptr.
    std::unique_ptr<AddressData> m_address;

    /// \brief Phone of the institute. May be nullptr.
    std::unique_ptr<PhoneData> m_phone;

    /// \brief Email of the institute. May be nullptr.
    std::unique_ptr<EmailData> m_email;
};

/// \brief A person (mandator or patient) that has contact coordinates and may be affiliated to an institute.
/// \date 28/04/2022
class FullPersonData
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    FullPersonData() = delete;

    /// \brief Person constructor.
    /// \param _person Person coordinates.
    /// \param _institute Institute coordinates
    FullPersonData(std::unique_ptr<PersonData> _person, std::unique_ptr<InstituteData> _institute);

    /// \brief Copy constructor is not supported because of unique pointers.
    FullPersonData(FullPersonData& _other) = delete;

    // Getters
    /// \brief Gets the person's coordinates.
    /// \return Person's coordinates reference.
    const PersonData& getPerson() const;

    /// \brief Gets the person's institute.
    /// \return Person's optional institute.
    const std::unique_ptr<InstituteData>& getInstitute() const;

protected:
    /// \brief Person's coordinates.
    std::unique_ptr<PersonData> m_person;

    /// \brief Peson's institute.
    std::unique_ptr<InstituteData> m_institute;
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

/// \brief Wrapper for all admin data of TuberXpert query.
/// \date 28/04/2022
class AdminData
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    AdminData() = delete;

    /// \brief Constructor of administrative data.
    /// \param _mandator Mandator information pointer. May be nullptr.
    /// \param _patient Client information pointer. May be nullptr.
    /// \param _clinicalData Client clinical data information pointer. May be nullptr.
    AdminData(
            std::unique_ptr<FullPersonData> _mandator,
            std::unique_ptr<FullPersonData> _patient,
            std::unique_ptr<ClinicalData> _clinicalData);

    /// \brief Copy constructor is not supported because of unique pointers.
    AdminData(AdminData& _other) = delete;

    // Getters
    /// \brief Gets the mandator full person.
    /// \return Optional mandator person. may be nullptr.
    const std::unique_ptr<FullPersonData>& getMandator() const;

    /// \brief Gets the patient full person.
    /// \return Optional patient person. May be nullptr.
    const std::unique_ptr<FullPersonData>& getPatient() const;

    /// \brief Gets the clinical data.
    /// \return Optional clinical data. May be nullptr.
    const std::unique_ptr<ClinicalData>& getClinicalData() const;

protected:
    /// \brief Mandator. May be nullptr.
    std::unique_ptr<FullPersonData> m_mandator;

    /// \brief Patient. May be nullptr.
    std::unique_ptr<FullPersonData> m_patient;

    /// \brief Clinical data. May be nullptr.
    std::unique_ptr<ClinicalData> m_clinicalData;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // ADMINDATA_H
