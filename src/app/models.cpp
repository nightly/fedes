#include "models.hpp"

#include <filesystem>
#include <string>

#include "fedes/model/parsers.hpp"
#include "fedes/model/writers.hpp"

/*
 * @brief Sets source and target models based on a pre-defined set of examples
 * 
 * @param source: the starting model
 * @param target: the model where data will/has been mapped to
 * @param id: default parameter = 1, dictates which model to use
 */
void SetModels(fedes::Model& source, fedes::Model& target, unsigned int id) {
	switch (id) {
	case 1:
		fedes::AnsysInputReadLis("../../../models/Example1-Vane-big/Model1_Input-Ansys.txt", source);
		fedes::AnsysOutputRead("../../../models/Example1-Vane-big/Model1_source-Ansys.txt", source);
		fedes::AbaqusInputRead("../../../models/Example1-Vane-big/Model2-Input-Abaqus.inp", target);
		break;
	case 2:
		fedes::AbaqusInputRead("../../../models/Example-3D-medium/model1-input-Abaqus.inp", source);
		fedes::AbaqusOutputRead("../../../models/Example-3D-medium/model1-output-Abaqus.dat", source);
		fedes::AbaqusInputRead("../../../models/Example-3D-medium/Model-input-Abaqus.inp", target);
		break;
	case 3:
		fedes::MorpheoInputOutputRead("../../../models/Example1-ManufacturingProcessChain-2ndLoop/Process1-HeatTreatment-XML-format.vtu", source);
		fedes::AbaqusInputRead("../../../models/Example1-ManufacturingProcessChain-2ndLoop/Process2-ShotPeening-Abaqus.inp", target);
		break;
	case 4:
		fedes::AbaqusInputRead("../../../models/Example1-ManufacturingProcessChain-2ndLoop/Process-1-Abaqus-Input.inp", source);
		fedes::AbaqusOutputRead("../../../models/Example1-ManufacturingProcessChain-2ndLoop/Process-1-Abaqus-Output.dat", source);
		fedes::MorpheoInputRead("../../../models/Example1-ManufacturingProcessChain-2ndLoop/Process2-Machining-XML-input.vtu", target);
		break;
	}
}

/*
 * @brief Will export the source and target models in .xml/.vtu format (visualisable with ParaView). Overwrites existing files.
 * 
 * @param source: the starting model
 * @param target: the model where data will/has been mapped to
 * @param file_suffix: string appended to end of filename, e.g. "1" -> source-1.xml, target-1.xml
 * @param path: the directory to which to place the models, @default parameter: "../../../exports"
 */
void ExportModels(fedes::Model& source, fedes::Model& target, const std::string& file_suffix, const std::filesystem::path& path) {
	std::string source_filename = "source" + file_suffix + ".xml";
	std::string target_filename = "target" + file_suffix + ".xml";
	fedes::CreateXML(path / source_filename, source);
	fedes::CreateXML(path / target_filename, target);
}