#ifndef FILESTRING_H
#define FILESTRING_H

#include <string>

namespace Tucuxi {
namespace Xpert {

/// \brief This class contains the static file strings for the HTML export.
///        Static files are the files that do not change between two
///        exports. These files are the CSS and JS files used by the index page.
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
///        For convenience, copy the minified results in a text editor and replace " by '
///        before copying them in the static strings of this class.
/// \date 23/06/2022
/// \author Herzig Melvyn
class FileString
{
public:

    /**************************************************************
     *                             CSS                            *
     * ************************************************************/

    /// \brief CSS from https://necolas.github.io/normalize.css/. It makes the rendering
    ///        of the elements more consistent between browsers.
    ///        It is in: tucuxi-tuberxpert/html/src/asset/css/normalize.css
    static const std::string normalizeCssStr;

    /// \brief CSS responsible for the style of the report.
    ///        It is in: tucuxi-tuberxpert/html/src/asset/css/tuberxpert.css
    static const std::string tuberxpertCssStr;

    /**************************************************************
     *                             JS                            *
     * ************************************************************/

    /// \brief JS which implements the data necessary for the realization of graphs.
    ///        It is in: tucuxi-tuberxpert/html/build/asset/js/graphdata.js
    static const std::string graphdataJsStr;

    /// \brief JS that implements the graph creation methods for the graph data.
    ///        It is in: tucuxi-tuberxpert/html/build/asset/js/graphing.js
    static const std::string graphingJsStr;

    /// \brief JS that takes the data from the index and creates the graph
    ///        data and starts the graph creation process.
    ///        It is in: tucuxi-tuberxpert/html/build/asset/js/tuberxpert.js
    static const std::string tuberxpertJsStr;


};

} // namespace Xpert
} // namespace Tucuxi

#endif // FILESTRING_H
