#ifndef TEST_XPERTUTILS_H
#define TEST_XPERTUTILS_H

#include "tucucore/dosage.h"
#include "tucucore/drugmodelchecker.h"
#include "tucucore/pkmodel.h"
#include "tucucore/drugmodelimport.h"
#include "tucucore/drugmodelrepository.h"
#include "tucucore/computingservice/computingresponse.h"
#include "tucucore/definitions.h"

#include "tuberxpert/query/xpertquerydata.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/result/xpertqueryresult.h"
#include "tuberxpert/language/languageexception.h"
#include "tuberxpert/utils/xpertutils.h"

#include "fructose/fructose.h"

/// \brief Tests for utility methods of TuberXpert.
/// \date 03/06/2022
/// \author Herzig Melvyn
struct TestXpertUtils : public fructose::test_base<TestXpertUtils>
{
    /// \brief Format used to create date and time during tests.
    const std::string date_format = "%Y-%m-%dT%H:%M:%S";

    /// \brief Drug model string of the imatinib used by some tests.
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

    /// \brief Sets up the environment for clean execution of some tests. Loads the query, makes the
    ///        GlobalResult object, loads the model, attributes it to the first XpertRequestResult of the GlobalResult.
    /// \param _queryString Query string to load.
    /// \param _model Model string to put as attribute of the XpertRequestResult of the first request.
    /// \param _globalResult Object that will contain the result of this function execution.
    void setupEnv(const std::string& _queryString,
                  const std::string& _model,
                  std::unique_ptr<Tucuxi::Xpert::XpertQueryResult>& _globalResult) {

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
        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;
        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, _queryString);

        if (importResult != Tucuxi::Xpert::XpertQueryImport::Status::Ok) {
            throw std::runtime_error("Setup failed");
        }

