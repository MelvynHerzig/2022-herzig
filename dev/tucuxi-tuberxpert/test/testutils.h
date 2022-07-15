#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <string>

#include "tuberxpert/flow/general/generalxpertflowstepprovider.h"
#include "tuberxpert/query/xpertquerydata.h"

/// \brief Class that regroup common elements used by multiple tests.
/// \date 15/07/2022
/// \author Herzig Melvyn
class TestUtils
{
public:
    /// \brief General flow step provider used to get diffrent flow steps.
    static const Tucuxi::Xpert::GeneralXpertFlowStepProvider flowStepProvider;

    /// \brief Format used to create date and time during tests.
    static const std::string date_format;

    /// \brief Standard drug model string of the imatinib.
    static const std::string originalImatinibModelString;

    /// \brief Standard english translations file.
    static const std::string englishTranslationFile;

    /// \brief Set up the test environment. Load the query, make the
    ///        XpertQueryResult object, load the model, attribute it to the first XpertRequestResult
    ///        of the XpertQueryResult and loads an english translationFile.
    /// \param _queryString Query string to load.
    /// \param _model Model string to put as drug model of the XpertRequestResult of the first request.
    /// \param _xpertQueryResult XpertQueryResult where to put the execution result of this method.
    static void setupEnv(const std::string& _queryString,
                         const std::string& _model,
                         std::unique_ptr<Tucuxi::Xpert::XpertQueryResult>& _xpertQueryResult);
};

#endif // TESTUTILS_H
