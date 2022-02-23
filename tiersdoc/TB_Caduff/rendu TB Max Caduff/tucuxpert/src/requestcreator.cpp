#include "requestcreator.h"
#include "tucuquery/querytocoreextractor.h"
#include "tucucore/computingservice/computingrequest.h"
#include "tucucore/intakeextractor.h"
#include "tucucommon/loggerhelper.h"
#include <cmath>


//#include "tools.h" // for debug (date printing)

namespace Tucuxi {
namespace Xpert {



RequestCreator::RequestCreator()
{

}


int RequestCreator::createRequests (XpertData& _xpertData,
                                    Query::QueryData*& _pAvailableData ) {


    Tucuxi::Common::LoggerHelper logHelper;


    // at least 1 drug must have been provided
    const std::vector <std::unique_ptr<Query::DrugData>>& drugs = _pAvailableData->getpParameters().getDrugs();
    if (drugs.size() < 1 ) {
        // TODO set better error code
        logHelper.error("No drug provided, aborting.");
        return EXIT_FAILURE;
    }

    // assumption is made for now that only one drug is present, can be extended with a loop.
    Query::DrugData* drug = drugs[0].get();

    // selecting best DrugModel
    std::string drugID = drug->getDrugID ();

    std::string wtf = ""; // "no known conversion from 'const char [1]' to 'std::string &' " -> LOL
    Query::RequestData dummyReq (wtf, drugID, wtf, nullptr);
    _xpertData.setDrugTreatment (Query::QueryToCoreExtractor().extractDrugTreatment( *_pAvailableData, dummyReq));
    if (!_xpertData.getDrugTreatment()) {
        logHelper.error("Drug treatment could not be extracted, aborting.");
        return EXIT_FAILURE;
    }

    DrugModelResult drugModelResult;
    if ( DrugModelSelector().getBestDrugModel(drugModelResult, drug->getDrugID(), _xpertData.getDrugTreatment()) == EXIT_FAILURE) {
        logHelper.error("No drug model could be selected.");
        return EXIT_FAILURE;
    }
    _xpertData.setDrugModelResult(drugModelResult);

    // setting flags to determine the state
    bool treatmentProvided = ! drug->getpTreatment().getpDosageHistory().isEmpty();
    bool samplesProvided = drug->getSamples().size() > 0;
    bool targetsProvided = drug->getTargets().size() > 0;
    bool covarProvided = _pAvailableData->getpParameters().getpPatient().getCovariates().size() != 0;

    // having samples without any treatment makes no sense
    if (samplesProvided && !treatmentProvided) {
        logHelper.error ("Samples were provided without a drug treatment.");
        return EXIT_FAILURE;
    }


    // creating appropriate requests based on the flags set

    // adjustmentTrait, always asked:

    Core::RequestResponseId adjustId = "adjustment";
    const Common::DateTime now = DateTime();
    Common::DateTime predBegin = now ;
    Common::DateTime predEnd = now;
    predBegin.addDays (-1);


    //logHelper.debug( "now: "+ Tools::printDateTime(now));



    double nbPointsPerHour = 20;

    _xpertData.setPredictionType( !covarProvided && !samplesProvided ? Core::PredictionParameterType::Population :
                                   covarProvided && !samplesProvided ? Core::PredictionParameterType::Apriori :
                                                                       Core::PredictionParameterType::Aposteriori);
    Core::ComputingOption computingOption ( _xpertData.getPredictionType(),
                                            Core::CompartmentsOption::AllActiveMoieties,
                                            Core::RetrieveStatisticsOption::RetrieveStatistics,
                                            Core::RetrieveParametersOption::RetrieveParameters,
                                            Core::RetrieveCovariatesOption::RetrieveCovariates);

    Common::DateTime adjustmentTime = now;
    // rounding to next hour, or next+1 if less than 30 min left in the current hour.
    // <=> the adjustment is asked between 30 and 90 min after the current time by default.
    // TODO: the "current time" is the UTC one, need to get the local time instead.
    adjustmentTime -= Duration(std::chrono::seconds(adjustmentTime.second()));
    adjustmentTime += Duration(std::chrono::minutes((adjustmentTime.minute() > 30 ? 120 : 60) - adjustmentTime.minute() ));

    // adjustmentTime set at next intake if a treatment is provided and the following intake is found in the next 10 days.
    if (treatmentProvided) {
        Core::IntakeExtractor intakeExtractor;
        Core::IntakeSeries intakes;
        Common::DateTime intakesEnd = now;
        intakesEnd.addDays (10);
        intakeExtractor.extract (drug->getpTreatment().getpDosageHistory(), now, intakesEnd, 1., intakes );
        for (const Core::IntakeEvent intake : intakes) {
            if (intake.getEventTime() > now) {
                adjustmentTime = intake.getEventTime();
                break;
            }
        }
        // if samples are provided, the prediction start time is set to the oldest sample in the last 2 days,
        // the core will automatically calculate the prediction from an interval start.
        if (samplesProvided) {
            DateTime maxSampleAge = now;
            maxSampleAge.addDays(-2);
            for (const std::unique_ptr<Tucuxi::Core::Sample>& sample : drug->getSamples() ) {
                if ( sample->getDate() < predBegin && sample->getDate() > maxSampleAge ) {
                    predBegin = sample->getDate();
                }
            }
        }
        // prediction ends at the precise time asked, so this finds the end of an interval superior to 24h after
        // the adjustmentTime, which is an interval start. If adjustmentTime has its default value, then intakes is
        // most probably empty and this will default to 24h after the adjustmentTime value.
        predEnd = adjustmentTime + Duration( std::chrono::hours ( intakes.size() ?
                     int(ceil (24. / intakes[0].getInterval().toHours()) * intakes[0].getInterval().toHours()) : 24));
    }
    Common::DateTime adjustEnd = adjustmentTime;
    adjustEnd.addDays (7);


    Core::BestCandidatesOption candidatesOption = Core::BestCandidatesOption::BestDosagePerInterval;
    Core::LoadingOption loadingOption = Core::LoadingOption::LoadingDoseAllowed;
    Core::RestPeriodOption restPeriodOption = Core::RestPeriodOption::RestPeriodAllowed;


    bool cumulativeAucInTargets = false;
    for (const std::unique_ptr<Core::Target>& target : drug->getTargets()) {
        if (target->getTargetType() == Core::TargetType::CumulativeAuc) {
            cumulativeAucInTargets = true;
            break;
        }
    }
    Core::SteadyStateTargetOption steadyStateTargetOption = cumulativeAucInTargets?
                Core::SteadyStateTargetOption::WithinTreatmentTimeRange :
                Core::SteadyStateTargetOption::AtSteadyState;
    Core::TargetExtractionOption targetExtractionOption = targetsProvided ? Core::TargetExtractionOption::IndividualTargets :
                                                                            Core::TargetExtractionOption::PopulationValues;
    Core::FormulationAndRouteSelectionOption formulationAndRouteSelectionOption =
                        Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute;

    _xpertData.getQueries().addComputingRequest (std::make_unique<Core::ComputingRequest>(
                                                adjustId,
                                                *_xpertData.getDrugModelResult().drugModel,
                                                *_xpertData.getDrugTreatment(),
                                                std::make_unique<Core::ComputingTraitAdjustment>(
                                                     adjustId,
                                                     adjustmentTime,
                                                     adjustEnd,
                                                     nbPointsPerHour,
                                                     computingOption,
                                                     adjustmentTime,
                                                     candidatesOption,
                                                     loadingOption,
                                                     restPeriodOption,
                                                     steadyStateTargetOption,
                                                     targetExtractionOption,
                                                     formulationAndRouteSelectionOption)));


    //logHelper.debug( "adjust asked from: "+ Tools::printDateTime(adjustmentTime) + " to: "+Tools::printDateTime(adjustEnd));

    if (treatmentProvided) {

        // concentration prediction:

        Core::RequestResponseId concentrationId = "concentration";

        _xpertData.getQueries().addComputingRequest (std::make_unique<Core::ComputingRequest >(
                                 concentrationId,
                                 *_xpertData.getDrugModelResult().drugModel,
                                 *_xpertData.getDrugTreatment(),
                                 std::make_unique<Core::ComputingTraitConcentration> (concentrationId,
                                                                                      predBegin,
                                                                                      predEnd,
                                                                                      nbPointsPerHour,
                                                                                      computingOption)));


        // percentiles:

        Core::RequestResponseId percentilesId = "percentiles";
        Core::PercentileRanks ranks = {5,25,75,95};


        _xpertData.getQueries().addComputingRequest (std::make_unique<Core::ComputingRequest >(
                                 percentilesId,
                                 *_xpertData.getDrugModelResult().drugModel,
                                 *_xpertData.getDrugTreatment(),
                                 std::make_unique<Core::ComputingTraitPercentiles> (
                                    percentilesId,
                                    predBegin,
                                    predEnd,
                                    ranks,
                                    nbPointsPerHour,
                                    computingOption)));

        //logHelper.debug( "pred & perc asked from: "+ Tools::printDateTime(predBegin) + " to: "+Tools::printDateTime(predEnd));


        // prediction at measure

        if (samplesProvided) {
            Core::RequestResponseId measureId = "measure";
            _xpertData.getQueries().addComputingRequest (std::make_unique<Core::ComputingRequest >(
                                     measureId,
                                     *_xpertData.getDrugModelResult().drugModel,
                                     *_xpertData.getDrugTreatment(),
                                     std::make_unique<Core::ComputingTraitAtMeasures>(measureId, computingOption)));
        }

    }



    return EXIT_SUCCESS;

}

}
}
