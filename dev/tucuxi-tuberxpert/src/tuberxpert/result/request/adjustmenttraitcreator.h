#ifndef ADJUSTMENTTRAITCREATOR_H
#define ADJUSTMENTTRAITCREATOR_H

#include "tucucore/drugmodel/formulationandroute.h"
#include "tucucore/computingservice/computingtrait.h"
#include "tucucore/drugtreatment/drugtreatment.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucommon/datetime.h"
#include "tucucore/dosage.h"

#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/query/xpertrequestdata.h"

namespace Tucuxi {
namespace XpertResult {

class AdjustmentTraitCreator
{
public:
    AdjustmentTraitCreator(Common::DateTime _computationDate = Common::DateTime::now());

    void createAdjustmentTrait(XpertRequestResult& _xpertRequestResult) const;

protected:

    Core::PredictionParameterType getPredictionParamterType(const std::unique_ptr<Core::DrugTreatment>& _drugTreatment) const;

    Common::DateTime getOldestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory) const;

    // Smaller than execution time
    Common::DateTime getLatestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory) const;

    Common::DateTime getAdjustmentTime(const XpertQuery::XpertRequestData& _request,
                                       const std::unique_ptr<Core::DrugTreatment>& _drugTreatment,
                                       const Core::DrugModel* _drugModel) const;

    Common::DateTime makeIntakeSeriesAndTryToExtractAdjustmentTime(const std::unique_ptr<Core::DrugTreatment>& _drugTreatment,
                                                                   const Core::DrugModel* _drugModel) const;

    Common::DateTime getTimeOfNearestFutureOrLatestIntake(Core::IntakeSeries _intakes) const;

    // using xpertRequestResult for accessing dosage history and request.
    //
    void getPeriod(const Core::FullFormulationAndRoute* _fullFormulationAndRoute,
                   const XpertRequestResult& _xpertRequestResult,
                   const Common::DateTime& _adjustmentTime,
                   Common::DateTime& _start,
                   Common::DateTime& _end) const;

    Core::LoadingOption getLoadingOption(const XpertQuery::XpertRequestData& _request,
                                         const Core::FullFormulationAndRoute* _fullFormulationAndRoute) const;

    Core::RestPeriodOption getRestPeriodOption(const XpertQuery::XpertRequestData& _request,
                                               const Core::FullFormulationAndRoute* _fullFormulationAndRoute) const;

    Core::SteadyStateTargetOption getSteadyStateTargetOption(const Core::FullFormulationAndRoute* _fullFormulationAndRoute) const;

protected:

    /// \brief Fixes the computation date to get the same values when testing
    ///        at different times.
    Common::DateTime m_computationDate;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // ADJUSTMENTTRAITCREATOR_H
