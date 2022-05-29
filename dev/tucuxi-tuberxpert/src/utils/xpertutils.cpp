#include "xpertutils.h"

#include <sstream>
#include <iomanip>

using namespace std;

namespace Tucuxi {
namespace XpertUtils {

string varToString(const double& _value)
{
    stringstream stream;
    stream << std::fixed << setprecision(2) << _value;
    return stream.str();
}

} // namespace XpertUtils
} // namespace Tucuxi
