#ifndef GRAPHCREATOR_H
#define GRAPHCREATOR_H
#include "tucucore/computingservice/computingtrait.h"
#include "xpertdata.h"
#include "ilanguage.h"
#include <QVector>

namespace Tucuxi {
namespace Xpert {


class GraphCreator
{
public:

    GraphCreator();
    int createGraphs(XpertData& _xpertData, const ILanguage* _lang);


private:
    void makeContiguousXYData (QVector<double>& _x, QVector<double>& _y,
                            const std::vector<Core::CycleData>& _datas, const double _timeShift = 0);
    void concatYData (QVector<double>& _y, const std::vector<Core::CycleData>& _datas);

    Common::DateTime getNextDay(Common::DateTime _moment);

};

}
}
#endif // GRAPHCREATOR_H
