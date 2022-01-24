// parsers.cpp -> port of FEDES v2 parsing functions.

#include "fedes/model/parsers.hpp"

#include <filesystem>
#include <fstream>
#include <vector>
#include <array>
#include <string>

#include "fedes/model/model.hpp"
#include "fedes/common/files.hpp"
#include "fedes/common/strings.hpp"

namespace fedes {

	/**
	 * @brief Reads an ANSYS input file exported with LIS, parses nodes and elements.
	 *
	 * @port - Port of code from FEDES v2
	 * @param path: Path to the file
	 * @param model: Model which will be updated by reference
	 * @exception Propagates std::ifstream::failure
	 */
	void AnsysInputReadLis(const std::filesystem::path& path, fedes::Model& model) {
		int br = -1, br_1 = -1, br_2 = -1, br_3 = -1, br_el = 0, action = 4, num_nblocks = 1, num_eblocks = 1;
		int anz_tokens = 0;
		std::vector<std::string> A, A1;
		size_t size = 0;
		std::string line;
		std::ifstream stream;
		try {
			fedes::SetInputFileStream(path, stream);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
		while (stream.peek() != EOF && std::getline(stream, line)) {
			if ((line.find("NODE") != std::string::npos) && (line.find("X") != std::string::npos)) {
				action = 1;
			} else if ((line.find("ELEM") != std::string::npos) && (line.find("MAT") != std::string::npos) && (line.find("TYP") != std::string::npos)) {
				action = 2;
			}

			if ((action == 1) && (line.find("NODE") == std::string::npos) && (fedes::Trim(line) != "") && (line.find("LIST") == std::string::npos) && (line.find("SORT") == std::string::npos)) {
				br_1++;
				anz_tokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				for (auto& s : A) {
					if (s != "") {
						A1.push_back(s);
					}
				}
				model.nodes.emplace_back(stod(fedes::Trim(A1[1])), stod(fedes::Trim(A1[2])), stod(fedes::Trim(A1[3])));
				A.clear();
				A1.clear();
			}
			else if ((action == 2) && (line.find("ELEM") == std::string::npos) && (fedes::Trim(line) != "") && (line.find("LIST") == std::string::npos) && (line.find("SORT") == std::string::npos)) {
				br_2++;
				anz_tokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				size = A1.size() - 1;
				if ((A1[size]) != (A1[size-1])) { // Hex
					model.elements.push_back({ stoi(A1[5]) - 1, stoi(A1[6]) - 1, stoi(A1[7]) - 1, stoi(A1[8]) - 1, stoi(A1[9]) - 1, stoi(A1[10]) - 1, stoi(A1[11]) - 1, stoi(A1[12]) - 1 });
				} else if ((A1[size] == A1[size-1]) && (A1[size] == A1[size-2])) { // Tetrahedron
					model.elements.push_back({ stoi(A1[6]) - 1, stoi(A1[7]) - 1, stoi(A1[8]) - 1, stoi(A1[10]) - 1 });
				} else { // Wedge
					model.elements.push_back({ stoi(A1[5]) - 1, stoi(A1[6]) - 1, stoi(A1[7]) - 1, stoi(A1[9]) - 1, stoi(A1[10]) - 1, stoi(A1[11]) - 1});
				}
			}
			A.clear();
			A1.clear();
		}
		try {
			fedes::CloseInputFileStream(stream);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
	}

	/**
	* @brief Reads an ANSYS output file (in .txt), parsing displacements, total strain and stress
	*
	* @port - Port of code from FEDES v2
	* @param path: Path to the file
	* @param model: Model which will be updated by reference
	* @exception Propagates std::ifstream::failure
	*/
	void AnsysOutputRead(const std::filesystem::path& path, fedes::Model& model) {
		int br = -1, br_1 = -1, br_2 = -1, br_3 = -1, action = 4, AnzTokens = 0;
		std::vector<std::string> A, A1;
		std::string line;
		std::ifstream stream;
		try {
			fedes::SetInputFileStream(path, stream);
		}
		catch (const std::ifstream::failure& e) {
			throw;
		}
		while (stream.peek() != EOF && std::getline(stream, line)) {
			if ((line.find("NODE") != std::string::npos) && (line.find("UX") != std::string::npos) && (line.find("UY") != std::string::npos)) {
				action = 1;
			}
			if ((line.find("NODE") != std::string::npos) && (line.find("SX") != std::string::npos) && (line.find("SY") != std::string::npos)) {
				action = 2;
			}
			if ((line.find("NODE") != std::string::npos) && (line.find("EX") != std::string::npos) && (line.find("EY") != std::string::npos)) {
				action = 3;
			}
			if ((line == "") || (line.find("--") != std::string::npos)) {
				action = 4;
			}

			if ((action == 1) && (fedes::Trim(line).find("NODE") == std::string::npos)) {
				br_1++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), '\t');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				model.displacement.resize(model.displacement.size() + 1);
				model.displacement[br_1].emplace_back(stod(A1[1]));
				model.displacement[br_1].emplace_back(stod(A1[2]));
				model.displacement[br_1].emplace_back(stod(A1[3]));
				A.clear();
				A1.clear();
			}
			if ((action == 2) && (fedes::Trim(line).find("NODE") == std::string::npos)) {
				br_2++;
				model.stress.resize(model.stress.size() + 1);
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), '\t');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				model.stress[br_2].emplace_back(stod(A1[1]));
				model.stress[br_2].emplace_back(stod(A1[2]));
				model.stress[br_2].emplace_back(stod(A1[3]));
				model.stress[br_2].emplace_back(stod(A1[4]));
				model.stress[br_2].emplace_back(stod(A1[5]));
				model.stress[br_2].emplace_back(stod(A1[6]));
				A.clear();
				A1.clear();
			}
			if ((action == 3) && (fedes::Trim(line).find("NODE") == std::string::npos)) {
				br_3++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), '\t');
				model.total_strain.resize(model.total_strain.size() + 1);
				for (auto& s : A) {
					A1.emplace_back(s);
				}
				model.total_strain[br_3].emplace_back(stod(A1[1]));
				model.total_strain[br_3].emplace_back(stod(A1[2]));
				model.total_strain[br_3].emplace_back(stod(A1[3]));
				model.total_strain[br_3].emplace_back(stod(A1[4]));
				model.total_strain[br_3].emplace_back(stod(A1[5]));
				model.total_strain[br_3].emplace_back(stod(A1[6]));
				A.clear();
				A1.clear();
			}
		}
		try {
			fedes::CloseInputFileStream(stream);
		}
		catch (const std::ifstream::failure& e) {
			throw;
		}
	}

	/*
	 * @brief Maps nodes and elements to fedes::Model from an Abaqus .inp file
	 *
	 * @port - Port of code from FEDES v2
	 * @param path: Path to the file
	 * @param model: Model which will be updated by reference
	 * @exception Propagates std::ifstream::failure
	 */
	void AbaqusInputRead(const std::filesystem::path& path, fedes::Model& model) {
		std::string c1, st = "";
		int AnzTokens = 0, br = -1, br_1 = -1, br_el = 0, action = 3, node = 0, element = 0;
		bool new_row = false;
		std::vector<std::string> A, A1;
		std::string line;
		std::ifstream stream;
		try {
			fedes::SetInputFileStream(path, stream);
		}
		catch (const std::ifstream::failure& e) {
			throw;
		}
		while (stream.peek() != EOF && std::getline(stream, line)) {
			line = fedes::ToLower(line);
			if (line.find("*") != std::string::npos) {
				action = 3;
			}
			if (line.find("*node") == 0) {
				action = 1;
				node++;
			} 
			if (line.find("*element") == 0) {
				action = 2;
				element++;
			}
			if (line.find("*element output") == 0) {
				action = 3;
			}
			
			if ((action == 1) && (line.find("*node") == std::string::npos) && (node == 1)) {
				br++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ',');
				model.nodes.emplace_back(stod(fedes::Trim(A[1])), stod(fedes::Trim(A[2])), stod(fedes::Trim(A[3])));
				A.clear();
			}
			if ((action == 2) && (line.find("*element") == std::string::npos)) {
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ',');
				if (fedes::Trim(A[A.size() - 1]) != "") {
					if (new_row) {
						br_1++;
						model.elements.resize(model.elements.size() + 1);
						st = st + line;
						AnzTokens = fedes::StringSplit(A1, st, ',');
						for (int j = 1; j < (A1.size()); j++) {
							model.elements[br_1].emplace_back(stoi(fedes::Trim(A1[j])));
						}
						A1.clear();
						st = "";
						new_row = false;
					}
					else {
						br_1++;
						model.elements.resize(model.elements.size() + 1);
						for (int j = 1; j < (A.size()); j++) {
							model.elements[br_1].emplace_back(stoi(fedes::Trim(A[j])) - 1);
						}
						A.clear();
					}
				}
				else {
					st = st + line;
					new_row = true;
				}
				A.clear();
			}
		}	
		try {
			fedes::CloseInputFileStream(stream);
		}
		catch (const std::ifstream::failure& e) {
			throw;
		}
	}

	/**
	* @brief Maps Abaqus Output data for a model where the input data was set prior (uses node.size)
	* 
	* Reads an ABAQUS output file (in .dat), parsing displacements, stress, total strain, platsic strain & accumulated strain
	*
	* @port - Port of code from FEDES v2
	* @param path: Path to the file
	* @param model: Model which will be updated by reference
	* @exception Propagates std::ifstream::failure
	*/
	void AbaqusOutputRead(const std::filesystem::path& path, fedes::Model& model) {
		int br = -1, br_1 = -1, br_2 = -1, br_3 = -1, action = 5, AnzTokens = 0;
		std::vector<std::string> operating_str_arr, A, A1;
		size_t length = model.nodes.size();
		std::string line;
		std::ifstream stream;
		try {
			fedes::SetInputFileStream(path, stream);
		}
		catch (const std::ifstream::failure& e) {
			throw;
		}
		while (stream.peek() != EOF && std::getline(stream, line)) {
			if ((line.find("S11") != std::string::npos) && (line.find("S22") != std::string::npos)) {
				br = 1;
			}
			if ((line.find("U1") != std::string::npos) && (line.find("U2") != std::string::npos)) {
				br_1 = 1;
			}
			if ((line.find("E11") != std::string::npos) && (line.find("E22") != std::string::npos)) {
				br_2 = 1;
			}
			if ((line.find("PE11") != std::string::npos) && line.find("PE22") != std::string::npos) {
				br_3 = 1;
			}
		}
		if (br == 1) {
			model.stress.resize(length);
		} 
		if (br_1 == 1) {
			model.displacement.resize(length);
		}
		if (br_2 == 1) {
			model.total_strain.resize(length);
		}
		if (br_3 == 1) {
			model.accumulated_strain.resize(length);
			model.plastic_strain.resize(length);
		}
		try {
			fedes::RewindInputStream(stream);
		}
		catch (const std::ifstream::failure& e) {
			throw;
		}
		br, br_1, br_2, br_3 = -1;

		while (stream.peek() != EOF && std::getline(stream, line)) {
			if ((line.find("S11") != std::string::npos) && (line.find("S22") != std::string::npos)) {
				action = 1;
			}
			if ((line.find("U1") != std::string::npos) && (line.find("U2") != std::string::npos)) {
				action = 2;
			}
			if ((line.find("E11") != std::string::npos) && (line.find("E22") != std::string::npos)) {
				action = 3;
			}
			if ((line.find("PE11") != std::string::npos) && (line.find("PE22") != std::string::npos)) {
				action = 4;
			}
			if ((line.find("MAXIMUM") != std::string::npos)) {
				action = 5;
			}

			if ((action == 1) && (line.find("NOTE") == std::string::npos) && (fedes::Trim(line) != "") && (line.find("S11") == std::string::npos) && (line.find("S22") == std::string::npos)) {
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				if (A1.size() == 7) {
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[3]));
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[4]));
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[6]));
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[5]));
				}
				if (A1.size() == 5) {
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[3]));
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[4]));
				}
				if (A1.size() == 4) {
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
					model.stress[stoi(A1[0]) - 1].emplace_back();
					model.stress[stoi(A1[0]) - 1].emplace_back(stod(A1[3]));
				}
				A.clear();
				A1.clear();
			}
			if ((action == 2) && (line.find("NOTE") == std::string::npos) && (line.find("U1") == std::string::npos) && (line.find("U2") == std::string::npos)) {
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				if (A1.size() == 4) {
					model.displacement[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.displacement[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
					model.displacement[stoi(A1[0]) - 1].emplace_back(stod(A1[3]));
				}

				if (A1.size() == 3) {
					model.displacement[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.displacement[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
				}
				A.clear();
				A1.clear();
			}
			if ((action == 3) && (line.find("NOTE") == std::string::npos) && (line != "") && (line.find("E11") == std::string::npos) && (line.find("E22") == std::string::npos)) {
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				if (A1.size() == 7) {
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[3]));
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[4]));
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[6]));
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[5]));
				}
				if (A1.size() == 5) {
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[3]));
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[4]));
				}
				if (A1.size() == 4) {
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
					model.total_strain[stoi(A1[0]) - 1].emplace_back();
					model.total_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[3]));
				}
				A.clear();
				A1.clear();
			}
			if ((action == 4) && (line.find("NOTE") == std::string::npos) && (line != "") && (fedes::ToLower(line).find("all") == std::string::npos) 
				&& (line.find("PE11") == std::string::npos) && (line.find("PE22") == std::string::npos)) {
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				if (A1.size() == 9) {
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[3]));
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[4]));
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[6]));
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[5]));

					model.accumulated_strain[stoi(A1[0]) - 1] = stod(A1[7]);
				}

				if (A1.size() == 7) {
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[3]));
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[4]));

					model.accumulated_strain[stoi(A1[0]) - 1] = stod(A1[5]);
				}

				if (A1.size() == 6) {
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[1]));
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[2]));
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back();
					model.plastic_strain[stoi(A1[0]) - 1].emplace_back(stod(A1[3]));

					model.accumulated_strain[stoi(A1[0]) - 1] = stod(A1[4]);
				}
				A.clear();
				A1.clear();
			}		
		}
		try {
			fedes::CloseInputFileStream(stream);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
	}

	/*
	 * @brief Maps nodes and elements to fedes::Model from an Morpheo .vtu or .xml file
	 *
	 * @port - Port of code from FEDES v2
	 * @param path: Path to the file
	 * @param model: Model which will be updated by reference
	 * @exception Propagates std::ifstream::failure
	 */
	void MorpheoInputRead(const std::filesystem::path& path, fedes::Model& model) {
		bool point = false;
		int counter = -1, counter_2 = -1, counter_3 = -1, counter_4 = -1, counter_5 = -1, counter_6 = -1;
		int action = 0, node_number = 0, element_number = 0, AnzTokens = 0, br = 0;
		std::vector<std::string> A, A1;
		std::string line;
		std::ifstream stream;
		try {
			fedes::SetInputFileStream(path, stream);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
		while (stream.peek() != EOF && std::getline(stream, line)) {
			if (line.find("<PointData") != std::string::npos) {
				point = true;
			}
			if (line.find("</PointData>") != std::string::npos) {
				point = false;
			}
			if ((line.find("<DataArray") != std::string::npos) && ((fedes::ToLower(line)).find("coordinates") != std::string::npos)) {
				action = 1;
			}
			if ((line.find("<DataArray") != std::string::npos) && ((fedes::ToLower(line)).find("connectivity") != std::string::npos)) {
				action = 2;
			}
			if (line.find("</DataArray>") != std::string::npos) {
				action = 0;
			}

			if ((action == 1) && line.find("<DataArray") == std::string::npos) {
				counter++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				model.nodes.emplace_back(stod(A1[0]), stod(A1[1]), stod(A1[2]));
				A.clear();
				A1.clear();
			}
			if ((action == 2) && line.find("<DataArray") == std::string::npos) {
				counter_2++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				model.elements.resize(model.elements.size() + 1);
				for (auto &s: A1) {
					model.elements[counter_2].emplace_back(stoi(s));
				}
				A.clear();
				A1.clear();
			}
		}
		try {
			fedes::CloseInputFileStream(stream);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
	};

	/*
	 * @brief Maps nodes, elements, stresses, displacements, total strains and accumulated strains from Morpheo .vtu/.xml
	 *
	 * @port - Port of code from FEDES v2
	 * @param path: Path to the file
	 * @param model: Model which will be updated by reference
	 * @exception Propagates std::ifstream::failure
	 */
	void MorpheoInputOutputRead(const std::filesystem::path& path, fedes::Model& model) {
		bool point = false;
		int counter = -1, counter_2 = -1, counter_3 = -1, counter_4 = -1, counter_5 = -1, counter_6 = -1, counter_7 = 0;
		int action = 0, node_number = 0, element_number = 0, AnzTokens = 0, br = 0;
		std::vector<std::string> A, A1;
		std::string line;
		std::ifstream stream;
		try {
			fedes::SetInputFileStream(path, stream);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
		while (stream.peek() != EOF && std::getline(stream, line)) {
			if (line.find("<PointData") != std::string::npos) {
				point = true;
			}
			if (line.find("</PointData>") != std::string::npos) {
				point = false;
			}
			if ((line.find("<DataArray") != std::string::npos) && ((fedes::ToLower(line)).find("coordinates") != std::string::npos)) {
				action = 1;
			}
			if ((line.find("<DataArray") != std::string::npos) && ((fedes::ToLower(line)).find("connectivity") != std::string::npos)) {
				action = 2;
			}
			if ((line.find("<DataArray") != std::string::npos) && ((fedes::ToLower(line)).find("stress") != std::string::npos)) {
				action = 3;
			}
			if ((line.find("<DataArray") != std::string::npos) && ((fedes::ToLower(line)).find("displacement") != std::string::npos)) {
				action = 4;
			}
			if ((line.find("<DataArray") != std::string::npos) && ((fedes::ToLower(line)).find("strain") != std::string::npos)) {
				action = 5;
			}
			if ((line.find("<DataArray") != std::string::npos) && ((fedes::ToLower(line)).find("accumulated_plastic_strain") != std::string::npos)) {
				action = 6;
			}
			if (line.find("</DataArray>") != std::string::npos) {
				action = 0;
			}

			if ((action == 1) && (line.find("<DataArray") == std::string::npos)) {
				counter++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				model.nodes.emplace_back(stod(A1[0]), stod(A1[1]), stod(A1[2]));
				A.clear();
				A1.clear();
			}
			if ((action == 2) && (line.find("<DataArray") == std::string::npos)) {
				counter_2++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				model.elements.resize(model.elements.size() + 1);
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				for (auto& s : A1) {
					model.elements[counter_2].emplace_back(stoi(s));
				}
				A.clear();
				A1.clear();
			}
			if ((action == 3) && (line.find("<DataArray") == std::string::npos)) {
				counter_3++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				model.stress.resize(model.stress.size() + 1);
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				model.stress[counter_3].emplace_back(stod(A1[0]));
				model.stress[counter_3].emplace_back(stod(A1[1]));
				model.stress[counter_3].emplace_back(stod(A1[2]));
				model.stress[counter_3].emplace_back(stod(A1[3]));
				model.stress[counter_3].emplace_back(stod(A1[4]));
				model.stress[counter_3].emplace_back(stod(A1[5]));
				A.clear();
				A1.clear();
			}
		
			if ((action == 4) && (line.find("<DataArray") == std::string::npos)) {
				counter_4++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				model.displacement.resize(model.displacement.size() + 1);
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				model.displacement[counter_4].emplace_back(stod(A1[0]));
				model.displacement[counter_4].emplace_back(stod(A1[1]));
				model.displacement[counter_4].emplace_back(stod(A1[2]));
				A.clear();
				A1.clear();
			}
			if ((action == 5) && (line.find("<DataArray") == std::string::npos)) {
				counter_5++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				model.total_strain.resize(model.total_strain.size() + 1);
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				model.total_strain[counter_5].emplace_back(stod(A1[0]));
				model.total_strain[counter_5].emplace_back(stod(A1[1]));
				model.total_strain[counter_5].emplace_back(stod(A1[2]));
				model.total_strain[counter_5].emplace_back(stod(A1[3]));
				model.total_strain[counter_5].emplace_back(stod(A1[4]));
				model.total_strain[counter_5].emplace_back(stod(A1[5]));
				A.clear();
				A1.clear();
			}
			if ((action == 6) && (line.find("<DataArray") == std::string::npos)) {
				counter_6++;
				AnzTokens = fedes::StringSplit(A, fedes::Trim(line), ' ');
				for (auto& s : A) {
					if (s != "") {
						A1.emplace_back(s);
					}
				}
				model.accumulated_strain.emplace_back(stod(A1[0]));
				A.clear();
				A1.clear();
			}
		}
		try {
			fedes::CloseInputFileStream(stream);
		} catch (const std::ifstream::failure& e) {
			throw;
		}
	}

} // namespace end 