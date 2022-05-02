#include "fedes/model/examples.h"

#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>

#include "fedes/model/parsers.h"
#include "fedes/model/writers.h"
#include "fedes/instrumentation/timer.h"
#include "fedes/common/log.h"

namespace fedes {

	/*
	 * @brief Sets source and target models based on a pre-defined set of examples (located in `models/`)
	 * @param source: starting model
	 * @param target: model where data will/has been mapped to
	 * @param id: dictates which example set to use (1, 2, 3, or 4). 
	 */
	void SetExampleModels(fedes::Model& source, fedes::Model& target, size_t id) {
#if (defined FEDES_VERBOSE == 1)
		fedes::internal::Timer timer("Mesh Parsing");
#endif
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
		target.SetTargetIndexes(source);
		FEDES_INFO("[Source mesh] Number of nodes: {}, Number of elements: {}", source.nodes.size(), source.elements.size());
		FEDES_INFO("[Target mesh] Number of nodes: {}, Number of elements: {}, Number of integration points: {}", 
			target.nodes.size(), target.elements.size(), target.integration.size());
	}
}

/*

   @Model 1:
	source — Number of nodes: 54128, Number of elements: 171188. 4 nodes per element.
	target — Number of nodes: 35617, Number of elements: 125361. 4 nodes per element.
   @Model 2:
	source — Number of nodes: 74183, Number of elements: 66720. 8 nodes per element.
	target — Number of nodes: 18664, Number of elements: 82206. 4 nodes per element.
   @Model 3:
	source — Number of nodes: 24543, Number of elements: 83316. 4 nodes per element.
	target — Number of nodes: 409140, Number of elements: 1609909. 4 nodes per element.
   @Model 4:
	source — Number of nodes: 24543, Number of elements: 83316. 4 nodes per element.
	target — Number of nodes: 41637, Number of elements: 154407. 4 nodes per element.

*/