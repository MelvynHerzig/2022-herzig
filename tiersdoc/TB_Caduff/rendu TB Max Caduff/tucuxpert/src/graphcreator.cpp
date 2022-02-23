#include "graphcreator.h"
#include "libs/qcustomplot.h"
#include "tools.h"


namespace Tucuxi {
namespace Xpert {

const char* predColors[] = {"#9F8E6F", "#E19A00", "#C80000"};
const char* innerPercColors[] = {"#D0C0A2", "#F2C054", "#FE9A9A"};
const char* outerPercColors[] = {"#F2ECDE", "#FEF2C0", "#FED4D4"};
const char* percLineColors[] = {"#B2A283", "#EFAC1C", "#FE6D6D"};


GraphCreator::GraphCreator()
{ }

int GraphCreator::createGraphs(XpertData& _xpertData, const ILanguage* _lang)
{
    // check if resp contains data (at least adjust)
    if (_xpertData.getResponses().getRequestResponses().size() == 0) {
        return EXIT_FAILURE;
    }

    QCustomPlot qcp;
    qcp.addLayer("above");
    qcp.xAxis->setLabel(QString::fromStdString(_lang->DATES));
    qcp.legend->setVisible(true);
    qcp.legend->setBrush(QColor(255, 255, 255, 200));
    //qcp.legend->setLayer("above");
    qcp.xAxis->setTickLabelRotation(45);

    const Core::AdjustmentData* adjustData = Tools::getComputedDataWithId <Core::AdjustmentData>(
                _xpertData.getResponses().getRequestResponses(), "adjustment" );
    const Core::SinglePredictionData* concPredData = Tools::getComputedDataWithId <Core::SinglePredictionData>(
                _xpertData.getResponses().getRequestResponses(), "concentration");
    QVector<double> concPredX;
    QVector<double> concPredY;
    QCPGraph* concPredGraph = nullptr;
    int predType = static_cast<int>(_xpertData.getPredictionType());

    // creating concentration data if present to show with adjustment
    if (concPredData) {

        makeContiguousXYData (concPredX, concPredY, concPredData->getData());

        concPredGraph = qcp.addGraph();
        concPredGraph->setLayer("above");
        concPredGraph->setData (concPredX, concPredY);
        concPredGraph->setPen(QPen(QColor(predColors[predType]), 2));
        concPredGraph->setName (QString::fromStdString(_lang->PREDICTED_BLOOD_CONCENTRATION));


    }

    // calculating the time shift once for all adjustments, and days for the X axis.
    double timeShift = 0.;
    DateTime firstDayToPlot;
    // this represents the duration in hours from the graph start to the next day at midnight.
    double nextDayOffset = 0;
    int totalNbDays = 0;
    if (concPredData && concPredData->getData().size() > 0) {
        // concentration prediction found
        firstDayToPlot = getNextDay(concPredData->getData()[0].m_start);
        nextDayOffset = (firstDayToPlot - concPredData->getData()[0].m_start).toHours() ;
        qcp.yAxis->setLabel(QString::fromStdString(_lang->BLOOD_CONCENTRATION + " (" +
                                                   concPredData->getData().front().m_unit.toString() + ")"));

        if (adjustData->getAdjustments().size() > 0 && adjustData->getAdjustments()[0].m_data.size() > 0) {
            // concentration prediction and adjustments found
            timeShift = (adjustData->getAdjustments()[0].m_data[0].m_start - concPredData->getData()[0].m_start).toHours() ;
            totalNbDays = ceil((adjustData->getAdjustments()[0].m_data.back().m_end - concPredData->getData()[0].m_start).toDays()) ;
        }
        else { // only concentration
            totalNbDays = ceil((concPredData->getData().back().m_end - concPredData->getData().front().m_start).toDays()) ;
        }
    }
    else if (adjustData->getAdjustments().size() > 0 && adjustData->getAdjustments()[0].m_data.size() > 0) {
        // only adjustment found
        firstDayToPlot = getNextDay(adjustData->getAdjustments()[0].m_data[0].m_start);
        nextDayOffset = (firstDayToPlot - adjustData->getAdjustments()[0].m_data[0].m_start).toHours() ;
        totalNbDays = ceil((adjustData->getAdjustments()[0].m_data.back().m_end -
                             adjustData->getAdjustments()[0].m_data.front().m_start).toDays()) ;

        qcp.yAxis->setLabel(QString::fromStdString(_lang->BLOOD_CONCENTRATION +
                            " (" + adjustData->getAdjustments().front().m_data.front().m_unit.toString() + ")"));
    }

    // set days in x axis
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(nextDayOffset - 18., QString::fromStdString("06h00"));
    textTicker->addTick(nextDayOffset - 12., QString::fromStdString("12h00"));
    textTicker->addTick(nextDayOffset - 6., QString::fromStdString("18h00"));

    for (int i = 0; i < totalNbDays; i++) {
        textTicker->addTick(nextDayOffset, QString::fromStdString(Tools::printDate(firstDayToPlot) ));
        textTicker->addTick(nextDayOffset + 6., QString::fromStdString("06h00"));
        textTicker->addTick(nextDayOffset + 12., QString::fromStdString("12h00"));
        textTicker->addTick(nextDayOffset + 18., QString::fromStdString("18h00"));
        nextDayOffset += 24.;
        firstDayToPlot.addDays(1);
    }
    qcp.xAxis->setTicker(textTicker);

    // generate all adjustments
    size_t adjustCount = 0;
    for (Core::DosageAdjustment adjust : adjustData->getAdjustments() ) {

        // concat x and y data
        QVector<double> adjX;
        QVector<double> adjY;
        makeContiguousXYData (adjX, adjY, adjust.m_data, timeShift);

        // draw curve
        QCPGraph* adjustGraph = qcp.addGraph();
        adjustGraph->setData(adjX, adjY);
        adjustGraph->setPen(QPen(QColor(0,0,0), 1));
        adjustGraph->setName(QString::fromStdString(_lang->SUGGESTED_ADJUSTMENT));

        // export and remove only the adjustment, so the prediction curve must not be redrawn.
        qcp.rescaleAxes();
        qcp.savePng( QString::fromStdString("adjust" + std::to_string( ++adjustCount) + ".png"), (totalNbDays + 5) / 3 * 400, 600);

        qcp.removeGraph (adjustGraph);
    }

    if (_xpertData.getResponses().getRequestResponses().size() < 3) {
        return EXIT_SUCCESS;
    }

    // if treatment, generate prediction curve (already in qcp) + perc.
    // X is the same for concentration and percentiles.
    const Core::PercentilesData* percData = Tools::getComputedDataWithId <Core::PercentilesData>(
                _xpertData.getResponses().getRequestResponses(), "percentiles");

    if (percData) {
        QVector<double> perc05;
        QVector<double> perc25;
        QVector<double> perc75;
        QVector<double> perc95;
        concatYData (perc05, percData->getPercentileData(0) );
        concatYData (perc25, percData->getPercentileData(1) );
        concatYData (perc75, percData->getPercentileData(2) );
        concatYData (perc95, percData->getPercentileData(3) );


        QPen percPen = QPen (QColor(percLineColors[predType]), 1);

        QCPGraph* graph05 = qcp.addGraph();
        graph05->setData(concPredX, perc05);
        graph05->setPen(percPen);
        graph05->removeFromLegend();

        QCPGraph *graph95 = qcp.addGraph();
        graph95->setData(concPredX, perc95);
        //graph95->setPen(Qt::NoPen);
        graph95->setPen(percPen);
        graph95->setBrush(QColor(outerPercColors[predType]));
        graph95->setChannelFillGraph(graph05);
        graph95->removeFromLegend();

        QCPGraph* graph25 = qcp.addGraph();
        graph25->setData(concPredX, perc25);
        graph25->setPen(percPen);
        graph25->removeFromLegend();
        //graph25->setPen(Qt::NoPen);

        QCPGraph *graph75 = qcp.addGraph();
        graph75->setData(concPredX, perc75);
        graph75->setPen(percPen);
        //graph75->setPen(Qt::NoPen);
        graph75->setBrush(QColor(innerPercColors[predType]));
        graph75->setChannelFillGraph(graph25);
        graph75->removeFromLegend();

        if (_xpertData.getDrugTreatment()->getSamples().size() > 0 ) {

            Unit graphUnit = ( concPredData && concPredData->getData().size() > 0 ) ?
                        concPredData->getData().front().m_unit.toString() :
                        adjustData->getAdjustments().front().m_data.front().m_unit.toString() ;
            QCPGraph* measureGraph = qcp.addGraph();
            QVector<double> measureTimes;
            QVector<double> measureValues;
            for (const std::unique_ptr<Core::Sample>& sample : _xpertData.getDrugTreatment()->getSamples()) {
                double relativeTime = (sample->getDate() - concPredData->getData()[0].m_start).toHours() ;
                if (relativeTime >= 0) {
                    measureTimes.push_back(relativeTime);
                    measureValues.push_back(Common::UnitManager::convertToUnit(sample->getValue(), sample->getUnit(), graphUnit) );
                }
            }
            measureGraph->setData (measureTimes, measureValues);
            measureGraph->setLineStyle(QCPGraph::lsNone);
            measureGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            measureGraph->setName (QString::fromStdString(_lang->MEASURED_SAMPLES));
        }

    }
    if (concPredData) {
        qcp.rescaleAxes();
        int concPredNbDays = ceil(concPredX.back() / 24);
        qcp.savePng("concPerc.png", (concPredNbDays + 5) / 3 * 400, 600);
    }

    return EXIT_SUCCESS;

}

void GraphCreator::makeContiguousXYData(QVector<double>& _x, QVector<double>& _y, const std::vector<Core::CycleData>& _datas, const double _timeShift)
{
    double lastX = _timeShift;
    for (Core::CycleData data : _datas ) {
        // setting continuous values of X starting at _timeShift
        QVector<double> temp =  QVector<double>::fromStdVector(data.getTimes()[0] );
        if (lastX) {
            for (size_t i = 0; i < temp.size (); i++) {
                temp[i] += lastX;
            }
        }
        _x += temp;
        lastX = temp.back();

        _y +=  QVector<double>::fromStdVector(static_cast <std::vector<double>>(data.getConcentrations()[0] ));
    }
}

void GraphCreator::concatYData (QVector<double>& _y, const std::vector<Core::CycleData>& _datas) {
    for (Core::CycleData data : _datas ) {
        _y +=  QVector<double>::fromStdVector(static_cast <std::vector<double>>(data.getConcentrations()[0] ));
    }
}

Common::DateTime GraphCreator::getNextDay(Common::DateTime _moment)
{
    _moment -= _moment.getTimeOfDay().getRealDuration();
    _moment.addDays(1);
    return _moment;
}





}
}
