#ifndef XPERTREQUESTRESULT_H
#define XPERTREQUESTRESULT_H

#include <string>
#include <map>

#include "tucucore/drugmodel/covariatedefinition.h"
#include "tucucore/drugmodel/drugmodel.h"

#include "./covariateresult.h"

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

    XpertRequestResult();

    /// \brief Gets the drug model to use.
    /// \return Return the drug model.
    const Core::DrugModel* getDrugModel() const;

    /// \brief Gets the covariate results map.
    /// \return Return the map which corresponds CovariateDefinition& to CovariateResult.
    const std::map<Core::CovariateDefinition*, CovariateResult>& getCovariateResults();



    void setDrugModel(const Core::DrugModel* _newDrugModel);

    void setCovariateResults(const std::map<Core::CovariateDefinition *, CovariateResult>& _newCovariateResults);

protected:

    /// Drug model to use when making adjustmentRequest.
    const Core::DrugModel* m_drugModel;

    /// Mapping of CovariateResult to CovariateDefinition of the drug model.
    std::map<Core::CovariateDefinition*, CovariateResult> m_covariateResults;

};

} // namespace XpertResult
} // namespace Tucuxi

#endif // XPERTREQUESTRESULT_H
