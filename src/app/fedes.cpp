#include <iostream>

#include "models.hpp"
#include "octree.hpp"

/*
* @brief Prompts the user for model, parameters etc,
*/
void prompt() {
	fedes::Model source, target;
	unsigned int model{};
	std::cout << "Enter model ID to use (1 - 4): \n";
	std::cin >> model;
	SetModels(source, target, model);

	size_t points_per_leaf{};
	size_t max_depth{};
	std::cout << "Enter points per leaf in Octree: \n";
	std::cin >> points_per_leaf;
	std::cout << "Enter maximum depth of Octree: \n";
	std::cin >> max_depth;

	OctreeNPM(source, target, points_per_leaf, max_depth);
	ExportModels(source, target, "oct-npm");
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