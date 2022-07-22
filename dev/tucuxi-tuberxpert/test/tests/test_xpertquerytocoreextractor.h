#ifndef TEST_XPERTQUERYTOCOREEXTRACTOR_H
#define TEST_XPERTQUERYTOCOREEXTRACTOR_H

#include <string>
#include <memory>

#include "fructose/fructose.h"

#include "tucucore/drugtreatment/drugtreatment.h"

#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/query/xpertquerytocoreextractor.h"

/// \brief Tests for XpertQueryToCoreExtractor.
///        This struct tests only the methods created by XpertQueryToCoreExtractor.
///        Therefore, it doesn't test the inherited methods. Here, to goal is to check
///        that the returned drugTreatment is not null and that the error message is empty string.
/// \date 25/05/2022
/// \author Herzig Melvyn
struct TestXpertQueryToCoreExtractor : public fructose::test_base<TestXpertQueryToCoreExtractor>
{

    /// \brief Check that the drug treatment is normally extracted for
    ///        the xpertRequest that requires a valid drug. A drug is valid
    ///        if it appears only once.
    ///        There are two xpertRequests.
    ///        One for imatinib and one for rifampicin. There is exactly
    ///        one drug element for each of these drugs. The extraction
    ///        must succeed without any error and the treatment pointers are not nullptr.
    /// \param _testName Name of the test
    void extractDrugTreatment_success_withDrugElementsPresentOnce(const std::string& _testName);

    /// \brief When there is no drug or multiple drugs that match a requested drug, it checks
    ///        that an error message is returned and that the drugTreatment pointers are nullptr.
    ///        There are two xpertRequests.
    ///        One for rifampicin and one for imatinib.
    ///        In this case, rifampicin is present twice and imatinib none.
    /// \param _testName Name of the test
    void extractDrugTreatment_failure_withMultipleOrNoDrugElements(const std::string& _testName);
};

#endif // TEST_XPERTQUERYTOCOREEXTRACTOR_H
