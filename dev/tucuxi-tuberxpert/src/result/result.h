#ifndef RESULT_H
#define RESULT_H

#include <memory>
#include <map>

#include "tucucommon/datetime.h"

#include "../query/xpertquerydata.h"
#include "drugresult.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief This is a wrapper class that contains the analysis for each drug of tuberXpert.
///
///        Concretely, this class is created with an empty constructor. It is passed
///        by refrence to each execution steps to be populated with results gathered.
///        It mainly consists of map [DrugData, DrugResult] of drug results that is initlially populated
///        when selecting the models in modelselector.h
///
///        This class closely work with information from query file. Since a lot of
///        element are not copyable, it uses references on these elements. Consequently,
///        it is important that this class shares the same lifecycle than XpertQueryData associated.
///
///        To enforce that, when the object Result is created, it creates an internal XpertQueryData
///        object, that can be retrieved by a getter and it should be used by XpertQueryImport.
///
/// \date 20/05/2022
/// \author Herzig Melvyn
class Result
{
public:

    /// \brief Constructor
    /// \param lang Language to use for translating.
    Result(const std::string& _lang);

    /// \brief Copy constructor is not supported.
    ///  The copy constructor is not supported because of the use of
    ///  unique_ptr wich can't be copied.
    Result(const Result& _other) = delete;

    /// \brief Gets the language to use for
    /// \return Return the language to be used if possible.
    std::string getLanguage();

    /// \brief Gets a reference on the query to be used in XpertQueryImport.
    /// \return Return the query pointer by reference.
    std::unique_ptr<XpertQuery::XpertQueryData>& getQuery();

    /// \brief Gets the generation date of the result.
    /// \return Return the generation date.
    Common::DateTime getGenerationDate();

    /// \brief Gets a reference on map of drug results.
    /// \return Returns a reference of the drug results map.
    std::map<Query::DrugData*, DrugResult>& getDrugResults();

protected:

    /// Language to use if possible.
    const std::string m_language;

    /// Date of generation.
    Common::DateTime m_generationDate;

    /// Query importation data.
    std::unique_ptr<XpertQuery::XpertQueryData> m_query;

    /// Maps DrugData to associated DrugResult.
    std::map<Query::DrugData*, DrugResult> m_drugResults;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // RESULT_H
