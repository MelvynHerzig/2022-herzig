#ifndef TEST_GENERALADJUSTMENTTRAITCREATOR_H
#define TEST_GENERALADJUSTMENTTRAITCREATOR_H

#include <memory>

#include "tucucore/drugmodelrepository.h"
#include "tucucore/drugmodelimport.h"
#include "tucucore/pkmodel.h"
#include "tucucore/drugmodelchecker.h"

#include "tuberxpert/flow/general/generalxpertflowstepprovider.h"
#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/result/xpertresult.h"

#include "fructose/fructose.h"

/// \brief Tests for AdjustmentTraitCreator from the GeneralXpertFlowStepProvider.
///        This class performs varius adjustment creations
///        to check if the result is what was expected. Each test tests an attribute (or a subset of attributes)
///        of the adjustment trait returned.
/// \date 17/06/2022
/// \author Herzig Melvyn
struct TestGeneralAdjustmentTraitCreator : public fructose::test_base<TestGeneralAdjustmentTraitCreator>
{

    /// \brief Format used to create date and time during test.
    const std::string DATE_FORMAT = "%Y-%m-%dT%H:%M:%S";

    /// \brief General flow step provider used to get the adjustment trait creator object to test.
    ///        We set the adjustment trait creator exectuion time to 2022-06-20 10h00.
    Tucuxi::XpertFlow::GeneralXpertFlowStepProvider generalFlowStepProvider{Tucuxi::Common::DateTime("2022-06-20T10:00:00", DATE_FORMAT)};

    /// \brief Imatinib model string used during tests when a drug without a standard treatment is needed.
    std::string imatinibModelString = R"(<?xml version="1.0" encoding="UTF-8"?>
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

    /// \brief Busulfan model string used during tests when a drug with a standard treatment is needed.
    std::string busulfanModelString = R"(<?xml version="1.0" encoding="UTF-8"?>
                                        <model version='0.6' xsi:noNamespaceSchemaLocation='drugfile.xsd' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>
                                            <!-- Drug history -->
                                            <history>
                                                <revisions>
                                                    <revision>
                                                        <revisionAction>creation</revisionAction>
                                                        <revisionAuthorName>Yann Thoma</revisionAuthorName>
                                                        <institution>HEIG-VD // REDS</institution>
                                                        <email>yann.thoma@heig-vd.ch</email>
                                                        <date>2018-09-14</date>
                                                        <comments>
                                                            <comment lang="en">This file is based on the first version of
                                                                busulfan validated by Sylvain Goutelle : ch.heig-vd.ezechiel.busulfan_children.xml
                                                            </comment>
                                                        </comments>
                                                    </revision>
                                                    <revision>
                                                        <revisionAction>modification</revisionAction>
                                                        <revisionAuthorName>Yann Thoma</revisionAuthorName>
                                                        <institution>HEIG-VD</institution>
                                                        <email>yann.thoma@heig-vd.ch</email>
                                                        <date>2021-01-04</date>
                                                        <comments>
                                                            <comment lang='en'>Added the age covariate, for domain checking</comment>
                                                        </comments>
                                                    </revision>
                                                </revisions>
                                            </history>
                                            <!-- Drug description -->
                                            <head>
                                                <drug>
                                                    <atcs>
                                                        <atc>L01AB01</atc>
                                                    </atcs>
                                                    <activeSubstances>
                                                        <activeSubstance>busulfan</activeSubstance>
                                                    </activeSubstances>
                                                    <drugName>
                                                        <name lang="en">Busulfan</name>
                                                        <name lang="fr">Busulfan</name>
                                                    </drugName>
                                                    <drugDescription>
                                                        <desc lang="en">TODO : Add a description here</desc>
                                                    </drugDescription>
                                                    <tdmStrategy>
                                                        <text lang="en">TODO : Add a TDM strategy</text>
                                                    </tdmStrategy>
                                                </drug>
                                                <study>
                                                    <studyName>
                                                        <name lang="en">Pharmacokinetic behavior and appraisal of intravenous busulfan dosing in infants and older children: the results of a population pharmacokinetic study from a large pediatric cohort undergoing hematopoietic stem-cell transplantation</name>
                                                    </studyName>
                                                    <studyAuthors>Paci A., Vassal G., Moshous D., Dalle J.H., Bleyzac N., Neven B., Galambrun C., Kemmel V., Abdi Z.D., Broutin S., Pétain A., Nguyen L.</studyAuthors>
                                                    <description>
                                                        <desc lang="en">Based on the article byPaci et al. Pharmacokinetic Behavior and Appraisal of Intravenous Busulfan Dosing in Infants and Older Children:
                                                        The Results of a Population Pharmacokinetic Study From a Large Pediatric Cohort Undergoing Hematopoietic Stem-Cell Transplantation.
                                                        Ther Drug Monit 2012, 34;:198-208</desc>
                                                    </description>
                                                    <references>
                                                        <reference type="bibtex">TODO : Add bibtex
                                                        </reference>
                                                    </references>
                                                </study>
                                                <comments/>
                                            </head>

                                            <!-- Drug data -->
                                            <drugModel>
                                                <drugId>busulfan</drugId> <!-- id d'une des substances actives -->
                                                <drugModelId>ch.tucuxi.busulfan.paci2012</drugModelId>

