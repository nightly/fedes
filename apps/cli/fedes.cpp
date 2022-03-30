#include <iostream>

#include "models.h"
#include "octree.h"


/*
* @brief Prompts the user for model, parameters etc
*/
void prompt() {
	fedes::Model source, target;
	unsigned int model{};
	std::cout << "Enter model ID to use (1 - 4): \n";
	std::cin >> model;
	if (model < 1 || model > 4) {
		std::cout << "[Error] Invalid model ID!";
		return;
	}
	SetModels(source, target, model);

	size_t points_per_leaf{};
	size_t max_depth{};
	std::cout << "Enter points per leaf in Octree: \n";
	std::cin >> points_per_leaf;
	std::cout << "Enter maximum depth of Octree: \n";
	std::cin >> max_depth;
	if (points_per_leaf < 0 || max_depth < 0) {
		std::cout << "[Error] Invalid Octree parameters!";
		return;
	}
	
	size_t interpolation_type;
	std::cout << "Enter interpolation type (1 - NPM, 2 - FOP, 3 - DMUE, 4 - ESF) \n";
	std::cin >> interpolation_type;
	if (interpolation_type < 1 || interpolation_type > 4) {
		std::cout << "[Error] Invalid interplation type!";
		return;
	}
	switch (interpolation_type) {
		case 1:
			OctreeNPM(source, target, points_per_leaf, max_depth);
			ExportModels(source, target, "mx-oct-npm");
			break;
		case 2:
			OctreeDMUFOP(source, target, points_per_leaf, max_depth);
			ExportModels(source, target, "mx-oct-fop");
			break;
		case 3:
			OctreeDMUE(source, target, points_per_leaf, max_depth);
			ExportModels(source, target, "mx-oct-dmue");
			break;
		case 4:
			OctreeESF(source, target, points_per_leaf, max_depth);
			ExportModels(source, target, "mx-oct-esf");
			break;
	}

	std::cout << "Models exported\n\n\n";
}

/*
* @brief Doesn't prompt the user, uses defaults for testing
*/
void no_prompt() {
	fedes::Model source, target;
	SetModels(source, target, 1);
	OctreeNPM(source, target, 8, 10);
	ExportModels(source, target, "oct-npm");
}

/*
* @brief Main program loop, terminates after first iteration if user_prompt = false
*/
int main() {
	bool prompt_user = true;
	while (true) {
		switch (prompt_user) {
		case(true):
			prompt();
			break;
		case(false):
			no_prompt();
			std::exit(0);
			break;
		}
	}
	return 0;
}