#ifndef TEST_XPERTQUERYIMPORT_H
#define TEST_XPERTQUERYIMPORT_H

#include "../src/query/xpertqueryimport.h"
#include "fructose/fructose.h"

/// \brief Tests for XpertQueryImport
/// \date 27/04/2022
/// \author Herzig Melvyn
struct TestXpertQueryImport : public fructose::test_base<TestXpertQueryImport>
{
    /// \brief Load an xml file with every possible data in admin node
    /// and check if the recieved values are expected.
    /// \param _testName Name of the test.
    void retrieveCompleteAdmin(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/complete_admin.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();
        const Tucuxi::Query::AdministrativeData& pAdmin = optAdmin->get();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);

        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getId(), "asdf");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getTitle(), "Dr.");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getFirstName(), "John");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getLastName(), "Doe");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getpAddress().getStreet(), "Av. de l'Ours 2");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getpAddress().getPostCode(), 1010);
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getpAddress().getCity(), "Lausanne");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getpAddress().getState(), "Vaud");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getpAddress().getCountry(), "Suisse");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getpPhone().getNumber(), "0213140002");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getpPhone().getType(), "professional");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getpEmail().getAddress(), "john.doe@chuv.com");
        fructose_assert_eq(pAdmin.getpMandator().getpPerson().getpEmail().getType(), "professional");
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getId(), "456789");
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getName(), "CHUV");
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getpAddress().getStreet(), "Av. de l'Ours 1");
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getpAddress().getPostCode(), 1010);
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getpAddress().getCity(), "Lausanne");
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getpAddress().getState(), "Vaud");
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getpAddress().getCountry(), "Suisse");
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getpPhone().getNumber(), "0213140001");
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getpPhone().getType(), "professional");
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getpEmail().getAddress(), "info@chuv.com");
        fructose_assert_eq(pAdmin.getpMandator().getpInstitute().getpEmail().getType(), "professional");

        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getId(), "123456");
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getFirstName(), "Alice");
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getLastName(), "Aupaysdesmerveilles");
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getpAddress().getStreet(), "Av. d'Ouchy 27");
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getpAddress().getPostCode(), 1006);
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getpAddress().getCity(), "Lausanne");
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getpAddress().getState(), "Vaud");
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getpAddress().getCountry(), "Suisse");
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getpPhone().getNumber(), "0216170002");
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getpPhone().getType(), "professional");
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getpEmail().getAddress(), "alice.apdm@gmail.com");
        fructose_assert_eq(pAdmin.getpPatient().getpPerson().getpEmail().getType(), "private");
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getId(), "1234");
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getName(), "EHNV");
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getpAddress().getStreet(), "Street name 2");
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getpAddress().getPostCode(), 1400);
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getpAddress().getCity(), "Yverdon-les-Bains");
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getpAddress().getState(), "Vaud");
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getpAddress().getCountry(), "Suisse");
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getpPhone().getNumber(), "0123456789");
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getpPhone().getType(), "professional");
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getpEmail().getAddress(), "info@ehnv.com");
        fructose_assert_eq(pAdmin.getpPatient().getpInstitute().getpEmail().getType(), "professional");

        fructose_assert_eq(pAdmin.getpClinicalData().getData().find("note")->second, "random note");
    }

    /// \brief Load an xml file with no admin element
    /// \param _testName Name of the test.
    void retrieveNoAdmin(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/no_admin.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(optAdmin.has_value(), false);
    }


};

#endif // TEST_XPERTQUERYIMPORT_H
