#ifndef TEST_XPERTQUERYIMPORT_H
#define TEST_XPERTQUERYIMPORT_H

#include "../src/query/xpertqueryimport.h"
#include "fructose/fructose.h"

struct TestXpertQueryImport : public fructose::test_base<TestXpertQueryImport>
{
    /// \brief Load an xml file with every possible data in admin node
    /// \param _testName Name of the test.
    void retrieveCompleteAdmin(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/complete_admin.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
    }


};

#endif // TEST_XPERTQUERYIMPORT_H
