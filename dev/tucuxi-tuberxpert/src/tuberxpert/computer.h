#ifndef COMPUTER_H
#define COMPUTER_H

#include <memory>
#include <string>

#include "tucucore/computingservice/computingtrait.h"

#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/flow/abstract/abstractxpertflowstepprovider.h"

namespace Tucuxi {
namespace Xpert {

/// \brief Enum whose values are used as the return value of TuberXpertComputer.
///        The values are:
///
///        ALL_REQUESTS_SUCCEEDED: Every XpertRequest could be fully handled until
///        the report printing.
///
///        SOME_REQUESTS_SUCCEEDED: Some XpertRequest failed during a step of the process.
///
///        NO_REQUESTS_SUCCEEDED: No XpertRequest could be fully handled.
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

/// \brief Given the required arguments, this class drives the execution flow of tuberXpert.
/// \date 03/06/2022
/// \author Herzig Melvyn
class Computer
{
public:

    /// \brief Constructor.
    Computer();

    /// \brief Entry point of the TuberXpert command Line Interface. This method imports
    ///        the query from file, loads the translation file, and handle each XpertRequest to finally
    ///        print the report of the requests handled successfully.
    /// \param _drugPath Folder containing the drug models.
    /// \param _inputFileName Path to the query file.
    /// \param _outputPath Output directory path. One file per successful request is created.
    /// \param _languagePath Path to the folder containing the translation files.
    /// \return A computingStatus the depends on the fact that the query could be loaded and the amount
    ///         of requestXpert handled successfully.
    ComputingStatus computeFromFile(const std::string& _drugPath,
                                    const std::string& _inputFileName,
                                    const std::string& _outputPath,
                                    const std::string& _languagePath) const;

    /// \brief Entry point of the TuberXpert command Line Interface. This method imports
    ///        the query from string, loads the translation file, and handle each XpertRequest to finally
    ///        print the report of the requests handled successfully.
    /// \param _drugPath Folder containing the drug models.
    /// \param _inputString Path to the query file.
    /// \param _outputPath Output directory path. One file per successful request is created.
    /// \param _languagePath Path to the folder containing the translation files.
    /// \return A computingStatus the depends on the fact that the query could be loaded and the amount
    ///         of requestXpert handled successfully.
    ComputingStatus computeFromString(const std::string& _drugPath,
                                      const std::string& _inputString,
                                      const std::string& _outputPath,
                                      const std::string& _languagePath) const;

protected:

    /// \brief For a given xpertRequest in _xpertRequestResult, this method executes the flow provided by the given
    ///        XpertFlowStepProvider.
    /// \param _xpertRequestResult Object containing the XpertRequest and treatment informations. This object will also
    ///                            be filled with the various validations of the system.
    /// \param _languagePath Folder containing the language files.
    /// \param _stepProvider Flow step provider responsible to give the each flow step for a given drug.
    void executeFlow(XpertRequestResult& _xpertRequestResult,
                     const std::string& _languagePath,
                     const std::unique_ptr<AbstractXpertFlowStepProvider>& _stepProvider) const;

    /// \brief For a given XpertRequestResult get the XpertFlowStepProvider for the related drug.
    /// \param _xpertRequestResult XpertRequestResult to get drug id from.
    /// \param _xpertFlowStepProvider Unique pointer in which create the corresponding XpertFlowStepProvider.
    void getXpertFlowStepProvider(XpertRequestResult& _xpertRequestResult,
                                  std::unique_ptr<AbstractXpertFlowStepProvider>& _xpertFlowStepProvider) const;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // COMPUTER_H
