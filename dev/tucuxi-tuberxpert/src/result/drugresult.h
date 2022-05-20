#ifndef DRUGRESULT_H
#define DRUGRESULT_H

namespace Tucuxi {
namespace XpertResult {

/// \brief This is a wrapper class that contains the analysis for drug of tuberXpert.
///        It contains a reference to an associated drugData object.
///
///        The object is constructed by the default constructor. Its state is progressively
///        created when running validations. For example the covariates checks is initialized
///        when selecting best drug model
///
/// \date 20/05/2022
/// \author Herzig Melvyn
class DrugResult
{
public:
    /// \brief Default constructor.
    DrugResult();

protected:

};

} // namespace XpertResult
} // namespace Tucuxi

#endif // DRUGRESULT_H
