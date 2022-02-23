#include "adjustmentcomputer.h"

using namespace std;
using namespace Tucuxi::Core;

namespace Tucuxi {
namespace Server {

AdjustmentComputer::AdjustmentComputer(const Query& _query)
    : Computer(_query)
{}

void AdjustmentComputer::compute()
{
    // Creating a response for the adjustments
    unique_ptr<ApiResponseAdjustment> apiResponse = make_unique<ApiResponseAdjustment>(m_query.getQueryID(), "123");

    // Getting the drug related to the request
    string drugID = m_query.getRequests().at(0)->getDrugID();
    const vector< unique_ptr<DrugData> >& drugs = m_query.getpParameters().getDrugs();
    size_t drugPosition = 0;
    for (; drugPosition < drugs.size(); ++drugPosition) {
        if (drugs.at(drugPosition)->getDrugID() == drugID) {
            break;
        }
    }

    IComputingService* component = dynamic_cast<IComputingService*>(ComputingComponent::createComponent());

    // Getting the drug model
    DrugModel* drugModel;
    DrugModelImport drugModelImport;
    DrugModelImport::Result importResult = drugModelImport.importFromFile(drugModel, "../tucuserver/drugs/ch.tucuxi.busulfan_children.xml");
    if (importResult != DrugModelImport::Result::Ok) {
        apiResponse->addError("DrugImportError", m_query.getLanguage(), "Error while importing drug model.");
        return;
    }

    // Getting the drug treatment
    DrugTreatment drugTreatment;

    // Getting the dosage history for the drug treatment
    const DosageTimeRangeList& dosageTimeRangeList = drugs.at(0)
                                                        ->getpTreatment()
                                                        .getpDosageHistory()
                                                        .getDosageTimeRanges();

    for (const unique_ptr<DosageTimeRange>& dosageTimeRange : dosageTimeRangeList) {
        //drugTreatment.addDosageTimeRange(make_unique<DosageTimeRange>(*dosageTimeRange));
        drugTreatment.getModifiableDosageHistory().addTimeRange(*dosageTimeRange);
    }

    // Getting the patient's covariates for the drug treatment
    PatientVariates patientVariates = extractPatientVariates();
    for (unique_ptr<PatientCovariate>& pc : patientVariates) {
        drugTreatment.addCovariate(move(pc));
    }

    // Getting the samples for the drug treatment
    Samples samples = extractSamples(drugPosition);
    for (unique_ptr<Sample>& sample : samples) {
        drugTreatment.addSample(move(sample));
    }

    // Getting the targets for the drug treatment
    Targets targets = extractTargets(drugPosition);
    for (unique_ptr<Target>& target : targets) {
        drugTreatment.addTarget(move(target));
    }

    // Creating the adjustment trait
    RequestResponseId requestResponseID = 1; //m_query.getRequests().at(0)->getRequestID();
    Common::DateTime start = m_query.getRequests().at(0)->getpDateInterval().getStart();
    Common::DateTime end = m_query.getRequests().at(0)->getpDateInterval().getEnd();
    Common::DateTime adjustmentTime("2018-07-10T08:00:00", "%Y-%m-%dT%H:%M:%S");
    //Common::DateTime adjustmentTime = computeAdjustmentTime(drugTreatment);

    if ((end - start).toDays() > Configuration::getInstance()->getMaxDateInterval()) {
        // the request date interval is too big, we reduce the date interval.
        end = start + Common::Duration(chrono::hours(24) * Configuration::getInstance()->getMaxDateInterval());
    }

    string predictionType = m_query.getRequests().at(0)->getPredictionType();
    PredictionParameterType predictionParameterType = PredictionParameterType::Population;
    if (predictionType == "population") {
        predictionParameterType = PredictionParameterType::Population;
    } else if (predictionType == "a priori") {
        predictionParameterType = PredictionParameterType::Apriori;
    } else if (predictionType == "a posteriori") {
        predictionParameterType = PredictionParameterType::Aposteriori;
    } else if (predictionType == "best") {
        bool covariatesExist = drugTreatment.getCovariates().empty() ? false : true;
        bool samplesExist = drugTreatment.getSamples().empty() ? false : true;

        if (covariatesExist && samplesExist) {
            predictionParameterType = PredictionParameterType::Aposteriori;
        } else if (covariatesExist && !samplesExist) {
            predictionParameterType = PredictionParameterType::Apriori;
        } else {
            predictionParameterType = PredictionParameterType::Population;
        }

    } else {
        apiResponse->addWarning("UnexpetedPredicitionType", m_query.getLanguage(), "Unexpected prediction type!");
        predictionParameterType = PredictionParameterType::Population;
    }
    ComputingOption computingOption(predictionParameterType, CompartmentsOption::MainCompartment);

    unique_ptr<ComputingTraitAdjustment> adjustmentsTraits =
                    make_unique<ComputingTraitAdjustment>(
                        requestResponseID, start, end, m_sCycleSize, computingOption, adjustmentTime,
                        BestCandidatesOption::AllDosages,
                        ChargingOption::NoChargingDose, RestPeriodOption::NoRestPeriod,
                        // SteadyStateTargetOption should be adapted when the drug model contains such option.
                        SteadyStateTargetOption::WithinTreatmentTimeRange,
                        TargetExtractionOption::DefinitionIfNoIndividualTarget,
                        // FormulationAndRouteSelectionOption is not yet supported in the xml query structure.
                        FormulationAndRouteSelectionOption::DefaultFormulationAndRoute);

    unique_ptr<ComputingTraits> computingTraits = make_unique<ComputingTraits>();
    computingTraits->addTrait(move(adjustmentsTraits));

    ComputingRequest request(requestResponseID, *drugModel, drugTreatment, move(computingTraits));

    unique_ptr<ComputingResponse> response = make_unique<ComputingResponse>(requestResponseID);

    ComputingResult result = component->compute(request, response);

    if (result != ComputingResult::Success) {
        apiResponse->addError("ComputingError", m_query.getLanguage(), "Error in computation!");
    }

    const vector< unique_ptr<SingleComputingResponse> >& responses = response.get()->getResponses();
    for(size_t i = 0; i < responses.size(); i++) {
        const AdjustmentResponse *resp = dynamic_cast<AdjustmentResponse*>(responses[i].get());

        size_t nbAdjustments = resp->getAdjustments().size();
        unsigned int maxAdjustments = Configuration::getInstance()->getMaxAdjustments();
        for (size_t j = 0; j < nbAdjustments && j < maxAdjustments; j++) {
            apiResponse->addAdjustment(resp->getAdjustments().at(j));
        }
    }

    // Affect the local ApiResponseAdjustment to the ApiResponse attribute
    m_result = move(apiResponse);

    // Delete all dynamically allocated objects
    delete drugModel;
    delete component;
}

Common::DateTime AdjustmentComputer::computeAdjustmentTime(const DrugTreatment& _drugTreatment) const
{
    size_t lastDosageTimeRangeIndex = 0;

    // Setting a date to the minimum and finding the most recent dosage with its start date.
    Common::DateTime lastStartDate(Duration(chrono::milliseconds(0)));
    const Core::DosageTimeRangeList& dosageTimeRangeList = _drugTreatment.getDosageHistory().getDosageTimeRanges();
    for (size_t i = 0; i < dosageTimeRangeList.size(); i++) {
        Common::DateTime start = dosageTimeRangeList.at(i)->getStartDate();
        if (start > lastStartDate) {
            lastStartDate = start;
            lastDosageTimeRangeIndex = i;
        }
    }

    // Computing the next dose intake date.
    const unique_ptr<DosageTimeRange>& dosageTimeRange = dosageTimeRangeList.at(lastDosageTimeRangeIndex);

    Core::IntakeExtractor intakeExtractor;
    Core::IntakeSeries intakeSeries;

    dosageTimeRange->getDosage()->extract(
                intakeExtractor,
                dosageTimeRange->getStartDate(),
                dosageTimeRange->getEndDate(),
                intakeSeries,
                Configuration::getInstance()->getCycleSize()
    );

    DateTime nextIntakeDate = dosageTimeRange->getStartDate();
    Duration meanInterval(chrono::milliseconds(0));
    for (IntakeEvent ie : intakeSeries) {
        meanInterval += ie.getInterval();
        nextIntakeDate += ie.getInterval();
    }

    meanInterval /= intakeSeries.size();
    nextIntakeDate += meanInterval;

    return nextIntakeDate;
}

} // namespace Server
} // namespace Tucuxi
