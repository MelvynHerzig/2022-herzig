#ifndef TUCUXI_XPERTUTILS_H
#define TUCUXI_XPERTUTILS_H

#include <string>

namespace Tucuxi {
namespace XpertUtils {

/// \brief Convert a floating point variable to a string with two decimals.
/// \param _value Variable to convert to a string.
/// \return String describing the variable.
std::string varToString(const double& _value);

} // namespace XpertUtils
} // namespace Tucuxi

#endif // TUCUXI_XPERTUTILS_H
