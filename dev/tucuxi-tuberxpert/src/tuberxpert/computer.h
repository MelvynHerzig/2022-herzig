#ifndef COMPUTER_H
#define COMPUTER_H

#include <memory>
#include <string>

#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/flow/abstract/abstractxpertflowstepprovider.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Enum whose values are used as return value of Computer.
///        The values are:
///
///        ALL_REQUESTS_SUCCEEDED: Each TuberXpert request could be fully processed until
///        the report was printed.
///
///        SOME_REQUESTS_SUCCEEDED: Some TuberXpert request failed at some stage of the process.
///
///        NO_REQUESTS_SUCCEEDED: No TuberXpert request could be fully processed.
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

/// \brief Given the required arguments, this class drives the flow of execution of TuberXpert.
/// \date 03/06/2022
/// \author Herzig Melvyn
class Computer
{
public:

    /// \brief Default constructor.
    Computer();

    /// \brief Entry point of the TuberXpert command Line Interface. This method imports
    ///        the query from a file, loads the translation file, and processes each xpertRequest to finally
    ///        print the reports of the successfully processed requests.
    /// \param _drugPath Path to the folder containing the drug models.
    /// \param _inputFileName Path to the query file.
    /// \param _outputPath Path ot the output directory. One file is created per successful request.
    /// \param _languagePath Path to the folder containing the translations files.
    /// \return  A computingStatus that depends on whether the query could be loaded and how much
    ///          requestXpert was successfully processed.
    ComputingStatus computeFromFile(const std::string& _drugPath,
                                    const std::string& _inputFileName,
                                    const std::string& _outputPath,
                                    const std::string& _languagePath) const;

    /// \brief This method imports the query from a string, loads the translation file,
    ///        and processes each xpertRequest to finally print the reports of the successfully
    ///        processed requests.
    /// \param _drugPath Path to the folder containing the drug models.
    /// \param _inputString Xml query string.
    /// \param _outputPath Path ot the output directory. One file is created per successful request.
    /// \param _languagePath Path to the folder containing the translations files.
    /// \return A computingStatus that depends on whether the query could be loaded and how much
    ///          requestXpert was successfully processed.
    ComputingStatus computeFromString(const std::string& _drugPath,
                                      const std::string& _inputString,
                                      const std::string& _outputPath,
                                      const std::string& _languagePath) const;

protected:

    /// \brief For a given xpertRequest in _xpertRequestResult, this method executes the flow steps
    ///        provided by the given AbstractXpertFlowStepProvider.
    /// \param _xpertRequestResult Object containing the xpertRequest and treatment informations. This object will also
    ///                            be filled with the different validations of the system.
    /// \param _languagePath Path to the folder containing the language files.
    /// \param _stepProvider Flow step provider responsible to give the each flow step for a given drug.
    void executeFlow(XpertRequestResult& _xpertRequestResult,
                     const std::string& _languagePath,
                     const std::unique_ptr<AbstractXpertFlowStepProvider>& _stepProvider) const;

    /// \brief For a given drug identifier, get the XpertFlowStepProvider that best matches.
    /// \param _drugId Drug identifier to search for the best AbstractXpertFlowStepProvider.
    /// \param _xpertFlowStepProvider Unique pointer in which to create the corresponding XpertFlowStepProvider.
    void getXpertFlowStepProvider(const std::string& _drugId,
                                  std::unique_ptr<AbstractXpertFlowStepProvider>& _xpertFlowStepProvider) const;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // COMPUTER_H
