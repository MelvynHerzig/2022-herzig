#include "dosevalidator.h"

#include <memory>

#include "tucucommon/unit.h"

#include "../../utils/xpertutils.h"
#include "../../language/languagemanager.h"
#include "../doseresult.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

DoseValidator::DoseValidator()
{}

void DoseValidator::getDoseValidations(XpertRequestResult& _xpertRequestResult) const
{
    // Checks treatment
    if (_xpertRequestResult.getTreatment() == nullptr) {
        _xpertRequestResult.setErrorMessage("No treatment set.");
        return;
    }

    // Checks drug model
    if (_xpertRequestResult.getDrugModel() == nullptr) {
        _xpertRequestResult.setErrorMessage("No drug model set.");
        return;
    }

    const Core::DosageHistory& dosageHistory = _xpertRequestResult.getTreatment()->getDosageHistory();
    const Core::FormulationAndRoutes& modelFormulationAndRoutes = _xpertRequestResult.getDrugModel()->getFormulationAndRoutes();

    try {
        map<const Core::SingleDose *, DoseResult> results;
        checkDoses(dosageHistory, modelFormulationAndRoutes, results);
        _xpertRequestResult.setDoseResults(move(results));
    } catch (invalid_argument& e) {
        _xpertRequestResult.setErrorMessage("Patient dosage error found, details: " + string(e.what()));
    }
}

void DoseValidator::checkDoses(const Core::DosageHistory& _dosageHistory,
                               const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                               map<const Core::SingleDose *, DoseResult>& _doseResults) const
{

    for(const unique_ptr<Core::DosageTimeRange>& timeRange : _dosageHistory.getDosageTimeRanges()){
        checkDoses(*timeRange, _modelFormulationAndRoutes, _doseResults);
    }

}

void DoseValidator::checkDoses(const Core::DosageTimeRange& _timeRange,
                               const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                               map<const Core::SingleDose*, DoseResult>& _doseResults) const
{
    checkDoses(*_timeRange.getDosage(), _modelFormulationAndRoutes, _doseResults);
}

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TRY_CHECK(Type)                                                                                       \
    if (dynamic_cast<const Tucuxi::Core::Type*>(&_dosage)) {                                                  \
    checkDoses(*dynamic_cast<const Tucuxi::Core::Type*>(&_dosage), _modelFormulationAndRoutes, _doseResults); \
}

void DoseValidator::checkDoses(const Core::Dosage& _dosage,
                               const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                               std::map<const Core::SingleDose*, DoseResult> &_doseResults) const
{
    // The calls order is important here.
    // First start with the subclasses, else it won't work
    TRY_CHECK(SingleDose);
    TRY_CHECK(ParallelDosageSequence);
    TRY_CHECK(DosageSequence);
    TRY_CHECK(DosageRepeat);
    TRY_CHECK(DosageLoop);
}

void DoseValidator::checkDoses(const Core::DosageLoop& _dosageLoop,
                               const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                               map<const Core::SingleDose*, DoseResult>& _doseResults) const
{
    checkDoses(*_dosageLoop.getDosage(), _modelFormulationAndRoutes, _doseResults);
}

void DoseValidator::checkDoses(const Core::DosageRepeat& _dosageRepeat,
                               const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                               map<const Core::SingleDose*, DoseResult>& _doseResults) const
{
    checkDoses(*_dosageRepeat.getDosage(), _modelFormulationAndRoutes, _doseResults);
}

void DoseValidator::checkDoses(const Core::DosageSequence& _dosageSequence,
                               const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                               map<const Core::SingleDose*, DoseResult>& _doseResults) const
{
    checkDosageBoundedList(_dosageSequence.getDosageList(), _modelFormulationAndRoutes, _doseResults);
}

void DoseValidator::checkDoses(const Core::ParallelDosageSequence& _parallelDosageSequence,
                               const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                               map<const Core::SingleDose*, DoseResult>& _doseResults) const
{
    checkDosageBoundedList(_parallelDosageSequence.getDosageList(), _modelFormulationAndRoutes, _doseResults);
}

void DoseValidator::checkDosageBoundedList(const Core::DosageBoundedList& _dosageBoundedList,
                                           const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                                           map<const Core::SingleDose*, DoseResult>& _doseResults) const
{
    for (const std::unique_ptr<Tucuxi::Core::DosageBounded>& dosage : _dosageBoundedList) {
        checkDoses(*dosage, _modelFormulationAndRoutes, _doseResults);
    }
}

void DoseValidator::checkDoses(const Core::SingleDose& _singleDose,
                               const Core::FormulationAndRoutes& _modelFormulationAndRoutes,
                               map<const Core::SingleDose*, DoseResult>& _doseResults) const
{
    // Get drug model dosage that correspond to the formulation and route.
    const std::vector<std::unique_ptr<Core::FullFormulationAndRoute>>& ffrList = _modelFormulationAndRoutes.getList();
    const Core::FullFormulationAndRoute* matchingFr = nullptr;

    const Core::FormulationAndRoute dosageFr = _singleDose.getLastFormulationAndRoute();

    auto it = find_if(ffrList.begin(), ffrList.end(), [&dosageFr](const std::unique_ptr<Core::FullFormulationAndRoute>& ffr){
        return dosageFr == ffr->getFormulationAndRoute();
    });

    // Converting and comparing.
    if (it != ffrList.end()) {
        matchingFr = it.base()->get();

        double value = Common::UnitManager::convertToUnit(_singleDose.getDose(), _singleDose.getDoseUnit(), matchingFr->getValidDoses()->getUnit());

        // Checking if in adviced limits and sets warning.
        string warning = "";
        if(value < matchingFr->getValidDoses()->getFromValue()) {
            // (for example in english) : warning = Minimum recommended dosage reached (1 mg/l)
            warning = XpertLanguage::LanguageManager::getInstance().translate("minimum_dosage_warning") +
                    " (" + XpertUtils::varToString(matchingFr->getValidDoses()->getFromValue()) + " " +  matchingFr->getValidDoses()->getUnit().toString() + ")";
        } else if (value > matchingFr->getValidDoses()->getToValue()) {
            // (for example in english) : warning = Maximum recommended dosage reached (1 mg/l)
            warning = XpertLanguage::LanguageManager::getInstance().translate("maximum_dosage_warning") +
                    " (" + XpertUtils::varToString(matchingFr->getValidDoses()->getToValue()) + " " +  matchingFr->getValidDoses()->getUnit().toString() + ")";
        }

        _doseResults.emplace(make_pair(&_singleDose, DoseResult(&_singleDose, warning)));

    } else {
        throw invalid_argument("no corresponding full formulation and route found for a dosage.");
    }

}

} // namespace XpertResult
} // namespace Tucuxi
