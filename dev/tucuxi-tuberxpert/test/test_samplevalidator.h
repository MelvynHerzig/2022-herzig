#ifndef TEST_SAMPLEVALIDATOR_H
#define TEST_SAMPLEVALIDATOR_H

#include <numeric>
#include <string>
#include <memory>

#include "tucucore/computingservice/computingresponse.h"
#include "tucucore/drugtreatment/sample.h"
#include "tucucore/drugmodel/analyte.h"
#include "tucucore/definitions.h"
#include "tucucommon/datetime.h"
#include "tucucommon/unit.h"

#include "../src/result/sampleresult.h"
#include "../src/result/validation/samplevalidator.h"
#include "../src/language/languagemanager.h"

#include "fructose/fructose.h"

/// \brief Tests for SampleValidator and SampleResult.
/// \date 06/06/2022
/// \author Herzig Melvyn
struct TestSampleValidator : public fructose::test_base<TestSampleValidator>
{

    /// \brief Format used to create date and time during test.
    const std::string DATE_FORMAT = "%Y-%m-%dT%H:%M:%S";

    /// \brief Unit used to create the percentiles data.
    const Tucuxi::Common::TucuUnit UNIT{"ug/l"};

    /// \brief Creates some percentiles data and insert them into pData.
    /// \param pData PercentilesData object that gets the preparation.
    void createPercentilesData( Tucuxi::Core::PercentilesData& pData){
        // Preparing the percentilesData
        // There are going to be 99 percentiles (1-99) with 2 CycleDatas:
        // 1.1.2022 10:00:00 - 1.1.2022 11:00:00, 1.1.2022 11:00:00 - 1.1.2022 12:00:00
        Tucuxi::Common::DateTime d1{"2022-01-01T10:00:00", DATE_FORMAT};
        Tucuxi::Common::DateTime d2{"2022-01-01T11:00:00", DATE_FORMAT};
        Tucuxi::Common::DateTime d3{"2022-01-01T12:00:00", DATE_FORMAT};

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

                cycles.emplace_back(cyclesStarts[cdi], cyclesEnds[cdi], UNIT);

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

    /// \brief Checks that the warnings are correct when retrieving them
    ///        from SampleResult objects with given "percentile groups".
    /// \param _testName Name of the test
    void warningForPositionOver100Percentiles(const std::string& _testName)
    {

        std::string dictionary = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <dictionary
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="dictionary.xsd">

                                        <entry key="population_below">% of the population is below this measure</entry>
                                        <entry key="population_above">% of the population is above this measure</entry>

                                    </dictionary>)";

        std::cout << _testName << std::endl;

        Tucuxi::XpertLanguage::LanguageManager& lm = Tucuxi::XpertLanguage::LanguageManager::getInstance();
        lm.loadDictionary(dictionary);

        // Creating SampleResult objects that are located in a different group. There are
        // 100 groups that are implicitly formed by the 99 percentiles.
        Tucuxi::XpertResult::SampleResult sr1 = Tucuxi::XpertResult::SampleResult(nullptr, 1);
        Tucuxi::XpertResult::SampleResult sr10 = Tucuxi::XpertResult::SampleResult(nullptr, 10);
        Tucuxi::XpertResult::SampleResult sr11 = Tucuxi::XpertResult::SampleResult(nullptr, 11);
        Tucuxi::XpertResult::SampleResult sr90 = Tucuxi::XpertResult::SampleResult(nullptr, 90);
        Tucuxi::XpertResult::SampleResult sr91 = Tucuxi::XpertResult::SampleResult(nullptr, 91);
        Tucuxi::XpertResult::SampleResult sr100 = Tucuxi::XpertResult::SampleResult(nullptr, 100);

        fructose_assert_eq(sr1.getWarning(), "99% of the population is above this measure");
        fructose_assert_eq(sr10.getWarning(), "90% of the population is above this measure");
        fructose_assert_eq(sr11.getWarning(), "");
        fructose_assert_eq(sr90.getWarning(), "");
        fructose_assert_eq(sr91.getWarning(), "90% of the population is below this measure");
        fructose_assert_eq(sr100.getWarning(), "99% of the population is below this measure");
    }