                                                <domain>
                                                    <description>
                                                        <desc lang="en">All children (Age range: 10 days - 15 years, Weight range: 3.5 - 62.5 kg)</desc>
                                                    </description>
                                                    <constraints>
                                                        <constraint>
                                                            <constraintType>hard</constraintType>
                                                            <errorMessage>
                                                                <text lang="en">The age shall be positive</text>
                                                            </errorMessage>
                                                            <requiredCovariates>
                                                                <covariateId>age</covariateId>
                                                            </requiredCovariates>
                                                            <checkOperation>
                                                                <softFormula>
                                                                    <inputs>
                                                                        <input>
                                                                            <id>age</id>
                                                                            <type>int</type>
                                                                        </input>
                                                                    </inputs>
                                                                    <code><![CDATA[return (age > 0);
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
                                                                <text lang="en">The weight should not be too much</text>
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
                                                                    <code><![CDATA[return (bodyweight < 100);
                                                                        ]]>
                                                                    </code>
                                                                </softFormula>
                                                                <comments/>
                                                            </checkOperation>
                                                            <comments/>
                                                        </constraint>
                                                    </constraints>
                                                </domain>



                                                <!-- Drug model covariates -->
                                                <covariates>

                                                    <covariate>
                                                        <covariateId>age</covariateId>
                                                        <covariateName>
                                                            <name lang='en'>Age</name>
                                                            <name lang='fr'>Age</name>
                                                        </covariateName>
                                                        <description>
                                                            <desc lang='en'>Age of the patient, in years, only used for domain checking</desc>
                                                            <desc lang='fr'>Âge du patient, en années, utilisé pour valider l'usage du modèle</desc>
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
                                                            <standardValue>10</standardValue>
                                                        </covariateValue>
                                                        <validation> <!-- pourrait être une contrainte -->
                                                            <errorMessage><text lang="fr"></text></errorMessage>
                                                            <operation>
                                                                <softFormula>
                                                                    <inputs>
                                                                        <input>
                                                                            <id>age</id>
                                                                            <type>double</type>
                                                                        </input>
                                                                    </inputs>
                                                                    <code><![CDATA[
                                                                        return (age  > 0);
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
                                                            <name lang="en">Total Body Weight</name>
                                                            <name lang="fr">Poids total</name>
                                                        </covariateName>
                                                        <description>
                                                            <desc lang="en">Total body weight of patient, in kilogramms</desc>
                                                            <desc lang="fr">Poids total du patient, en kilogramme</desc>
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
                                                            <standardValue>9</standardValue>
                                                        </covariateValue>
                                                        <validation> <!-- pourrait être une contrainte -->
                                                            <errorMessage><text lang="fr"></text></errorMessage>
                                                            <operation>
                                                                <softFormula>
                                                                    <inputs>
                                                                        <input>
                                                                            <id>bodyweight</id>
                                                                            <type>double</type>
                                                                        </input>
                                                                    </inputs>
                                                                    <code><![CDATA[
                                                                        return (bodyweight < 300) && (bodyweight > 0);
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
                                                        <activeMoietyId>busulfan</activeMoietyId>
                                                        <activeMoietyName>
                                                            <name lang="en">Busulfan</name>
                                                        </activeMoietyName>
                                                        <unit>mg/l</unit>
                                                        <analyteIdList>
                                                            <analyteId>busulfan</analyteId>
                                                        </analyteIdList>
                                                        <analytesToMoietyFormula>
                                                            <hardFormula>direct</hardFormula>
                                                            <comments/>
                                                        </analytesToMoietyFormula>
                                                        <!-- Drug targets -->
                                                        <targets>
                                                            <target>
                                                                <targetType>peak</targetType>
                                                                <targetValues>
                                                                <unit>mg/l</unit>
                                                                    <min>
                                                                        <standardValue>1.0</standardValue>
                                                                    </min>
                                                                    <max>
                                                                        <standardValue>1.4</standardValue>
                                                                    </max>
                                                                    <best>
                                                                        <standardValue>1.2</standardValue>
                                                                    </best>
                                                                    <toxicityAlarm><standardValue>10000.0</standardValue></toxicityAlarm>
                                                                    <inefficacyAlarm><standardValue>0.0</standardValue></inefficacyAlarm>
                                                                </targetValues>
                                                                <times>
                                                                    <unit>h</unit>
                                                                    <min>
                                                                        <standardValue>0.9</standardValue>
                                                                    </min>
                                                                    <max>
                                                                        <standardValue>3</standardValue>
                                                                    </max>
                                                                    <best>
                                                                        <standardValue>2.5</standardValue>
                                                                    </best>
                                                                </times>
                                                                <comments>
                                                                    <comment lang="en">The time values shall be checked. They initially were not aligned with possible infusion times. A Toxicity and inefficacyAlarm should be added</comment>
                                                                </comments>
                                                            </target>
                                                            <target>
                                                                <targetType>cumulativeAuc</targetType>
                                                                <targetValues>
                                                                    <unit>mg*h/l</unit>
                                                                    <min>
                                                                        <standardValue>59.0</standardValue>
                                                                    </min>
                                                                    <max>
                                                                        <standardValue>99.0</standardValue>
                                                                    </max>
                                                                    <best>
                                                                        <standardValue>72.0</standardValue>
                                                                    </best>
                                                                    <!-- TODO : Check that -->
                                                                    <toxicityAlarm><standardValue>10000.0</standardValue></toxicityAlarm>
                                                                    <inefficacyAlarm><standardValue>0.0</standardValue></inefficacyAlarm>
                                                                </targetValues>
                                                                <comments>
                                                                    <comment lang="en">This is the target cumulative for a regimen with 16 doses over 4 days, i.e. a dose every 6h.</comment>
                                                                </comments>
                                                            </target>
                                                        </targets>
                                                    </activeMoiety>
                                                </activeMoieties>

                                                <analyteGroups>
                                                    <analyteGroup>
                                                        <groupId>busulfan</groupId>
                                                        <pkModelId>linear.1comp.macro</pkModelId>
                                                        <analytes>
                                                            <analyte>
                                                                <analyteId>busulfan</analyteId>
                                                                <unit>mg/l</unit>
                                                                <molarMass>
                                                                  <value>246.292</value>
                                                                    <unit>g/mol</unit>
                                                                </molarMass>
                                                                <description>
                                                                    <desc lang="en"></desc>
                                                                </description> <!-- peut être vide -->

                                                                <errorModel> <!-- optional -->
                                                                    <errorModelType>mixed</errorModelType>
                                                                    <sigmas>
                                                                        <sigma>
                                                                            <standardValue>0.057</standardValue>
                                                                        </sigma>
                                                                        <sigma>
                                                                            <standardValue>0.11</standardValue>
                                                                        </sigma>
                                                                    </sigmas>
                                                                    <comments>
                                                                        <comment lang='fr'>add: en mg/L; prop, en % ie 0.11 = 11.0%</comment>
                                                                    </comments>
                                                                </errorModel>
                                                                <comments/>
                                                            </analyte>
                                                        </analytes>

                                                        <!-- Drug parameters -->
                                                        <dispositionParameters>
                                                            <parameters>
                                                                <parameter>
                                                                    <parameterId>CL</parameterId>
                                                                    <unit>l/h</unit>
                                                                    <parameterValue>
                                                                        <standardValue>2.18</standardValue>
                                                                        <aprioriComputation>

                                                                            <softFormula>
                                                                                <inputs>
                                                                                    <input>
                                                                                        <id>bodyweight</id>
                                                                                        <type>double</type>
                                                                                    </input>
                                                                                </inputs>
                                                                                <code><![CDATA[
                                                                        theta_1 = 2.18;

                                                                        if (bodyweight < 9.0)
                                                                          theta_2=1.25;
                                                                        else
                                                                          theta_2=0.76;

                                                                        return theta_1 * Math.pow((bodyweight/ 9),theta_2);

                                                                 ]]>
                                                                                </code>
                                                                            </softFormula>
                                                                            <comments/>
                                                                        </aprioriComputation>
                                                                    </parameterValue>
                                                                    <bsv>
                                                                        <bsvType>proportional</bsvType> <!-- même chose que le modèle d'erreur -->
                                                                        <stdDevs>
                                                                            <stdDev>0.23</stdDev>
                                                                        </stdDevs>
                                                                    </bsv>
                                                                    <validation>
                                                                        <errorMessage><text lang="fr"></text></errorMessage>
                                                                        <operation>
                                                                            <softFormula>
                                                                                <inputs>
                                                                                    <input>
                                                                                        <id>CL</id>
                                                                                        <type>double</type>
                                                                                    </input>
                                                                                </inputs>
                                                                                <code><![CDATA[
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
                                                                        <standardValue>6.6</standardValue>
                                                                        <aprioriComputation>

                                                                            <softFormula>
                                                                                <inputs>
                                                                                    <input>
                                                                                        <id>bodyweight</id>
                                                                                        <type>double</type>
                                                                                    </input>
                                                                                </inputs>
                                                                                <code><![CDATA[
                                                                        theta_3=0.86;
                                                        return Math.pow(bodyweight,theta_3);
                                                                                    ]]>
                                                                                </code>
                                                                            </softFormula>
                                                                            <comments/>
                                                                        </aprioriComputation>
                                                                    </parameterValue>
                                                                    <bsv>
                                                                        <bsvType>proportional</bsvType>
                                                                        <stdDevs>
                                                                            <stdDev>0.22</stdDev>
                                                                        </stdDevs>
                                                                    </bsv>
                                                                    <validation>
                                                                        <errorMessage><text lang="fr"></text></errorMessage>
                                                                        <operation>
                                                                            <softFormula>
                                                                                <inputs>
                                                                                    <input>
                                                                                        <id>V</id>
                                                                                        <type>double</type>
                                                                                    </input>
                                                                                </inputs>
                                                                                <code><![CDATA[
                                                                                    return V < 300.0 and V > 0.0;
                                                                                    ]]>
                                                                                </code>
                                                                            </softFormula>
                                                                            <comments/>
                                                                        </operation>
                                                                        <comments/>
                                                                    </validation>
                                                                    <comments>
                                                                        <comment lang="en">Typical volume calculated for a patients with weight = 75 kg</comment>
                                                                    </comments>
                                                                </parameter>
                                                            </parameters>

                                                            <!-- elimination parameters correlations -->
                                                            <correlations />
                                                        </dispositionParameters>
                                                    </analyteGroup>
                                                </analyteGroups>

                                                <!-- We can have various formulation and routes, and for each one a set of absorption parameters and available dosages -->

                                                <formulationAndRoutes default="id0">
                                                    <formulationAndRoute>
                                                        <formulationAndRouteId>id0</formulationAndRouteId>
                                                        <formulation>parenteralSolution</formulation><!-- dictionnaire -->
                                                        <administrationName>champ libre</administrationName>
                                                        <administrationRoute>intravenousDrip</administrationRoute> <!-- dictionnaire -->
                                                        <absorptionModel>infusion</absorptionModel>


                                                        <!-- Drug dosages -->
                                                        <dosages>


                                                            <standardTreatment>
                                                                <isFixedDuration>true</isFixedDuration>
                                                                <timeValue>
                                                                    <unit>d</unit>
                                                                    <value>4</value>
                                                                </timeValue>
                                                            </standardTreatment>

                                                            <analyteConversions>
                                                                <analyteConversion>
                                                                    <analyteId>busulfan</analyteId>
                                                                    <factor>1</factor>
                                                                </analyteConversion>
                                                            </analyteConversions>

                                                            <availableDoses>
                                                                <unit>mg</unit>
                                                                <default>
                                                                    <standardValue>10</standardValue>
                                                                </default>
                                                                <rangeValues>
                                                                    <from>
                                                                        <standardValue>1</standardValue>
                                                                    </from>
                                                                    <to>
                                                                        <standardValue>400</standardValue>
                                                                    </to>
                                                                    <step>
                                                                        <standardValue>1</standardValue>
                                                                    </step>
                                                                </rangeValues>
                                                            </availableDoses>

                                                            <availableIntervals>
                                                                <unit>h</unit>
                                                                <default>
                                                                    <standardValue>6</standardValue>
                                                                </default>
                                                                <fixedValues>
                                                                    <value>6</value>
                                                                    <value>24</value>
                                                                </fixedValues>
                                                            </availableIntervals>
                                                            <availableInfusions>
                                                                <unit>min</unit>
                                                                <default>
                                                                    <standardValue>120</standardValue>
                                                                </default>
                                                                <fixedValues>
                                                                    <value>60</value>
                                                                    <value>120</value>
                                                                    <value>180</value>
                                                                </fixedValues>
                                                            </availableInfusions>
                                                            <comments/>
                                                        </dosages>

                                                        <absorptionParameters/>

                                                    </formulationAndRoute>
                                                </formulationAndRoutes>



                                                <timeConsiderations>
                                                    <!-- Drug half-life -->
                                                    <halfLife>
                                                        <unit>h</unit>
                                                        <duration>
                                                            <standardValue>12</standardValue>
                                                        </duration>
                                                        <multiplier>10</multiplier>
                                                        <comments>
                                                            <comment lang="en">TODO : Find the half life</comment>
                                                        </comments>
                                                    </halfLife>


                                                    <outdatedMeasure>
                                                        <unit>d</unit>
                                                        <duration>
                                                            <standardValue>100</standardValue>
                                                        </duration>
                                                        <comments>
                                                            <comment lang="en">TODO : This value is not set now</comment>
                                                        </comments>
                                                    </outdatedMeasure>
                                                </timeConsiderations>

                                                <!-- Drug general comments -->
                                                <comments>
                                                </comments>
                                            </drugModel>
                                        </model>
                                        )";


    /// \brief Sets up environment for clean execution of the adjustment trait creator. Loads the query, makes the
    ///        XpertResult object, loads the model and attributes it to the first XpertRequestResult of the XpertResult.
    /// \param _queryString Query string to load.
    /// \param _model Model string to put as drug model attribute of the XpertRequestResult of the first request.
    /// \param _xpertResult Object that will contain the result of this function execution.
    void setupEnv(const std::string& _queryString,
                  const std::string& _model,
                  std::unique_ptr<Tucuxi::XpertResult::XpertResult>& _xpertResult) {

        // Drug models repository creation
        Tucuxi::Common::ComponentManager* pCmpMgr = Tucuxi::Common::ComponentManager::getInstance();

        auto drugModelRepository =
                dynamic_cast<Tucuxi::Core::DrugModelRepository*>(Tucuxi::Core::DrugModelRepository::createComponent());

        pCmpMgr->registerComponent("DrugModelRepository", drugModelRepository);

        Tucuxi::Core::DrugModelImport drugModelImport;
        std::unique_ptr<Tucuxi::Core::DrugModel> drugModel;
        if (drugModelImport.importFromString(drugModel, _model) == Tucuxi::Core::DrugModelImport::Status::Ok) {

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
        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;
        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, _queryString);

        if (importResult != Tucuxi::XpertQuery::XpertQueryImport::Status::Ok) {
            throw std::runtime_error("Setup failed");
        }

        _xpertResult = std::make_unique<Tucuxi::XpertResult::XpertResult>(move(query));
        Tucuxi::XpertResult::XpertRequestResult& xrr =  _xpertResult->getXpertRequestResults()[0];
        xrr.setDrugModel(drugModelRepository->getDrugModelsByDrugId(xrr.getXpertRequest().getDrugID())[0]);
    }

    /// \brief Checks that there is an error if the treatment of XpertRequestResult is nullptr.
    /// \param _testName Name of the test
    void errorWhenNoTreatment(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::XpertResult::XpertRequestResult xrr{nullptr, nullptr, ""};

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), false);
        fructose_assert_eq(xrr.getErrorMessage(), "No treatment set.");
        fructose_assert_eq(xrr.getSampleResults().size(), 0);
    }

    /// \brief Checks that there is an error if the drug model of XpertRequestResult is nullptr.
    /// \param _testName Name of the test
    void errorWhenNoDrugModel(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
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
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertQuery::XpertQueryData> query = nullptr;

        Tucuxi::XpertQuery::XpertQueryImport importer;
        Tucuxi::XpertQuery::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

        if (importResult != Tucuxi::XpertQuery::XpertQueryImport::Status::Ok) {
            throw std::runtime_error("import failded.");
        }

        Tucuxi::XpertResult::XpertResult xr{move(query)};

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xr.getXpertRequestResults()[0]);

        fructose_assert_eq(xr.getXpertRequestResults()[0].shouldBeHandled(), false);
        fructose_assert_eq(xr.getXpertRequestResults()[0].getErrorMessage(), "No drug model set.");
    }

    /// \brief This methods checks that the adjustment trait creator sets the expected number of points
    ///        per hours (i.e. 20) and that there is no error.
    /// \param _testName Name of the test
    void nbPointsPerJourIsTwenty(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
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
                                            </requestXpert>
                                        </requests>
                                    </query>)";



        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getNbPointsPerHour(), 20);
    }

    /// \brief This methods checks that the adjustment trait creator sets the expected computing option
    ///        (i.e. AllActiveMoieties, RetrieveStatistics, RetrieveParameters, RetrieveCovariates) and that there is no error.
    /// \param _testName Name of the test
    void computingOptionIsRetrieveAllAndAllActiveMoieties(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
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
                                            </requestXpert>
                                        </requests>
                                    </query>)";



        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);

        Tucuxi::Core::ComputingOption computingOption = xrr.getAdjustmentTrait()->getComputingOption();
        fructose_assert_eq(computingOption.getCompartmentsOption() == Tucuxi::Core::CompartmentsOption::AllActiveMoieties, true);
        fructose_assert_eq(computingOption.retrieveStatistics() == Tucuxi::Core::RetrieveStatisticsOption::RetrieveStatistics, true);
        fructose_assert_eq(computingOption.retrieveParameters() == Tucuxi::Core::RetrieveParametersOption::RetrieveParameters, true);
        fructose_assert_eq(computingOption.retrieveCovariates() == Tucuxi::Core::RetrieveCovariatesOption::RetrieveCovariates, true);
    }

