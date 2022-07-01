#ifndef FILESTRING_H
#define FILESTRING_H

#include <string>

namespace Tucuxi {
namespace Xpert {

/// \brief This class contains the static file strings for the HTML export.
///        The static files are the files that does not change between two
///        exports. Those files are the CSS and JS files used by the index.
///
///        The CSS files are minified versions of the files found in
///        tucuxi-tuberxpert/html/src/asset/css.
///
///        The JS files are minified versions of the transpiled files
///        found in tucuxi-tuberxpert/html/build/asset/js.
///
///        To minify the files, I used:
///        - CSS: https://www.toptal.com/developers/cssminifier
///        - JS: https://www.toptal.com/developers/javascript-minifier
///
///        For conveniance, copy the minified results in a text editor and replace " by '
///        before copying them in the static strings of this class.
/// \date 23/06/2022
/// \author Herzig Melvyn
class FileString
{
public:

    /**************************************************************
     *                             CSS                            *
     * ************************************************************/

    static const std::string normalizeCssStr;

    static const std::string tuberxpertCssStr;

    /**************************************************************
     *                             JS                            *
     * ************************************************************/

    static const std::string graphdataJsStr;

    static const std::string graphingJsStr;

    static const std::string tuberxpertJsStr;


};

} // namespace Xpert
} // namespace Tucuxi

#endif // FILESTRING_H
