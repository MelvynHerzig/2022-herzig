#ifndef XPERTRESULT_H
#define XPERTRESULT_H

#include <memory>
#include <map>

#include "tucucommon/datetime.h"
#include "tucucore/drugtreatment/drugtreatment.h"

#include "../query/xpertquerydata.h"
#include "xpertrequestresult.h"

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
class XpertResult
{
public:

    /// \brief Constructor
    XpertResult(std::unique_ptr<XpertQuery::XpertQueryData> _xpertQuery);

    /// \brief Copy constructor is not supported.
    ///  The copy constructor is not supported because of the use of
    ///  unique_ptr wich can't be copied.
    XpertResult(const XpertResult& _other) = delete;


    /// \brief Gets the generation date of the result.
    /// \return Return the generation date.
    Common::DateTime getGenerationDate();

    const std::unique_ptr<XpertQuery::AdministrativeData>& getAdministrative() const;

    const std::vector<XpertRequestResult>& getXpertRequestResults() const;

protected:

    /// Date of generation.
    Common::DateTime m_generationDate;

    /// Administrative information.
    std::unique_ptr<XpertQuery::AdministrativeData> m_pAdministrative;

    std::vector<XpertRequestResult> m_xpertRequestResults;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // XPERTRESULT_H