    /// \brief This methods checks that the methods SampleValidator::findPosOver100Percentile returns
    ///        the good positions for a given set of samples and a given percentileData.
    ///
    ///         We test the SampleValidator class with SampleValidator::findPosOver100Percentile and not
    ///         with SampleValidator::getSampleValidations because this last methods only performs a request
    ///         on the core which return "unpredictable" percentiles. So basically 90% of the behavior of
    ///         the SampleValidator can be checked with SampleValidator::findPosOver100Percentile.
    ///
    ///         Thus, this struct if the friend of SampleValidator to acces this protected method.
    /// \param _testName Name of the test
    void findTheGoodPosOver100PercentileGroups(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Core::PercentilesData pData{""};
        createPercentilesData(pData);

        // We perform 9 series of tests.
        // The times tested are from 1.1.2022 10:00:00 to 1.1.2022 12:00:00 every 15 minutes.
        // 18 concentrations are tested, 2 at each time: one slightly below the percentile
        //                                               one slightly above the percentile
        std::vector<Tucuxi::Common::DateTime> testDates{
            Tucuxi::Common::DateTime{"2022-01-01T10:00:00", DATE_FORMAT},
            Tucuxi::Common::DateTime{"2022-01-01T10:15:00", DATE_FORMAT},
            Tucuxi::Common::DateTime{"2022-01-01T10:30:00", DATE_FORMAT},
            Tucuxi::Common::DateTime{"2022-01-01T10:45:00", DATE_FORMAT},
            Tucuxi::Common::DateTime{"2022-01-01T11:00:00", DATE_FORMAT},
            Tucuxi::Common::DateTime{"2022-01-01T11:15:00", DATE_FORMAT},
            Tucuxi::Common::DateTime{"2022-01-01T11:30:00", DATE_FORMAT},
            Tucuxi::Common::DateTime{"2022-01-01T11:45:00", DATE_FORMAT},
            Tucuxi::Common::DateTime{"2022-01-01T12:00:00", DATE_FORMAT},
        };

        Tucuxi::XpertResult::SampleValidator sv;
        // For each percentile
        for (size_t pi = 1; pi <= 99 ; ++pi){
            for (size_t ti = 0; ti < 9; ++ti) {
                double percentileValue = pi * 10 + ti * 0.25;
                std::unique_ptr<Tucuxi::Core::Sample> slightlyAbove = std::make_unique<Tucuxi::Core::Sample>(testDates[ti], Tucuxi::Core::AnalyteId{""}, percentileValue + 0.01 , UNIT);
                std::unique_ptr<Tucuxi::Core::Sample> slightlyBelow = std::make_unique<Tucuxi::Core::Sample>(testDates[ti], Tucuxi::Core::AnalyteId{""}, percentileValue - 0.01 , UNIT);

                unsigned groupSlightlyAbove = sv.findPosOver100PercentileGroups(&pData, slightlyAbove);
                unsigned groupSlightlyBelow = sv.findPosOver100PercentileGroups(&pData, slightlyBelow);

                fructose_assert_eq(groupSlightlyAbove, pi + 1);
                fructose_assert_eq(groupSlightlyBelow, pi);
            }
        }

        fructose_assert_eq(true, true);
    }

    /// \brief This methods checks that the methods SampleValidator::findPosOver100Percentile throws
    ///        an invalid_argument exception when a sample unit can't be converted.
    /// \param _testName Name of the test
    void getExceptionUnitConversion(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Core::PercentilesData pData{""};
        createPercentilesData(pData);

        Tucuxi::XpertResult::SampleValidator sv;

        // We set the unit to kg to get the exception.
        std::unique_ptr<Tucuxi::Core::Sample> sample = std::make_unique<Tucuxi::Core::Sample>(
                    Tucuxi::Common::DateTime{"2022-01-01T11:30:00", DATE_FORMAT},
                    Tucuxi::Core::AnalyteId{""},
                    1,
                    Tucuxi::Common::TucuUnit{"kg"});

        fructose_assert_exception(sv.findPosOver100PercentileGroups(&pData, sample), std::invalid_argument);
    }

    /// \brief This methods checks that the methods SampleValidator::findPosOver100Percentile throws
    ///        an invalid_argument exception when a sample date can't be found in the cycleData objects.
    /// \param _testName Name of the test
    void getExceptionDateNotFound(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Core::PercentilesData pData{""};
        createPercentilesData(pData);

        Tucuxi::XpertResult::SampleValidator sv;

        // We set the date to 2023 to be out of bound of the cycleData objects
        std::unique_ptr<Tucuxi::Core::Sample> sample = std::make_unique<Tucuxi::Core::Sample>(
                    Tucuxi::Common::DateTime{"2023-01-01T11:30:00", DATE_FORMAT},
                    Tucuxi::Core::AnalyteId{""},
                    1,
                    UNIT);

        fructose_assert_exception(sv.findPosOver100PercentileGroups(&pData, sample), std::invalid_argument);
    }
};

#endif // TEST_SAMPLEVALIDATOR_H
