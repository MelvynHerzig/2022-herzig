#ifndef ENLANGUAGE_H
#define ENLANGUAGE_H

#include "ilanguage.h"


namespace Tucuxi {
namespace Xpert {

class EnLanguage : public ILanguage
{
public:
    EnLanguage();
    virtual ~EnLanguage() {}

    virtual const std::string tellPredictionType (const Core::PredictionParameterType _predictionType) const override;
    virtual const std::string boundWarning() const override;
    virtual const std::string sampleWarning (const std::string _date, const bool _upperBound ) const override;

};

}
}

#endif // ENLANGUAGE_H
