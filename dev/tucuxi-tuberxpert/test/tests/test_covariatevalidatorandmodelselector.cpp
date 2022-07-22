#include "test_covariatevalidatorandmodelselector.h"

using namespace std;
using namespace Tucuxi;


void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_whenTreatmentNullptr(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                </covariates>
                                            </patient>
                                            <!-- List of the drugs informations we have concerning the patient -->
                                            <drugs>

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
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

    cout << _testName << endl;

    // Prepare the XpertRequestResult
    vector<string> models {TestUtils::originalImatinibModelString};
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No treatment set.");
    fructose_assert_eq(xpertRequestResult.getDrugModel(), nullptr);
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_whenNoDrugModelForDrugId(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                </covariates>
                                            </patient>
                                            <!-- List of the drugs informations we have concerning the patient -->
                                            <drugs>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

    cout << _testName << endl;

    // Prepare the XpertRequestResult
    vector<string> models {}; // We prepare the environment with 0 model.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "The drug files directory does not contain a drug model for the given drug: imatinib");
    fructose_assert_eq(xpertRequestResult.getDrugModel(), nullptr);
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_whenTreatmentFormulationsAndRoutesAreDifferent(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

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
                                                            <dosageTimeRange>
                                                                <start>2018-07-09T08:00:00</start>
                                                                <end>2018-07-11T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>12:00:00</interval>
                                                                            <dose>
                                                                                <value>450</value>
                                                                                <unit>mg</unit>
                                                                                <infusionTimeInMinutes>60</infusionTimeInMinutes>
                                                                            </dose>
                                                                            <formulationAndRoute>
                                                                                <formulation>parenteralSolution</formulation>
                                                                                <administrationName>foo bar</administrationName>
                                                                                <administrationRoute>nasal</administrationRoute>
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
                                                <drug>
                                                    <drugId>rifampicin</drugId>
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
                                                                                <value>450</value>
                                                                                <unit>mg</unit>
                                                                                <infusionTimeInMinutes>60</infusionTimeInMinutes>
                                                                            </dose>
                                                                            <formulationAndRoute>
                                                                                <formulation>parenteralSolution</formulation>
                                                                                <administrationName>foo bar</administrationName>
                                                                                <administrationRoute>oral</administrationRoute>
                                                                                <absorptionModel>infusion</absorptionModel>
                                                                            </formulationAndRoute>
                                                                        </lastingDosage>
                                                                    </dosageLoop>
                                                                </dosage>
                                                            </dosageTimeRange>
                                                            <dosageTimeRange>
                                                                <start>2018-07-09T08:00:00</start>
                                                                <end>2018-07-11T08:00:00</end>
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
                                            </xpertRequest>
                                            <xpertRequest>
                                                <drugId>rifampicin</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

    cout << _testName << endl;

    // Import the query and get the xpertRequest results.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;
    vector<string> models;
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResultDiffRoute = xpertQueryResult->getXpertRequestResults()[0];
    Xpert::XpertRequestResult& xpertRequestResultDiffAbsorption = xpertQueryResult->getXpertRequestResults()[1];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResultDiffRoute);
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResultDiffAbsorption);

    // Compare
    fructose_assert_eq(xpertRequestResultDiffRoute.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResultDiffRoute.getErrorMessage(), "All formulations and routes must be equal.");
    fructose_assert_eq(xpertRequestResultDiffAbsorption.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResultDiffAbsorption.getErrorMessage(), "All formulations and routes must be equal.");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_withOnlyOneModelWithDifferentFormulationAndRoute(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

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
                                                                                <absorptionModel>intravascular</absorptionModel>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>)";

    cout << _testName << endl;

    // Import the query and get the xpertRequest result.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;
    vector<string> models = {TestUtils::originalImatinibModelString};
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No valid drug model found.");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_success_withOneModelWithDifferentAndOneModelWithSameFormulationAndRoute(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

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

    string incompatibleImatinibModelString = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                                 <drugModelId>ch.tucuxi.imatinib.gotta2012_bad</drugModelId>
                                                 <domain>
                                                     <description>
                                                         <desc lang='en'>Adult, (Disease: CML and GIST, Age range: 20-88 yrs, Weight range: 44-110kg, AGP plasma concentration range: 0.4–2.0 g/L)</desc>
                                                     </description>
                                                     <constraints>
                                                         <constraint>
                                                             <constraintType>hard</constraintType>
                                                             <errorMessage>
                                                                 <text lang='en'>The age shall be in the range [20,88]</text>
                                                             </errorMessage>
                                                             <requiredCovariates>
                                                                 <covariateId>age</covariateId>
                                                             </requiredCovariates>
                                                             <checkOperation>
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
                                                             </checkOperation>
                                                             <comments/>
                                                         </constraint>
                                                         <constraint>
                                                             <constraintType>soft</constraintType>
                                                             <errorMessage>
                                                                 <text lang='en'>The weight should be in the range [44,110]</text>
                                                             </errorMessage>
                                                             <requiredCovariates>
                                                                 <covariateId>bodyweight</covariateId>
                                                             </requiredCovariates>
                                                             <checkOperation>
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
                                                             </checkOperation>
                                                             <comments/>
                                                         </constraint>
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
                                                                 <absorptionModel>bolus</absorptionModel>
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

    cout << _testName << endl;

    // Import the query and get the xpertRequest result.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;
    vector<string> models = {incompatibleImatinibModelString, TestUtils::originalImatinibModelString};
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "");
    fructose_assert_eq(xpertRequestResult.getDrugModel()->getDrugModelId(), "ch.tucuxi.imatinib.gotta2012");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_withOneModelHardConstraintNotMet(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                        <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                        <drugTreatment>
                                            <!-- All the information regarding the patient -->
                                            <patient>
                                                <covariates>
                                                    <covariate>
                                                        <covariateId>birthdate</covariateId>
                                                        <date>2018-07-11T10:45:30</date>
                                                        <value>1890-01-01T00:00:00</value>
                                                        <unit></unit>
                                                          <dataType>date</dataType>
                                                        <nature>discrete</nature>
                                                    </covariate>
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
                                            </xpertRequest>
                                        </requests>
                                    </query>)";



    cout << _testName << endl;

    // Import the query and get the xpertRequest result.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;
    vector<string> models = {TestUtils::originalImatinibModelString};
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "No valid drug model found.");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_success_withOneModelHardConstraintNotMetAndOtherModelOk(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                <query version="1.0"
                                    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                    xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                    <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                    <drugTreatment>
                                        <!-- All the information regarding the patient -->
                                        <patient>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>birthdate</covariateId>
                                                    <date>2018-07-11T10:45:30</date>
                                                    <value>1890-01-01T00:00:00</value>
                                                    <unit></unit>
                                                      <dataType>date</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
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
                                        </xpertRequest>
                                    </requests>
                                </query>)";

    string modifiedImatinibModelString = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                                        <drugModelId>ch.tucuxi.imatinib.gotta2012_modified</drugModelId>
                                                        <domain>
                                                            <description>
                                                                <desc lang='en'>Adult, (Disease: CML and GIST, Age range: 20-88 yrs, Weight range: 44-110kg, AGP plasma concentration range: 0.4–2.0 g/L)</desc>
                                                            </description>
                                                            <constraints>
                                                                <constraint>
                                                                    <constraintType>hard</constraintType>
                                                                    <errorMessage>
                                                                        <text lang='en'>The age shall be in the range [20,88]</text>
                                                                    </errorMessage>
                                                                    <requiredCovariates>
                                                                        <covariateId>age</covariateId>
                                                                    </requiredCovariates>
                                                                    <checkOperation>
                                                                        <softFormula>
                                                                            <inputs>
                                                                                <input>
                                                                                    <id>age</id>
                                                                                    <type>double</type>
                                                                                </input>
                                                                            </inputs>
                                                                            <code>
                                                                                <![CDATA[return true;
                                                                                ]]>
                                                                            </code>
                                                                        </softFormula>
                                                                        <comments/>
                                                                    </checkOperation>
                                                                    <comments/>
                                                                </constraint>
                                                                <constraint>
                                                                    <constraintType>soft</constraintType>
                                                                    <errorMessage>
                                                                        <text lang='en'>The weight should be in the range [44,110]</text>
                                                                    </errorMessage>
                                                                    <requiredCovariates>
                                                                        <covariateId>bodyweight</covariateId>
                                                                    </requiredCovariates>
                                                                    <checkOperation>
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
                                                                    </checkOperation>
                                                                    <comments/>
                                                                </constraint>
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

    cout << _testName << endl;

    // Import the query and get the xpertRequest result.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;
    vector<string> models = {TestUtils::originalImatinibModelString, modifiedImatinibModelString};
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "");
    fructose_assert_eq(xpertRequestResult.getDrugModel()->getDrugModelId(), "ch.tucuxi.imatinib.gotta2012_modified");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_success_withOneModelSoftConstraintNotMet(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                <query version="1.0"
                                    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                    xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                    <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                    <drugTreatment>
                                        <!-- All the information regarding the patient -->
                                        <patient>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>bodyweight</covariateId>
                                                    <date>2018-07-06T08:00:00</date>
                                                    <value>170</value>
                                                    <unit>kg</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
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

    cout << _testName << endl;

    // Import the query and get the xpertRequest result.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;
    vector<string> models = {TestUtils::originalImatinibModelString};
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "");
    fructose_assert_eq(xpertRequestResult.getDrugModel()->getDrugModelId(), "ch.tucuxi.imatinib.gotta2012");
}