        _globalResult = std::make_unique<Tucuxi::Xpert::XpertQueryResult>(move(query), "");
        Tucuxi::Xpert::XpertRequestResult& xrr =  _globalResult->getXpertRequestResults()[0];
        xrr.setDrugModel(drugModelRepository->getDrugModelsByDrugId(xrr.getXpertRequest().getDrugId())[0]);
    }

    /// \brief Converts dataType to string. Checks that the string contains the correct value.
    /// \param _testName Name of the test.
    void convertDataTypeToString(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::dataTypeToString(Tucuxi::Core::DataType::Bool), "bool");
        fructose_assert_eq(Tucuxi::Xpert::dataTypeToString(Tucuxi::Core::DataType::Int), "int");
        fructose_assert_eq(Tucuxi::Xpert::dataTypeToString(Tucuxi::Core::DataType::Double), "double");
        fructose_assert_eq(Tucuxi::Xpert::dataTypeToString(Tucuxi::Core::DataType::Date), "date");
    }

    /// \brief Converts double to string. Checks that the string contains two decimals.
    /// \param _testName Name of the test.
    void convertDoubleToString(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::doubleToString(5.411111), "5.41");
        fructose_assert_eq(Tucuxi::Xpert::doubleToString(6), "6.00");
    }

    /// \brief Converts covariate type to string.
    /// \param _testName Name of the test.
    void convertCovariateTypeToString(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::covariateTypeToString(Tucuxi::Xpert::CovariateType::Model), "default");
        fructose_assert_eq(Tucuxi::Xpert::covariateTypeToString(Tucuxi::Xpert::CovariateType::Patient), "patient");
        fructose_assert_exception(Tucuxi::Xpert::covariateTypeToString(Tucuxi::Xpert::CovariateType(-1)), std::invalid_argument);
    }

    /// \brief Converts output lang to string. Checks that the requested language get its corresponding
    ///        string or an exception if it is not supported.
    /// \param _testName Name of the test.
    void convertOutputLangToString(const std::string& _testName)
    {

        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::outputLangToString(Tucuxi::Xpert::OutputLang::ENGLISH), "en");
        fructose_assert_eq(Tucuxi::Xpert::outputLangToString(Tucuxi::Xpert::OutputLang::FRENCH), "fr");
        fructose_assert_exception(Tucuxi::Xpert::outputLangToString(Tucuxi::Xpert::OutputLang(-1)), Tucuxi::Xpert::LanguageException);
    }

    /// \brief Check that the string returned by dateTimeToXmlString is the
    ///        expected value.
    /// \param _testName Name of the test.
    void convertWarningLevelToString(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::warningLevelToString(Tucuxi::Xpert::WarningLevel::CRITICAL), "critical");
        fructose_assert_eq(Tucuxi::Xpert::warningLevelToString(Tucuxi::Xpert::WarningLevel::NORMAL), "normal");
        fructose_assert_exception(Tucuxi::Xpert::warningLevelToString(Tucuxi::Xpert::WarningLevel(-1)), std::invalid_argument);
    }

    /// \brief Converts covariate value string with beautifyString.
    ///        Check that the result are expected depending of the data type and
    ///        definition id.
    /// \param _testName Name of the test.
    void stringBeautification(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Dictionary to get resulting values
        const std::string dictionary = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                          <translations
                                              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                              xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                                <translation key="yes">Yes</translation>
                                                <translation key="no">No</translation>
                                                <translation key="undefined">Undefined</translation>
                                                <translation key="man">Man</translation>
                                                <translation key="woman">Woman</translation>

                                           </translations>)";

        Tucuxi::Xpert::LanguageManager& lm = Tucuxi::Xpert::LanguageManager::getInstance();
        lm.loadTranslations(dictionary);

        fructose_assert_eq(Tucuxi::Xpert::beautifyString("1.0", Tucuxi::Core::DataType::Bool, ""), "Yes");
        fructose_assert_eq(Tucuxi::Xpert::beautifyString("0.0", Tucuxi::Core::DataType::Bool, ""), "No");

        fructose_assert_eq(Tucuxi::Xpert::beautifyString("1.0", Tucuxi::Core::DataType::Double, "sex"), "Man");
        fructose_assert_eq(Tucuxi::Xpert::beautifyString("0.0", Tucuxi::Core::DataType::Double, "sex"), "Woman");
        fructose_assert_eq(Tucuxi::Xpert::beautifyString("0.5", Tucuxi::Core::DataType::Double, "sex"), "Undefined");

        fructose_assert_eq(Tucuxi::Xpert::beautifyString("42", Tucuxi::Core::DataType::Int, "age"), "42");

        fructose_assert_eq(Tucuxi::Xpert::beautifyString("72.652222", Tucuxi::Core::DataType::Double, "bodyweight"), "72.65");
    }

    /// \brief Converts a date time to a formatted date time string.
    /// \param _testName Name of the test.
    void dateTimeToXmlStringReturnCorrectValue(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        std::string formatedDateTimeString = "2022-01-01T10:00:00";

        Tucuxi::Common::DateTime formatedDateTime{formatedDateTimeString, date_format};

        fructose_assert_eq(Tucuxi::Xpert::dateTimeToXmlString(formatedDateTime), formatedDateTimeString);
    }

    /// \brief Check that the string returned by timeToString is the
    ///        expected value.
    /// \param _testName Name of the test.
    void timeToStringReturnCorrectValue(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Dictionary to get hours acronym
        const std::string dictionary = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                          <translations
                                              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                              xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                                <translation key="hour_acronym">h</translation>

                                           </translations>)";

        Tucuxi::Xpert::LanguageManager& lm = Tucuxi::Xpert::LanguageManager::getInstance();
        lm.loadTranslations(dictionary);


        Tucuxi::Common::TimeOfDay timeOfDayWithoutMinute{Tucuxi::Common::Duration(
                        std::chrono::hours(8),
                        std::chrono::minutes(0),
                        std::chrono::seconds(1))};

        Tucuxi::Common::TimeOfDay timeOfDayWithMinute{Tucuxi::Common::Duration(
                        std::chrono::hours(8),
                        std::chrono::minutes(30),
                        std::chrono::seconds(1))};

        fructose_assert_eq(Tucuxi::Xpert::TimeOfDayToString(timeOfDayWithoutMinute), "8h");
        fructose_assert_eq(Tucuxi::Xpert::TimeOfDayToString(timeOfDayWithMinute), "8h30");
    }

    /// \brief Check that the string returned by dateTimeToString is the
    ///        expected value.
    /// \param _testName Name of the test.
    void dateTimeToStringReturnCorrectValue(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Dictionary to get hours acronym
        const std::string dictionary = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                                          <translation
                                              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                                              xsi:noNamespaceSchemaLocation="translations_file.xsd">

                                                <translation key="hour_acronym">h</translation>

                                           </translations>)";

        Tucuxi::Xpert::LanguageManager& lm = Tucuxi::Xpert::LanguageManager::getInstance();
        lm.loadTranslations(dictionary);

        std::string formatedDateTimeString = "2022-01-01T10:00:00";

        Tucuxi::Common::DateTime formatedDateTime{formatedDateTimeString, date_format};

        fructose_assert_eq(Tucuxi::Xpert::dateTimeToString(formatedDateTime), "1.1.2022 10h");
        fructose_assert_eq(Tucuxi::Xpert::dateTimeToString(formatedDateTime, false), "1.1.2022");
    }

    /// \brief Tests that the getStringWithEnglishFallback returns the targeted language or
    ///        the default language (en) or empty string.
    /// \param _testName Name of the test.
    void getStringFromTranslatableWithFallback(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        Tucuxi::Common::TranslatableString ts1("translatable string in en");
        ts1.setString("chaine de caractere traductible en fr", "fr");

        Tucuxi::Common::TranslatableString ts2("translatable string in en");


        fructose_assert_eq(Tucuxi::Xpert::getStringWithEnglishFallback(ts1, Tucuxi::Xpert::OutputLang::FRENCH), "chaine de caractere traductible en fr");
        fructose_assert_eq(Tucuxi::Xpert::getStringWithEnglishFallback(ts2, Tucuxi::Xpert::OutputLang::FRENCH), "translatable string in en");
    }

    /// \brief Tests that the getOldestDosageTimeRangeStart works as expected.
    ///        This test forms a dosage history with two time ranges:
    ///         1) 2022-01-01 10h00 - 2022-01-02 13h00
    ///         2) 2022-01-03 14h00 - 2022-01-04 16h00
    ///
    ///        Once it requieres the oldest start time with a reference time of 2022-01-01 14h00
    ///        (just after the first time range) which should return 2022-01-01 10h00
    ///
    ///        Once it requieres the oldest start time with a reference time of 2022-01-01 09h00
    ///        (before both time ranges) which should return the reference time itself.
    ///
    /// \param _testName Name of the test.
    void getOldestDosageTimeRangeStartReturnsCorrectValues(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Common elements
        Tucuxi::Core::Unit unit{"mg"};
        Tucuxi::Core::FormulationAndRoute formulationAndRoute{Tucuxi::Core::AbsorptionModel::Extravascular};
        Tucuxi::Common::Duration duration, interval{std::chrono::hours(1)};

        // Making first time range "2022-01-01 10h00 - 2022-01-02 13h00"
        Tucuxi::Core::LastingDose lastingDose1{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange1{
            Tucuxi::Common::DateTime("2022-01-01T10:00:00", date_format),
            Tucuxi::Common::DateTime("2022-01-02T13:00:00", date_format),
            lastingDose1
        };

        // Making second time range "2022-01-03 14h00 - 2022-01-04 16h00"
        Tucuxi::Core::LastingDose lastingDose2{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange2{
            Tucuxi::Common::DateTime("2022-01-03T14:00:00", date_format),
            Tucuxi::Common::DateTime("2022-01-04T16:00:00", date_format),
            lastingDose1
        };

        // Making dsage history
        Tucuxi::Core::DosageHistory dosageHistory;
        dosageHistory.addTimeRange(timeRange1);
        dosageHistory.addTimeRange(timeRange2);

        fructose_assert_eq(Tucuxi::Xpert::getOldestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-01T14:00:00", date_format)),
                           Tucuxi::Common::DateTime("2022-01-01T10:00:00", date_format));

        fructose_assert_eq(Tucuxi::Xpert::getOldestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-01T09:00:00", date_format)),
                           Tucuxi::Common::DateTime("2022-01-01T09:00:00", date_format));
    }

    /// \brief Tests that the getLatestDosageTimeRangeStart works as expected.
    ///        This test forms a dosage history with two time ranges:
    ///         1) 2022-01-01 10h00 - 2022-01-02 13h00
    ///         2) 2022-01-03 14h00 - 2022-01-04 16h00
    ///
    ///        Once it requieres the latest start time with a reference time of 2022-01-07 15h00
    ///        (before both time ranges) which should return 2022-01-03 14h00
    ///
    ///        Once it requieres the oldest start time with a reference time of 2022-01-01 09h00
    ///        (before both time ranges) which should return an undefined date time.
    ///
    /// \param _testName Name of the test.
    void getLatestDosageTimeRangeStartReturnsCorrectValues(const std::string& _testName)
    {
        std::cout << _testName << std::endl;

        // Common elements
        Tucuxi::Core::Unit unit{"mg"};
        Tucuxi::Core::FormulationAndRoute formulationAndRoute{Tucuxi::Core::AbsorptionModel::Extravascular};
        Tucuxi::Common::Duration duration, interval{std::chrono::hours(1)};

        // Making first time range "2022-01-01 10h00 - 2022-01-02 13h00"
        Tucuxi::Core::LastingDose lastingDose1{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange1{
            Tucuxi::Common::DateTime("2022-01-01T10:00:00", date_format),
            Tucuxi::Common::DateTime("2022-01-02T13:00:00", date_format),
            lastingDose1
        };

        // Making second time range "2022-01-03 14h00 - 2022-01-04 16h00"
        Tucuxi::Core::LastingDose lastingDose2{1, unit, formulationAndRoute, duration, interval};
        Tucuxi::Core::DosageTimeRange timeRange2{
            Tucuxi::Common::DateTime("2022-01-03T14:00:00", date_format),
            Tucuxi::Common::DateTime("2022-01-04T16:00:00", date_format),
            lastingDose1
        };

        // Making dsage history
        Tucuxi::Core::DosageHistory dosageHistory;
        dosageHistory.addTimeRange(timeRange1);
        dosageHistory.addTimeRange(timeRange2);

        fructose_assert_eq(Tucuxi::Xpert::getLatestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-07T15:00:00", date_format)),
                           Tucuxi::Common::DateTime("2022-01-03T14:00:00", date_format));

        fructose_assert_eq(Tucuxi::Xpert::getLatestDosageTimeRangeStart(dosageHistory, Tucuxi::Common::DateTime("2022-01-01T09:00:00", date_format)).isUndefined(),
                           true);
    }

    /// \brief This method tests the computeFileName method.
    ///        It computes a XpertRequestResult and check that the file name recieved is expected.
    /// \param _testName Name of the test
    void computeFileNameReturnsCorrectValues(const std::string& _testName)
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
                                                            <sampleDate>2018-07-07T07:00:00</sampleDate>
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
                                                            <sampleDate>2018-07-07T13:00:00</sampleDate>
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

        std::cout << _testName << std::endl;

        // Query import
        std::unique_ptr<Tucuxi::Xpert::XpertQueryData> query = nullptr;
        Tucuxi::Xpert::XpertQueryImport importer;
        Tucuxi::Xpert::XpertQueryImport::Status importResult = importer.importFromString(query, queryString);

        if (importResult != Tucuxi::Xpert::XpertQueryImport::Status::Ok) {
            throw std::runtime_error("Setup failed");
        }

        Tucuxi::Xpert::XpertQueryResult globalResult{move(query), "random/path"};
        globalResult.incrementRequestIndexBeingProcessed();
        Tucuxi::Xpert::XpertRequestResult& xrr =  globalResult.getXpertRequestResults()[0];


        fructose_assert_eq(Tucuxi::Xpert::computeFileName(xrr), "random/path/imatinib_1_11-7-2018_13h45m30s.xml");
        fructose_assert_eq(Tucuxi::Xpert::computeFileName(xrr, false), "imatinib_1_11-7-2018_13h45m30s.xml");
        fructose_assert_eq(Tucuxi::Xpert::computeFileName(xrr, false, false), "imatinib_1_11-7-2018_13h45m30s");
    }

    /// \brief Check that the method executeRequestAndGetResult set the result to nullptr when
    ///        the execution fails or that the resulting pointer is correctly retrieved. The test
    ///        prepares two percentile traits one that must fail and one that must succeed.
    ///        One trait is prepared with a too large period which will fail.
    ///        The other just set a valid trait like the one used in samplevalidator.cpp.
    /// \param _testName Name of the test
    void executeRequestAndGetResultReturnsCorrectValues(const std::string& _testName)
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
                                                            <sampleDate>2018-07-07T13:00:00</sampleDate>
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

        std::cout << _testName << std::endl;

        std::unique_ptr<Tucuxi::Xpert::XpertQueryResult> result = nullptr;

        setupEnv(queryString, imatinibModelString, result);

        Tucuxi::Xpert::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        // Preparing common values for the traits
        std::string responseId = "";
        Tucuxi::Core::PercentileRanks ranks = {1.0, 2.0, 3.0};
        double nbPointsPerHour = 1;
        Tucuxi::Core::ComputingOption computingOption{Tucuxi::Core::PredictionParameterType::Apriori, Tucuxi::Core::CompartmentsOption::AllActiveMoieties};
        Tucuxi::Common::DateTime start = Tucuxi::Common::DateTime("2018-07-07T13:00:00", date_format);

        // Normal end
        Tucuxi::Common::DateTime normalEnd = start + std::chrono::hours(1);

        // Too long start/end (+ 1'000'000 hours)
        Tucuxi::Common::DateTime tooFarEnd = start + std::chrono::hours(1000000);

        // Making traits and response pointers
        std::unique_ptr<Tucuxi::Core::ComputingTraitPercentiles> goodTrait =
                std::make_unique<Tucuxi::Core::ComputingTraitPercentiles>(responseId, start, normalEnd, ranks, nbPointsPerHour, computingOption);

        std::unique_ptr<Tucuxi::Core::PercentilesData> goodResult = nullptr;

        std::unique_ptr<Tucuxi::Core::ComputingTraitPercentiles> failTrait =
                std::make_unique<Tucuxi::Core::ComputingTraitPercentiles>(responseId, start, tooFarEnd, ranks, nbPointsPerHour, computingOption);

        std::unique_ptr<Tucuxi::Core::PercentilesData> failResult = nullptr;

        // Execution
        Tucuxi::Xpert::executeRequestAndGetResult(std::move(goodTrait), xrr, goodResult);
        Tucuxi::Xpert::executeRequestAndGetResult(std::move(failTrait), xrr, failResult);

        fructose_assert_eq(failResult.get(), nullptr);

        fructose_assert_ne(goodResult.get(), nullptr);
        fructose_assert_eq(goodResult->getNbRanks(), 3);
    }

    /// \brief Test the camel case key to phrase method.
    /// \param _testName Name of the test
    void convertKeyToPhrase(const std::string& _testName)
    {

        std::cout << _testName << std::endl;

        fructose_assert_eq(Tucuxi::Xpert::keyToPhrase(""), "");
        fructose_assert_eq(Tucuxi::Xpert::keyToPhrase("abc"), "Abc");
        fructose_assert_eq(Tucuxi::Xpert::keyToPhrase("camelCase"), "Camel case");
    }

    /// \brief Test the age calculator getAgeIn method.
    /// \param _testName Name of the test
    void computeAge(const std::string& _testName)
    {

        std::cout << _testName << std::endl;

        Tucuxi::Common::DateTime birthDate = Tucuxi::Common::DateTime("2000-01-01T12:00:00", date_format);
        Tucuxi::Common::DateTime computationTime = Tucuxi::Common::DateTime("2010-01-01T12:00:00", date_format);

        fructose_assert_eq(int(Tucuxi::Xpert::getAgeIn(Tucuxi::Core::CovariateType::AgeInDays, birthDate, computationTime)), 3653);
        fructose_assert_eq(int(Tucuxi::Xpert::getAgeIn(Tucuxi::Core::CovariateType::AgeInWeeks, birthDate, computationTime)), 521);
        fructose_assert_eq(int(Tucuxi::Xpert::getAgeIn(Tucuxi::Core::CovariateType::AgeInMonths, birthDate, computationTime)), 119);
        fructose_assert_eq(int(Tucuxi::Xpert::getAgeIn(Tucuxi::Core::CovariateType::AgeInYears, birthDate, computationTime)), 10);
        fructose_assert_exception(Tucuxi::Xpert::getAgeIn(Tucuxi::Core::CovariateType(-1), birthDate, computationTime), std::invalid_argument);
    }
};

#endif // TEST_XPERTUTILS_H
