#pragma once

#include <vector>
#include <array>
#include <iostream>
#include <filesystem>
#include <string>

#include "fedes/maths/vector3.hpp"

namespace fedes {

	struct Model {
	public:
		std::vector<fedes::Vector3<double>> nodes;
		std::vector<std::vector<int>> elements;
		std::vector<std::vector<double>> displacement;
		std::vector<std::vector<double>> stress;
		std::vector<std::vector<double>> total_strain;
		std::vector<std::vector<double>> plastic_strain;
		std::vector<double> accumulated_strain;
		std::vector<fedes::Vector3<double>> integration;
	public:
		void SetIndexes(const fedes::Model& source);
		void AssignIntegration();

		void Export(const std::string& file_name, bool by_integration, const std::filesystem::path& path = "../../../exports");

		void WriteStats(std::ostream& output_stream = std::cout);
		void WriteStatsParalllel(std::ostream& output_steram = std::cout);
		bool operator==(const Model& other) const;
	};

}