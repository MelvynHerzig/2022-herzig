#ifndef DRUGRESULT_H
#define DRUGRESULT_H

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
class DrugResult
{
public:
    /// \brief Default constructor.
    /// \param _drugModel Drug model associated to the current drug result
    DrugResult(const Core::DrugModel* _drugModel);

    /// \brief Gets the drug model to use.
    /// \return Return the drug model.
    const Core::DrugModel* getDrugModel() const;

    /// \brief Gets the covariate results map.
    /// \return Return the map which corresponds CovariateDefinition& to CovariateResult.
    std::map<Core::CovariateDefinition*, AbsractCovariateResult*>& getCovariateResults();

protected:

    /// Drug model to use when making adjustmentRequest.
    const Core::DrugModel* m_drugModel;

    /// Mapping of CovariateResult to CovariateDefinition of the drug model.
    std::map<Core::CovariateDefinition*, AbsractCovariateResult*> m_covariateResults;

};

} // namespace XpertResult
} // namespace Tucuxi

#endif // DRUGRESULT_H
