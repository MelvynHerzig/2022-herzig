#include "htmlexporter.h"
#include <fstream>
#include <QFile>
#include "graphcreator.h"
#include "tools.h"


namespace Tucuxi {
namespace Xpert {



HtmlExporter::HtmlExporter()
{

}

int HtmlExporter::exportToHtml(XpertData& _xpertData, std::string _outputPath)
{
    ILanguage* lang = ILanguage::getLang(_xpertData.getLang());

    std::string out = R"(<!DOCTYPE html><html><head><meta charset="utf-8"/><title>TucuXpert Results</title>
<style>
body { margin: 5vh 10vw;}
section {border-top: solid 1px black; padding: 5vh 2vw;}
table {margin: 3vh auto;}
td, th {padding: 1vh 2vw;}
img {max-width: 100%;}
.warning {color: red; border: solid 2px red; font-size: 1.2em; padding: 3vh 5vw; margin: 3vh 5vw;}
.dosage {margin: 1vh 10vw; border-top: solid 1px black; padding: 2vh 5vw;}
.zoom {max-width: none;}
.redBg {background-color: red;}

</style></head>
<body><h1> TucuXpert Results </h1>
<section>)";

    out += _xpertData.getQueries().m_queryId + "</section>\n";

    // TODO fill admin data

    // drug model infos
    out += "<section>\n<h2>" + lang->DRUG_MODEL + ":</h2>\n"
        "<b>" + lang->DRUG_USED + ":</b> " + _xpertData.getDrugModelResult().drugModel->getDrugId() + "<br />\n"
        "<b>" + lang->DRUG_MODEL_USED + ":</b> " + _xpertData.getDrugModelResult().drugModel->getDrugModelId() +
        "\n<h3>" + lang->CONSTRAINTS_FOR_THIS_MODEL + ":</h3>\n<table>\n";
    for (Core::DrugDomainConstraintsEvaluator::EvaluationResult cstr : _xpertData.getDrugModelResult().constraintsResults) {
        std::string result = cstr.m_result == Core::DrugDomainConstraintsEvaluator::Result::Compatible ? lang->COMPATIBLE :
                                                                                                         lang->PARTIALY_COMPATIBLE;
        out += "<tr><td>"+ cstr.m_constraint->getErrorMessage().getString() + "</td><td>"+ result + "</td></tr>\n";
    }
    const Core::SinglePredictionData* concPredData = Tools::getComputedDataWithId <Core::SinglePredictionData>
                                                        (_xpertData.getResponses().getRequestResponses(), "concentration");

    out += "</table>\n<h3>"+ lang->COVARIATES_DEFINED +":</h3>\n<table>\n<tr><th>"+lang->NAME+":</th><th>"+lang->DEFAULT_VALUE+
            ":</th><th>" + lang->PROVIDED_VALUE + ":</th>"+ (concPredData ? "<th>"+ lang->USED_VALUE + ": (" +
            Tools::printDateTime(concPredData->getData().back().m_start) +")</th>" : "") +"</tr>\n";

    for (const std::unique_ptr<Core::CovariateDefinition>& cov : _xpertData.getDrugModelResult().drugModel->getCovariates()) {

        // inserting values extracted from drugTreatment if present, coloring cell in red if not found.
        std::string covVal = "";
        std::string usedCovVal = "";
        bool found = false;

        for (const std::unique_ptr<Core::PatientCovariate>& patientCov : _xpertData.getDrugTreatment()->getCovariates()) {

            if ( ! patientCov->getId().compare(cov->getId()) ||
                 ( ! cov->getId().compare("age") && ! patientCov->getId().compare("birthdate") ) ||
                 ( ! cov->getId().compare("pna") && ! patientCov->getId().compare("birthdate") )) {
                if (!found) {
                    found = true;
                    covVal = patientCov->getValue() + " " + patientCov->getUnit().toString();
                }
                else {
                    covVal = lang->MULTIPLE_VALUES;
                    break;
                }
            }
        }
        if ( ! found && cov->isComputed() ) {
            found = true;
            covVal = lang->COMPUTED;
        }
        // showing used values if a concentration prediction has been made. they could be shown
        // also for adjustments but currently no covariates are set in the results, even if asked.
        if (concPredData) {
            for (const Core::CovariateValue& patientCov : concPredData->getData().back().m_covariates) {

                if ( ! patientCov.m_covariateId.compare(cov->getId()) ) {
                    usedCovVal = std::to_string(patientCov.m_value) + " " + cov->getUnit().toString();
                    break;
                }
            }
        }

        out += "<tr><td>"+ cov->getId() +"</td><td>"+ std::to_string(cov->getValue())+" "+cov->getUnit().toString() +
                "</td><td" + (found ? "" : " class=\"redBg\" ") + ">" + (found ? covVal : lang->MISSING) + "</td>"+
                (concPredData ? "<td>"+ usedCovVal +"</td>" : "") +"</tr>\n";
    }

