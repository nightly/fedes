#include "models.hpp"

#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>

#include "fedes/model/parsers.hpp"
#include "fedes/model/writers.hpp"
#include "fedes/instrumentation/timer.hpp"

/*
 * @brief Sets source and target models based on a pre-defined set of examples
 * @param source: starting model
 * @param target: model where data will/has been mapped to
 * @param id: dictates which example model to use. @default = 1.
 */
void SetModels(fedes::Model& source, fedes::Model& target, unsigned int id) {
	fedes::internal::Timer timer("Mesh Parsing");
	switch (id) {
	case 1:
		fedes::AnsysInputReadLis("../../models/Example1-Vane-big/Model1_Input-Ansys.txt", source);
		fedes::AnsysOutputRead("../../models/Example1-Vane-big/Model1_output-Ansys.txt", source);
		fedes::AbaqusInputRead("../../models/Example1-Vane-big/Model2-Input-Abaqus.inp", target);
		break;
	case 2:
		fedes::AbaqusInputRead("../../models/Example-3D-medium/model1-input-Abaqus.inp", source);
		fedes::AbaqusOutputRead("../../models/Example-3D-medium/model1-output-Abaqus.dat", source);
		fedes::AbaqusInputRead("../../models/Example-3D-medium/Model-input-Abaqus.inp", target);
		break;
	case 3:
		fedes::MorpheoInputOutputRead("../../models/Example1-ManufacturingProcessChain-2ndLoop/Process1-HeatTreatment-XML-format.vtu", source);
		fedes::AbaqusInputRead("../../models/Example1-ManufacturingProcessChain-2ndLoop/Process2-ShotPeening-Abaqus.inp", target);
		break;
	case 4:
		fedes::AbaqusInputRead("../../models/Example1-ManufacturingProcessChain-2ndLoop/Process-1-Abaqus-Input.inp", source);
		fedes::AbaqusOutputRead("../../models/Example1-ManufacturingProcessChain-2ndLoop/Process-1-Abaqus-Output.dat", source);
		fedes::MorpheoInputRead("../../models/Example1-ManufacturingProcessChain-2ndLoop/Process2-Machining-XML-input.vtu", target);
		break;
	}		
	target.SetIndexes(source);
}

/*
 * @brief Will export the source and target models in .xml/.vtu format (visualisable with ParaView). Overwrites existing files.
 * @param source: starting model
 * @param target: model where data will/has been mapped to
 * @param file_suffix: filename suffixes before file extension to "source" + "target". E.g. "oct-npm" = "source-oct-npm.xml"
 * @exception Propagtes std::ofstream::failure
 */
void ExportModels(fedes::Model& source, fedes::Model& target, const std::string& file_suffix) {
	try {
		source.Export("source-" + file_suffix, false, "../../exports");
		target.Export("target-" + file_suffix, true, "../../exports");
	} catch (const std::ofstream::failure& e) {
		throw;
	}
}