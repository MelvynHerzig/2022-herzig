#ifndef TUBERXPERTCOMPUTER_H
#define TUBERXPERTCOMPUTER_H

#include <string>

namespace Tucuxi {
namespace XpertComputer {

/// \brief Enum whose values are used as the return value of TuberXpertComputer.
///        The values are:
///
///        ALL_REQUESTS_SUCCEEDED: Every xpert request could be fully handled until
///        the report printing.
///
///        SOME_REQUESTS_SUCCEEDED: Some xpert request failed during a step of the process.
///
///        NO_REQUESTS_SUCCEEDED: No xpert request could be fully handled.
///
///        IMPORT_ERROR: The query file could not be loaded.
/// \date 03/06/2022
/// \author Herzig Melvyn
enum class ComputingStatus {

    ALL_REQUESTS_SUCCEEDED,
    SOME_REQUESTS_SUCCEEDED,
    NO_REQUESTS_SUCCEEDED,
    IMPORT_ERROR
};

/// \brief Given the required arguments, this class drives the execution of tuberXpert.
/// \date 03/06/2022
/// \author Herzig Melvyn
class TuberXpertComputer
{
public:

    /// \brief Constructor.
    TuberXpertComputer();

    /// \brief Entry point of the TuberXpert command Line Interface. This method imports
    ///        the query, loaded the translation file, and handle each XpertRequest to finally
    ///        print the report of the requests handled successfully.
    /// \param _drugPath Folder containing the drug models.
    /// \param _inputFileName Path to the query file.
    /// \param _outputFileName Output file name. One file per successful request is created.
    ///                        Therefore, the resulting file is suffixed by '_<number>'. The number
    ///                        represent the position of the requestXpert in the query file, the first beeing
    ///                        number 1.
    /// \param _languagePath Path to the folder containing the translation files.
    /// \return A computingStatus the depends on the fact that the query could be loaded and the amount
    ///         of requestXpert handled successfully.
    ComputingStatus compute(const std::string& _drugPath,
                            const std::string& _inputFileName,
                            const std::string& _outputFileName,
                            const std::string& _languagePath) const;
};

} // namespace XpertComputer
} // namespace Tucuxi

#endif // TUBERXPERTCOMPUTER_H
