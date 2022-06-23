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
namespace Xpert {

/// \brief This class is responsible to create the adjustment trait the will be submitted
///        to tucuxi core in a computing request. It uses the whole dosage history.
/// \date 20/06/2022
/// \author Herzig Melvyn
class AdjustmentTraitCreator : public AbstractXpertFlowStep
{
public:

    /// \brief Constructor.
    AdjustmentTraitCreator();

    /// \brief Create the adjustment trait based on the information in the xpertRequestResult.
    /// \param _xpertRequestResult XpertRequestResult containing the XpertRequest, the treatment,
    ///        the drug model and that will recieve the trait when created.
    void perform(XpertRequestResult& _xpertRequestResult);

protected:

    /// \brief Extract the prediction parameter type based on the treatment.
    /// \param _drugTreatment Drug treatment used to extract the parameter type.
    /// \return Aposteriori if there are dosages and samples in the treatment otherwise Apriori.
    Core::PredictionParameterType getPredictionParameterType(const std::unique_ptr<Core::DrugTreatment>& _drugTreatment) const;

    /// \brief Get the adjustment time. Additionaly, set the last
    ///        intake of the patient in the XpertRequestResult if there is one.
    ///        - If the adjustment time is set in the XpertRequest, it returns it.
    ///        - Otherwise:
    ///            - If there is an ongoing treatment, it returns the next intake time.
    ///            - If there is an over treatment, its adds 2*half life of the drug until the computation
    ///              time is reached. The resulting time is returned.
    ///            - If there is no treatment, it returns the computing time plus 1 hour.
    /// \param _xpertRequestResult XpertRequestResult containing the treatment and the drug model to extract intakes
    ///        and the request.
    /// \param _fullFormulationAndRoute Full formulation and route associated to the treatment.
    /// \return Returns the adjustment time extracted.
    Common::DateTime getAdjustmentTimeAndLastIntake(XpertRequestResult& _xpertRequestResult, const Core::FullFormulationAndRoute* _fullFormulationAndRoute) const;

    /// \brief Apprixmate the adjustment time from the given intake series.
    ///        This function is called by "getAdjustmentTimeAndLastIntake".
    /// \param _xpertRequestResult XpertRequestResult to use drug moodel in order to get the half life.
    /// \param _intakes Intake series used for approximation.
    /// \return Returns the adjustment time if found one otherwise an undefined time.
    Common::DateTime approximateAdjustmentTimeFromIntakes(XpertRequestResult& _xpertRequestResult, Core::IntakeSeries _intakes) const;

    /// \brief Given an intake series, try to extract the closest intake in the future. If none is found,
    ///        extract the closed in the past. By closest, we mean "the closest to the computation time".
    /// \param _intakes Intake series to work with.
    /// \return Returns the time found otherwise an undefined time if the series is empty.
    Common::DateTime getTimeOfNearestFutureOrLatestIntake(Core::IntakeSeries _intakes) const;

    /// \brief Get a pointer on the last intake of the intake series that is before the computation time.
    /// \param _intakes Intakes series to parse.
    /// \param _lastIntake Unique pointer to store the result.
    void getLatestIntake(Core::IntakeSeries _intakes, std::unique_ptr<Core::IntakeEvent>& _lastIntake) const;

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
                   const XpertRequestResult& _xpertRequestResult,
                   const Common::DateTime& _adjustmentTime,
                   Common::DateTime& _start,
                   Common::DateTime& _end) const;

    /// \brief Retrieve the loading option of the XpertRequest or from the full
    ///        formulation and route if not set in request.
    /// \param _request Request to look in.
    /// \param _fullFormulationAndRoute Full formulation and route from which use "isLoadingDoseRecommended"
    ///        if the XpertRequest does not specify a choice.
    /// \return Return the resulting loading option.
    Core::LoadingOption getLoadingOption(const XpertRequestData& _request,
                                         const Core::FullFormulationAndRoute* _fullFormulationAndRoute) const;

    /// \brief Retrieve the rest period option of the XpertRequest or from the full
    ///        formulation and route if not set in request.
    /// \param _request Request to look in.
    /// \param _fullFormulationAndRoute Full formulation and route from which use "isRestPeriodRecommended"
    ///        if the XpertRequest does not specify a choice.
    /// \return Return the resulting rest period option.
    Core::RestPeriodOption getRestPeriodOption(const XpertRequestData& _request,
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
    ///        at different times. Retrieved from the xpertRequestResult from perform.
    Common::DateTime m_computationTime;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // ADJUSTMENTTRAITCREATOR_H