void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_multipleBirthDateOneModelWithAgeOneModelWithout(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                <query version="1.0"
                                    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                    xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                    <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                    <drugTreatment>
                                        <!-- All the information regarding the patient -->
                                        <patient>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>birthdate</covariateId>
                                                    <date>2018-07-11T10:45:30</date>
                                                    <value>1990-01-01T00:00:00</value>
                                                    <unit></unit>
                                                      <dataType>date</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>birthdate</covariateId>
                                                    <date>2018-07-11T10:45:30</date>
                                                    <value>1980-01-01T00:00:00</value>
                                                    <unit></unit>
                                                      <dataType>date</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
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
                                        </xpertRequest>
                                    </requests>
                                </query>)";

    string noAgeImatinibModelString = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                                <constraint>
                                                    <constraintType>soft</constraintType>
                                                    <errorMessage>
                                                        <text lang='en'>The weight should be in the range [44,110]</text>
                                                    </errorMessage>
                                                    <requiredCovariates>
                                                        <covariateId>bodyweight</covariateId>
                                                    </requiredCovariates>
                                                    <checkOperation>
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
                                                    </checkOperation>
                                                    <comments/>
                                                </constraint>
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
                                                                            FAGE = (50 - MEANAG) / MEANAG;

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

    cout << _testName << endl;

    // Import the query and get the xpertRequest result.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;
    vector<string> models = {TestUtils::originalImatinibModelString, noAgeImatinibModelString};
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "Patient covariate error found when handling model ch.tucuxi.imatinib.gotta2012, details: Multiple birthdate not allowed.");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_badBirthDateDataTypeOneModelWithAgeOneModelWithout(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                <query version="1.0"
                                    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                    xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                    <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                    <drugTreatment>
                                        <!-- All the information regarding the patient -->
                                        <patient>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>birthdate</covariateId>
                                                    <date>2018-07-11T10:45:30</date>
                                                    <value>1990-01-01T00:00:00</value>
                                                    <unit></unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
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
                                        </xpertRequest>
                                    </requests>
                                </query>)";

    string noAgeImatinibModelString = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                                                <constraint>
                                                                    <constraintType>soft</constraintType>
                                                                    <errorMessage>
                                                                        <text lang='en'>The weight should be in the range [44,110]</text>
                                                                    </errorMessage>
                                                                    <requiredCovariates>
                                                                        <covariateId>bodyweight</covariateId>
                                                                    </requiredCovariates>
                                                                    <checkOperation>
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
                                                                    </checkOperation>
                                                                    <comments/>
                                                                </constraint>
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
                                                                                            FAGE = (50 - MEANAG) / MEANAG;

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

    cout << _testName << endl;

    // Import the query and get the xpertRequest result.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;
    vector<string> models = {TestUtils::originalImatinibModelString, noAgeImatinibModelString};
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "Patient covariate error found when handling model ch.tucuxi.imatinib.gotta2012, details: Invalid data type of birthdate.");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_badGistUnitOneModelWithGistOneModelWithout(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                <query version="1.0"
                                    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                    xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                    <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                    <drugTreatment>
                                        <!-- All the information regarding the patient -->
                                        <patient>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>gist</covariateId>
                                                    <date>2018-07-06T08:00:00</date>
                                                    <value>40</value>
                                                    <unit>kg</unit>
                                                    <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
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





    string noGistImatinibModelString = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                                                                            FBW = (70 - MEANBW) / MEANBW;

                                                                                            MEANAG = 50;
                                                                                            FAGE = (50 - MEANAG) / MEANAG;

                                                                                            if (true)
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

    cout << _testName << endl;

    // Import the query and get the xpertRequest result.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;
    vector<string> models = {TestUtils::originalImatinibModelString,  noGistImatinibModelString};
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "Covariates extraction failed for drug model: ch.tucuxi.imatinib.gotta2012. It may be caused by covariates that could not be converted.");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_getTheBestModel_betweenThreeModelsAndNoTie(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                <query version="1.0"
                                    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                    xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                    <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                    <drugTreatment>
                                        <!-- All the information regarding the patient -->
                                        <patient>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>bodyweight3</covariateId>
                                                    <date>2018-07-06T08:00:00</date>
                                                    <value>70</value>
                                                    <unit>kg</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>bodyweight4</covariateId>
                                                    <date>2018-07-06T08:00:00</date>
                                                    <value>70</value>
                                                    <unit>kg</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>bodyweight5</covariateId>
                                                    <date>2018-07-06T08:00:00</date>
                                                    <value>70</value>
                                                    <unit>kg</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>bodyweight6</covariateId>
                                                    <date>2018-07-06T08:00:00</date>
                                                    <value>70</value>
                                                    <unit>kg</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
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
                                        </xpertRequest>
                                    </requests>
                                </query>)";

    string model0String = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                        <drugModelId>ch.tucuxi.imatinib.gotta2012_1</drugModelId>
                                        <domain>
                                            <description>
                                                <desc lang='en'>Adult, (Disease: CML and GIST, Age range: 20-88 yrs, Weight range: 44-110kg, AGP plasma concentration range: 0.4–2.0 g/L)</desc>
                                            </description>
                                            <constraints>
                                            </constraints>
                                        </domain>
                                        <covariates>
                                            <covariate>
                                                <covariateId>bodyweight1</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Total Body Weight1</name>
                                                    <name lang='fr'>Poids total1</name>
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
                                                                    <id>bodyweight1</id>
                                                                    <type>double</type>
                                                                </input>
                                                            </inputs>
                                                            <code>
                                                                <![CDATA[return ((bodyweight1 >= 44) && (bodyweight1 <= 110));
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
                                                <covariateId>bodyweight2</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Total Body Weight2</name>
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
                                                                    <id>bodyweight2</id>
                                                                    <type>double</type>
                                                                </input>
                                                            </inputs>
                                                            <code>
                                                                <![CDATA[return ((bodyweight2 >= 44) && (bodyweight2 <= 110));
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
                                                <covariateId>bodyweight</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Total Body Weight2</name>
                                                    <name lang='fr'>Poids total2</name>
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

    string model1String = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                        <drugModelId>ch.tucuxi.imatinib.gotta2012_2</drugModelId>
                                        <domain>
                                            <description>
                                                <desc lang='en'>Adult, (Disease: CML and GIST, Age range: 20-88 yrs, Weight range: 44-110kg, AGP plasma concentration range: 0.4–2.0 g/L)</desc>
                                            </description>
                                            <constraints>
                                            </constraints>
                                        </domain>
                                        <covariates>
                                            <covariate>
                                                <covariateId>bodyweight3</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Total Body Weight3</name>
                                                    <name lang='fr'>Poids total3</name>
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
                                                                    <id>bodyweight3</id>
                                                                    <type>double</type>
                                                                </input>
                                                            </inputs>
                                                            <code>
                                                                <![CDATA[return ((bodyweight3 >= 44) && (bodyweight3 <= 110));
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
                                                <covariateId>bodyweight4</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Total Body Weight4</name>
                                                    <name lang='fr'>Poids total4</name>
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
                                                        <text lang='en'>The body weight shall be in the interval [440,1100].</text>
                                                    </errorMessage>
                                                    <operation>
                                                        <softFormula>
                                                            <inputs>
                                                                <input>
                                                                    <id>bodyweight4</id>
                                                                    <type>double</type>
                                                                </input>
                                                            </inputs>
                                                            <code>
                                                                <![CDATA[return ((bodyweight4 >= 440) && (bodyweight4 <= 1100));
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

    string model2String = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                        <drugModelId>ch.tucuxi.imatinib.gotta2012_3</drugModelId>
                                        <domain>
                                            <description>
                                                <desc lang='en'>Adult, (Disease: CML and GIST, Age range: 20-88 yrs, Weight range: 44-110kg, AGP plasma concentration range: 0.4–2.0 g/L)</desc>
                                            </description>
                                            <constraints>
                                            </constraints>
                                        </domain>
                                        <covariates>
                                            <covariate>
                                                <covariateId>bodyweight5</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Total Body Weight5</name>
                                                    <name lang='fr'>Poids total5</name>
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
                                                        <text lang='en'>The body weight shall be in the interval [440,1100].</text>
                                                    </errorMessage>
                                                    <operation>
                                                        <softFormula>
                                                            <inputs>
                                                                <input>
                                                                    <id>bodyweight5</id>
                                                                    <type>double</type>
                                                                </input>
                                                            </inputs>
                                                            <code>
                                                                <![CDATA[return ((bodyweight5 >= 440) && (bodyweight5 <= 1100));
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
                                                <covariateId>bodyweight6</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Total Body Weight6</name>
                                                    <name lang='fr'>Poids total6</name>
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
                                                        <text lang='en'>The body weight shall be in the interval [440,1100].</text>
                                                    </errorMessage>
                                                    <operation>
                                                        <softFormula>
                                                            <inputs>
                                                                <input>
                                                                    <id>bodyweight6</id>
                                                                    <type>double</type>
                                                                </input>
                                                            </inputs>
                                                            <code>
                                                                <![CDATA[return ((bodyweight6 >= 440) && (bodyweight6 <= 1100));
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

    cout << _testName << endl;

    // Import the query and get the xpertRequest result.
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult = nullptr;
    vector<string> models = {model0String, model1String, model2String};
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "");
    fructose_assert_eq(xpertRequestResult.getDrugModel()->getDrugModelId() ,"ch.tucuxi.imatinib.gotta2012_2");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_getTheBestModel_betweenTwoModelsWithTie(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                <query version="1.0"
                                    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                    xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                    <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                    <drugTreatment>
                                        <!-- All the information regarding the patient -->
                                        <patient>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>bodyweight3</covariateId>
                                                    <date>2018-07-06T08:00:00</date>
                                                    <value>70</value>
                                                    <unit>kg</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
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

    string model0String = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                        <drugModelId>ch.tucuxi.imatinib.gotta2012_1</drugModelId>
                                        <domain>
                                            <description>
                                                <desc lang='en'>Adult, (Disease: CML and GIST, Age range: 20-88 yrs, Weight range: 44-110kg, AGP plasma concentration range: 0.4–2.0 g/L)</desc>
                                            </description>
                                            <constraints>
                                            </constraints>
                                        </domain>
                                        <covariates>
                                            <covariate>
                                                <covariateId>bodyweight1</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Total Body Weight1</name>
                                                    <name lang='fr'>Poids total1</name>
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
                                                                    <id>bodyweight1</id>
                                                                    <type>double</type>
                                                                </input>
                                                            </inputs>
                                                            <code>
                                                                <![CDATA[return ((bodyweight1 >= 44) && (bodyweight1 <= 110));
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

    string model1String = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                        <drugModelId>ch.tucuxi.imatinib.gotta2012_2</drugModelId>
                                        <domain>
                                            <description>
                                                <desc lang='en'>Adult, (Disease: CML and GIST, Age range: 20-88 yrs, Weight range: 44-110kg, AGP plasma concentration range: 0.4–2.0 g/L)</desc>
                                            </description>
                                            <constraints>
                                            </constraints>
                                        </domain>
                                        <covariates>
                                            <covariate>
                                                <covariateId>bodyweight2</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Total Body Weight2</name>
                                                    <name lang='fr'>Poids total2</name>
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
                                                                    <id>bodyweight2</id>
                                                                    <type>double</type>
                                                                </input>
                                                            </inputs>
                                                            <code>
                                                                <![CDATA[return ((bodyweight2 >= 44) && (bodyweight2 <= 110));
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
                                                <covariateId>bodyweight3</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Total Body Weight3</name>
                                                    <name lang='fr'>Poids total3</name>
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
                                                        <text lang='en'>The body weight shall be in the interval [44,110].</text>
                                                    </errorMessage>
                                                    <operation>
                                                        <softFormula>
                                                            <inputs>
                                                                <input>
                                                                    <id>bodyweight3</id>
                                                                    <type>double</type>
                                                                </input>
                                                            </inputs>
                                                            <code>
                                                                <![CDATA[return ((bodyweight3 >= 44) && (bodyweight3 <= 110));
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

    cout << _testName << endl;

    // Prepare the XpertRequestResult
    vector<string> models {model0String, model1String};
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "");
    fructose_assert_eq(xpertRequestResult.getDrugModel()->getDrugModelId() ,"ch.tucuxi.imatinib.gotta2012_2");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_failure_whenSelectedModelNotSupportingRequestedLanguageAndEnglish(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                <query version="1.0"
                                    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                    xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                    <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                    <drugTreatment>
                                        <!-- All the information regarding the patient -->
                                        <patient>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>birthdate</covariateId>
                                                    <date>2018-07-11T10:45:30</date>
                                                    <value>1990-01-01T00:00:00</value>
                                                    <unit></unit>
                                                      <dataType>date</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>bodyweight</covariateId>
                                                    <date>2017-07-06T08:00:00</date>
                                                    <value>70</value>
                                                    <unit>kg</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>bodyweight</covariateId>
                                                    <date>2018-07-06T08:00:00</date>
                                                    <value>150000</value>
                                                    <unit>g</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
                                            </covariates>
                                        </patient>
                                        <!-- List of the drugs informations we have concerning the patient -->
                                        <drugs>
                                            <!-- All the information regarding the drug -->
                                            <drug>
                                                <drugId>imatinib0</drugId>
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
                                                </samples>
                                                <!-- Personalised targets -->
                                                <targets>
                                                </targets>
                                            </drug>
                                            <drug>
                                                <drugId>imatinib1</drugId>
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
                                                </samples>
                                                <!-- Personalised targets -->
                                                <targets>
                                                </targets>
                                            </drug>
                                            <drug>
                                                <drugId>imatinib2</drugId>
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
                                            <drugId>imatinib0</drugId>
                                            <output>
                                                <format>xml</format>
                                                <language>fr</language>
                                            </output>
                                        </xpertRequest>
                                        <xpertRequest>
                                            <drugId>imatinib1</drugId>
                                            <output>
                                                <format>xml</format>
                                                <language>fr</language>
                                            </output>
                                        </xpertRequest>
                                        <xpertRequest>
                                            <drugId>imatinib2</drugId>
                                            <output>
                                                <format>xml</format>
                                                <language>fr</language>
                                            </output>
                                        </xpertRequest>
                                    </requests>
                                </query>)";

    string modelString0 = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                        <drugId>imatinib0</drugId>
                                        <drugModelId>ch.tucuxi.imatinib0.gotta2012</drugModelId>
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
                                                    <name lang='de'>Total Body Weight</name>
                                                </covariateName>
                                                <description>
                                                    <desc lang='en'>Total body weight of patient, in kilogramms</desc>
                                                    <desc lang='fr'>Total body weight of patient, in kilogramms</desc>
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
                                                        <text lang='fr'>The body weight shall be in the interval [44,100].</text>
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
    string modelString1 = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                        <drugId>imatinib1</drugId>
                                        <drugModelId>ch.tucuxi.imatinib1.gotta2012</drugModelId>
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
                                                    <name lang='fr'>Total Body Weight</name>
                                                </covariateName>
                                                <description>
                                                    <desc lang='de'>Total body weight of patient, in kilogramms</desc>
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
                                                        <text lang='fr'>The body weight shall be in the interval [44,100].</text>
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
    string modelString2 = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                        <drugId>imatinib2</drugId>
                                        <drugModelId>ch.tucuxi.imatinib2.gotta2012</drugModelId>
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
                                                    <name lang='fr'>Total Body Weight</name>
                                                </covariateName>
                                                <description>
                                                    <desc lang='fr'>Total body weight of patient, in kilogramms</desc>
                                                    <desc lang='en'>Total body weight of patient, in kilogramms</desc>
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
                                                        <text lang='de'>The body weight shall be in the interval [44,100].</text>
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

    cout << _testName << endl;

    // Prepare the XpertRequestResults
    vector<string> models {modelString0, modelString1, modelString2};
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResultNameProblem = xpertQueryResult->getXpertRequestResults()[0];
    Xpert::XpertRequestResult& xpertRequestResultDescProblem = xpertQueryResult->getXpertRequestResults()[1];
    Xpert::XpertRequestResult& xpertRequestResultValErrorProblem = xpertQueryResult->getXpertRequestResults()[2];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResultNameProblem);
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResultDescProblem);
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResultValErrorProblem);

    // Compare
    fructose_assert_eq(xpertRequestResultNameProblem.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResultNameProblem.getErrorMessage(), "Best drug model found but covariate definitions dont't support requested language.");

    fructose_assert_eq(xpertRequestResultDescProblem.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResultDescProblem.getErrorMessage(), "Best drug model found but covariate definitions dont't support requested language.");

    fructose_assert_eq(xpertRequestResultValErrorProblem.shouldContinueProcessing(), false);
    fructose_assert_eq(xpertRequestResultValErrorProblem.getErrorMessage(), "Best drug model found but covariate definitions dont't support requested language.");
}

void TestCovariateValidatorAndModelSelector::covariateValidatorAndModelSelector_getGoodTranslations_withModelThatSupportsRequestLanguageAndModelOnlyEnglish(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                <query version="1.0"
                                    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                    xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                    <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->

                                    <drugTreatment>
                                        <!-- All the information regarding the patient -->
                                        <patient>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>bodyweight</covariateId>
                                                    <date>2017-07-06T08:00:00</date>
                                                    <value>700</value>
                                                    <unit>kg</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
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
                                                </samples>
                                                <!-- Personalised targets -->
                                                <targets>
                                                </targets>
                                            </drug>
                                            <drug>
                                                <drugId>en_imatinib</drugId>
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
                                                <language>fr</language>
                                            </output>
                                        </xpertRequest>
                                        <xpertRequest>
                                            <drugId>en_imatinib</drugId>
                                            <output>
                                                <format>xml</format>
                                                <language>fr</language>
                                            </output>
                                        </xpertRequest>
                                    </requests>
                                </query>)";

    string onlyEnglishImatinibModelString = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                        <drugId>en_imatinib</drugId>
                                        <drugModelId>ch.tucuxi.en_imatinib.gotta2012</drugModelId>
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
                                                </covariateName>
                                                <description>
                                                    <desc lang='en'>Total body weight of patient, in kilogramms</desc>
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
                                                </covariateName>
                                                <description>
                                                    <desc lang='en'>Gastrointestinal stromal tumour</desc>
                                                </description>
                                                <unit>-</unit>
                                                <covariateType>standard</covaPATIENTpe>
                                                <dataType>bool</dataType>
                                                <interpolationType>direct</interpolationType>
                                                <refreshPeriod>
                                                  <unit>y</unit>
                                                  <value>1</value>
                                                </refreshPeriod>
                                                <covariateValue>
        MODEL                                       <standardValue>0</standardValue>
                                                </covariateValue>
                                                <validation>
                                                    <errorMessage>
                                                        <text lang='en'>Always correct.</text>
                                                    </errorMessage>
                                                    <operation>
         MODEL                                          <softFormula>
                                                            <inputs>
                                                                <input>
                                                                    <id>gist</id>
                                                                    <type>bool</type>
                                                                </input>
                                                            </inputs>
   PATIENT                                                  <code>
                                                                <![CDATA[return true;
                                                                ]]>
                                                            </code>
                                                        </softFormula>
                                                        <comments/>
                                                    </operation>
        PATIENT                                     <comments/>
                                                </validation>
                                                <comments/>
                                            </covariate>
                                            <covariate>
                                                <covariateId>sex</covariateId>
                                                <covariateName>
                                                    <name lang='en'>Sex</name>
                                                </covariateName>
                                                <description>
                                                    <desc lang='en'>Sex of the patient</desc>
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
                                                </covariateName>
                                                <description>
                                                    <desc lang='en'>Age of the patient, in years</desc>
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



    cout << _testName << endl;

    // Prepare the XpertRequestResult
    vector<string> models {TestUtils::originalImatinibModelString, onlyEnglishImatinibModelString};
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResultFrench = xpertQueryResult->getXpertRequestResults()[0];
    Xpert::XpertRequestResult& xpertRequestResultEnglish = xpertQueryResult->getXpertRequestResults()[1];

    Xpert::OutputLang Lang1 = xpertRequestResultFrench.getXpertRequest().getOutputLang();
    Xpert::OutputLang Lang2 = xpertRequestResultEnglish.getXpertRequest().getOutputLang();;

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResultFrench);
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResultEnglish);

    // Compare

    // French supported
    fructose_assert_eq(xpertRequestResultFrench.shouldContinueProcessing(), true);
    fructose_assert_eq(xpertRequestResultFrench.getErrorMessage(), "");
    fructose_assert_eq(Xpert::getStringWithEnglishFallback(xpertRequestResultFrench.getCovariateValidationResults()[2].getSource()->getName(), Lang1),
            "Poids total");
    fructose_assert_eq(Xpert::getStringWithEnglishFallback(xpertRequestResultFrench.getCovariateValidationResults()[2].getSource()->getDescription(), Lang1),
            "Poids total du patient, en kilogramme");
    fructose_assert_eq(xpertRequestResultFrench.getCovariateValidationResults()[2].getWarning(), "Le poids doit etre dans compris entre [44,100].");

    // Only english supported
    fructose_assert_eq(xpertRequestResultEnglish.shouldContinueProcessing(), true);
    fructose_assert_eq(xpertRequestResultEnglish.getErrorMessage(), "");
    fructose_assert_eq(Xpert::getStringWithEnglishFallback(xpertRequestResultEnglish.getCovariateValidationResults()[3].getSource()->getName(), Lang2),
            "Total Body Weight");
    fructose_assert_eq(Xpert::getStringWithEnglishFallback(xpertRequestResultEnglish.getCovariateValidationResults()[3].getSource()->getDescription(), Lang2),
            "Total body weight of patient, in kilogramms");
    fructose_assert_eq(xpertRequestResultEnglish.getCovariateValidationResults()[3].getWarning(), "The body weight shall be in the interval [44,100].");


}

void TestCovariateValidatorAndModelSelector::getCovariateValidationResults_returnsCorrectValues_whenCovariateValidatorAndModelSelectorSuccess(const string& _testName)
{

    string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                <query version="1.0"
                                    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                    xsi:noNamespaceSchemaLocation="tuberxpert_computing_query.xsd">

                                    <date>2018-07-11T13:45:30</date> <!-- Date the xml has been sent -->

                                    <drugTreatment>
                                        <!-- All the information regarding the patient -->
                                        <patient>
                                            <covariates>
                                                <covariate>
                                                    <covariateId>birthdate</covariateId>
                                                    <date>2018-07-11T10:45:30</date>
                                                    <value>1990-01-01T00:00:00</value>
                                                    <unit></unit>
                                                      <dataType>date</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>bodyweight</covariateId>
                                                    <date>2017-07-06T08:00:00</date>
                                                    <value>70</value>
                                                    <unit>kg</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
                                                <covariate>
                                                    <covariateId>bodyweight</covariateId>
                                                    <date>2018-07-06T08:00:00</date>
                                                    <value>150000</value>
                                                    <unit>g</unit>
                                                      <dataType>double</dataType>
                                                    <nature>discrete</nature>
                                                </covariate>
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
                                        </xpertRequest>
                                    </requests>
                                </query>)";

    cout << _testName << endl;

    // Prepare the XpertRequestResult
    vector<string> models {TestUtils::originalImatinibModelString};
    unique_ptr<Xpert::XpertQueryResult> xpertQueryResult;
    TestUtils::setupEnv(queryString, models, xpertQueryResult);

    Xpert::XpertRequestResult& xpertRequestResult = xpertQueryResult->getXpertRequestResults()[0];

    // Execute
    TestUtils::flowStepProvider.getCovariateValidatorAndModelSelector()->perform(xpertRequestResult);

    // Compare
    fructose_assert_eq(xpertRequestResult.shouldContinueProcessing(), true);
    fructose_assert_eq(xpertRequestResult.getErrorMessage(), "");
    fructose_assert_eq(xpertRequestResult.getDrugModel()->getDrugModelId() ,"ch.tucuxi.imatinib.gotta2012");

    const vector<Xpert::CovariateValidationResult>& results = xpertRequestResult.getCovariateValidationResults();
    fructose_assert_eq(results.size(), 5); // 3 covariates ( 2 weights + 1 birthdate) + Gist definition + Sex definition

    fructose_assert_eq(results[0].getSource()->getId(), "age");
    fructose_assert_eq(results[0].getValue(), "1990-01-01 00:00:00");
    fructose_assert_eq(results[0].getUnit().toString(), "");
    fructose_assert_eq(Xpert::dataTypeToString(results[0].getDataType()), "date");
    fructose_assert_eq(results[0].getType() == Xpert::CovariateType::PATIENT, true);
    fructose_assert_eq(results[0].getWarning(), "");

    fructose_assert_eq(results[1].getSource()->getId(), "gist");
    fructose_assert_eq(results[1].getValue(), "0.000000");
    fructose_assert_eq(results[1].getUnit().toString(), "-");
    fructose_assert_eq(Xpert::dataTypeToString(results[1].getDataType()), "bool");
    fructose_assert_eq(results[1].getType() == Xpert::CovariateType::MODEL, true);
    fructose_assert_eq(results[1].getWarning(), "");

    fructose_assert_eq(results[2].getSource()->getId(), "sex");
    fructose_assert_eq(results[2].getValue(), "0.500000");
    fructose_assert_eq(results[2].getUnit().toString(), "-");
    fructose_assert_eq(Xpert::dataTypeToString(results[2].getDataType()), "double");
    fructose_assert_eq(results[2].getType() == Xpert::CovariateType::MODEL, true);
    fructose_assert_eq(results[2].getWarning(), "");

    fructose_assert_eq(results[3].getSource()->getId(), "bodyweight");
    fructose_assert_eq(results[3].getValue(), "70");
    fructose_assert_eq(results[3].getUnit().toString(), "kg");
    fructose_assert_eq(Xpert::dataTypeToString(results[3].getDataType()), "double");
    fructose_assert_eq(results[3].getType() == Xpert::CovariateType::PATIENT, true);
    fructose_assert_eq(results[3].getWarning(), "");

    fructose_assert_eq(results[4].getSource()->getId(), "bodyweight");
    fructose_assert_eq(results[4].getValue(), "150000");
    fructose_assert_eq(results[4].getUnit().toString(), "g");
    fructose_assert_eq(Xpert::dataTypeToString(results[4].getDataType()), "double");
    fructose_assert_eq(results[4].getType() == Xpert::CovariateType::PATIENT, true);
    fructose_assert_eq(results[4].getWarning(), "The body weight shall be in the interval [44,100].");
}
