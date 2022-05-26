#ifndef XPERTREQUESTRESULT_H
#define XPERTREQUESTRESULT_H

#include <string>
#include <vector>
#include <optional>

#include "tucucore/drugmodel/covariatedefinition.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucore/drugtreatment/drugtreatment.h"

#include "./covariateresult.h"
#include "../query/xpertrequestdata.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief This is a wrapper class that contains the analysis for drug of tuberXpert.
///        It contains a reference to an associated drugData object.
///
///        The object is constructed by the default constructor. Its state is progressively
///        created when running validations. For example the covariates checks is initialized
///        when selecting best drug model
/// \date 20/05/2022
/// \author Herzig Melvyn
class XpertRequestResult
{
public:
    /// \brief Default constructor.

    XpertRequestResult(
            std::unique_ptr<XpertQuery::XpertRequestData> _xpertRequest,
            std::unique_ptr<Core::DrugTreatment> _dTreatment,
            const std::optional<std::string>& _errorMessage);

    const std::unique_ptr<XpertQuery::XpertRequestData>& getXpertRequest() const;

    const std::unique_ptr<Core::DrugTreatment>& getTreatment() const;

    const std::optional<std::string>& getErrorMessage() const;

    const Core::DrugModel* getDrugModel() const;

    const std::vector<CovariateResult>& getCovariateResults();

    void setErrorMessage(const std::string& _message);

    void setDrugModel(const Core::DrugModel* _newDrugModel);

    void setCovariateResults(std::vector<CovariateResult>&& _newCovariateResults);

    bool shouldBeHandled() const;

protected:

    std::unique_ptr<XpertQuery::XpertRequestData> m_xpertRequest;

    std::unique_ptr<Core::DrugTreatment> m_dTreatment;

    std::optional<std::string> m_errorMessage;

    /// Drug model to use when making adjustmentRequest.
    Core::DrugModel* m_drugModel;

    /// Mapping of CovariateResult to CovariateDefinition of the drug model.
    std::vector<CovariateResult> m_covariateResults;

};

} // namespace XpertResult
} // namespace Tucuxi

#endif // XPERTREQUESTRESULT_H