    out += "</table>\n<p>"+lang->tellPredictionType(_xpertData.getPredictionType())+"</p>\n</section>\n";


    // create graphs
    GraphCreator graphCreator;
    graphCreator.createGraphs(_xpertData, lang);


    if (concPredData) {
        // displaying current dosage prediction if any
        QFile concPercFile("concPerc.png");
        concPercFile.open(QIODevice::ReadOnly);
        QByteArray concPercEncoded = concPercFile.readAll();
        concPercEncoded = concPercEncoded.toBase64();
        concPercFile.close();

        out += "<section>\n<h2>" + lang->CURRENT_DOSAGE_PREDICTION +
                ":</h2>\n<img src=\"data:image/png;base64, "+ concPercEncoded.toStdString()+"\" "
                "onclick=\"toggleZoom(this)\" />\n";

        const Core::TargetDefinition& target = *_xpertData.getDrugModelResult().drugModel->getActiveMoieties()[0]->getTargetDefinitions()[0];
        // current treatment check
        // Some drug files targets have incompatible units, like hours, so this check is only made for compatible units.
/*  The isCompatible() method creates a linker error, while others methods from unit are working well...
         if (Common::UnitManager::isCompatible(target.getUnit(), concPredData->getData()[0].m_unit )) {

            double lowerBound = Common::UnitManager::convertToUnit(target.getInefficacyAlarm().getValue(),
                                                                   target.getUnit(), concPredData->getData()[0].m_unit);
            double upperBound = Common::UnitManager::convertToUnit(target.getToxicityAlarm().getValue(),
                                                                   target.getUnit(), concPredData->getData()[0].m_unit);
            // checking each value against bounds
            for (const Core::CycleData& cycle : concPredData->getData()) {
                for (const Core::Concentration& val : cycle.m_concentrations[0]) {
                    if (val < lowerBound || val > upperBound) {
                        out += "<div class=\"warning\">"+lang->boundWarning()+"</div>\n";
                        goto endLoop;
                    }
                }
            }
        }
endLoop:
*/

        // samples check
        const Core::PercentilesData* percData = Tools::getComputedDataWithId <Core::PercentilesData>
                                                            (_xpertData.getResponses().getRequestResponses(), "percentiles");
        if (! percData) {
            out += "<div class=\"warning\">Error: concentration prediction found without percentiles. (Should never happen!)</div>\n";
        }
        else {
            for (const std::unique_ptr<Core::Sample>& sample : _xpertData.getDrugTreatment()->getSamples()) {
                out += checkSample (sample.get(), percData->getPercentileData(0), lang, false) +
                       checkSample (sample.get(), percData->getPercentileData(3), lang, true );
            }
        }
        out += "</section>\n";
    }

