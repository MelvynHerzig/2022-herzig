#ifndef ADJUSTMENTTRAITCREATOR_H
#define ADJUSTMENTTRAITCREATOR_H

#include "tucucore/drugmodel/formulationandroute.h"
#include "tucucore/computingservice/computingtrait.h"
#include "tucucore/drugtreatment/drugtreatment.h"
#include "tucucore/drugmodel/drugmodel.h"
#include "tucucommon/datetime.h"
#include "tucucore/dosage.h"

#include "tuberxpert/flow/abstract/abstractxpertflowstep.h"
#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/query/xpertrequestdata.h"

namespace Tucuxi {
namespace XpertFlow {

/// \brief This class is responsible to create the adjustment trait the will be submitted
///        to tucuxi core in a computing request. It uses the whole dosage history.
/// \date 20/06/2022
/// \author Herzig Melvyn
class AdjustmentTraitCreator : public XpertFlow::AbstractXpertFlowStep
{
public:

    /// \brief Constructor.
    /// \param _computationTime This attribute is used for testing purpose. It specifies "when is
    ///        the class executed". For example, it allows to get the start/end time when executed
    ///        at different times.
    AdjustmentTraitCreator(Common::DateTime _computationTime = Common::DateTime::now());

    /// \brief Create the adjustment trait based on the information in the xpertRequestResult.
    /// \param _xpertRequestResult XpertRequestResult containing all the XpertRequest, the treatment,
    ///        the drug model and that will recieve the trait when created.
    void perform(XpertResult::XpertRequestResult& _xpertRequestResult) const;

protected:

    /// \brief Extract the prediction parameter type based on the treatment.
    /// \param _drugTreatment Drug treatment used to extract the parameter type.
    /// \return Aposteriori if there are dosages and samples in the treatment otherwise Apriori.
    Core::PredictionParameterType getPredictionParameterType(const std::unique_ptr<Core::DrugTreatment>& _drugTreatment) const;

    /// \brief Extract the dosage time range starting time that is the oldest of the dosage history.
    /// \param _dosageHistory Dosage history to extract the oldest time range starting time.
    /// \return Return the oldest starting time found or the computation date if the dosage history is empty.
    Common::DateTime getOldestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory) const;

    /// \brief Extract the latest (the last in the past / youngest) dosage time range start.
    /// \param _dosageHistory Dosage history to extract the latest time range starting time.
    /// \return Return the latest starting time found or an undefined date if the dosage history is empty or in the future.
    Common::DateTime getLatestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory) const;

    /// \brief Get the adjustment time.
    ///        - If the adjustment time is set in the XpertRequest, it returns it.
    ///        - Otherwise:
    ///            - If there is an ongoing treatment, it returns the next intake time.
    ///            - If there is an over treatment, its adds 2*half life of the drug until the computation
    ///              time is reached. The resulting time is returned.
    ///            - If there is no treatment, it returns the computing time plus 1 hour.
    /// \param _request XpertRequestData that may old an adjustment time set by the user.
    /// \param _drugTreatment Treatment followed by the patient.
    /// \param _drugModel Drug model associated to the treatment to get the half life.
    /// \return Returns the adjustment time extracted.
    Common::DateTime getAdjustmentTime(const XpertQuery::XpertRequestData& _request,
                                       const std::unique_ptr<Core::DrugTreatment>& _drugTreatment,
                                       const Core::DrugModel* _drugModel) const;

    /// \brief Try to extract the intake series from the first intake until computation time in order to extract
    ///        the adjustment time out of it. This function is called by "getAdjustmentTime".
    /// \param _drugTreatment Drug treatment to extract intakes.
    /// \param _drugModel Drug model associated to the treatment.
    /// \return Returns the adjustment time if found one otherwise an undefined time.
    Common::DateTime makeIntakeSeriesAndTryToExtractAdjustmentTime(const std::unique_ptr<Core::DrugTreatment>& _drugTreatment,
                                                                   const Core::DrugModel* _drugModel) const;

    /// \brief Given an intake series, try to extract the closest intake in the future. If none is found,
    ///        extract the closed in the past. By closest, we mean "the closest to the computation time".
    /// \param _intakes Intake series to work with.
    /// \return Returns the time found otherwise an undefined time if the series is empty.
    Common::DateTime getTimeOfNearestFutureOrLatestIntake(Core::IntakeSeries _intakes) const;

    /// \brief Extract the starting and ending time of the adjustment.
    ///        If the treatment is not related to a standard treatment:
    ///            start = adjustmentTime - 1 hour
    ///            end = start + 7 days
    ///        else, if the treatment is related to a standard treatment:
    ///            start = start time of the oldest time range of the dosage history or
    ///                    computation time of the dosage history is empty.
    ///            end = start + standard treatment duration.
    /// \param _fullFormulationAndRoute Full formulation and route associated to the treatment.
    /// \param _xpertRequestResult XpertRequestResult used to get the dosage history and the XpertRequest.
    /// \param _adjustmentTime Estimated adjustment time.
    /// \param _start Resulting start time.
    /// \param _end Resulting end time.
    void getPeriod(const Core::FullFormulationAndRoute* _fullFormulationAndRoute,
                   const XpertResult::XpertRequestResult& _xpertRequestResult,
                   const Common::DateTime& _adjustmentTime,
                   Common::DateTime& _start,
                   Common::DateTime& _end) const;

    /// \brief Retrieve the loading option of the XpertRequest or from the full
    ///        formulation and route if not set in request.
    /// \param _request Request to look in.
    /// \param _fullFormulationAndRoute Full formulation and route from which use "isLoadingDoseRecommended"
    ///        if the XpertRequest does not specify a choice.
    /// \return Return the resulting loading option.
    Core::LoadingOption getLoadingOption(const XpertQuery::XpertRequestData& _request,
                                         const Core::FullFormulationAndRoute* _fullFormulationAndRoute) const;

    /// \brief Retrieve the rest period option of the XpertRequest or from the full
    ///        formulation and route if not set in request.
    /// \param _request Request to look in.
    /// \param _fullFormulationAndRoute Full formulation and route from which use "isRestPeriodRecommended"
    ///        if the XpertRequest does not specify a choice.
    /// \return Return the resulting rest period option.
    Core::RestPeriodOption getRestPeriodOption(const XpertQuery::XpertRequestData& _request,
                                               const Core::FullFormulationAndRoute* _fullFormulationAndRoute) const;

    /// \brief Defines the steady state target option to use.
    ///        If the full formulation and route defines a standard treatment:
    ///             within treatment time range.
    ///        Else:
    ///             at steady state.
    /// \param _fullFormulationAndRoute Full formulation and route from the drug model used with
    ///        the drug treatment.
    /// \return Return the corresponding steady state target option.
    Core::SteadyStateTargetOption getSteadyStateTargetOption(const Core::FullFormulationAndRoute* _fullFormulationAndRoute) const;

protected:

    /// \brief Fix the computation time to get the same values when testing
    ///        at different times.
    Common::DateTime m_computationTime;
};

} // namespace XpertFlow
} // namespace Tucuxi

#endif // ADJUSTMENTTRAITCREATOR_H
