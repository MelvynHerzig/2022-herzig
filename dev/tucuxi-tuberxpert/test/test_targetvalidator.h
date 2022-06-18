#ifndef TEST_TARGETVALIDATOR_H
#define TEST_TARGETVALIDATOR_H

#include <memory>

#include "tucucore/drugmodelrepository.h"
#include "tucucore/drugmodelimport.h"
#include "tucucore/pkmodel.h"
#include "tucucore/drugmodelchecker.h"

#include "tuberxpert/result/xpertresult.h"
#include "tuberxpert/query/xpertqueryimport.h"
#include "tuberxpert/result/validation/targetvalidator.h"

#include "fructose/fructose.h"

/// \brief Tests for TargetValidator.
/// \date 09/06/2022
/// \author Herzig Melvyn
struct TestTargetValidator : public fructose::test_base<TestTargetValidator>
{

    /// \brief Drug model string of the imatinib used by the tests.
    std::string originalImatinibModelString = R"(<?xml version="1.0" encoding="UTF-8"?>
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
                                                        <drugModelId>ch.tucuxi.imatinib.gotta2012_original</drugModelId>
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
                                                                        <text lang='fr'>Le poids doit etre dans compris entre [44,100].</text>
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

    /// \brief Sets up environment for clean execution of the target validator. Loads the query, makes the
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

        Tucuxi::XpertResult::TargetValidator tv;
        tv.getTargetValidations(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), false);
        fructose_assert_eq(xrr.getErrorMessage(), "No treatment set.");
    }

    /// \brief Checks that there is an error if the drug model of XpertRequestResult is nullptr.
    ///        The test uses the original imatinib drug model.
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

        Tucuxi::XpertResult::TargetValidator tv;
        tv.getTargetValidations(xr.getXpertRequestResults()[0]);

        fructose_assert_eq(xr.getXpertRequestResults()[0].shouldBeHandled(), false);
        fructose_assert_eq(xr.getXpertRequestResults()[0].getErrorMessage(), "No drug model set.");
    }

    /// \brief This method performs a target validation with a treatment that does not specify custom targets.
    ///        The XpertRequestResult must still be handlable (not having a warning set).
    ///        The test uses the original imatinib drug model.
    /// \param _testName Name of the test
    void noErrorWhenNoTarget(const std::string& _testName)
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

        setupEnv(queryString, originalImatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        Tucuxi::XpertResult::TargetValidator tv;
        tv.getTargetValidations(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getErrorMessage(), "");
    }

    /// \brief This method performs a target validation with a treatment that specifes custom targets
    ///        that have the same active moiety and target type. The XpertRequestResult must have an error.
    ///        The test uses the original imatinib drug model.
    /// \param _testName Name of the test
    void errorWhenTargetsWithSameActiveMoietyAndType(const std::string& _testName)
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
                                                        <target>
                                                            <activeMoietyId>imatinib</activeMoietyId>
                                                            <targetType>residual</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>20</min>
                                                            <best>25</best>
                                                            <max>30</max>
                                                            <inefficacyAlarm>15</inefficacyAlarm>
                                                            <toxicityAlarm>50</toxicityAlarm>
                                                        </target>
                                                        <target>
                                                            <activeMoietyId>imatinib</activeMoietyId>
                                                            <targetType>residual</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>30</min>
                                                            <best>35</best>
                                                            <max>40</max>
                                                            <inefficacyAlarm>25</inefficacyAlarm>
                                                            <toxicityAlarm>60</toxicityAlarm>
                                                        </target>
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

        setupEnv(queryString, originalImatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        Tucuxi::XpertResult::TargetValidator tv;
        tv.getTargetValidations(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), false);
        fructose_assert_eq(xrr.getErrorMessage(), "Two patient's targets with the same active moiety and the same target type detected.");
    }

    /// \brief This method performs a target validation with a treatment that specifes custom targets
    ///        that have the same active moiety but different target type. The XpertRequestResult must not have an error.
    ///        The test uses the original imatinib drug model.
    /// \param _testName Name of the test
    void noErrorWhenTargetsWithSameActiveMoietyButDifferentType(const std::string& _testName)
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
                                                        <target>
                                                            <activeMoietyId>imatinib</activeMoietyId>
                                                            <targetType>mean</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>20</min>
                                                            <best>25</best>
                                                            <max>30</max>
                                                            <inefficacyAlarm>15</inefficacyAlarm>
                                                            <toxicityAlarm>50</toxicityAlarm>
                                                        </target>
                                                        <target>
                                                            <activeMoietyId>imatinib</activeMoietyId>
                                                            <targetType>residual</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>30</min>
                                                            <best>35</best>
                                                            <max>40</max>
                                                            <inefficacyAlarm>25</inefficacyAlarm>
                                                            <toxicityAlarm>60</toxicityAlarm>
                                                        </target>
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

        setupEnv(queryString, originalImatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        Tucuxi::XpertResult::TargetValidator tv;
        tv.getTargetValidations(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), true);
        fructose_assert_eq(xrr.getErrorMessage(), "");
    }

    /// \brief This method performs a target validation with a treatment that specifes a custom target
    ///        that have a different active moiety that the ones from the drug model.
    ///        The XpertRequestResult must have an error.
    ///        The test uses the original imatinib drug model.
    /// \param _testName Name of the test
    void errorWhenTargetsWithActiveMoietyNotInDrugModel(const std::string& _testName)
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
                                                        <target>
                                                            <activeMoietyId>randomactivemoiety</activeMoietyId>
                                                            <targetType>mean</targetType>
                                                            <unit>mg/l</unit>
                                                            <min>20</min>
                                                            <best>25</best>
                                                            <max>30</max>
                                                            <inefficacyAlarm>15</inefficacyAlarm>
                                                            <toxicityAlarm>50</toxicityAlarm>
                                                        </target>
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

        setupEnv(queryString, originalImatinibModelString, result);

        Tucuxi::XpertResult::XpertRequestResult& xrr = result->getXpertRequestResults()[0];

        Tucuxi::XpertResult::TargetValidator tv;
        tv.getTargetValidations(xrr);

        fructose_assert_eq(xrr.shouldBeHandled(), false);
        fructose_assert_eq(xrr.getErrorMessage(), "A target is related to an active moiety that does not belong to the drug model: ch.tucuxi.imatinib.gotta2012_original");
    }
};

#endif // TEST_TARGETVALIDATOR_H
