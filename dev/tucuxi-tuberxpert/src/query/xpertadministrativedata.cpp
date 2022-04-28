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


} // namespace XpertQuery
} // namespace Tucuxi
