#ifndef ADMINISTRATIVEDATA_H
#define ADMINISTRATIVEDATA_H

#include "tucucommon/datetime.h"

#include <string>
#include <memory>
#include <map>

namespace Tucuxi {
namespace XpertQuery {

/// \brief Data class for storing an address
/// \date 23.04.2022
/// \author Herzig Melvyn
class Address
{
public:
    // Constructors

    /// \brief Default constructor is not supported.
    Address() = delete;

    /// \brief Address constructor.
    /// \param _street Street of the address.
    /// \param _postCode City post code of the address.
    /// \param _city City ame of the address.
    /// \param _state State of the address.
    /// \param _country Country of the address.
    Address(
            const std::string& _street,
            int _postCode,
            const std::string& _city,
            const std::string& _state,
            const std::string& _country
            );

    /// \brief Copy constructor is not supported.
    Address(const Address& _other) = delete;

    // Getters
    std::string getStreet() const;
    int getPostCode() const;
    std::string getCity() const;
    std::string getState() const;
    std::string getCountry() const;

protected:
    const std::string m_street;
    const int m_postCode;
    const std::string m_city;
    const std::string m_state;
    const std::string m_country;
};

/// \brief Data class for contact information about a person or an institut
/// \date 23.04.20224
/// \author Herzig Melvyn
class ContactGroup
{
public:
    /// \brief Default constructor is not supported.
    ContactGroup() = delete;

    /// \brief Personal contact constructor.
    /// \param _pAddress Address.
    /// \param _pPhone Phone number.
    /// \param _pEmail Email address.
    ContactGroup( const std::unique_ptr<Address> _pAddress,
                  const std::string& _phone,
                  const std::string& _email
                  );

    /// \brief Copy constructor is not supported.
    ContactGroup(const ContactGroup& _other) = delete;

    // Getters
    const Address& getpAddress() const;
    std::string getpPhone() const;
    std::string getpEmail() const;

protected:
    std::unique_ptr<Address> m_pAddress;
    const std::string m_pPhone;
    const std::string m_pEmail;

};

/// \brief Data class for storing institute information.
/// \date 24.04.2022
/// \author Herzig Melvyn
class Institute
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    Institute() = delete;

    /// \brief Institute constructor
    /// \param _id Institute identifier.
    /// \param _name Institute name.
    /// \param _pContactGroup Institute contact information.
    Institute(
            std::string& _id,
            std::string& _name,
            std::unique_ptr<ContactGroup> _pContactGroup
            );

    /// \brief Copy constructor is not supported.
    Institute(const Institute& _other) = delete;

    // Getters
    const std::string getId() const;
    const std::string getName() const;
    const ContactGroup& getpContactGroup() const;

protected:
    const std::string m_id;
    const std::string m_name;
    std::unique_ptr<ContactGroup> m_pContactGroup;
};

/// \brief Data class for storing person information (class mandator and class patient parent).
/// \date 24.04.2022
/// \author Herzig Melvyn
class Person
{
public:
    // Constructors

    /// \brief Default constructor is not supported.
    Person() = delete;

    /// \brief Person constructor
    /// \param _firstname Firstname.
    /// \param _lastname Lastname.
    /// \param _pContactGroup Contact information.
    /// \param _pInstitute Affiliated institute.
    Person( const std::string& _firstname,
            const std::string& _lastname,
            std::unique_ptr<ContactGroup> _pContactGroup,
            std::unique_ptr<Institute> _pInstitute );

    /// \brief Copy constructor is not supported.
    Person(const Person& _other) = delete;

    // Getters
    std::string getFirstname() const;
    std::string getLastname() const;
    const ContactGroup& getpContactGroup() const;
    const Institute& getpInstitute() const;

protected:
    std::string m_firstname;
    std::string m_lastname;
    std::unique_ptr<ContactGroup> m_pContactGroup;
    std::unique_ptr<Institute> m_pInstitute;
};

/// \brief Data class for storing mendator information.
/// \date 24.04.2022
/// \author Herzig Melvyn
class Mendator : public Person
{
public:
    // Constructors

    /// \brief Default constructor is not supported.
    Mendator() = delete;

    /// \brief Person constructor.
    /// \param _id Mendator identifier.
    /// \param _firstname Firstname.
    /// \param _lastname Lastname.
    /// \param _pContactGroup Contact information.
    /// \param _pInstitute Affiliated institute.
    Mendator( const std::string& _id,
              const std::string& _firstname,
              const std::string& _lastname,
              std::unique_ptr<ContactGroup> _pContactGroup,
              std::unique_ptr<Institute> _pInstitute );

    /// \brief Copy constructor is not supported.
    Mendator(const Mendator& _other) = delete;

    // Getters
    std::string getId() const;

protected:
    std::string m_id;
};

/// \brief Data class for storing patient information.
/// \date 24.04.2022
/// \author Herzig Melvyn
class Patient : public Person
{
public:
    // Constructors

    /// \brief Default constructor is not supported.
    Patient() = delete;

    /// \brief Patient constructor
    /// \param _firstname Firstname.
    /// \param _lastname Lastname.
    /// \param _pBirthDate Patient's birth date.
    /// \param _gender Patient's gender
    /// \param _pContactGroup Contact information.
    /// \param _pInstitute Affiliated institute.
    Patient(  const std::string& _firstname,
              const std::string& _lastname,
              const Tucuxi::Common::DateTime& _pBirthDate,
              const std::string& _gender,
              std::unique_ptr<ContactGroup> _pContactGroup,
              std::unique_ptr<Institute> _pInstitute );

    /// \brief Copy constructor is not supported.
    Patient(const Patient& _other) = delete;

    // Getters
    const Tucuxi::Common::DateTime getpBirthDate() const;
    std::string getGender() const;

protected:
    std::string m_id;
};

/// \brief Class for storing administrative information
/// \date 24.04.2022
/// \author Herzig Melvyn
class AdministrativeData
{
public:
    // Constructors

    /// \brief Default constructor is not supported.
    AdministrativeData() = delete;

    /// \brief AdministrativeData constructor.
    /// \param _pMandator Mandator information.
    /// \param _pPatient Patient information.
    /// \param _pClinicalData Clinical data information.
    AdministrativeData( std::unique_ptr<Person> _pMandator,
                        std::unique_ptr<Person> _pPatient,
                        const std::map<std::string, std::string>& _clinicalData);

    /// \brief Copy constructor is not supported.
    AdministrativeData(AdministrativeData& _other) = delete;

    // Getters
    const Person& getpMandator() const;
    const Person& getpPatient() const;
    const std::map<std::string, std::string>& getclinicalData() const;

protected:
    std::unique_ptr<Person> m_pMandator;
    std::unique_ptr<Person> m_pPatient;
    std::map<std::string, std::string> m_clinicalData;
};

} // namespace XpertQuery
} // namespace Tucuxi


#endif // ADMINISTRATIVEDATA_H
