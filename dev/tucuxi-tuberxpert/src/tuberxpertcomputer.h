#ifndef TUBERXPERTCOMPUTER_H
#define TUBERXPERTCOMPUTER_H

#include <memory>
#include <string>

#include "tuberxpert/result/xpertrequestresult.h"
#include "tuberxpert/flow/abstract/abstractxpertflowstepprovider.h"

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
    /// \param _outputPath Output directory path. One file per successful request is created.
    /// \param _languagePath Path to the folder containing the translation files.
    /// \return A computingStatus the depends on the fact that the query could be loaded and the amount
    ///         of requestXpert handled successfully.
    ComputingStatus compute(const std::string& _drugPath,
                            const std::string& _inputFileName,
                            const std::string& _outputPath,
                            const std::string& _languagePath) const;

protected:

    /// \brief For a given xpertRequest in _xpertRequestResult and its treatment, this methods validate the inputs
    ///        (are the dosages, samples, covariates and targets plausible), selects a drug model and creates the associated
    ///        adjustment trait.
    /// \param _xpertRequestResult Object containing the XpertRequest and treatment informations. This object will also
    ///                            be filled with the various validations of the system.
    /// \param _languagePath Folder containing the language files.
    /// \param _stepProvider Flow step provider responsible to give the each step for a given drug.
    /// \return True if everything went well (i.e. the XpertRequest result is ready to be submitted to the core) or false if the
    ///         XpertRquest needs to be reviewed (i.e. no language file found for the desired language, no drug model found ...)
    bool validateAndPrepareXpertRequest(Tucuxi::Xpert::XpertRequestResult& _xpertRequestResult,
                                        const std::string& _languagePath,
                                        const std::unique_ptr<Tucuxi::Xpert::AbstractXpertFlowStepProvider>& _stepProvider) const;

    /// \brief For a given XpertRequestResult get the XpertFlowStepProvider for the related drug.
    /// \param _xpertRequestResult XpertRequestResult to get drug id from.
    /// \param _xpertFlowStepProvider Unique pointer in which create the corresponding XpertFlowStepProvider.
    void getXpertFlowStepProvider(Tucuxi::Xpert::XpertRequestResult& _xpertRequestResult,
                                  std::unique_ptr<Tucuxi::Xpert::AbstractXpertFlowStepProvider>& _xpertFlowStepProvider) const;

    /// \brief Extracts the adjustment trait from the XpertRequestResult, makes the request for the core and submits it.
    /// \param _xpertRequestResult XpertRequestResult containing the adjustment trait to use.
    /// \return True if the request execution went well, else false.
    bool makeAndExecuteAdjustmentRequest(Tucuxi::Xpert::XpertRequestResult& _xpertRequestResult) const;
};

#endif // TUBERXPERTCOMPUTER_H
