#ifndef TEST_XPERTQUERYIMPORT_H
#define TEST_XPERTQUERYIMPORT_H

#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/query/xpertrequestdata.h"

#include "fructose/fructose.h"

/// \brief Tests for XpertQueryImport
/// \date 27/04/2022
/// \author Herzig Melvyn
struct TestXpertQueryImport : public fructose::test_base<TestXpertQueryImport>
{
    /// \brief Load an xml file with every possible data in admin node
    ///        and check if the received values are expected. There should be no import errors.
    /// \param _testName Name of the test.
    void xpertQueryImport_getsAllValues_withCompleteAdmin(const std::string& _testName);

    /// \brief Load an xml file with no admin element.
    ///        The admin data pointer must be nullptr and there should be no import errors.
    /// \param _testName Name of the test.
    void xpertQueryImport_getsNullptrAdminData_withNoAdminElement(const std::string& _testName);

    /// \brief Load an xml file with an admin element that has no mandator, no patient and no clinical data.
    ///        The AdminData pointer is not nullptr but its getters return nullptr. There should be no import errors.
    /// \param _testName Name of the test.
    void xpertQueryImport_adminDataGettersReturnNullptr_withEmptyAdminElement(const std::string& _testName);

    /// \brief Load an xml file with an admin element that has minimal person elements in
    ///        mandator and patient. There should be no import errors.
    ///        The missing values must be nullptr or empty string.
    /// \param _testName Name of the test.
    void xpertQueryImport_missingValuesAreNullptrOrEmptyString_withMinimalPersonInMandatorAndPatient(const std::string& _testName);

    /// \brief Load an xml file with an admin element that has minimal institutes elements
    ///        in mandator and patient elements. There should be no import errors.
    ///        The missing values must be nullptr or empty string.
    /// \param _testName Name of the test.
    void xpertQueryImport_missingValuesAreNullptrOrEmptyString_withMinimalInstituteInMandatorAndPatient(const std::string& _testName);

    /// \brief Load an xml file with an admin element that has address phone and email in institute
    ///        and person elements. There should be no import errors. The missing values must empty string.
    /// \param _testName Name of the test.
    void xpertQueryImport_missingValuesAreEmptyString_withMinimalAddressPhoneAndEmail(const std::string& _testName);


    /// \brief Load an xml file with an admin element but the mandatory values are missing in the mandator person.
    ///        There should be an import error. The error message contains all the elements missing.
    /// \param _testName Name of the test.
    void xpertQueryImport_importError_withMissingMandatoryValuesInCompleteMandatorPerson(const std::string& _testName);

    /// \brief Load an xml file with an admin element but the mandatory values are missing in the mandator institute.
    ///        There should be an import error. The error message contains all the elements missing.
    /// \param _testName Name of the test.
    void xpertQueryImport_importError_withMissingMandatoryValuesInCompleteMandatorInstitute(const std::string& _testName);

    /// \brief Load an xml file with an admin element but the mandatory values are missing in the patient person.
    ///        There should be an import error. The error message contains all the elements missing.
    /// \param _testName Name of the test.
    void xpertQueryImport_importError_withMissingMandatoryValuesInCompletePatientPerson(const std::string& _testName);

    /// \brief Load an xml file with an admin element but the mandatory values are missing in the patient institute.
    ///        There should be an import error. The error message contains all the elements missing.
    /// \param _testName Name of the test.
    void xpertQueryImport_importError_withMissingMandatoryValuesInCompletePatientInstitute(const std::string& _testName);

    /// \brief Load an xml file with every possible data in xpertRequest node
    ///        and check if the received values are expected. There should be no import errors.
    /// \param _testName Name of the test.
    void xpertQueryImport_getsAllValues_withCompleteXpertRequest(const std::string& _testName);

    /// \brief Load an xml file with the default data in an xpertRequest node
    ///        and check if the received values are expected. There should be no import errors.
    /// \param _testName Name of the test.
    void xpertQueryImport_getDefaultValues_withMinimalXpertRequest(const std::string& _testName);

    /// \brief Load an xml file without any xpertRequest and check that an error is returned.
    /// \param _testName Name of the test.
    void xpertQueryImport_importError_withoutXpertRequest(const std::string& _testName);

    /// \brief Load an xml file without mandatory values in the xpertRequest node and check
    ///        that error is returned. The error message tells what is missing.
    /// \param _testName Name of the test.
    void xpertQueryImport_importError_withMissingMandatoryInXpertRequest(const std::string& _testName);

    /// \brief Load an xml file not well formatted.
    ///        There is an import error and the import status is "CantCreateXmlDocument".
    /// \param _testName Name of the test.
    void xpertQueryImport_importError_withFileBadlyFormatted(const std::string& _testName);

    /// \brief Try to open a file that does not exist.
    ///        There is an import error and the import status is "CantOpenFile".
    /// \param _testName Name of the test.
    void xpertQueryImport_importError_withNonExistingFile(const std::string& _testName);
};

#endif // TEST_XPERTQUERYIMPORT_H
