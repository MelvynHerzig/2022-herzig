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
        const Tucuxi::XpertQuery::AdministrativeData& pAdmin = optAdmin.value();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);

        const Tucuxi::XpertQuery::PersonalContact& mandator = pAdmin.getpMandator()->get().getpPerson();
        const Tucuxi::XpertQuery::Address& mandatorAddress = mandator.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& mandatorPhone = mandator.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& mandatorEmail = mandator.getpEmail()->get();
        const Tucuxi::XpertQuery::InstituteContact& mandatorInstitute = pAdmin.getpMandator()->get().getpInstitute()->get();
        const Tucuxi::XpertQuery::Address& mandatorInstituteAddress = mandatorInstitute.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& mandatorInstitutePhone = mandatorInstitute.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& mandatorInstituteEmail = mandatorInstitute.getpEmail()->get();

        fructose_assert_eq(mandator.getId(), "asdf");
        fructose_assert_eq(mandator.getTitle(), "Dr.");
        fructose_assert_eq(mandator.getFirstName(), "John");
        fructose_assert_eq(mandator.getLastName(), "Doe");
        fructose_assert_eq(mandatorAddress.getStreet(), "Av. de l'Ours 2");
        fructose_assert_eq(mandatorAddress.getPostCode(), 1010);
        fructose_assert_eq(mandatorAddress.getCity(), "Lausanne");
        fructose_assert_eq(mandatorAddress.getState(), "Vaud");
        fructose_assert_eq(mandatorAddress.getCountry(), "Suisse");
        fructose_assert_eq(mandatorPhone.getNumber(), "0213140002");
        fructose_assert_eq(mandatorPhone.getType(), "professional");
        fructose_assert_eq(mandatorEmail.getAddress(), "john.doe@chuv.com");
        fructose_assert_eq(mandatorEmail.getType(), "professional");
        fructose_assert_eq(mandatorInstitute.getId(), "456789");
        fructose_assert_eq(mandatorInstitute.getName(), "CHUV");
        fructose_assert_eq(mandatorInstituteAddress.getStreet(), "Av. de l'Ours 1");
        fructose_assert_eq(mandatorInstituteAddress.getPostCode(), 1010);
        fructose_assert_eq(mandatorInstituteAddress.getCity(), "Lausanne");
        fructose_assert_eq(mandatorInstituteAddress.getState(), "Vaud");
        fructose_assert_eq(mandatorInstituteAddress.getCountry(), "Suisse");
        fructose_assert_eq(mandatorInstitutePhone.getNumber(), "0213140001");
        fructose_assert_eq(mandatorInstitutePhone.getType(), "professional");
        fructose_assert_eq(mandatorInstituteEmail.getAddress(), "info@chuv.com");
        fructose_assert_eq(mandatorInstituteEmail.getType(), "professional");

        const Tucuxi::XpertQuery::PersonalContact& patient = pAdmin.getpPatient()->get().getpPerson();
        const Tucuxi::XpertQuery::Address& patientAddress = patient.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& patientPhone = patient.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& patientEmail = patient.getpEmail()->get();
        const Tucuxi::XpertQuery::InstituteContact& patientInstitute = pAdmin.getpPatient()->get().getpInstitute()->get();
        const Tucuxi::XpertQuery::Address& patientInstituteAddress = patientInstitute.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& patientInstitutePhone = patientInstitute.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& patientInstituteEmail = patientInstitute.getpEmail()->get();

        fructose_assert_eq(patient.getId(), "123456");
        fructose_assert_eq(patient.getFirstName(), "Alice");
        fructose_assert_eq(patient.getLastName(), "Aupaysdesmerveilles");
        fructose_assert_eq(patientAddress.getStreet(), "Av. d'Ouchy 27");
        fructose_assert_eq(patientAddress.getPostCode(), 1006);
        fructose_assert_eq(patientAddress.getCity(), "Lausanne");
        fructose_assert_eq(patientAddress.getState(), "Vaud");
        fructose_assert_eq(patientAddress.getCountry(), "Suisse");
        fructose_assert_eq(patientPhone.getNumber(), "0216170002");
        fructose_assert_eq(patientPhone.getType(), "professional");
        fructose_assert_eq(patientEmail.getAddress(), "alice.apdm@gmail.com");
        fructose_assert_eq(patientEmail.getType(), "private");
        fructose_assert_eq(patientInstitute.getId(), "1234");
        fructose_assert_eq(patientInstitute.getName(), "EHNV");
        fructose_assert_eq(patientInstituteAddress.getStreet(), "Street name 2");
        fructose_assert_eq(patientInstituteAddress.getPostCode(), 1400);
        fructose_assert_eq(patientInstituteAddress.getCity(), "Yverdon-les-Bains");
        fructose_assert_eq(patientInstituteAddress.getState(), "Vaud");
        fructose_assert_eq(patientInstituteAddress.getCountry(), "Suisse");
        fructose_assert_eq(patientInstitutePhone.getNumber(), "0123456789");
        fructose_assert_eq(patientInstitutePhone.getType(), "professional");
        fructose_assert_eq(patientInstituteEmail.getAddress(), "info@ehnv.com");
        fructose_assert_eq(patientInstituteEmail.getType(), "professional");

        fructose_assert_eq(pAdmin.getpClinicalData()->get().getData().find("note")->second, "random note");
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

    /// \brief Load an xml file with admin but no mandator, no patient and no clinical data.
    /// \param _testName Name of the test.
    void retrieveEmptyAdmin(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/empty_admin.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(optAdmin.has_value(), true);

        const Tucuxi::XpertQuery::AdministrativeData& pAdmin = optAdmin.value();

        fructose_assert_eq(pAdmin.getpMandator().has_value(), false);
        fructose_assert_eq(pAdmin.getpPatient().has_value(), false);
        fructose_assert_eq(pAdmin.getpClinicalData().has_value(), false);
    }

    /// \brief Load an xml file with admin and minimal persons element
    /// \param _testName Name of the test.
    void retrieveMinimalPerson(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/minimal_person.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(optAdmin.has_value(), true);

        const Tucuxi::XpertQuery::AdministrativeData& pAdmin = optAdmin.value();
        const Tucuxi::XpertQuery::PersonalContact& mandator = pAdmin.getpMandator()->get().getpPerson();
        const Tucuxi::XpertQuery::PersonalContact& patient = pAdmin.getpPatient()->get().getpPerson();


        fructose_assert_eq(mandator.getId(), "");
        fructose_assert_eq(mandator.getTitle(), "");
        fructose_assert_eq(mandator.getpAddress().has_value(), false);
        fructose_assert_eq(mandator.getpPhone().has_value(), false);
        fructose_assert_eq(mandator.getpEmail().has_value(), false);
        fructose_assert_eq(pAdmin.getpMandator()->get().getpInstitute().has_value(), false);

        fructose_assert_eq(patient.getId(), "");
        fructose_assert_eq(patient.getTitle(), "");
        fructose_assert_eq(patient.getpAddress().has_value(), false);
        fructose_assert_eq(patient.getpPhone().has_value(), false);
        fructose_assert_eq(patient.getpEmail().has_value(), false);
        fructose_assert_eq(pAdmin.getpPatient()->get().getpInstitute().has_value(), false);
    }

    /// \brief Load an xml file with admin and minimal institutes elements
    /// \param _testName Name of the test.
    void retrieveMinimalInstitute(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/minimal_institute.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(optAdmin.has_value(), true);

        const Tucuxi::XpertQuery::AdministrativeData& pAdmin = optAdmin.value();
        const Tucuxi::XpertQuery::InstituteContact& mandatorInstitute = pAdmin.getpMandator()->get().getpInstitute()->get();
        const Tucuxi::XpertQuery::InstituteContact& patientInstitute = pAdmin.getpPatient()->get().getpInstitute()->get();


        fructose_assert_eq(mandatorInstitute.getId(), "");
        fructose_assert_eq(mandatorInstitute.getpAddress().has_value(), false);
        fructose_assert_eq(mandatorInstitute.getpPhone().has_value(), false);
        fructose_assert_eq(mandatorInstitute.getpEmail().has_value(), false);

        fructose_assert_eq(patientInstitute.getId(), "");
        fructose_assert_eq(patientInstitute.getpAddress().has_value(), false);
        fructose_assert_eq(patientInstitute.getpPhone().has_value(), false);
        fructose_assert_eq(patientInstitute.getpEmail().has_value(), false);
    }

    /// \brief Load an xml file with admin and minimal coordinates elements (address, phone and email)
    /// \param _testName Name of the test.
    void retrieveMinimalCoordinates(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/minimal_coordinates.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        auto optAdmin = query->getpAdministrative();
        const Tucuxi::XpertQuery::AdministrativeData& pAdmin = optAdmin.value();

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Ok);
        fructose_assert_eq(optAdmin.has_value(), true);

        const Tucuxi::XpertQuery::PersonalContact& mandator = pAdmin.getpMandator()->get().getpPerson();
        const Tucuxi::XpertQuery::Address& mandatorAddress = mandator.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& mandatorPhone = mandator.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& mandatorEmail = mandator.getpEmail()->get();
        const Tucuxi::XpertQuery::InstituteContact& mandatorInstitute = pAdmin.getpMandator()->get().getpInstitute()->get();
        const Tucuxi::XpertQuery::Address& mandatorInstituteAddress = mandatorInstitute.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& mandatorInstitutePhone = mandatorInstitute.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& mandatorInstituteEmail = mandatorInstitute.getpEmail()->get();

        const Tucuxi::XpertQuery::PersonalContact& patient = pAdmin.getpPatient()->get().getpPerson();
        const Tucuxi::XpertQuery::Address& patientAddress = patient.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& patientPhone = patient.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& patientEmail = patient.getpEmail()->get();
        const Tucuxi::XpertQuery::InstituteContact& patientInstitute = pAdmin.getpPatient()->get().getpInstitute()->get();
        const Tucuxi::XpertQuery::Address& patientInstituteAddress = patientInstitute.getpAddress()->get();
        const Tucuxi::XpertQuery::Phone& patientInstitutePhone = patientInstitute.getpPhone()->get();
        const Tucuxi::XpertQuery::Email& patientInstituteEmail = patientInstitute.getpEmail()->get();

        fructose_assert_eq(mandatorAddress.getState(), "");
        fructose_assert_eq(mandatorInstituteAddress.getState(), "");
        fructose_assert_eq(patientAddress.getState(), "");
        fructose_assert_eq(patientInstituteAddress.getState(), "");

        fructose_assert_eq(mandatorAddress.getCountry(), "");
        fructose_assert_eq(mandatorInstituteAddress.getCountry(), "");
        fructose_assert_eq(patientAddress.getCountry(), "");
        fructose_assert_eq(patientInstituteAddress.getCountry(), "");

        fructose_assert_eq(mandatorPhone.getType(), "");
        fructose_assert_eq(mandatorInstitutePhone.getType(), "");
        fructose_assert_eq(patientPhone.getType(), "");
        fructose_assert_eq(patientInstitutePhone.getType(), "");

        fructose_assert_eq(mandatorEmail.getType(), "");
        fructose_assert_eq(mandatorInstituteEmail.getType(), "");
        fructose_assert_eq(patientEmail.getType(), "");
        fructose_assert_eq(patientInstituteEmail.getType(), "");
    }

    /// \brief Load an xml file with admin but mandatory values are missing.
    /// \param _testName Name of the test.
    void errorWhenMissingMandatoryInMandatorPerson(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/missing_mandatory_mandator_person.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("firstName"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("lastName"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("street"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("postCode"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("city"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("number"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("address"), std::string::npos);
    }

    /// \brief Load an xml file with admin but mandatory values are missing.
    /// \param _testName Name of the test.
    void errorWhenMissingMandatoryInMandatorInstitute(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/missing_mandatory_mandator_institute.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("name"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("street"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("postCode"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("city"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("number"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("address"), std::string::npos);
    }

    /// \brief Load an xml file with admin but mandatory values are missing.
    /// \param _testName Name of the test.
    void errorWhenMissingMandatoryInPatientPerson(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/missing_mandatory_patient_person.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("firstName"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("lastName"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("street"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("postCode"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("city"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("number"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("address"), std::string::npos);
    }

    /// \brief Load an xml file with admin but mandatory values are missing.
    /// \param _testName Name of the test.
    void errorWhenMissingMandatoryInPatientInstitute(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::ifstream ifs("../test/query/missing_mandatory_patient_institute.tqf");
        std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, xmlString);

        fructose_assert_eq(importResult, Tucuxi::XpertQuery::XpertQueryImport::Status::Error);
        fructose_assert_ne(importer.getErrorMessage().find("name"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("street"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("postCode"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("city"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("number"), std::string::npos);
        fructose_assert_ne(importer.getErrorMessage().find("address"), std::string::npos);
    }
};

#endif // TEST_XPERTQUERYIMPORT_H