    /// \brief This methods checks that the adjustment trait creator sets the computing option
    ///        prediction paramters type to Apriori when there is no dosage and no sample.
    /// \param _testName Name of the test
    void computingOptionIsAprioriWhenNoDosageAndNoSample(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                                    </samples>
                                                    <!-- Personalised targets -->
                                                    <targets>
                                                    </targets>
                                                </drug>
                                            </drugs>
                                        </drugTreatment>
                                        <!-- List of the requests we want the server to take care of -->
                                        <requests>
                                            <requestXpert>
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
                                            </requestXpert>
                                        </requests>
                                    </query>)";



        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getComputingOption().getParametersType() == Tucuxi::Core::PredictionParameterType::Apriori, true);
    }

    /// \brief This methods checks that the adjustment trait creator sets the computing option
    ///        prediction paramters type to Apriori when there is dosage but no sample.
    /// \param _testName Name of the test
    void computingOptionIsAprioriWhenDosageButNoSample(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                                    </samples>
                                                    <!-- Personalised targets -->
                                                    <targets>
                                                    </targets>
                                                </drug>
                                            </drugs>
                                        </drugTreatment>
                                        <!-- List of the requests we want the server to take care of -->
                                        <requests>
                                            <requestXpert>
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
                                            </requestXpert>
                                        </requests>
                                    </query>)";



        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getComputingOption().getParametersType() == Tucuxi::Core::PredictionParameterType::Apriori, true);
    }

    /// \brief This methods checks that the adjustment trait creator sets the computing option
    ///        prediction paramters type to Aposteriori when there is dosages and samples.
   /// \param _testName Name of the test
    void computingOptionIsAposterioriWhenDosageAndSample(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
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
                                            </requestXpert>
                                        </requests>
                                    </query>)";



        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);

        Tucuxi::Core::ComputingOption computingOption = xrr.getAdjustmentTrait()->getComputingOption();
        fructose_assert_eq(computingOption.getCompartmentsOption() == Tucuxi::Core::CompartmentsOption::AllActiveMoieties, true);
        fructose_assert_eq(computingOption.retrieveStatistics() == Tucuxi::Core::RetrieveStatisticsOption::RetrieveStatistics, true);
        fructose_assert_eq(computingOption.retrieveParameters() == Tucuxi::Core::RetrieveParametersOption::RetrieveParameters, true);
        fructose_assert_eq(computingOption.retrieveCovariates() == Tucuxi::Core::RetrieveCovariatesOption::RetrieveCovariates, true);
    }

    /// \brief This method checks that the adjustment time is set to the adjustment time of the request when it is set.
    /// \param _testName Name of the test
    void adjustmentTimeIsRequestAdjustmentTimeWhenManuallySet(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
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
                                            </requestXpert>
                                        </requests>
                                    </query>)";



        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getAdjustmentTime(),  Tucuxi::Common::DateTime("2018-07-06T08:00:00", DATE_FORMAT));
    }

    /// \brief This method checks that the adjustment time is set to the computing time (2022-06-20 10h) plus one hour when there
    ///        is no dosage history.
    /// \param _testName Name of the test
    void adjustmentTimeIsComputingTimePlusOneHourWithoutDosageHistoryAndNotManuallySet(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";



        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getAdjustmentTime(),  Tucuxi::Common::DateTime("2022-06-20T11:00:00", DATE_FORMAT));
    }

    /// \brief This method checks that the adjustment time is set to the computing time (2022-06-20 10h00) plus one hour when there
    ///        is a dosage history but in the future and the adjustment time is not set in the request.
    /// \param _testName Name of the test
    void adjustmentTimeIsComputingTimePlusOneHourWithFutureDosageHistoryAndNotManuallySet(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                                                <start>2023-07-06T08:00:00</start>
                                                                <end>2023-07-08T08:00:00</end>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getAdjustmentTime(),  Tucuxi::Common::DateTime("2022-06-20T11:00:00", DATE_FORMAT));
    }

    /// \brief This method checks that the adjustment time is set to the next intake time when there
    ///        is an ongoing dosage history and not manually set in the request.
    /// \param _testName Name of the test
    void adjustmentTimeIsNextIntakeTimeWhenOngoingDosageHistoryAndNotManuallySet(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
                                        <clientId>124568</clientId>
                                        <date>2022-06-20T10:00:00</date> <!-- Date the xml has been sent -->
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
                                                                <start>2022-06-19T08:00:00</start>
                                                                <end>2022-06-20T08:00:00</end>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getAdjustmentTime(),  Tucuxi::Common::DateTime("2022-06-20T20:00:00", DATE_FORMAT));
    }

    /// \brief This method checks that the adjustment time is the resulting time of the half life (x2) added
    ///        on top of the last intake until the computation time is reached. In the test, the dosage history is in
    ///        2018 (07-06 9h00 - 07-08 9h00). Therefore, the last intake is at 2018-07-07 21h00.
    ///        Since the half life of the imatinib is 12 hours, 2 x hal life is 1 day. Thus, if we add 1 day on top of
    ///        2018-07-08 9h00 until the computation time is reached (2022-06-20 10h00), the adjustment time should be
    ///        2022-06-20 21h00.
    /// \param _testName Name of the test
    void adjustmentTimeIsHalfLifeApproximatedWhenDosageHistoryOverAndNotManuallySet(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                                                <start>2018-07-06T09:00:00</start>
                                                                <end>2018-07-08T09:00:00</end>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getAdjustmentTime(),  Tucuxi::Common::DateTime("2022-06-20T21:00:00", DATE_FORMAT));
    }


    /// \brief This method checks that start and end date times of the adjustment are the starting time plus
    ///        the standard treatment duration when there is a standard treatment and a dosage history.
    ///        In that case, the dosage history starts on 2022-06-19 8h00 and the standard treatment lasts
    ///        4 days. So, start and end date times should be 2022-06-23 8h00 and 2018-07-10 8h00.
    /// \param _testName Name of the test
    void startEndDatesAreDosageHistoryStartPlusStandardTreatmentDurationWhenOngoingStandardTreatmentAndDosageHistory(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>busulfan</queryId>
                                        <clientId>124568</clientId>
                                        <date>2022-06-20T13:45:30</date> <!-- Date the xml has been sent -->
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
                                                    <drugId>busulfan</drugId>
                                                    <activePrinciple>something</activePrinciple>
                                                    <brandName>somebrand</brandName>
                                                    <atc>something</atc>
                                                    <!-- All the information regarding the treatment -->
                                                    <treatment>
                                                        <dosageHistory>
                                                            <dosageTimeRange>
                                                                <start>2022-06-19T08:00:00</start>
                                                                <end>2022-06-21T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>06:00:00</interval>
                                                                            <dose>
                                                                                <value>10</value>
                                                                                <unit>mg</unit>
                                                                                <infusionTimeInMinutes>120</infusionTimeInMinutes>
                                                                            </dose>
                                                                            <formulationAndRoute>
                                                                                <formulation>parenteralSolution</formulation>
                                                                                <administrationName>foo bar</administrationName>
                                                                                <administrationRoute>intravenousDrip</administrationRoute>
                                                                                <absorptionModel>infusion</absorptionModel>
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
                                            <requestXpert>
                                                <drugId>busulfan</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, busulfanModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getStart(),  Tucuxi::Common::DateTime("2022-06-19T08:00:00", DATE_FORMAT));
        fructose_assert_eq(xrr.getAdjustmentTrait()->getEnd(),  Tucuxi::Common::DateTime("2022-06-23T08:00:00", DATE_FORMAT));
    }

    /// \brief This method checks that start and end date times of the adjustment are the computation time plus
    ///        the standard treatment duration when there is a standard treatment but no dosage history.
    ///        In that case, the computation time is on 2022-06-20 10h00 and the standard treatment lasts
    ///        4 days. So, start and end date times should be 2022-06-20 10h00 and 2022-06-24 10h00.
    /// \param _testName Name of the test
    void startEndDatesAreComputationTimePlusStandardTreatmentDurationWhenStandardTreatmentButNoDosageHistory(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>busulfan</queryId>
                                        <clientId>124568</clientId>
                                        <date>2022-06-20T13:45:30</date> <!-- Date the xml has been sent -->
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
                                                    <drugId>busulfan</drugId>
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
                                            <requestXpert>
                                                <drugId>busulfan</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, busulfanModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getStart(),  Tucuxi::Common::DateTime("2022-06-20T10:00:00", DATE_FORMAT));
        fructose_assert_eq(xrr.getAdjustmentTrait()->getEnd(),  Tucuxi::Common::DateTime("2022-06-24T10:00:00", DATE_FORMAT));
    }

    /// \brief This method checks that the adjustment trait creation fails when there is a standard treatment
    ///        that is over before the computation time. In this test, the dosage history starts on the
    ///        2018-07-06 8h00 and the standard treatment lasts 4 days. Thus, the start and end date of the adjustment
    ///        should be 2018-07-06 8h00 - 2018-07-10 8h00. In consequences, the treatment is already over on the computation
    ///        time ( 2018-07-10 8h00 < 2022-06-20 10h00).
    /// \param _testName Name of the test
    void errorWhenStandardTreatmentIsOverBeforeComputationTime(const std::string& _testName)
    {

        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>busulfan</queryId>
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
                                                    <drugId>busulfan</drugId>
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
                                                                            <interval>06:00:00</interval>
                                                                            <dose>
                                                                                <value>10</value>
                                                                                <unit>mg</unit>
                                                                                <infusionTimeInMinutes>120</infusionTimeInMinutes>
                                                                            </dose>
                                                                            <formulationAndRoute>
                                                                                <formulation>parenteralSolution</formulation>
                                                                                <administrationName>foo bar</administrationName>
                                                                                <administrationRoute>intravenousDrip</administrationRoute>
                                                                                <absorptionModel>infusion</absorptionModel>
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
                                            <requestXpert>
                                                <drugId>busulfan</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, busulfanModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), false);
        fructose_assert_eq(xrr.getErrorMessage(), "Based on the standard treatment in the model:ch.tucuxi.busulfan.paci2012, considering the oldest dosage is the treatment start, the treatment is already over.");
    }

    /// \brief This method checks that start and end date is placed regarding the adjustment time.
    ///        Start should be adjustment time minus one hour.
    ///        End should be start plus seven days.
    ///        In this test, the adjustment time is 2022-06-20 20h00. Therefore, the start should be 2022-06-20 19h00
    ///        and the end should be 2022-06-27 19h00.
    ///
    /// \param _testName Name of the test
    void startEndDatesAreDefinedOnAdjustmentTimeWhenNoStandardTreatment(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
                                        <clientId>124568</clientId>
                                        <date>2022-06-20T13:45:30</date> <!-- Date the xml has been sent -->
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
                                                                <start>2022-06-19T08:00:00</start>
                                                                <end>2022-06-21T08:00:00</end>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getStart(),  Tucuxi::Common::DateTime("2022-06-20T19:00:00", DATE_FORMAT));
        fructose_assert_eq(xrr.getAdjustmentTrait()->getEnd(),  Tucuxi::Common::DateTime("2022-06-27T19:00:00", DATE_FORMAT));
    }

    /// \brief This method checks that best candidates option is best dosage per interval.
    ///
    /// \param _testName Name of the test
    void bestCandidatesOptionIsBestDosagePerInterval(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getBestCandidatesOption() == Tucuxi::Core::BestCandidatesOption::BestDosagePerInterval, true);
    }

    /// \brief This method checks that the loading dose option is retrieved from request
    ///        when it is sets. The request sets that no loading dose is allowed.
    ///
    /// \param _testName Name of the test
    void loadingOptionIsRetrievedFromRequestWhenSet(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getLoadingOption() == Tucuxi::Core::LoadingOption::NoLoadingDose, true);
    }

    /// \brief This method checks that the loading dose option is retrieved from the drug model
    ///        when the request doesn't set it.
    ///
    ///        At the moment, the "isLoadingDoseRecommended" is not set in the drug model files. By default,
    ///        the drug model importer sets it to true. The result of this test might by tweaked if the imatinib
    ///        model string is updated.
    ///
    /// \param _testName Name of the test
    void loadingOptionIsRetrievedFromDrugModelWhenNotSet(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <!-- There is no loading set -->
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getLoadingOption() == Tucuxi::Core::LoadingOption::LoadingDoseAllowed, true);
    }

    /// \brief This method checks that the rest period option is retrieved from request
    ///        when it is sets. The request sets that no rest period is allowed.
    ///
    /// \param _testName Name of the test
    void restPeriodOptionIsRetrievedFromRequestWhenSet(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getRestPeriodOption() == Tucuxi::Core::RestPeriodOption::NoRestPeriod, true);
    }

    /// \brief This method checks that the rest period option is retrieved from the drug model
    ///        when the request doesn't set it.
    ///
    ///        At the moment, the "isRestPeriodRecommended" is not set in the drug model files. By default,
    ///        the drug model importer sets it to true. The result of this test might by tweaked if the imatinib
    ///        model string is updated.
    ///
    /// \param _testName Name of the test
    void restPeriodOptionIsRetrievedFromDrugModelWhenNotSet(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <!-- There is no rest period set -->
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getRestPeriodOption() == Tucuxi::Core::RestPeriodOption::RestPeriodAllowed, true);
    }

    /// \brief This method checks that the steady state target option is within treatment time range
    ///        when there is a standard treatment.
    ///
    /// \param _testName Name of the test
    void steadyStateTargetOptionIsWithinTreatmentTimeRangeWhenStandardTreatment(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>busulfan</queryId>
                                        <clientId>124568</clientId>
                                        <date>2022-06-20T13:45:30</date> <!-- Date the xml has been sent -->
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
                                                    <drugId>busulfan</drugId>
                                                    <activePrinciple>something</activePrinciple>
                                                    <brandName>somebrand</brandName>
                                                    <atc>something</atc>
                                                    <!-- All the information regarding the treatment -->
                                                    <treatment>
                                                        <dosageHistory>
                                                            <dosageTimeRange>
                                                                <start>2022-06-19T08:00:00</start>
                                                                <end>2022-06-21T08:00:00</end>
                                                                <dosage>
                                                                    <dosageLoop>
                                                                        <lastingDosage>
                                                                            <interval>06:00:00</interval>
                                                                            <dose>
                                                                                <value>10</value>
                                                                                <unit>mg</unit>
                                                                                <infusionTimeInMinutes>120</infusionTimeInMinutes>
                                                                            </dose>
                                                                            <formulationAndRoute>
                                                                                <formulation>parenteralSolution</formulation>
                                                                                <administrationName>foo bar</administrationName>
                                                                                <administrationRoute>intravenousDrip</administrationRoute>
                                                                                <absorptionModel>infusion</absorptionModel>
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
                                            <requestXpert>
                                                <drugId>busulfan</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";


        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, busulfanModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getSteadyStateTargetOption() == Tucuxi::Core::SteadyStateTargetOption::WithinTreatmentTimeRange, true);
    }

    /// \brief This method checks that the steady state target option is at steady state
    ///        when there is no standard treatment.
    ///
    /// \param _testName Name of the test
    void steadyStateTargetOptionIsAtSteadyStateWhenNoStandardTreatment(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>populationValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getSteadyStateTargetOption() == Tucuxi::Core::SteadyStateTargetOption::AtSteadyState, true);
    }

    /// \brief This method checks that the target extraction option from request is used when it
    ///        is set. Here, the request sets it to aprioriValues.
    ///
    /// \param _testName Name of the test
    void targetExtractionOptionIsRetrievedFromRequestWhenSet(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>aprioriValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getTargetExtractionOption() == Tucuxi::Core::TargetExtractionOption::AprioriValues, true);
    }

    /// \brief This method checks that the target extraction option is definition if no individual target
    ///        when it is not set in the request.
    ///
    /// \param _testName Name of the test
    void targetExtractionOptionIsDefinitionIfNoIndividualTargetWhenNotSet(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <!-- target extraction option is not set -->
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getTargetExtractionOption() == Tucuxi::Core::TargetExtractionOption::DefinitionIfNoIndividualTarget, true);
    }

    /// \brief This method checks that the formulation and route selection option from request is used when it
    ///        is set. Here, the request sets it to all formulation and routes.
    ///
    /// \param _testName Name of the test
    void formulationAndRouteSelectionOptionIsRetrievedFromRequestWhenSet(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>aprioriValues</targetExtractionOption>
                                                    <formulationAndRouteSelectionOption>allFormulationAndRoutes</formulationAndRouteSelectionOption>
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getFormulationAndRouteSelectionOption() == Tucuxi::Core::FormulationAndRouteSelectionOption::AllFormulationAndRoutes, true);
    }

    /// \brief This method checks that the last formulation and route selection option is
    ///        last formulation and route when it is not set in the request.
    ///
    /// \param _testName Name of the test
    void formulationAndRouteSelectionOptionIsLastFormulationAndRouteWhenNotSet(const std::string& _testName)
    {
        std::string queryString = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                    <query version="1.0"
                                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                        xsi:noNamespaceSchemaLocation="computing_query.xsd">

                                        <queryId>imatinib</queryId>
                                        <clientId>124568</clientId>
                                        <date>2022-07-11T13:45:30</date> <!-- Date the xml has been sent -->
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
                                                                <start>2022-06-19T08:00:00</start>
                                                                <end>2022-06-21T08:00:00</end>
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
                                            <requestXpert>
                                                <drugId>imatinib</drugId>
                                                <output>
                                                    <format>xml</format>
                                                    <language>en</language>
                                                </output>
                                                <options>
                                                    <loadingOption>noLoadingDose</loadingOption>
                                                    <restPeriodOption>noRestPeriod</restPeriodOption>
                                                    <targetExtractionOption>aprioriValues</targetExtractionOption>
                                                    <!-- formulation and route selection option not set -->
                                                </options>
                                            </requestXpert>
                                        </requests>
                                    </query>)";

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::XpertResult::XpertResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        generalFlowStepProvider.getAdjustmentTraitCreator()->perform(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getAdjustmentTrait()->getFormulationAndRouteSelectionOption() == Tucuxi::Core::FormulationAndRouteSelectionOption::LastFormulationAndRoute, true);
    }
};

#endif // TEST_GENERALADJUSTMENTTRAITCREATOR_H
