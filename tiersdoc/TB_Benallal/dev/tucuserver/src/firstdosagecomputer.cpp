#include "firstdosagecomputer.h"

using namespace std;
using namespace Tucuxi::Core;

namespace Tucuxi {
namespace Server {

FirstDosageComputer::FirstDosageComputer(const Query& _query)
    : Computer (_query)
{}

void FirstDosageComputer::compute()
{
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
        m_result->addError("DrugImportError", m_query.getLanguage(), "Error while importing drug model.");
        return;
    }

    // Creating an empty drug treatment
    DrugTreatment drugTreatment;

    // Getting the patient's covariates for the drug treatment
    PatientVariates patientVariates = extractPatientVariates();
    for (unique_ptr<PatientCovariate>& pc : patientVariates) {
        drugTreatment.addCovariate(move(pc));
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
    Common::DateTime adjustmentTime(start);

    if ((end - start).toDays() > Configuration::getInstance()->getMaxDateInterval()) {
        // the request date interval is too big, we reduce the date interval.
        end = start + Common::Duration(chrono::hours(24) * Configuration::getInstance()->getMaxDateInterval());
    }

    string predictionType = m_query.getRequests().at(0)->getPredictionType();
    PredictionParameterType predictionParamaterType = PredictionParameterType::Population;
    if (predictionType == "population") {
        predictionParamaterType = PredictionParameterType::Population;
    } else if (predictionType == "a priori") {
        predictionParamaterType = PredictionParameterType::Apriori;
    } else if (predictionType == "best") {
        bool covariatesExist = drugTreatment.getCovariates().empty() ? false : true;

        if (covariatesExist) {
            predictionParamaterType = PredictionParameterType::Apriori;
        } else {
            predictionParamaterType = PredictionParameterType::Population;
        }

    } else {
        // Generate some warning ?
        predictionParamaterType = PredictionParameterType::Population;
    }
    ComputingOption computingOption(predictionParamaterType, CompartmentsOption::MainCompartment);

    unique_ptr<ComputingTraitAdjustment> adjustmentsTraits =
                    make_unique<ComputingTraitAdjustment>(
                        requestResponseID, start, end, m_sCycleSize, computingOption, adjustmentTime,
                        BestCandidatesOption::AllDosages,
                        ChargingOption::NoChargingDose, RestPeriodOption::NoRestPeriod,
                        // SteadyStateTargetOption should be adapted when the drug model contains such option
                        SteadyStateTargetOption::WithinTreatmentTimeRange,
                        TargetExtractionOption::DefinitionIfNoIndividualTarget,
                        FormulationAndRouteSelectionOption::DefaultFormulationAndRoute);

    unique_ptr<ComputingTraits> computingTraits = make_unique<ComputingTraits>();
    computingTraits->addTrait(move(adjustmentsTraits));

    ComputingRequest request(requestResponseID, *drugModel, drugTreatment, move(computingTraits));

    unique_ptr<ComputingResponse> response = make_unique<ComputingResponse>(requestResponseID);

    ComputingResult result = component->compute(request, response);

    const vector<unique_ptr<SingleComputingResponse> > &responses = response.get()->getResponses();
    for(size_t i = 0; i < responses.size(); i++) {
        const AdjustmentResponse *resp = dynamic_cast<AdjustmentResponse*>(responses[i].get());

        for (FullDosage const & adj : resp->getAdjustments()) {
            double globalScore = adj.getGlobalScore();
            // TODO forge a real response
            //m_result = globalScore;
        }
    }

    // Delete all dynamically allocated objects
    delete drugModel;
    delete component;
}

} // namespace Server
} // namespace Tucuxi
