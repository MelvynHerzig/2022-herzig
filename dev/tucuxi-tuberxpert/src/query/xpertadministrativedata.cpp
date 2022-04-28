#include "xpertadministrativedata.h"

using namespace std;
using namespace Tucuxi;

namespace Tucuxi {
namespace XpertQuery {

/**************************************************
 |                    Address                     |
 **************************************************/

Address::Address(const string& _street, int _postCode, const string& _city, const string& _state, const string& _country)
    : m_street(std::move(_street)), m_postCode(_postCode), m_city(std::move(_city)), m_state(std::move(_state)),
      m_country(std::move(_country))
{
}

const string& Address::getStreet() const
{
    return m_street;
}

int Address::getPostCode() const
{
    return m_postCode;
}

const string& Address::getCity() const
{
    return m_city;
}

const string& Address::getState() const
{
    return m_state;
}

const string& Address::getCountry() const
{
    return m_country;
}

/**************************************************
 |                     Phone                      |
 **************************************************/

Phone::Phone(const string& _number, const string& _type) : m_number(_number), m_type(_type) {}

const string& Phone::getNumber() const
{
    return m_number;
}

const string& Phone::getType() const
{
    return m_type;
}

/**************************************************
 |                     Email                      |
 **************************************************/

Email::Email(const string& _address, const string& _type) : m_address(_address), m_type(_type) {}

const string& Email::getAddress() const
{
    return m_address;
}

const string& Email::getType() const
{
    return m_type;
}

/**************************************************
 |               PersonnalContact                 |
 **************************************************/

PersonalContact::PersonalContact(
        const string& _id,
        const string& _title,
        const string& _firstName,
        const string& _lastName,
        unique_ptr<Address> _pAddress,
        unique_ptr<Phone> _pPhone,
        unique_ptr<Email> _pEmail)
    : m_id(_id), m_title(_title), m_firstName(_firstName), m_lastName(_lastName), m_pAddress(move(_pAddress)),
      m_pPhone(move(_pPhone)), m_pEmail(move(_pEmail))
{
}

const string& PersonalContact::getId() const
{
    return m_id;
}

const string& PersonalContact::getTitle() const
{
    return m_title;
}

const string& PersonalContact::getFirstName() const
{
    return m_firstName;
}

const string& PersonalContact::getLastName() const
{
    return m_lastName;
}

std::optional<std::reference_wrapper<const Address>> PersonalContact::getpAddress() const
{
    if(m_pAddress != nullptr){
        return optional<reference_wrapper<const Address>>{*m_pAddress};
    }
    return nullopt;
}

std::optional<std::reference_wrapper<const Phone>> PersonalContact::getpPhone() const
{
    if(m_pPhone != nullptr){
        return optional<reference_wrapper<const Phone>>{*m_pPhone};
    }
    return nullopt;
}

std::optional<std::reference_wrapper<const Email>> PersonalContact::getpEmail() const
{
    if(m_pEmail != nullptr){
        return optional<reference_wrapper<const Email>>{*m_pEmail};
    }
    return nullopt;
}

/**************************************************
 |               InstituteContact                 |
 **************************************************/

InstituteContact::InstituteContact(
        const string& _id,
        const string& _name,
        unique_ptr<Address> _pAddress,
        unique_ptr<Phone> _pPhone,
        unique_ptr<Email> _pEmail)
    : m_id(_id), m_name(_name), m_pAddress(move(_pAddress)), m_pPhone(move(_pPhone)), m_pEmail(move(_pEmail))
{
}

const string& InstituteContact::getId() const
{
    return m_id;
}

const std::string& InstituteContact::getName() const
{
    return m_name;
}

std::optional<std::reference_wrapper<const Address>> InstituteContact::getpAddress() const
{
    if(m_pAddress != nullptr){
        return optional<reference_wrapper<const Address>>{*m_pAddress};
    }
    return nullopt;
}

std::optional<std::reference_wrapper<const Phone>> InstituteContact::getpPhone() const
{
    if(m_pPhone != nullptr){
        return optional<reference_wrapper<const Phone>>{*m_pPhone};
    }
    return nullopt;
}

std::optional<std::reference_wrapper<const Email>> InstituteContact::getpEmail() const
{
    if(m_pEmail != nullptr){
        return optional<reference_wrapper<const Email>>{*m_pEmail};
    }
    return nullopt;
}

/**************************************************
 |                    Person                      |
 **************************************************/

Person::Person(std::unique_ptr<PersonalContact> _pPerson, std::unique_ptr<InstituteContact> _pInstitute)
    : m_pPerson(move(_pPerson)), m_pInstitute(move(_pInstitute))
{
}

const PersonalContact& Person::getpPerson() const
{
    return *m_pPerson;
}

optional<reference_wrapper<const InstituteContact>> Person::getpInstitute() const
{
    if(m_pInstitute != nullptr){
        return optional<reference_wrapper<const InstituteContact>>{*m_pInstitute};
    }
    return nullopt;
}

/**************************************************
 |                 ClinicalData                   |
 **************************************************/

ClinicalData::ClinicalData(const std::map<std::string, std::string>& _data) : m_data(_data) {}

const std::map<std::string, std::string>& ClinicalData::getData() const
{
    return m_data;
}

/**************************************************
 |              AdministrativeData                |
 **************************************************/

AdministrativeData::AdministrativeData(
        unique_ptr<Person> _pMandator,
        unique_ptr<Person> _pPatient,
        unique_ptr<ClinicalData> _pClinicalData)
    : m_pMandator(move(_pMandator)), m_pPatient(move(_pPatient)), m_pClinicalData(move(_pClinicalData))
{
}

std::optional<std::reference_wrapper<const Person>> AdministrativeData::getpMandator() const
{
    if(m_pMandator != nullptr){
        return optional<reference_wrapper<const Person>>{*m_pMandator};
    }
    return nullopt;
}

std::optional<std::reference_wrapper<const Person>> AdministrativeData::getpPatient() const
{
    if(m_pPatient != nullptr){
        return optional<reference_wrapper<const Person>>{*m_pPatient};
    }
    return nullopt;
}

std::optional<std::reference_wrapper<const ClinicalData>> AdministrativeData::getpClinicalData() const
{
    if(m_pClinicalData != nullptr){
        return optional<reference_wrapper<const ClinicalData>>{*m_pClinicalData};
    }
    return nullopt;
}

} // namespace XpertQuery
} // namespace Tucuxi
