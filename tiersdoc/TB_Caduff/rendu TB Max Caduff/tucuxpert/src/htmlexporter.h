#ifndef HTMLEXPORTER_H
#define HTMLEXPORTER_H
#include "xpertdata.h"
#include "ilanguage.h"
#include "tucuquery/computingqueryresponse.h"

namespace Tucuxi {
namespace Xpert {


class HtmlExporter
{
public:
    HtmlExporter();

    int exportToHtml(XpertData& _xpertData, std::string _outputPath);

private:

    std::string checkSample(const Core::Sample *_sample, const std::vector<Core::CycleData> &_perc, const ILanguage *_lang, const bool _upperbound);
};


}
}

#endif // HTMLEXPORTER_H
