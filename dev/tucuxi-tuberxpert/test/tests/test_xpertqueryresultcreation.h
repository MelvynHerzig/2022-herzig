#ifndef TEST_GLOBALRESULTCREATION_H
#define TEST_GLOBALRESULTCREATION_H

#include "fructose/fructose.h"

#include "testutils.h"

#include "tuberxpert/language/languagemanager.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/result/xpertqueryresult.h"

/// \brief Tests for the creation of XpertQueryResult.
///        This struct tests the XpertQueryResult constructor with various queries and checks
///        that the expected values can be correctly retrieved.
/// \date 25/05/2022
/// \author Herzig Melvyn
struct TestXpertQueryResultCreation : public fructose::test_base<TestXpertQueryResultCreation>
{

    /// \brief Check that the ownership of the xpertQueryData is taken by XpertRequestResult.
    ///        The test succeeds if the XpertQueryData pointer is not nullptr before the
    ///        XpertQueryResult construction and if the XpertQueryData pointer is nullptr
    ///        after the XpertQueryResult construction.
    /// \param _testName Name of the test
    void xpertQueryResultCreation_takesXpertQueryDataOwnership(const std::string& _testName);

    /// \brief Check that the admin data are correctly retrieved.
    ///        The test imports two xpert queries.
    ///        The first one has no admin element. Using XpertQueryResult::getAdminData, the value is nullptr.
    ///        The second one has a full admin element. When using XpertQueryResult::getAdminData, all
    ///        values of the admin element can be retrieved.
    /// \param _testName Name of the test
    void getAdminDataOfXpertQueyResult_returnsCorrectValues_withOrWithoutAdminInQuery(const std::string& _testName);


    /// \brief Check that the XpertRequestResult are correctly created.
    ///        The query has two xpertRequests. One if for imatinib and the other
    ///        is for rifampicin.
    ///
    ///        Rifampicin:
    ///           - The XpertRequestResult has no errors and can still be processed.
    ///           - The treatment is not nullptr.
    ///           - The drug model is nullptr.
    ///
    ///        Imatinib:
    ///           - The XpertRequestResult has an error because the query doesn't have a
    ///             drug element for imatinib.
    ///           - It should no longer be processed.
    ///           - The treatment is nullptr.
    ///           - The drug model is nullptr.
    /// \param _testName Name of the test
    void getXpertRequestResultsOfXpertQueryResult_returnsCorrectValues_withValidAndInvalidXpertRequest(const std::string& _testName);

    /// \brief Check that the xpertRequest data are correctly retrieved in the XpertRequestResult.
    ///        The test imports a query with two xpertRequest elements.
    ///        The first element is a complete xpertRequest. All values must be retrieved correctly.
    ///        The second element is a minimal xpertRequest. All values must be retrieved correctly, but
    ///        the missing values have default values.
    /// \param _testName Name of the test
    void getXpertRequestDataOfXpertRequestResult_returnsCorrectValues_withFullAndMinimalXpertRequest(const std::string& _testName);

    /// \brief Check that computation time of the XpertRequestResult is retrieved from the xpert query.
    ///        At top of the xpert query, the date element is "2018-07-11T13:45:30".
    ///        XpertQueryResult::getComputationTime should return the equivalent date of "2018-07-11T13:45:30".
    /// \param _testName Name of the test
    void getComputationTimeOfXpertRequestResult_returnsCorrectDateTime(const std::string& _testName);

};

#endif // TEST_GLOBALRESULTCREATION_H
