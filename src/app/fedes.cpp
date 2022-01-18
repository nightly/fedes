#include <iostream>

#include "models.hpp"
#include "octree.hpp"


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
* @brief Prompts the user for model, parameters etc,
*/
void prompt() {
	fedes::Model source, target;
	unsigned int model{};
	std::cout << "Enter model ID to use (1 - 4): \n";
	std::cin >> model;
	if (model <= 0 || model >= 5) {
		std::cout << "Invalid model selected\n\n\n";
		return;
	}
	SetModels(source, target, model);

	size_t points_per_leaf{};
	size_t max_depth{};
	std::cout << "Enter points per leaf in Octree: \n";
	std::cin >> points_per_leaf;
	std::cout << "Enter maximum depth of Octree: \n";
	std::cin >> max_depth;
	if (max_depth < 1 || points_per_leaf < 1) {
		std::cout << "Invalid parameters\n\n\n";
		return;
	}

	OctreeNPM(source, target, points_per_leaf, max_depth);
	ExportModels(source, target, "oct-npm");
	std::cout << "Models exported\n\n\n";
}

/*
* @brief Main program loop, terminates after first iteration if user_prompt = false
*/
int main() {
	bool prompt_user = true;
	while (true) {
		switch (prompt_user) {
		case(false):
			no_prompt();
			std::exit(0);
			break;
		case(true):
			prompt();
			break;
		}
	}
	return 0;
}