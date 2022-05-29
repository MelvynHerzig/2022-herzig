#ifndef DOSEVALIDATOR_H
#define DOSEVALIDATOR_H

#include "tucucore/drugmodel/formulationandroute.h"

#include "../xpertrequestresult.h"

namespace Tucuxi {
namespace XpertResult {

class DoseValidator
{
public:
    DoseValidator();

    void getDoseValidations(XpertRequestResult& _xpertRequestResult) const;

protected:

    void checkDoses(const Core::DosageHistory& _dosageHistory,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    void checkDoses(const Core::DosageTimeRange& _timeRange,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    void checkDoses(const Core::Dosage& _dosage,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    void checkDoses(const Core::DosageLoop& _dosageLoop,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    void checkDoses(const Core::DosageRepeat& _dosageRepeat,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    void checkDoses(const Core::DosageSequence& _dosageSequence,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    void checkDoses(const Core::ParallelDosageSequence& _parallelDosageSequence,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    void checkDosageBoundedList(const Core::DosageBoundedList& _dosageBoundedList,
                                const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                                std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;

    // May throw invalid argument if conversiont fails or if no fullformulationAndRoute found.
    void checkDoses(const Core::SingleDose& _singleDose,
                    const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                    std::map<const Core::SingleDose*, DoseResult>& _doseResults) const;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // DOSEVALIDATOR_H
