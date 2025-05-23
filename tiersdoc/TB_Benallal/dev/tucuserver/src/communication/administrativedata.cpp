#include "query.h"

using namespace std;
using namespace Tucuxi;

namespace Tucuxi {
namespace Server {

// Address class

Address::Address(
        string _street,
        int _postCode,
        string _city,
        string _state,
        string _country
) : m_street(_street), m_postCode(_postCode), m_city(_city),
    m_state(_state), m_country(_country)
{}

const string Address::getStreet() const
{
    return m_street;
}

int Address::getPostCode() const
{
    return m_postCode;
}

const string Address::getCity() const
{
    return m_city;
}

const string Address::getState() const
{
    return m_state;
}

const string Address::getCountry() const
{
    return m_country;
}

// Phone class

Phone::Phone(string& _number, string& _type)
    : m_number(_number), m_type(_type)
{}

const string Phone::getNumber() const
{
    return m_number;
}

const string Phone::getType() const
{
    return m_type;
}

// Email class

Email::Email(string& _address, string& _type)
    : m_address(_address), m_type(_type)
{}

const string Email::getAddress() const
{
    return m_address;
}

const string Email::getType() const
{
    return m_type;
}

// PersonnalContact class

PersonalContact::PersonalContact(
        string& _id,
        string& _title,
        string& _firstName,
        string& _lastName,
        unique_ptr<Address> _pAddress,
        unique_ptr<Phone> _pPhone,
        unique_ptr<Email> _pEmail
) : m_id(_id), m_title(_title), m_firstName(_firstName),
    m_lastName(_lastName), m_pAddress(move(_pAddress)),
    m_pPhone(move(_pPhone)), m_pEmail(move(_pEmail))
{}

const string PersonalContact::getId() const
{
    return m_id;
}

const string PersonalContact::getTitle() const
{
    return m_title;
}

const string PersonalContact::getFirstName() const
{
    return m_firstName;
}

const string PersonalContact::getLastName() const
{
    return m_lastName;
}

const Address& PersonalContact::getpAddress() const
{
    return *m_pAddress;
}

const Phone& PersonalContact::getpPhone() const
{
    return *m_pPhone;
}

const Email& PersonalContact::getpEmail() const
{
    return *m_pEmail;
}

// InstituteContact class

InstituteContact::InstituteContact(
        string& _id,
        string& _name,
        unique_ptr<Address> _pAddress,
        unique_ptr<Phone> _pPhone,
        unique_ptr<Email> _pEmail
) : m_id(_id), m_name(_name), m_pAddress(move(_pAddress)),
    m_pPhone(move(_pPhone)), m_pEmail(move(_pEmail))
{}

const string InstituteContact::getId() const
{
    return m_id;
}

const string InstituteContact::getName() const
{
    return m_name;
}

const Address& InstituteContact::getpAddress() const
{
    return *m_pAddress;
}

const Phone& InstituteContact::getpPhone() const
{
    return *m_pPhone;
}

const Email& InstituteContact::getpEmail() const
{
    return *m_pEmail;
}

// Person class

Person::Person(std::unique_ptr<PersonalContact> _pPerson, std::unique_ptr<InstituteContact> _pInstitute)
    : m_pPerson(move(_pPerson)), m_pInstitute(move(_pInstitute))
{}

const PersonalContact& Person::getpPerson() const
{
    return *m_pPerson;
}

const InstituteContact& Person::getpInstitute() const
{
    return *m_pInstitute;
}

// ClinicalData class

ClinicalData::ClinicalData(
        std::map<std::string, std::string>& _data
) : m_data(_data)
{}

const std::map<std::string, std::string>& ClinicalData::getData() const
{
    return m_data;
}

// AdministrativeData class

AdministrativeData::AdministrativeData(
        unique_ptr<Person> _pMandator,
        unique_ptr<Person> _pPatient,
        unique_ptr<ClinicalData> _pClinicalData
) : m_pMandator(move(_pMandator)), m_pPatient(move(_pPatient)),
    m_pClinicalData(move(_pClinicalData))
{}

const Person& AdministrativeData::getpMandator() const
{
    return *m_pMandator;
}

const Person& AdministrativeData::getpPatient() const
{
    return *m_pPatient;
}

const ClinicalData& AdministrativeData::getpClinicalData() const
{
    return *m_pClinicalData;
}

} // namespace Server
} // namespace Tucuxi
