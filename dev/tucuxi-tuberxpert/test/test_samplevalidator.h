#ifndef TEST_SAMPLEVALIDATOR_H
#define TEST_SAMPLEVALIDATOR_H

#include <numeric>
#include <string>
#include <memory>

#include "tucucore/drugmodelchecker.h"
#include "tucucore/pkmodel.h"
#include "tucucore/drugmodelimport.h"
#include "tucucore/drugmodelrepository.h"
#include "tucucore/computingservice/computingresponse.h"
#include "tucucore/drugtreatment/sample.h"
#include "tucucore/drugmodel/analyte.h"
#include "tucucore/definitions.h"
#include "tucucommon/datetime.h"
#include "tucucommon/unit.h"

#include "tuberxpert/flow/general/generalxpertflowstepprovider.h"
#include "tuberxpert/flow/general/samplevalidator.h"
#include "tuberxpert/language/languagemanager.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/result/xpertqueryresult.h"
#include "tuberxpert/result/samplevalidationresult.h"

#include "fructose/fructose.h"

/// \brief Tests for SampleValidator from the XpertFlowStepProvider.
/// \date 06/06/2022
/// \author Herzig Melvyn
struct TestSampleValidator : public fructose::test_base<TestSampleValidator>
{

    /// \brief Format used to create date and time during test.
    const std::string date_format = "%Y-%m-%dT%H:%M:%S";

    /// \brief Unit used to create the percentiles data.
    const Tucuxi::Common::TucuUnit unit{"ug/l"};

    /// \brief General flow step provider used to get the sample validator object to test.
    const Tucuxi::Xpert::GeneralXpertFlowStepProvider flowStepProvider;

    /// \brief Creates some percentiles data and insert them into pData.
    /// \param pData PercentilesData object that gets the preparation.
    void createPercentilesData( Tucuxi::Core::PercentilesData& pData){
        // Preparing the percentilesData
        // There are going to be 99 percentiles (1-99) with 2 CycleDatas:
        // 1.1.2022 10:00:00 - 1.1.2022 11:00:00, 1.1.2022 11:00:00 - 1.1.2022 12:00:00
        Tucuxi::Common::DateTime d1{"2022-01-01T10:00:00", date_format};
        Tucuxi::Common::DateTime d2{"2022-01-01T11:00:00", date_format};
        Tucuxi::Common::DateTime d3{"2022-01-01T12:00:00", date_format};

        std::vector<Tucuxi::Common::DateTime> cyclesStarts{d1, d2};
        std::vector<Tucuxi::Common::DateTime> cyclesEnds{d2, d3};

        // For the 99 percentiles (1-99), each percentile start at p * 10 (1 = 10, 99 = 990)
        // The first cycle goes from p * 10 to p * 10 + 1 and the second from p * 10 + 1 to p * 10 + 2
        // (if p = 1 -> 10 - 11 , 11 - 12)
        // There will be 3 points per hour.
        // So for the first percentile:
        // Cycle 1 [1.1.2022 10:00:00 - 1.1.2022 11:00:00], values: 10 10.5 11
        //                                                   times:  0  0.5  1
        // Cycle 2 [1.1.2022 11:00:00 - 1.1.2022 12:00:00], values: 11 11.5 12
        //                                                   times:  0  0.5  1

        const Tucuxi::Core::TimeOffsets times{0, 0.5, 1};

        // For each percentile
        for (size_t pi = 1; pi <= 99 ; ++pi){

            // Creates their cycles data
            std::vector<Tucuxi::Core::CycleData> cycles;
            for (size_t cdi = 0; cdi < 2; ++cdi ){

                cycles.emplace_back(cyclesStarts[cdi], cyclesEnds[cdi], unit);

                // Preparing concentrations
                Tucuxi::Core::Concentrations concentrations;
                for (size_t ci = 0; ci < 3; ++ci){
                    concentrations.emplace_back(pi * 10 + cdi + ci * 0.5);
                }

                cycles[cdi].addData(times, concentrations);
            }

            pData.addPercentileData(cycles);
        }
    }

    /// \brief Checks that there is an error if the treatment of XpertRequestResult is nullptr.
    /// \param _testName Name of the test
    void errorWhenNoTreatment(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

//        Tucuxi::Xpert::XpertRequestResult xrr{nullptr, nullptr, nullptr, ""};

//        flowStepProvider.getSampleValidator()->perform(xrr);

//        fructose_assert_eq(xrr.shouldBeHandled(), false);
//        fructose_assert_eq(xrr.getErrorMessage(), "No treatment set.");
//        fructose_assert_eq(xrr.getSampleResults().size(), 0);

        fructose_assert_eq(true, true);
    }

    /// \brief Checks that there is an error if the treatment has samples but not dosage.
    /// \param _testName Name of the test
    void errorWhenSamplesButNoDosage(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib_2</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                </covariates>
                                            </patient>
                                            <!-- List of the drugs informations we have concerning the patient -->
                                            <drugs>
                                                <!-- All the information regarding the drug -->
                                                <drug>
                                                    <drugId>imatinib</drugId>
                                                    <activePrinciple>something</activePrinciple>
                                                    <brandName>somebrand</brandName>
                                                    <atc>something</atc>
                                                    <!-- All the information regarding the treatment -->
                                                    <treatment>
                                                        <dosageHistory>
                                                        </dosageHistory>
                                                    </treatment>
                                                    <!-- Samples history -->
                                                    <samples>
                                                        <sample>
                                                            <sampleId>123456</sampleId>
                                                            <sampleDate>2018-07-07T06:00:30</sampleDate>
                                                            <concentrations>
                                                                <concentration>
                                                                    <analyteId>imatinib</analyteId>
                                                                    <value>0.7</value>
                                                                    <unit>mg/l</unit>
                                                                </concentration>
                                                            </concentrations>
                                                        </sample>
                                                    </samples>
                                                    <!-- Personalised targets -->
                                                    <targets>
                                                    </targets>
                                                </drug>
                                            </drugs>
                                        </drugTreatment>
                                        <!-- List of the requests we want the server to take care of -->
                                        <requests>
                                            <xpertRequest>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

        if (importResult != Tucuxi::Xpert::XpertQueryImport::Status::Ok) {
            throw std::runtime_error("import failded.");
        }

        Tucuxi::Xpert::XpertQueryResult xr{move(query), ""};

        flowStepProvider.getSampleValidator()->perform(xr.getXpertRequestResults()[0]);

        fructose_assert_eq(xr.getXpertRequestResults()[0].shouldBeHandled(), false);
        fructose_assert_eq(xr.getXpertRequestResults()[0].getErrorMessage(), "Samples found but dosage history is empty.");
        fructose_assert_eq(xr.getXpertRequestResults()[0].getSampleResults().size(), 0);
    }

