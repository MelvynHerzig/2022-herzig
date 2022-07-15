#include "tuberxpert/query/admindata.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

/**************************************************
 |                   AddressData                  |
 **************************************************/

AddressData::AddressData(const string& _street,
                         int _postCode,
                         const string& _city,
                         const string& _state,
                         const string& _country)
    : m_street(move(_street)),
      m_postalCode(_postCode),
      m_city(move(_city)),
      m_state(move(_state)),
      m_country(move(_country))
{
}

string AddressData::getStreet() const
{
    return m_street;
}

int AddressData::getPostalCode() const
{
    return m_postalCode;
}

string AddressData::getCity() const
{
    return m_city;
}

string AddressData::getState() const
{
    return m_state;
}

string AddressData::getCountry() const
{
    return m_country;
}

/**************************************************
 |                    PhoneData                   |
 **************************************************/

PhoneData::PhoneData(const string& _number, const string& _type) : m_number(_number), m_type(_type) {}

string PhoneData::getNumber() const
{
    return m_number;
}

string PhoneData::getType() const
{
    return m_type;
}

/**************************************************
 |                   EmailData                    |
 **************************************************/

EmailData::EmailData(const string& _address, const string& _type) : m_address(_address), m_type(_type) {}

string EmailData::getAddress() const
{
    return m_address;
}

string EmailData::getType() const
{
    return m_type;
}

/**************************************************
 |                  PersonData                    |
 **************************************************/

PersonData::PersonData(
        const string& _id,
        const string& _title,
        const string& _firstName,
        const string& _lastName,
        unique_ptr<AddressData> _address,
        unique_ptr<PhoneData> _phone,
        unique_ptr<EmailData> _email)
    : m_id(_id), m_title(_title),
      m_firstName(_firstName),
      m_lastName(_lastName),
      m_address(move(_address)),
      m_phone(move(_phone)),
      m_email(move(_email))
{
}

string PersonData::getId() const
{
    return m_id;
}

string PersonData::getTitle() const
{
    return m_title;
}

string PersonData::getFirstName() const
{
    return m_firstName;
}

string PersonData::getLastName() const
{
    return m_lastName;
}

const unique_ptr<AddressData>& PersonData::getAddress() const
{
    return m_address;
}

const unique_ptr<PhoneData>& PersonData::getPhone() const
{
    return m_phone;
}

const unique_ptr<EmailData>& PersonData::getEmail() const
{
    return m_email;
}

/**************************************************
 |                InstituteData                   |
 **************************************************/

InstituteData::InstituteData(
        const string& _id,
        const string& _name,
        unique_ptr<AddressData> _address,
        unique_ptr<PhoneData> _phone,
        unique_ptr<EmailData> _email)
    : m_id(_id),
      m_name(_name),
      m_address(move(_address)),
      m_phone(move(_phone)),
      m_email(move(_email))
{
}

string InstituteData::getId() const
{
    return m_id;
}

string InstituteData::getName() const
{
    return m_name;
}

const unique_ptr<AddressData>& InstituteData::getAddress() const
{
    return m_address;
}

const unique_ptr<PhoneData>& InstituteData::getPhone() const
{
    return m_phone;
}

const unique_ptr<EmailData>& InstituteData::getEmail() const
{
    return m_email;
}

/**************************************************
 |                   FullPersonData               |
 **************************************************/

FullPersonData::FullPersonData(unique_ptr<PersonData> _person,
                               unique_ptr<InstituteData> _institute)
    : m_person(move(_person)),
      m_institute(move(_institute))
{
}

const PersonData& FullPersonData::getPerson() const
{
    return *m_person;
}

const unique_ptr<InstituteData>& FullPersonData::getInstitute() const
{
    return m_institute;
}

/**************************************************
 |                 ClinicalDatas                   |
 **************************************************/

ClinicalDatas::ClinicalDatas(const map<string, string>& _data) : m_data(_data) {}

const map<string, string>& ClinicalDatas::getData() const
{
    return m_data;
}

/**************************************************
 |                   AdminData                    |
 **************************************************/

AdminData::AdminData(
        unique_ptr<FullPersonData> _mandator,
        unique_ptr<FullPersonData> _patient,
        unique_ptr<ClinicalDatas> _clinicalDatas)
    : m_mandator(move(_mandator)),
      m_patient(move(_patient)),
      m_clinicalDatas(move(_clinicalDatas))
{
}

const unique_ptr<FullPersonData>& AdminData::getMandator() const
{
    return m_mandator;
}

const unique_ptr<FullPersonData>& AdminData::getPatient() const
{
    return m_patient;
}

const unique_ptr<ClinicalDatas>& AdminData::getClinicalDatas() const
{
    return m_clinicalDatas;
}

} // namespace Xpert
} // namespace Tucuxi