    // adjustments
    const Core::AdjustmentData* adjustData = Tools::getComputedDataWithId <Core::AdjustmentData>
                    (_xpertData.getResponses().getRequestResponses(), "adjustment" );
    if (adjustData) {
        size_t nbAdjust = adjustData->getAdjustments().size();
        out += "<section>\n<h2>" + (nbAdjust > 1 ? lang->SUGGESTED_ADJUSTMENTS : lang->SUGGESTED_ADJUSTMENT) + ":</h2>\n" ;
        for (size_t i = 0; i < nbAdjust; i++) {
            const Core::DosageAdjustment currentAdjust = adjustData->getAdjustments()[i];
            QFile adjustFile (QString::fromStdString("adjust"+std::to_string(i+1)+".png"));
            adjustFile.open(QIODevice::ReadOnly);
            QByteArray adjustEncoded = adjustFile.readAll();
            adjustEncoded = adjustEncoded.toBase64();
            adjustFile.close();
            out += "<img src=\"data:image/png;base64, " + adjustEncoded.toStdString() + "\" "
                   "onclick=\"toggleZoom(this)\" />\n"
                   "<h3>" + lang->DATA_FOR_THIS_ADJUSTMENT + ":</h3>\n<b>" +
                    lang->TARGET_EVALUATION + ":</b><br />\n" + lang->GLOBAL_SCORE + ": " +
                    std::to_string(currentAdjust.getGlobalScore()) + "<br />\n<table><tr><th>" +
                    lang->TARGET_TYPE + "</th><th>" + lang->VALUE + "</th><th>" + lang->SCORE + "</th></tr>\n";

            for (const Core::TargetEvaluationResult& targetResult : currentAdjust.m_targetsEvaluation) {
                out +=  "<tr><td>" + Core::toString(targetResult.getTargetType()) + "</td><td>" +
                        std::to_string(targetResult.getValue()) + " " + targetResult.getUnit().toString() +
                        "</td><td>" + std::to_string(targetResult.getScore()) + "</td></tr>\n";
            }
            out += "</table>\n<b>"+ lang->DOSAGE + ":</b><br />\n";

            for ( const std::unique_ptr<Core::DosageTimeRange>& tr :
                     currentAdjust.m_history.getDosageTimeRanges() ) {

                const Core::DosageRepeat* dosageRepeat = dynamic_cast<const Core::DosageRepeat*>(tr->getDosage());
                if (dosageRepeat) {
                    const Core::LastingDose* lastingDose = dynamic_cast<const Core::LastingDose*>(dosageRepeat->getDosage());
                    if (lastingDose) {
                        out += "<div class=\"dosage\">\n" + lang->START_DATE + ": " + Tools::printDateTime(tr->getStartDate()) + " - " + lang->END_DATE + ": " +
                               Tools::printDateTime(tr->getEndDate()) + "<br />\n" + lang->DOSE + ": " +
                               std::to_string(lastingDose->getDose()) + " " + lastingDose->getDoseUnit().toString() +
                               (lastingDose->getInfusionTime().isEmpty() ? "" : ( ", " + lang->INFUSION_TIME + ": " +
                               std::to_string(lastingDose->getInfusionTime().toMinutes()) + " min")) + "<br />\n" +
                               lang->INTERVAL + ": " + std::to_string(lastingDose->getTimeStep().toHours()) + "h, " +
                               lang->NB_ITERATIONS + ": " + std::to_string(dosageRepeat->getNbTimes()) + "\n</div>\n";
                    } else {
                        out += "<div class=\"warning\">dose type not recognized.</div>\n";
                    }
                } else {
                    out += "<div class=\"warning\">dose type not recognized.</div>\n";
                }
            }
        }
        out += (nbAdjust ? "" : lang->NO_SUITABLE_ADJUSTMENT ) +"</section>\n";
    } else {
        out += "<section><div class=\"warning\">Error: no adjustment result found.</div></section>\n";
    }

    out += "</body>\n<script>\nfunction toggleZoom (img) {\n"
           "if (img.classList.contains(\"zoom\")) {img.classList.remove(\"zoom\")}\n"
           "else { img.classList.add(\"zoom\") }\n}</script>\n</html>";

    std::ofstream file;
    file.open(_outputPath);
    if ((file.rdstate() & std::ostream::failbit) != 0) {
        return EXIT_FAILURE;
    }
    file << out;
    file.close();

    delete lang;
    return EXIT_SUCCESS;
}

std::string HtmlExporter::checkSample (const Core::Sample* _sample, const std::vector<Core::CycleData>& _perc, const ILanguage* _lang, const bool _upperbound) {

    for (const Core::CycleData& percCycle : _perc ) {
        double relativeTime = (_sample->getDate() - percCycle.m_start).toHours();
        const std::vector<double>& cycleTimes = percCycle.m_times[0];
        const std::vector<double>& cycleConc = percCycle.m_concentrations[0];
        if (relativeTime >= 0 && percCycle.m_end > _sample->getDate() ) {
            int i = 0;
            while (cycleTimes[i] < relativeTime) {
                i++;
            }
            double sampleValNorm = Common::UnitManager::convertToUnit(_sample->getValue(), _sample->getUnit(), percCycle.m_unit);
            // the value of the percentile at the exact sample time is linearly interpolated.
            double interpolatedValue = cycleConc[i-1] + (cycleConc[i] - cycleConc[i-1] ) *
                   ( relativeTime - cycleTimes[i-1] ) / (cycleTimes[i] - cycleTimes[i-1] );
            if ( (! _upperbound && sampleValNorm < interpolatedValue) ||
                  ( _upperbound && sampleValNorm > interpolatedValue) ) {

                return "<div class=\"warning\">" + _lang->sampleWarning(Tools::printDateTime(_sample->getDate()), _upperbound) +
                       "</div>\n";
            }
        }
    }
    return "";
}



}
}

