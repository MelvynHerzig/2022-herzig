#ifndef DOSEVALIDATOR_H
#define DOSEVALIDATOR_H

#include "tucucore/drugmodel/formulationandroute.h"

#include "../xpertrequestresult.h"

namespace Tucuxi {
namespace XpertResult {

/// \brief This is a utility class responsible to check each dosage plausibility.
///        In order to do this, it compares each patient's doses to the recommanded
///        doses from the drug model.
///
///        The current implementation consider each doses. A futur improvement for specific
///        drug could be to consider only the doses that are not oldest than X years/months/days...
/// \date 01/06/2022
/// \author Herzig Melvyn
class DoseValidator
{
public:

    /// \brief Constructor.
    DoseValidator();

    /// \brief Evaluates all the doses in the treatment from the XpertRequestResult.
    ///        There must be a compatible drug model set and a treatment.
    ///        If the evaluation fails (for example: incompatible dose) the XpertRequestResult
    ///        is invalidated (i.e: it gets an error).
    /// \param _xpertRequestResult XpertRequestResult to evaluate.
    void getDoseValidations(XpertRequestResult& _xpertRequestResult) const;

protected:

    /// \brief Parses a given DosageHistory and evaluates the contained doses.
    /// \param _dosageHistory Dosage history to parse.
    /// \param _modelFormulationAndRoutes Full formulations and routes available in the model.
    /// \param _doseResults Map of DoseResult to stores result of underlying doses evaluation.
    /// \throw invalid_argument If the no compatible formulation and route found or if the unit
    ///                         conversions of doses failed.
    void checkDoses(const Core::DosageHistory& _dosageHistory,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    /// \brief Parses a given TimeRange and evaluates the contained doses.
    /// \param _timeRange Time range to parse.
    /// \param _modelFormulationAndRoutes Full formulations and routes available in the model.
    /// \param _doseResults Map of DoseResult to stores result of underlying doses evaluation.
    /// \throw invalid_argument If the no compatible formulation and route found or if the unit
    ///                         conversions of doses failed.
    void checkDoses(const Core::DosageTimeRange& _timeRange,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    /// \brief Converts an abstract dosage to its real type to call the corresponding checDoses method.
    /// \param _dosage Dosage to convert.
    /// \param _modelFormulationAndRoutes Full formulations and routes available in the model.
    /// \param _doseResults Map of DoseResult to stores result of underlying doses evaluation.
    /// \throw invalid_argument If the no compatible formulation and route found or if the unit
    ///                         conversions of doses failed.
    void checkDoses(const Core::Dosage& _dosage,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    /// \brief Parses a dosage loop and evaluates the contained doses.
    /// \param _dosageLoop Dosage loop to parse.
    /// \param _modelFormulationAndRoutes Full formulations and routes available in the model.
    /// \param _doseResults Map of DoseResult to stores result of underlying doses evaluation.
    /// \throw invalid_argument If the no compatible formulation and route found or if the unit
    ///                         conversions of doses failed.
    void checkDoses(const Core::DosageLoop& _dosageLoop,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    /// \brief Parses a dosage repeat and evaluates the contained doses.
    /// \param _dosageRepeat Dosage repeat to parse.
    /// \param _modelFormulationAndRoutes Full formulations and routes available in the model.
    /// \param _doseResults Map of DoseResult to stores result of underlying doses evaluation.
    /// \throw invalid_argument If the no compatible formulation and route found or if the unit
    ///                         conversions of doses failed.
    void checkDoses(const Core::DosageRepeat& _dosageRepeat,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    /// \brief Parses a dosage sequence and evaluates the contained doses.
    /// \param _dosageSequence Dosage sequence to parse.
    /// \param _modelFormulationAndRoutes Full formulations and routes available in the model.
    /// \param _doseResults Map of DoseResult to stores result of underlying doses evaluation.
    /// \throw invalid_argument If the no compatible formulation and route found or if the unit
    ///                         conversions of doses failed.
    void checkDoses(const Core::DosageSequence& _dosageSequence,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    /// \brief Parses a parallel dosage sequence and evaluates the contained doses.
    /// \param _parallelDosageSequence Parallel dosage sequence to parse.
    /// \param _modelFormulationAndRoutes Full formulations and routes available in the model.
    /// \param _doseResults Map of DoseResult to stores result of underlying doses evaluation.
    /// \throw invalid_argument If the no compatible formulation and route found or if the unit
    ///                         conversions of doses failed.
    void checkDoses(const Core::ParallelDosageSequence& _parallelDosageSequence,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    /// \brief Checks all dose from a dosage bounded list. Mainly used by check dose method with
    ///        dosage sequence and parallel dosage sequence.
    /// \param _dosageBoundedList Dosage bounded list to evaluates underlying doses.
    /// \param _modelFormulationAndRoutes Full formulations and routes available in the model.
    /// \param _doseResults Map of DoseResult to stores result of underlying doses evaluation.
    /// \throw invalid_argument If the no compatible formulation and route found or if the unit
    ///                         conversions of doses failed.
    void checkDosageBoundedList(const Core::DosageBoundedList& _dosageBoundedList,
                                const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                                std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;




    /// \brief For a single dose, checks its compatibility with the recommended doses from the drug model.
    ///        The drug model doses are located inside the full formulations and routes list in argument.
    /// \param _singleDose Single dose to check.
    /// \param _modelFormulationAndRoutes Full formulations and routes available in the model.
    /// \param _doseResults Map of DoseResult to stores result of the single dose evaluation.
    /// \throw invalid_argument If the no compatible formulation and route found or if the unit
    ///                         conversions of doses failed.
    void checkDoses(const Core::SingleDose& _singleDose,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // DOSEVALIDATOR_H
