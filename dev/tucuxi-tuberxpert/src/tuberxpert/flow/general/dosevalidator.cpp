#include "dosevalidator.h"

#include <memory>

#include "tucucommon/unit.h"

#include "tuberxpert/utils/xpertutils.h"
#include "tuberxpert/language/languagemanager.h"
#include "tuberxpert/result/dosevalidationresult.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

void DoseValidator::perform(XpertRequestResult& _xpertRequestResult)
{
    // Check if there is a treatment.
    if (_xpertRequestResult.getTreatment() == nullptr) {
        _xpertRequestResult.setErrorMessage("No treatment set.");
        return;
    }

    // Check if there is drug model
    if (_xpertRequestResult.getDrugModel() == nullptr) {
        _xpertRequestResult.setErrorMessage("No drug model set.");
        return;
    }

    const Core::DosageHistory& dosageHistory = _xpertRequestResult.getTreatment()->getDosageHistory();
    const Core::FormulationAndRoutes& modelFormulationAndRoutes = _xpertRequestResult.getDrugModel()->getFormulationAndRoutes();

    // Explore the dosage history to validate doses.
    try {
        map<const Core::SingleDose *, DoseValidationResult> results;
        checkDoses(dosageHistory, modelFormulationAndRoutes, results);
        _xpertRequestResult.setDoseResults(move(results));
    } catch (invalid_argument& e) {
        _xpertRequestResult.setErrorMessage("Patient dosage error found, details: " + string(e.what()));
    }
}

void DoseValidator::checkDoses(const Core::DosageHistory& _dosageHistory,
                               const Core::FormulationAndRoutes& _modelFormulationsAndRoutes,
                               map<const Core::SingleDose *, DoseValidationResult>& _results) const
{

    // For each dosage time range.
    for(const unique_ptr<Core::DosageTimeRange>& timeRange : _dosageHistory.getDosageTimeRanges()){
        checkDoses(*timeRange, _modelFormulationsAndRoutes, _results);
    }

}

void DoseValidator::checkDoses(const Core::DosageTimeRange& _timeRange,
                               const Core::FormulationAndRoutes& _modelFormulationsAndRoutes,
                               map<const Core::SingleDose*, DoseValidationResult>& _results) const
{
    checkDoses(*_timeRange.getDosage(), _modelFormulationsAndRoutes, _results);
}

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TRY_CHECK(Type)                                                                                    \
    if (dynamic_cast<const Tucuxi::Core::Type*>(&_dosage)) {                                               \
    checkDoses(*dynamic_cast<const Tucuxi::Core::Type*>(&_dosage), _modelFormulationsAndRoutes, _results); \
}

void DoseValidator::checkDoses(const Core::Dosage& _dosage,
                               const Core::FormulationAndRoutes& _modelFormulationsAndRoutes,
                               std::map<const Core::SingleDose*, DoseValidationResult>& _results) const
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
                               const Core::FormulationAndRoutes& _modelFormulationsAndRoutes,
                               map<const Core::SingleDose*, DoseValidationResult>& _results) const
{
    checkDoses(*_dosageLoop.getDosage(), _modelFormulationsAndRoutes, _results);
}

void DoseValidator::checkDoses(const Core::DosageRepeat& _dosageRepeat,
                               const Core::FormulationAndRoutes& _modelFormulationsAndRoutes,
                               map<const Core::SingleDose*, DoseValidationResult>& _results) const
{
    checkDoses(*_dosageRepeat.getDosage(), _modelFormulationsAndRoutes, _results);
}

void DoseValidator::checkDoses(const Core::DosageSequence& _dosageSequence,
                               const Core::FormulationAndRoutes& _modelFormulationsAndRoutes,
                               map<const Core::SingleDose*, DoseValidationResult>& _results) const
{
    checkDosageBoundedList(_dosageSequence.getDosageList(), _modelFormulationsAndRoutes, _results);
}

void DoseValidator::checkDoses(const Core::ParallelDosageSequence& _parallelDosageSequence,
                               const Core::FormulationAndRoutes& _modelFormulationsAndRoutes,
                               map<const Core::SingleDose*, DoseValidationResult>& _results) const
{
    checkDosageBoundedList(_parallelDosageSequence.getDosageList(), _modelFormulationsAndRoutes, _results);
}

void DoseValidator::checkDosageBoundedList(const Core::DosageBoundedList& _dosageBoundedList,
                                           const Core::FormulationAndRoutes& _modelFormulationsAndRoutes,
                                           map<const Core::SingleDose*, DoseValidationResult>& _results) const
{
    // For each dosage.
    for (const std::unique_ptr<Tucuxi::Core::DosageBounded>& dosage : _dosageBoundedList) {
        checkDoses(*dosage, _modelFormulationsAndRoutes, _results);
    }
}

void DoseValidator::checkDoses(const Core::SingleDose& _singleDose,
                               const Core::FormulationAndRoutes& _modelFormulationsAndRoutes,
                               map<const Core::SingleDose*, DoseValidationResult>& _results) const
{
    // Get the formulation and route from the model that is equal to the
    // single dose formulation and route.
    const Core::FormulationAndRoute dosageFr = _singleDose.getLastFormulationAndRoute();

    auto compatibleFormulationAndRouteIt = find_if(_modelFormulationsAndRoutes.getList().begin(),
                                                   _modelFormulationsAndRoutes.getList().end(),
                                                   [&dosageFr](const std::unique_ptr<Core::FullFormulationAndRoute>& ffr){
        return dosageFr == ffr->getFormulationAndRoute();
    });

    // Converting and comparing.
    if (compatibleFormulationAndRouteIt !=  _modelFormulationsAndRoutes.getList().end()) {
        const Core::FullFormulationAndRoute* ModelFormAndRoute = compatibleFormulationAndRouteIt.base()->get();

        // Convert the single dose value to match model formulation and route unit.
        double value = Common::UnitManager::convertToUnit(_singleDose.getDose(), _singleDose.getDoseUnit(), ModelFormAndRoute->getValidDoses()->getUnit());

        // Checking if limits are respected.
        string warning = "";

        // Too low.
        if(value < ModelFormAndRoute->getValidDoses()->getFromValue()) {

            // Set the warning message (for example in english) : warning = Minimum recommended dosage reached (1 mg/l)
            warning = LanguageManager::getInstance().translate("minimum_dosage_warning") +
                    " (" + doubleToString(ModelFormAndRoute->getValidDoses()->getFromValue()) + " " +  ModelFormAndRoute->getValidDoses()->getUnit().toString() + ")";

        // Too high.
        } else if (value > ModelFormAndRoute->getValidDoses()->getToValue()) {

            // Set the warning message (for example in english) : warning = Maximum recommended dosage reached (1 mg/l)
            warning = LanguageManager::getInstance().translate("maximum_dosage_warning") +
                    " (" + doubleToString(ModelFormAndRoute->getValidDoses()->getToValue()) + " " +  ModelFormAndRoute->getValidDoses()->getUnit().toString() + ")";
        }

        _results.emplace(make_pair(&_singleDose, DoseValidationResult(&_singleDose, warning)));

    } else {
        throw invalid_argument("No corresponding full formulation and route found for a dosage.");
    }

}

} // namespace Xpert
} // namespace Tucuxi