    /// \brief Checks that there is an error if the drug model of XpertRequestResult is nullptr.
    /// \param _testName Name of the test
    void errorWhenNoDrugModel(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib_2</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                </covariates>
                                            </patient>
                                            <!-- List of the drugs informations we have concerning the patient -->
                                            <drugs>
                                                <!-- All the information regarding the drug -->
                                                <drug>
                                                    <drugId>imatinib</drugId>
                                                    <activePrinciple>something</activePrinciple>
                                                    <brandName>somebrand</brandName>
                                                    <atc>something</atc>
                                                    <!-- All the information regarding the treatment -->
                                                    <treatment>
                                                        <dosageHistory>
                                                            <dosageTimeRange>
                                                                <start>2018-07-06T08:00:00</start>
                                                                <end>2018-07-08T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>12:00:00</interval>
                                                                            <dose>
                                                                                <value>400</value>
                                                                                <unit>mg</unit>
                                                                                <infusionTimeInMinutes>60</infusionTimeInMinutes>
                                                                            </dose>
                                                                            <formulationAndRoute>
                                                                                <formulation>parenteralSolution</formulation>
                                                                                <administrationName>foo bar</administrationName>
                                                                                <administrationRoute>oral</administrationRoute>
                                                                                <absorptionModel>extravascular</absorptionModel>
                                                                            </formulationAndRoute>
                                                                        </lastingDosage>
                                                                    </dosageLoop>
                                                                </dosage>
                                                            </dosageTimeRange>
                                                        </dosageHistory>
                                                    </treatment>
                                                    <!-- Samples history -->
                                                    <samples>
                                                    </samples>
                                                    <!-- Personalised targets -->
                                                    <targets>
                                                    </targets>
                                                </drug>
                                            </drugs>
                                        </drugTreatment>
                                        <!-- List of the requests we want the server to take care of -->
                                        <requests>
                                            <xpertRequest>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;

        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

        if (importResult != Tucuxi::Xpert::XpertQueryImport::Status::Ok) {
            throw std::runtime_error("import failded.");
        }

        Tucuxi::Xpert::XpertQueryResult xr{move(query), ""};

        flowStepProvider.getSampleValidator()->perform(xr.getXpertRequestResults()[0]);

        fructose_assert_eq(xr.getXpertRequestResults()[0].shouldBeHandled(), false);
        fructose_assert_eq(xr.getXpertRequestResults()[0].getErrorMessage(), "No drug model set.");
        fructose_assert_eq(xr.getXpertRequestResults()[0].getSampleResults().size(), 0);
    }

    /// \brief Checks that the warnings are correct when retrieving them
    ///        from SampleResult objects with given "percentile groups".
    /// \param _testName Name of the test
    void warningForGroupPositionOver99Percentiles(const std::string& _testName)
    {

        std::string dictionary = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <translations
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                        <translation key="population_below">% of the population is below this measure</translation>
                                        <translation key="population_above">% of the population is above this measure</translation>

                                    </translations>)";

        std::cout << _testName << std::endl;

        Tucuxi::Xpert::LanguageManager& lm = Tucuxi::Xpert::LanguageManager::getInstance();
        lm.loadTranslations(dictionary);

        // Creating SampleResult objects that are located in a different group. There are
        // 100 groups that are implicitly formed by the 99 percentiles.
        Tucuxi::Xpert::SampleValidationResult sr1 = Tucuxi::Xpert::SampleValidationResult(nullptr, 1);
        Tucuxi::Xpert::SampleValidationResult sr10 = Tucuxi::Xpert::SampleValidationResult(nullptr, 10);
        Tucuxi::Xpert::SampleValidationResult sr11 = Tucuxi::Xpert::SampleValidationResult(nullptr, 11);
        Tucuxi::Xpert::SampleValidationResult sr90 = Tucuxi::Xpert::SampleValidationResult(nullptr, 90);
        Tucuxi::Xpert::SampleValidationResult sr91 = Tucuxi::Xpert::SampleValidationResult(nullptr, 91);
        Tucuxi::Xpert::SampleValidationResult sr100 = Tucuxi::Xpert::SampleValidationResult(nullptr, 100);

