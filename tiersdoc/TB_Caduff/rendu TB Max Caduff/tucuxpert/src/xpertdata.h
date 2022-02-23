#ifndef XPERTDATA_H
#define XPERTDATA_H
#include <string>
#include <vector>

#include "tucuquery/computingquery.h"
#include "tucucore/computingservice/computingrequest.h"
#include "tucuquery/computingqueryresponse.h"
#include "drugmodelselector.h"

namespace Tucuxi {
namespace Xpert {

class InstituteContact
{
public:
    InstituteContact(){}
    InstituteContact( std::string id,
                      std::string name,
                      std::string address,
                      std::string phone,
                      std::string email);

    std::string id;
    std::string name;
    std::string address;
    std::string phone;
    std::string email;

};

class PersonalContact
{
public:
    PersonalContact(){}
    PersonalContact(std::string id,
                    std::string title,
                    std::string firstName,
                    std::string lastName,
                    std::string address,
                    std::string phone,
                    std::string email);

    std::string id;
    std::string title;
    std::string firstName;
    std::string lastName;
    std::string address;
    std::string phone;
    std::string email;

};

class Person
{
public:

    Person(){}
    Person(PersonalContact pc, InstituteContact ic);

    PersonalContact personalContact;
    InstituteContact instituteContact;

};

class ClinicalData
{
public:

    ClinicalData(){}
    ClinicalData(std::string key, std::string value);

    std::string key;
    std::string value;

};

typedef std::vector<ClinicalData> ClinicalDatas;

class AdminData
{
public:
    AdminData();


    Person mandator;
    Person patient;

    ClinicalDatas clinicalDatas;

};


class XpertData
{
public:


    DrugModelResult getDrugModelResult() const;
    void setDrugModelResult(const DrugModelResult& _drugModelResult);

    Query::ComputingQuery& getQueries();
//    void setQueries(const Query::ComputingQuery &queries);

    std::string getLang() const;
    void setLang(const std::string& lang);


    Core::DrugTreatment* getDrugTreatment() const;
    void setDrugTreatment(Core::DrugTreatment* _drugTreatment);

    Core::PredictionParameterType getPredictionType() const;
    void setPredictionType(const Core::PredictionParameterType& _predictionType);

    Query::ComputingQueryResponse& getResponses();

private:
    Query::ComputingQuery m_queries = Query::ComputingQuery("default"); // move to constr

    DrugModelResult m_drugModelResult;

    std::unique_ptr<Core::DrugTreatment> m_drugTreatment;

    std::string m_lang;

    Core::PredictionParameterType m_predictionType;

    Query::ComputingQueryResponse m_responses;

    // TODO: add AdminData

};


}
}


#endif // XPERTDATA_H
