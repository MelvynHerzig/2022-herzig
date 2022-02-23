#include "xpertdata.h"

namespace Tucuxi {
namespace Xpert {

AdminData::AdminData()
{ }

InstituteContact::InstituteContact(std::string id, std::string name, std::string address, std::string phone, std::string email)
    : id(id), name(name), address(address), phone(phone), email(email)
{ }

PersonalContact::PersonalContact(std::string id, std::string title, std::string firstName, std::string lastName, std::string address, std::string phone, std::string email)
    : id(id), title(title), firstName(firstName), lastName(lastName), address(address), phone(phone), email(email)
{ }

Person::Person(PersonalContact pc, InstituteContact ic)
    : personalContact(pc), instituteContact(ic)
{ }

ClinicalData::ClinicalData(std::string key, std::string value)
    : key(key), value(value)
{ }


DrugModelResult XpertData::getDrugModelResult() const
{
    return m_drugModelResult;
}

void XpertData::setDrugModelResult(const DrugModelResult &drugModelResult)
{
    m_drugModelResult = drugModelResult;
}

Query::ComputingQuery& XpertData::getQueries()
{
    return m_queries;
}

std::string XpertData::getLang() const
{
    return m_lang;
}

void XpertData::setLang(const std::string& lang)
{
    m_lang = lang;
}

Core::DrugTreatment* XpertData::getDrugTreatment() const
{
    return m_drugTreatment.get();
}

void XpertData::setDrugTreatment(Core::DrugTreatment* _drugTreatment)
{
    m_drugTreatment = std::unique_ptr<Core::DrugTreatment>(_drugTreatment);
}

Core::PredictionParameterType XpertData::getPredictionType() const
{
    return m_predictionType;
}

void XpertData::setPredictionType(const Core::PredictionParameterType& _predictionType)
{
    m_predictionType = _predictionType;
}

Query::ComputingQueryResponse& XpertData::getResponses()
{
    return m_responses;
}


//void XpertData::setQueries(const Query::ComputingQuery &queries)
//{
//    m_queries = queries;
//}


}
}
