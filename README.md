<h1> TuberXpert </h1>

<h3> <b>Author</b> </h3> 

Herzig Melvyn

<h3> <b>Last update</b> </h3> 
11.07.2022

<hr>

<h2> What is this readme made for ? </h2>
<hr>

This readme is a brief introduction to the TuberXpert project.

What it does not do:
- Explain the concepts 
- Explain the thinking behind the project

What it does do:
- Explain the organization of the files
- Explain how to set up the project
- Explain how to launch the project

<h2> What is TuberXpert ? </h2>
<hr>

TuberXpert is a [Tucuxi](http://www.tucuxi.ch/) project during a Bachelor's thesis. The objective is to automate the decision making process in the context of terapeuthic drug monitoring. In its current state, TuberXpert is more a proof of concept than a usable medical software. This software takes patient information in an xml file, validates it, prepares the adjustments and prints the final result in a report.

<h2> What can I find in this repository ? </h2>
<hr>

This repository is diveded into three directories:

- dev: This is where you can find the source code, the dependencies, the tests and some example entries.

- publi: It contains all the documentation made for this project.

- tiersdoc: In this directory there is additional documentation that was not made by the author but was used to develop this project.

<h2> Documentation </h2>
<hr>

If this readme file does not answer all your questions or if you are interested in the overall project documentation: <br>
* You can check "_/publi/tb_Herzig_Melvyn_2022_intermediate.pdf_"

For the automatically generated documentation of the code: <br>
* COMING SOON

<h2> Installation </h2>
<hr>

<h3> System requirements </h3>

<b> Operating system: </b> Windows 10 64 bit, Windows 11 64 bit or Ubuntu 22.04 AMD 64. \
<b> RAM: </b> 4 GB

<h3> Software requirements </h3>

<b> IDE: </b> QT creator 6.0.2 \
<b> Runtime environment:</b> MinGW-w64 6.3.0 (Windows) or GCC 11.2.0 (Ubuntu) \
<b> Version control system: </b> Git 2.37.0 \
<b> (ubuntu specific) Python: </b> Python 3.10.5

<h3> Repository access </h3>

* reds-gitlab.heig-vd.ch / REDS-Tucuxi / Tucuxi-core 
* reds-gitlab.heig-vd.ch / REDS-Tucuxi / Tucuxi-drugs

<h3> Windows installation process </h3>

* Clone the TuberXpert repository

* Init and update the git submodules ( "_/dev/tucuxi-core_" & "_/dev/tucuxi-drugs_")

* Open the file "_/dev/tucuxi-tuberxpert/tucuxi-tuberxpert.pro_" with QT creator and you are good to go.

<h3> Ubuntu installation process </h3>

* Clone the TuberXpert repository

* Init and update the git submodules ( "_/dev/tucuxi-core_" & "_/dev/tucuxi-drugs_")

* Run the script "_/dev/tucuxi-core/make/build.sh_" 

```
$ ./build.sh lib
```

> Troubleshoot 1: When building botan, the file  "_/dev/tucuxi-core/make/linuxrules.mak_" tries to use "python" command. Maybe only python3 is available. Try to replace (line 14) "PYTHON := python" by "PYTHON := python3".

> Troubleshoot 2: When building botan, if the error "invalid_argument is not a member of std" happens, get rid of botan. To do this, go to the file "_/dev/tucuxi-core/make/qtcreator/general.pri_" and remove "$$PWD/../../bin/botan.a" from QT LIBS. Actually, botan is not used by this project so this not mandatory to have it.

* Open the file "_/dev/tucuxi-tuberxpert/tucuxi-tuberxpert.pro_" with QT creator and you are good to go.

<h2> Usage </h2>
<hr>

<h3> Input/Query file </h3>

TuberXpert receives the runtime data trough an XML input file. The file follows the same specification than the one used by tucuxi-core. The base specification is available in the file "_/tiersdoc/Tucuxi_CLI_Usability_Specification.pdf_". It tells how to provides the patient covariates, his treatments, his samples and his targets.

> Unlike tucuxi-core in which the date element is not really used. TuberXpert uses it to set the computation time. The computation time allows the user to define the "present" time to use when launching the computation. For example, to calculate an age from a date of birth, the difference is made between the computation time and the date of birth. This allows the user to simulate the execution not only in the present, but also in the past and the future.

Only two elements differ from the base specification:

<h4> <u>Administrative data:</u> </h4>

This element provides information about the adjustment mandator and the patient involved.

For a complete specification of this element, check the overall project documentation:
"_/publi/tb_Herzig_Melvyn_2022_intermediate.pdf_" in the implementation section.

Example of administrative data:

```
<query>
    [...]
    <requests>
    <admin>
		<mandator>
			<person>
			    <id>asdf</id>
				<title>Dr.</title>
				<firstName>John</firstName>
				<lastName>Doe</lastName>
				<address>
					<street>Av. de l'Ours 2</street>
					<postCode>1010</postCode>
					<city>Lausanne</city>
					<state>Vaud</state>
					<country>Suisse</country>
				</address>
				<phone>
					<number>0213140002</number>
					<type>professional</type>
				</phone>
				<email>
					<address>john.doe@chuv.com</address>
					<type>professional</type>
				</email>
			</person>
			<institute>
				<id>456789</id>
				<name>CHUV</name>
				<address>
					<street>Av. de l'Ours 1</street>
					<postCode>1010</postCode>
					<city>Lausanne</city>
					<state>Vaud</state>
					<country>Suisse</country>
				</address>
				<phone>
					<number>0213140001</number>
					<type>private</type>
				</phone>
				<email>
					<address>info@chuv.com</address>
					<type>professional</type>
				</email>
			</institute>
		</mandator>
		<patient>
			<person>
				<id>123456</id>
				<firstName>Alice</firstName>
				<lastName>Aupaysdesmerveilles</lastName>
				<address>
					<street>Av. d'Ouchy 27</street>
					<postCode>1006</postCode>
					<city>Lausanne</city>
					<state>Vaud</state>
					<country>Suisse</country>
				</address>
				<phone>
					<number>0216170002</number>
					<type>professional</type>
				</phone>
				<email>
					<address>alice.apdm@gmail.com</address>
					<type>private</type>
				</email>
			</person>
		</patient>
		<clinicalData>
			<clinicalDataEntry key="note">random note</clinicalDataEntry>
		</clinicalData>
	</admin>
    [...]
</query>    
```

<h4> <u>TuberXpert request:</u> </h4>
This element tells TuberXpert which drug it should adjust and how the final report should be (format and language).

For a complete specification of this element, check the overall project documentation:
"_/publi/tb_Herzig_Melvyn_2022_intermediate.pdf_" in the implementation section.

Example of TuberXpert request:

```
<query>
    [...]
    <requests>
		<requestXpert>
            <drugId>imatinib</drugId>
            <output>
                <format>pdf</format>
                <language>fr</language>
            </output>
            <adjustmentDate>2022-07-06T08:00:00</adjustmentDate>
            <options>
                <loadingOption>noLoadingDose</loadingOption>
                <restPeriodOption>noRestPeriod</restPeriodOption>
                <targetExtractionOption>definitionIfNoIndividualTarget</targetExtractionOption>
                <formulationAndRouteSelectionOption>lastFormulationAndRoute</formulationAndRouteSelectionOption>
            </options>
        </requestXpert>
        [...]
    </requests>
</query>    
```

<br>
<h3> Program arguments </h3>

The program requires four arguments to run:

* -i </path/to/xml/query> to indicate the location of the input/query xml file. There is a few examples in "_/dev/tucuxi-tuberxpert/xml/query_".
* -o </path/to/directory/> to indicate where the result report should be printed.
* -l </path/to/directory/with/translations> to indicate where are the translation files. Actually, TuberXpert supports English and French in "_/dev/tucuxi-tuberxpert/language_".
* -d </path/to/drugfiles> to indicate where are the drug files (drug behavior information). There is a basic collection in "_/dev/tucuxi-drugs/drugfiles_".

<br>
<h3> Report </h3>

If the execution succeed, TuberXpert prints a final report in xml, html or pdf in the required language. The content of the report is influenced by the information contained in the input file. For example:
- The administrative data
- The patient covariates
- The treatments
- The samples
- The targets
- The requestXpert (selected drug, adjustment date, options...)

A report is printed by requestXpert found in the query file. The report name is formed as follows: \
\<drug id\>_\<request xpert position in query\>_\<date in the query\>.<file format>

If a requestXpert fails, the program skips and tries to process the next one. If you don't know what might be wrong with your query, TuberXpert logs everything in a file called "_tuberxpert.log_" located in the build directory created by QT.