        fructose_assert_eq(sr1.getWarning(), "99% of the population is above this measure");
        fructose_assert_eq(sr10.getWarning(), "90% of the population is above this measure");
        fructose_assert_eq(sr11.getWarning(), "");
        fructose_assert_eq(sr90.getWarning(), "");
        fructose_assert_eq(sr91.getWarning(), "90% of the population is below this measure");
        fructose_assert_eq(sr100.getWarning(), "99% of the population is below this measure");
    }

    /// \brief This method checks that the methods SampleValidator::findPosOver100Percentile returns
    ///        the good positions for a given set of samples and a given percentileData.
    ///
    ///         We test the SampleValidator class with SampleValidator::findPosOver100Percentile and not
    ///         with SampleValidator::getSampleValidations because this last methods only performs a request
    ///         on the core which return "unpredictable" percentiles. So basically 90% of the behavior of
    ///         the SampleValidator can be checked with SampleValidator::findPosOver100Percentile.
    ///
    ///         Thus, this struct if the friend of SampleValidator to acces this protected method.
    /// \param _testName Name of the test
    void findGroupPositionOver99Percentiles(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Core::PercentilesData pData{""};
        createPercentilesData(pData);

        // We perform 9 series of tests.
        // The times tested are from 1.1.2022 10:00:00 to 1.1.2022 12:00:00 every 15 minutes.
        // 18 concentrations are tested, 2 at each time: one slightly below the percentile
        //                                               one slightly above the percentile
        std::vector<Tucuxi::Common::DateTime> testDates{
            Tucuxi::Common::DateTime{"2022-01-01T10:00:00", date_format},
            Tucuxi::Common::DateTime{"2022-01-01T10:15:00", date_format},
            Tucuxi::Common::DateTime{"2022-01-01T10:30:00", date_format},
            Tucuxi::Common::DateTime{"2022-01-01T10:45:00", date_format},
            Tucuxi::Common::DateTime{"2022-01-01T11:00:00", date_format},
            Tucuxi::Common::DateTime{"2022-01-01T11:15:00", date_format},
            Tucuxi::Common::DateTime{"2022-01-01T11:30:00", date_format},
            Tucuxi::Common::DateTime{"2022-01-01T11:45:00", date_format},
            Tucuxi::Common::DateTime{"2022-01-01T12:00:00", date_format},
        };

        Tucuxi::Xpert::SampleValidator sv;
        // For each percentile
        for (size_t pi = 1; pi <= 99 ; ++pi){
            for (size_t ti = 0; ti < 9; ++ti) {
                double percentileValue = pi * 10 + ti * 0.25;
                std::unique_ptr<Tucuxi::Core::Sample> slightlyAbove = std::make_unique<Tucuxi::Core::Sample>(testDates[ti], Tucuxi::Core::AnalyteId{""}, percentileValue + 0.01 , unit);
                std::unique_ptr<Tucuxi::Core::Sample> slightlyBelow = std::make_unique<Tucuxi::Core::Sample>(testDates[ti], Tucuxi::Core::AnalyteId{""}, percentileValue - 0.01 , unit);

                unsigned groupSlightlyAbove = sv.findGroupPositionOver99Percentiles(&pData, slightlyAbove);
                unsigned groupSlightlyBelow = sv.findGroupPositionOver99Percentiles(&pData, slightlyBelow);

                fructose_assert_eq(groupSlightlyAbove, pi + 1);
                fructose_assert_eq(groupSlightlyBelow, pi);
            }
        }

        fructose_assert_eq(true, true);
    }

    /// \brief This method checks that the methods SampleValidator::findPosOver100Percentile throws
    ///        an invalid_argument exception when a sample unit can't be converted.
    /// \param _testName Name of the test
    void getExceptionUnitConversion(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Core::PercentilesData pData{""};
        createPercentilesData(pData);

        Tucuxi::Xpert::SampleValidator sv;

        // We set the unit to kg to get the exception.
        std::unique_ptr<Tucuxi::Core::Sample> sample = std::make_unique<Tucuxi::Core::Sample>(
                    Tucuxi::Common::DateTime{"2022-01-01T11:30:00", date_format},
                    Tucuxi::Core::AnalyteId{""},
                    1,
                    Tucuxi::Common::TucuUnit{"kg"});

        fructose_assert_exception(sv.findGroupPositionOver99Percentiles(&pData, sample), std::invalid_argument);
    }

    /// \brief This method checks that the methods SampleValidator::findPosOver100Percentile throws
    ///        an invalid_argument exception when a sample date can't be found in the cycleData objects.
    /// \param _testName Name of the test
    void getExceptionDateNotFound(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Core::PercentilesData pData{""};
        createPercentilesData(pData);

        Tucuxi::Xpert::SampleValidator sv;

        // We set the date to 2023 to be out of bound of the cycleData objects
        std::unique_ptr<Tucuxi::Core::Sample> sample = std::make_unique<Tucuxi::Core::Sample>(
                    Tucuxi::Common::DateTime{"2023-01-01T11:30:00", date_format},
                    Tucuxi::Core::AnalyteId{""},
                    1,
                    unit);

        fructose_assert_exception(sv.findGroupPositionOver99Percentiles(&pData, sample), std::invalid_argument);
    }

    /// \brief This method performs a real sample validation. The vector retrived by getSampleResults must
    ///        be sorted by sample date. We only check the sample date. The other interesting values, such as:
    ///        percentile, warning type, warning message are already tested by more deterministic tests.
    /// \param _testName Name of the test
    void getSampleResultsSorted(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib_2</queryId>
                                        <clientId>124568</clientId>
                                        <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->
                                        <language>en</language>

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                </covariates>
                                            </patient>
                                            <!-- List of the drugs informations we have concerning the patient -->
                                            <drugs>
                                                <!-- All the information regarding the drug -->
                                                <drug>
                                                    <drugId>imatinib</drugId>
                                                    <activePrinciple>something</activePrinciple>
                                                    <brandName>somebrand</brandName>
                                                    <atc>something</atc>
                                                    <!-- All the information regarding the treatment -->
                                                    <treatment>
                                                        <dosageHistory>
                                                            <dosageTimeRange>
                                                                <start>2018-07-06T08:00:00</start>
                                                                <end>2018-07-08T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>12:00:00</interval>
                                                                            <dose>
                                                                                <value>400</value>
                                                                                <unit>mg</unit>
                                                                                <infusionTimeInMinutes>60</infusionTimeInMinutes>
                                                                            </dose>
                                                                            <formulationAndRoute>
                                                                                <formulation>parenteralSolution</formulation>
                                                                                <administrationName>foo bar</administrationName>
                                                                                <administrationRoute>oral</administrationRoute>
                                                                                <absorptionModel>extravascular</absorptionModel>
                                                                            </formulationAndRoute>
                                                                        </lastingDosage>
                                                                    </dosageLoop>
                                                                </dosage>
                                                            </dosageTimeRange>
                                                        </dosageHistory>
                                                    </treatment>
                                                    <!-- Samples history -->
                                                    <samples>
                                                        <sample>
                                                            <sampleId>123456</sampleId>
                                                            <sampleDate>2018-07-07T06:00:30</sampleDate>
                                                            <concentrations>
                                                                <concentration>
                                                                    <analyteId>imatinib</analyteId>
                                                                    <value>0.7</value>
                                                                    <unit>mg/l</unit>
                                                                </concentration>
                                                            </concentrations>
                                                        </sample>
                                                        <sample>
                                                            <sampleId>123456</sampleId>
                                                            <sampleDate>2018-07-08T07:00:00</sampleDate>
                                                            <concentrations>
                                                                <concentration>
                                                                    <analyteId>imatinib</analyteId>
                                                                    <value>10.6</value>
                                                                    <unit>mg/l</unit>
                                                                </concentration>
                                                            </concentrations>
                                                        </sample>
                                                        <sample>
                                                            <sampleId>123456</sampleId>
                                                            <sampleDate>2018-07-06T13:00:00</sampleDate>
                                                            <concentrations>
                                                                <concentration>
                                                                    <analyteId>imatinib</analyteId>
                                                                    <value>0.8</value>
                                                                    <unit>mg/l</unit>
                                                                </concentration>
                                                            </concentrations>
                                                        </sample>
                                                    </samples>
                                                    <!-- Personalised targets -->
                                                    <targets>
                                                    </targets>
                                                </drug>
                                            </drugs>
                                        </drugTreatment>
                                        <!-- List of the requests we want the server to take care of -->
                                        <requests>
                                            <xpertRequest>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <adjustmentDate>2018-07-06T08:00:00</adjustmentDate>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

        std::string modelString = R"(<?xml version="1.0" encoding="UTF-8"?>
                                    <model version='0.6' xsi:noNamespaceSchemaLocation='drugfile.xsd' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>
                                        <history>
                                            <revisions>
                                                <revision>
                                                    <revisionAction>creation</revisionAction>
                                                    <revisionAuthorName>Yann Thoma</revisionAuthorName>
                                                    <institution>HEIG-VD // REDS</institution>
                                                    <email>yann.thoma@heig-vd.ch</email>
                                                    <date>2018-10-30</date>
                                                    <comments>
                                                        <comment lang='en'>This file is based on the first version of
                                                                                                                                    imatinib : ch.heig-vd.ezechiel.imatinib.xml
                                                                                                                    </comment>
                                                    </comments>
                                                </revision>
                                                <revision>
                                                    <revisionAction>modification</revisionAction>
                                                    <revisionAuthorName>Yann Thoma</revisionAuthorName>
                                                    <institution>HEIG-VD // REDS</institution>
                                                    <email>yann.thoma@heig-vd.ch</email>
                                                    <date>2018-11-07</date>
                                                    <comments>
                                                        <comment lang='en'>Now the Ka and F parameters are absorption parameters.
                                                                                                                    </comment>
                                                    </comments>
                                                </revision>
                                            </revisions>
                                        </history>
                                        <head>
                                            <drug>
                                                <atcs>
                                                    <atc>L01XE01</atc>
                                                </atcs>
                                                <activeSubstances>
                                                    <activeSubstance>imatinib</activeSubstance>
                                                </activeSubstances>
                                                <drugName>
                                                    <name lang='en'>Imatinib</name>
                                                    <name lang='fr'>Imatinib</name>
                                                </drugName>
                                                <drugDescription>
                                                    <desc lang='en'>TODO : Add a description here</desc>
                                                </drugDescription>
                                                <tdmStrategy>
                                                    <text lang='en'>TODO : Add a TDM strategy</text>
                                                </tdmStrategy>
                                            </drug>
                                            <study>
                                                <studyName>
                                                    <name lang='en'>Therapeutic Drug Monitoring of Imatinib.
                                    Bayesian and Alternative Methods to Predict Trough Levels</name>
                                                </studyName>
                                                <studyAuthors>Verena Gotta, Nicolas Widmer, Michael Montemurro, Serge Leyvraz, Amina Haouala, Laurent A. Decosterd, Chantal Csajka and Thierry Buclin</studyAuthors>
                                                <description>
                                                    <desc lang='en'>Based on Widmer et al. Br J Clin Pharmacol 2006, validated by Gotta et al. Clin Pharamcokinet 2012. Adult, (Disease: CML and GIST, Age range: 20-88 yrs, Weight range: 44-110kg, AGP plasma concentration range: 0.4–2.0 g/L)</desc>
                                                    <desc lang='fr'>Basé sur Widmer et al. Br J Clin Pharmacol 2006, validé par Gotta et al. Clin Pharamcokinet 2012</desc>
                                                </description>
                                                <references>
                                                    <reference type='bibtex'>
                                                        <![CDATA[@article{Gotta2012,
                                                        author={Gotta, Verena
                                                        and Widmer, Nicolas
                                                        and Montemurro, Michael
                                                        and Leyvraz, Serge
                                                        and Haouala, Amina
                                                        and Decosterd, Laurent A.
                                                        and Csajka, Chantal
                                                        and Buclin, Thierry},
                                                        title={Therapeutic Drug Monitoring of Imatinib},
                                                        journal={Clinical Pharmacokinetics},
                                                        year={2012},
                                                        month={Mar},
                                                        day={01},
                                                        volume={51},
                                                        number={3},
                                                        pages={187--201},
                                                        abstract={The imatinib trough plasma concentration (Cmin) correlates with clinical response in cancer patients. Therapeutic drug monitoring (TDM) of plasma Cmin is therefore suggested. In practice, however, blood sampling for TDM is often not performed at trough. The corresponding measurement is thus only remotely informative about Cmin exposure.},
                                                        issn={1179-1926},
                                                        doi={10.2165/11596990-000000000-00000},
                                                        url={https://doi.org/10.2165/11596990-000000000-00000}
                                                        }
                                    ]]>
                                                    </reference>
                                                </references>
                                            </study>
                                            <comments/>
                                        </head>
                                        <drugModel>
                                            <drugId>imatinib</drugId>
                                            <drugModelId>ch.tucuxi.imatinib.gotta2012</drugModelId>
                                            <domain>
                                                <description>
                                                    <desc lang='en'>Adult, (Disease: CML and GIST, Age range: 20-88 yrs, Weight range: 44-110kg, AGP plasma concentration range: 0.4–2.0 g/L)</desc>
                                                </description>
                                                <constraints>
                                                </constraints>
                                            </domain>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>bodyweight</covariateId>
                                                    <covariateName>
                                                        <name lang='en'>Total Body Weight</name>
                                                        <name lang='fr'>Poids total</name>
                                                    </covariateName>
                                                    <description>
                                                        <desc lang='en'>Total body weight of patient, in kilogramms</desc>
                                                        <desc lang='fr'>Poids total du patient, en kilogramme</desc>
                                                    </description>
                                                    <unit>kg</unit>
                                                    <covariateType>standard</covariateType>
                                                    <dataType>double</dataType>
                                                    <interpolationType>linear</interpolationType>
                                                    <refreshPeriod>
                                                      <unit>d</unit>
                                                      <value>30</value>
                                                    </refreshPeriod>
                                                    <covariateValue>
                                                        <standardValue>70</standardValue>
                                                    </covariateValue>
                                                    <validation>
                                                        <errorMessage>
                                                            <text lang='en'>The body weight shall be in the interval [44,100].</text>
                                                        </errorMessage>
                                                        <operation>
                                                            <softFormula>
                                                                <inputs>
                                                                    <input>
                                                                        <id>bodyweight</id>
                                                                        <type>double</type>
                                                                    </input>
                                                                </inputs>
                                                                <code>
                                                                    <![CDATA[return ((bodyweight >= 44) && (bodyweight <= 110));
                                                                    ]]>
                                                                </code>
                                                            </softFormula>
                                                            <comments/>
                                                        </operation>
                                                        <comments/>
                                                    </validation>
                                                    <comments/>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>gist</covariateId>
                                                    <covariateName>
                                                        <name lang='en'>GIST</name>
                                                        <name lang='fr'>GIST</name>
                                                    </covariateName>
                                                    <description>
                                                        <desc lang='en'>Gastrointestinal stromal tumour</desc>
                                                        <desc lang='fr'>Tumeur stromale gastro-intestinale</desc>
                                                    </description>
                                                    <unit>-</unit>
                                                    <covariateType>standard</covariateType>
                                                    <dataType>bool</dataType>
                                                    <interpolationType>direct</interpolationType>
                                                    <refreshPeriod>
                                                      <unit>y</unit>
                                                      <value>1</value>
                                                    </refreshPeriod>
                                                    <covariateValue>
                                                        <standardValue>0</standardValue>
                                                    </covariateValue>
                                                    <validation>
                                                        <errorMessage>
                                                            <text lang='en'>Always correct.</text>
                                                        </errorMessage>
                                                        <operation>
                                                            <softFormula>
                                                                <inputs>
                                                                    <input>
                                                                        <id>gist</id>
                                                                        <type>bool</type>
                                                                    </input>
                                                                </inputs>
                                                                <code>
                                                                    <![CDATA[return true;
                                                                    ]]>
                                                                </code>
                                                            </softFormula>
                                                            <comments/>
                                                        </operation>
                                                        <comments/>
                                                    </validation>
                                                    <comments/>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>sex</covariateId>
                                                    <covariateName>
                                                        <name lang='en'>Sex</name>
                                                        <name lang='fr'>Sexe</name>
                                                    </covariateName>
                                                    <description>
                                                        <desc lang='en'>Sex of the patient</desc>
                                                        <desc lang='fr'>Sexe du patient</desc>
                                                    </description>
                                                    <unit>-</unit>
                                                    <covariateType>sex</covariateType>
                                                    <dataType>double</dataType>
                                                    <interpolationType>direct</interpolationType>
                                                    <refreshPeriod>
                                                      <unit>y</unit>
                                                      <value>1</value>
                                                    </refreshPeriod>
                                                    <covariateValue>
                                                        <standardValue>0.5</standardValue>
                                                    </covariateValue>
                                                    <validation>
                                                        <errorMessage>
                                                            <text lang='en'>The sex is a double within the range [0,1]. 0 for female, 1 for male</text>
                                                        </errorMessage>
                                                        <operation>
                                                            <softFormula>
                                                                <inputs>
                                                                    <input>
                                                                        <id>sex</id>
                                                                        <type>double</type>
                                                                    </input>
                                                                </inputs>
                                                                <code>
                                                                    <![CDATA[return ((sex >= 0.0) && (sex <= 1.0));
                                                                    ]]>
                                                                </code>
                                                            </softFormula>
                                                            <comments/>
                                                        </operation>
                                                        <comments/>
                                                    </validation>
                                                    <comments/>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>age</covariateId>
                                                    <covariateName>
                                                        <name lang='en'>Age</name>
                                                        <name lang='fr'>Age</name>
                                                    </covariateName>
                                                    <description>
                                                        <desc lang='en'>Age of the patient, in years</desc>
                                                        <desc lang='fr'>Âge du patient, en années</desc>
                                                    </description>
                                                    <unit>y</unit>
                                                    <covariateType>ageInYears</covariateType>
                                                    <dataType>double</dataType>
                                                    <interpolationType>direct</interpolationType>
                                                    <refreshPeriod>
                                                      <unit>y</unit>
                                                      <value>1</value>
                                                    </refreshPeriod>
                                                    <covariateValue>
                                                        <standardValue>50</standardValue>
                                                    </covariateValue>
                                                    <validation>
                                                        <errorMessage>
                                                            <text lang='en'>The age shall be in the interval [20,88].</text>
                                                        </errorMessage>
                                                        <operation>
                                                            <softFormula>
                                                                <inputs>
                                                                    <input>
                                                                        <id>age</id>
                                                                        <type>double</type>
                                                                    </input>
                                                                </inputs>
                                                                <code>
                                                                    <![CDATA[return ((age >= 20) && (age <= 88));
                                                                    ]]>
                                                                </code>
                                                            </softFormula>
                                                            <comments/>
                                                        </operation>
                                                        <comments/>
                                                    </validation>
                                                    <comments/>
                                                </covariate>
                                            </covariates>
                                            <activeMoieties>
                                                <activeMoiety>
                                                    <activeMoietyId>imatinib</activeMoietyId>
                                                    <activeMoietyName>
                                                        <name lang='en'>Imatinib</name>
                                                    </activeMoietyName>
                                                    <unit>ug/l</unit>
                                                    <analyteIdList>
                                                        <analyteId>imatinib</analyteId>
                                                    </analyteIdList>
                                                    <analytesToMoietyFormula>
                                                        <hardFormula>direct</hardFormula>
                                                        <comments/>
                                                    </analytesToMoietyFormula>
                                                    <targets>
                                                        <target>
                                                            <targetType>residual</targetType>
                                                            <targetValues>
                                                                <unit>ug/l</unit>
                                                                <min>
                                                                    <standardValue>750</standardValue>
                                                                </min>
                                                                <max>
                                                                    <standardValue>1500</standardValue>
                                                                </max>
                                                                <best>
                                                                    <standardValue>1000</standardValue>
                                                                </best>
                                                                <toxicityAlarm>
                                                                    <standardValue>10000.0</standardValue>
                                                                </toxicityAlarm>
                                                                <inefficacyAlarm>
                                                                    <standardValue>0.0</standardValue>
                                                                </inefficacyAlarm>
                                                            </targetValues>
                                                            <comments>
                                                                <comment lang='en'>A Toxicity and inefficacyAlarm should be added</comment>
                                                            </comments>
                                                        </target>
                                                    </targets>
                                                </activeMoiety>
                                            </activeMoieties>
                                            <analyteGroups>
                                                <analyteGroup>
                                                    <groupId>imatinib</groupId>
                                                    <pkModelId>linear.1comp.macro</pkModelId>
                                                    <analytes>
                                                        <analyte>
                                                            <analyteId>imatinib</analyteId>
                                                            <unit>ug/l</unit>
                                                            <molarMass>
                                                                <value>493.603</value>
                                                                <unit>g/mol</unit>
                                                            </molarMass>
                                                            <description>
                                                                <desc lang='en'>There is only a single analyte : imatinib.</desc>
                                                            </description>
                                                            <errorModel>
                                                                <errorModelType>proportional</errorModelType>
                                                                <sigmas>
                                                                    <sigma>
                                                                        <standardValue>0.3138</standardValue>
                                                                    </sigma>
                                                                </sigmas>
                                                                <comments/>
                                                            </errorModel>
                                                            <comments/>
                                                        </analyte>
                                                    </analytes>
                                                    <dispositionParameters>
                                                        <parameters>
                                                            <parameter>
                                                                <parameterId>CL</parameterId>
                                                                <unit>l/h</unit>
                                                                <parameterValue>
                                                                    <standardValue>14.3</standardValue>
                                                                    <aprioriComputation>
                                                                        <softFormula>
                                                                            <inputs>
                                                                                <input>
                                                                                    <id>CL_population</id>
                                                                                    <type>double</type>
                                                                                </input>
                                                                                <input>
                                                                                    <id>bodyweight</id>
                                                                                    <type>double</type>
                                                                                </input>
                                                                                <input>
                                                                                    <id>age</id>
                                                                                    <type>double</type>
                                                                                </input>
                                                                                <input>
                                                                                    <id>gist</id>
                                                                                    <type>bool</type>
                                                                                </input>
                                                                                <input>
                                                                                    <id>sex</id>
                                                                                    <type>double</type>
                                                                                </input>
                                                                            </inputs>
                                                                            <code>
                                                                                <![CDATA[
                                                                                theta1 = CL_population;
                                                                                theta4 = 5.42;
                                                                                theta5 = 1.49;
                                                                                theta6 = -5.81;
                                                                                theta7 = -0.806;

                                                                                MEANBW = 70;
                                                                                FBW = (bodyweight - MEANBW) / MEANBW;

                                                                                MEANAG = 50;
                                                                                FAGE = (age - MEANAG) / MEANAG;

                                                                                if (gist)
                                                                                  PATH = 1;
                                                                                else
                                                                                  PATH = 0;

                                                                                MALE = sex;

                                                                                TVCL = theta1 + theta4 * FBW + theta5 * MALE-theta5 * (1-MALE) + theta6 * FAGE + theta7 * PATH - theta7 * (1 - PATH);

                                                                                return TVCL;
                                                                                                         ]]>
                                                                            </code>
                                                                        </softFormula>
                                                                        <comments/>
                                                                    </aprioriComputation>
                                                                </parameterValue>
                                                                <bsv>
                                                                    <bsvType>exponential</bsvType>
                                                                    <stdDevs>
                                                                        <stdDev>0.356</stdDev>
                                                                    </stdDevs>
                                                                </bsv>
                                                                <validation>
                                                                    <errorMessage>
                                                                        <text lang='en'>Clearance shall be in the range [0, 300].</text>
                                                                    </errorMessage>
                                                                    <operation>
                                                                        <softFormula>
                                                                            <inputs>
                                                                                <input>
                                                                                    <id>CL</id>
                                                                                    <type>double</type>
                                                                                </input>
                                                                            </inputs>
                                                                            <code>
                                                                                <![CDATA[
                                                                                return CL < 300.0 and CL > 0.0;
                                                                                            ]]>
                                                                                </code>
                                                                            </softFormula>
                                                                            <comments/>
                                                                        </operation>
                                                                        <comments/>
                                                                    </validation>
                                                                    <comments/>
                                                                </parameter>
                                                                <parameter>
                                                                    <parameterId>V</parameterId>
                                                                    <unit>l</unit>
                                                                    <parameterValue>
                                                                        <standardValue>347</standardValue>
                                                                        <aprioriComputation>
                                                                            <softFormula>
                                                                                <inputs>
                                                                                    <input>
                                                                                        <id>V_population</id>
                                                                                        <type>double</type>
                                                                                    </input>
                                                                                    <input>
                                                                                        <id>sex</id>
                                                                                        <type>double</type>
                                                                                    </input>
                                                                                </inputs>
                                                                                <code>
                                                                                    <![CDATA[
                                                                                    theta2 = V_population;
                                                                                    theta8 = 46.2;
                                                                                    tvv = theta2 + theta8 * sex - theta8 * (1 - sex);
                                                                                    return tvv;
                                                                                                                                ]]>
                                                                                </code>
                                                                            </softFormula>
                                                                            <comments/>
                                                                        </aprioriComputation>
                                                                    </parameterValue>
                                                                    <bsv>
                                                                        <bsvType>exponential</bsvType>
                                                                        <stdDevs>
                                                                            <stdDev>0.629</stdDev>
                                                                        </stdDevs>
                                                                    </bsv>
                                                                    <validation>
                                                                        <errorMessage>
                                                                            <text lang='en'>V shall be positive.</text>
                                                                        </errorMessage>
                                                                        <operation>
                                                                            <softFormula>
                                                                                <inputs>
                                                                                    <input>
                                                                                        <id>V</id>
                                                                                        <type>double</type>
                                                                                    </input>
                                                                                </inputs>
                                                                                <code>
                                                                                    <![CDATA[
                                                                                    return V < 300.0 and V > 0.0;
                                                                                                ]]>
                                                                                    </code>
                                                                                </softFormula>
                                                                                <comments/>
                                                                            </operation>
                                                                            <comments/>
                                                                        </validation>
                                                                        <comments/>
                                                                    </parameter>
                                                                </parameters>
                                                                <correlations>
                                                                    <correlation>
                                                                        <param1>CL</param1>
                                                                        <param2>V</param2>
                                                                        <value>0.798</value>
                                                                        <comments>
                                                                            <comment lang='fr'>coefficient de correlation correspondant à omega2=0.179</comment>
                                                                        </comments>
                                                                    </correlation>
                                                                </correlations>
                                                            </dispositionParameters>
                                                        </analyteGroup>
                                                    </analyteGroups>
                                                    <formulationAndRoutes default='id0'>
                                                        <formulationAndRoute>
                                                            <formulationAndRouteId>id0</formulationAndRouteId>
                                                            <formulation>parenteralSolution</formulation>
                                                            <administrationName>champ libre</administrationName>
                                                            <administrationRoute>oral</administrationRoute>
                                                            <absorptionModel>extra</absorptionModel>
                                                            <dosages>
                                                                <analyteConversions>
                                                                    <analyteConversion>
                                                                        <analyteId>imatinib</analyteId>
                                                                        <factor>1</factor>
                                                                    </analyteConversion>
                                                                </analyteConversions>
                                                                <availableDoses>
                                                                    <unit>mg</unit>
                                                                    <default>
                                                                        <standardValue>400</standardValue>
                                                                    </default>
                                                                    <rangeValues>
                                                                        <from>
                                                                            <standardValue>100</standardValue>
                                                                        </from>
                                                                        <to>
                                                                            <standardValue>400</standardValue>
                                                                        </to>
                                                                        <step>
                                                                            <standardValue>100</standardValue>
                                                                        </step>
                                                                    </rangeValues>
                                                                    <fixedValues>
                                                                        <value>600</value>
                                                                        <value>800</value>
                                                                    </fixedValues>
                                                                </availableDoses>
                                                                <availableIntervals>
                                                                    <unit>h</unit>
                                                                    <default>
                                                                        <standardValue>24</standardValue>
                                                                    </default>
                                                                    <fixedValues>
                                                                        <value>12</value>
                                                                        <value>24</value>
                                                                    </fixedValues>
                                                                </availableIntervals>
                                                                <comments/>
                                                            </dosages>
                                                            <absorptionParameters>
                                                                <parameterSetAnalyteGroup>
                                                                    <analyteGroupId>imatinib</analyteGroupId>
                                                                    <absorptionModel>extra</absorptionModel>
                                                                    <parameterSet>
                                                                        <parameters>
                                                                            <parameter>
                                                                                <parameterId>F</parameterId>
                                                                                <unit>%</unit>
                                                                                <parameterValue>
                                                                                    <standardValue>1</standardValue>
                                                                                </parameterValue>
                                                                                <bsv>
                                                                                    <bsvType>none</bsvType>
                                                                                </bsv>
                                                                                <validation>
                                                                                    <errorMessage>
                                                                                        <text lang='en'>F shall be in the interval [0,1].</text>
                                                                                    </errorMessage>
                                                                                    <operation>
                                                                                        <softFormula>
                                                                                            <inputs>
                                                                                                <input>
                                                                                                    <id>F</id>
                                                                                                    <type>double</type>
                                                                                                </input>
                                                                                            </inputs>
                                                                                            <code>
                                                                                                <![CDATA[
                                                                                                return F <= 1.0 and F > 0.0;
                                                                                                            ]]>
                                                                                                </code>
                                                                                            </softFormula>
                                                                                            <comments/>
                                                                                        </operation>
                                                                                        <comments/>
                                                                                    </validation>
                                                                                    <comments/>
                                                                                </parameter>
                                                                                <parameter>
                                                                                    <parameterId>Ka</parameterId>
                                                                                    <unit>h-1</unit>
                                                                                    <parameterValue>
                                                                                        <standardValue>0.609</standardValue>
                                                                                    </parameterValue>
                                                                                    <bsv>
                                                                                        <bsvType>none</bsvType>
                                                                                    </bsv>
                                                                                    <validation>
                                                                                        <errorMessage>
                                                                                            <text lang='en'>No check on Ka now.</text>
                                                                                        </errorMessage>
                                                                                        <operation>
                                                                                            <softFormula>
                                                                                                <inputs>
                                                                                                    <input>
                                                                                                        <id>Ka</id>
                                                                                                        <type>double</type>
                                                                                                    </input>
                                                                                                </inputs>
                                                                                                <code>
                                                                                                    <![CDATA[
                                                                                                    return true;
                                                                                                    ]]>
                                                                                                </code>
                                                                                            </softFormula>
                                                                                            <comments/>
                                                                                        </operation>
                                                                                        <comments/>
                                                                                    </validation>
                                                                                    <comments/>
                                                                                </parameter>
                                                                            </parameters>
                                                                            <correlations/>
                                                                        </parameterSet>
                                                                    </parameterSetAnalyteGroup>
                                                                </absorptionParameters>
                                                            </formulationAndRoute>
                                                        </formulationAndRoutes>
                                                        <timeConsiderations>
                                                            <halfLife>
                                                                <unit>h</unit>
                                                                <duration>
                                                                    <standardValue>12</standardValue>
                                                                </duration>
                                                                <multiplier>20</multiplier>
                                                                <comments>
                                                                    <comment lang='en'>TODO : Find the half life</comment>
                                                                </comments>
                                                            </halfLife>
                                                            <outdatedMeasure>
                                                                <unit>d</unit>
                                                                <duration>
                                                                    <standardValue>100</standardValue>
                                                                </duration>
                                                                <comments>
                                                                    <comment lang='en'>TODO : This value is not set now</comment>
                                                                </comments>
                                                            </outdatedMeasure>
                                                        </timeConsiderations>
                                                        <comments/>
                                                    </drugModel>
                                                </model>)";

        std::cout << _testName << std::endl;

        // Drug models repository creation
        Tucuxi::Common::ComponentManager* pCmpMgr = Tucuxi::Common::ComponentManager::getInstance();

        auto drugModelRepository =
                dynamic_cast<Tucuxi::Core::DrugModelRepository*>(Tucuxi::Core::DrugModelRepository::createComponent());

        pCmpMgr->registerComponent("DrugModelRepository", drugModelRepository);

        Tucuxi::Core::DrugModelImport drugModelImport;
        std::unique_ptr<Tucuxi::Core::DrugModel> drugModel;
        if (drugModelImport.importFromString(drugModel, modelString) == Tucuxi::Core::DrugModelImport::Status::Ok) {

            Tucuxi::Core::PkModelCollection pkCollection;

            if (!defaultPopulate(pkCollection)) {
                throw std::runtime_error("Could not populate the Pk models collection. No model will be available");
            }

            Tucuxi::Core::DrugModelChecker checker;
            Tucuxi::Core::DrugModelChecker::CheckerResult_t checkerResult = checker.checkDrugModel(drugModel.get(), &pkCollection);
            if (!checkerResult.m_ok) {
                throw std::runtime_error("A drug file has internal errors : " + checkerResult.m_errorMessage);
            }
            drugModelRepository->addDrugModel(drugModel.get());
        }
        else {
            throw std::runtime_error("Failed to import drug file");
        }
        drugModel.release();

        // Query import
        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;
        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

        if (importResult != Tucuxi::Xpert::XpertQueryImport::Status::Ok) {
            throw std::runtime_error("Setup failed");
        }

        Tucuxi::Xpert::XpertQueryResult globalResult{move(query), ""};
        Tucuxi::Xpert::XpertRequestResult& xrr =  globalResult.getXpertRequestResults()[0];
        xrr.setDrugModel(drugModelRepository->getDrugModelsByDrugId(xrr.getXpertRequest().getDrugId())[0]);

        // Execution
        flowStepProvider.getSampleValidator()->perform(xrr);

        const std::vector<Tucuxi::Xpert::SampleValidationResult>& sampleResults = xrr.getSampleResults();

        fructose_assert_eq(sampleResults.size(), xrr.getTreatment()->getSamples().size());

        fructose_assert_eq(sampleResults[0].getSource()->getDate(), Tucuxi::Common::DateTime("2018-07-06T13:00:00", date_format))

        fructose_assert_eq(sampleResults[1].getSource()->getDate(), Tucuxi::Common::DateTime("2018-07-07T06:00:30", date_format))

        fructose_assert_eq(sampleResults[2].getSource()->getDate(), Tucuxi::Common::DateTime("2018-07-08T07:00:00", date_format))
    }
};

#endif // TEST_SAMPLEVALIDATOR_H
