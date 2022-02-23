#include "tools.h"


namespace Tucuxi {
namespace Xpert {



Tools::Tools()
{

}

const std::string Tools::printDate (const Common::DateTime& _date) {
    return getStrWithPrecedingZero(_date.day()) + "/" +
           getStrWithPrecedingZero(_date.month()) + "/" +
           std::to_string(_date.year()) ;
}

const std::string Tools::printDateTime(const Common::DateTime& _date) {
    return printDate(_date) + " " +
           getStrWithPrecedingZero(_date.hour()) + ":" +
           getStrWithPrecedingZero(_date.minute()) + ":" +
           getStrWithPrecedingZero(_date.second()) ;
}

const std::string Tools::getStrWithPrecedingZero(const int _dateElement) {
    return (_dateElement < 10 ? "0": "") + std::to_string(_dateElement) ;
}



}
}
