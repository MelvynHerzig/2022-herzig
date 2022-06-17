#ifndef ADJUSTMENTTRAITCREATOR_H
#define ADJUSTMENTTRAITCREATOR_H

#include "tucucore/drugmodel/formulationandroute.h"
#include "tucucore/computingservice/computingtrait.h"
#include "tucucore/drugtreatment/drugtreatment.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucommon/datetime.h"
#include "tucucore/dosage.h"

#include "../xpertrequestresult.h"
#include "../../query/xpertrequestdata.h"

namespace Tucuxi {
namespace XpertResult {

class AdjustmentTraitCreator
{
public:
    AdjustmentTraitCreator(Common::DateTime _computationDate = Common::DateTime::now());

    void createAdjustmentTrait(XpertRequestResult& _xpertRequestResult) const;

protected:

    Core::PredictionParameterType getPredictionParamterType(const std::unique_ptr<Core::DrugTreatment>& _drugTreatment) const;

    // using xpertRequestResult for accessing dosage history and request.
    //
    void getPeriod(const Core::FullFormulationAndRoute* _fullFormulationAndRoute,
                   const XpertRequestResult& _xpertRequestResult,
                   Common::DateTime& _start,
                   Common::DateTime& _end) const;


    Common::DateTime getOldestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory) const;

    Common::DateTime getAdjustmentDate(const XpertQuery::XpertRequestData& _request, const std::unique_ptr<Core::DrugTreatment>& _drugTreatment, const Core::DrugModel* _drugModel) const;

    // Smaller than execution time
    Common::DateTime getLatestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory) const;

    Common::DateTime getTimeOfNearestFutureOrLatestIntake(Core::IntakeSeries _intakes) const;

protected:

    /// \brief Fixes the computation date to get the same values when testing
    ///        at different times.
    Common::DateTime m_computationDate;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // ADJUSTMENTTRAITCREATOR_H
