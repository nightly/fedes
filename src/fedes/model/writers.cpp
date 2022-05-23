// writers.cpp -> Port of FEDES v2 code

#include "fedes/model/writers.h"

#include <filesystem>
#include <fstream>
#include <format>

#include "fedes/common/files.h"
#include "fedes/maths/vector3.h"
#include "fedes/model/model.h"

namespace fedes {

	/*
	 * @brief CreateXML will export an XML file with data that can be visualised with ParaView or Morpheo by providing a model and output file path.
	 * @port This function is ported from FEDES v2
	 * @param path: Path to write file to, e.g. "dir/a.xml" or "b.vtu"
	 * @param model: Model that will be used to write data to the file from
	 * @param by_integration: differentiates whether or not we are dealing with certain values by integration or node points,
	 * in FEDES v2, this corresponds to the difference between createXML1 and create XML2
	 * @exception Propagates std::ofstream failure
	 */
	void CreateXML(const std::filesystem::path& output_file_path, fedes::Model& model, bool by_integration, bool has_fea_data) {
		int num = 0;
		double z_coord = 0.00;
		std::string str;
		std::ofstream stream;
		try {
			fedes::SetOutputFileStream(output_file_path, stream);
		} catch (const std::ofstream::failure& e) {
			throw;
		}

		stream << "<?xml version=\"1.0\"?>\n";
		stream << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\" >\n";
		stream << "<UnstructuredGrid>\n";
		stream << "<Piece NumberOfPoints=\"" << model.nodes.size() << "\" NumberOfCells=\"" << model.elements.size() << "\" >\n";

		if (!model.displacement.empty() && has_fea_data) {
			stream << "<PointData Tensors=\"Vector\" >\n";
			stream << "<DataArray type=\"Float64\" Name=\"displacement\" NumberOfComponents=\"3\" format=\"ascii\" >\n";
			for (auto& d : model.displacement) {	
				stream << std::format("{} {} {}", d[0], d[1], d[2]) << '\n';
			}
			stream << "</DataArray>\n";
		}
		if (by_integration) {
			stream << "</PointData>\n";
		}
		if (has_fea_data && (!model.stress.empty()) || (!model.plastic_strain.empty()) || (!model.total_strain.empty()) || (!model.accumulated_strain.empty())) {
			if (by_integration) {
				stream << "<CellData Tensors=\"stress\" >\n";
			}
			if (!model.stress.empty()) {
				stream << "<DataArray type=\"Float64\" Name=\"Stress\" NumberOfComponents=\"6\" format=\"ascii\" >\n";
				for (auto& s : model.stress) {
					stream << std::format("{} {} {} {} {} {}", s[0], s[1], s[2], s[3], s[4], s[5]) << "\n";
				}
				stream << "</DataArray>\n";
			}
			if (!model.total_strain.empty()) {
				stream << "<DataArray type=\"Float64\" Name=\"TotalStrain\" NumberOfComponents=\"6\" format=\"ascii\" >\n";
				for (auto& s : model.total_strain) {
					stream << std::format("{} {} {} {} {} {}", s[0], s[1], s[2], s[3], s[4], s[5]) << "\n";
				}
				stream << "</DataArray>\n";
			}
			if (!model.plastic_strain.empty()) {
				stream << "<DataArray type=\"Float64\" Name=\"PlasticStrain\" NumberOfComponents=\"6\" format=\"ascii\" >\n";
				for (auto& s : model.plastic_strain) {
					stream << std::format("{} {} {} {} {} {}", s[0], s[1], s[2], s[3], s[4], s[5]) << "\n";
				}
				stream << "</DataArray>\n";
			}
			if (!model.accumulated_strain.empty()) {
				stream << "<DataArray type=\"Float64\" Name=\"AccumulatedStrain\" NumberOfComponents=\"1\" format=\"ascii\" >\n";
				for (auto& s : model.accumulated_strain) {
					stream << std::format("{}", s) << "\n";
				}
				stream << "</DataArray>\n";
			}
			if (by_integration) {
				stream << "</CellData>\n";
			}
		}
		if (!by_integration && has_fea_data) {
			stream << "</PointData>\n";
		}
		stream << "<Cells>\n";
		stream << "<DataArray type=\"UInt32\" Name=\"connectivity\" NumberOfComponents=\"1\" format=\"ascii\" >\n";

		for (auto& e : model.elements) {
			for (auto& j : e) {
				str = str + std::to_string(j) + ' ';
			}
			stream << str << '\n';
			str = "";
		}
		stream << "</DataArray>\n";
		stream << "<DataArray type=\"UInt32\" Name=\"offsets\" NumberOfComponents=\"1\" format=\"ascii\" >\n";

		for (auto& e : model.elements) {
			num = num + e.size();
			stream << num << '\n';
		}
		stream << "</DataArray>\n";
		stream << "<DataArray type=\"UInt8\" Name=\"types\" NumberOfComponents=\"1\" format=\"ascii\" >\n";

		for (auto& n : model.nodes) {
			z_coord += n.z;
		}

		for (auto& e : model.elements) {
			if (e.size() == 8) {
				if (z_coord != 0) {
					stream << "12\n";
				}
				else {
					stream << "23\n";
				}
			}
			if (e.size() == 6) {
				if (z_coord != 0) {
					stream << "13\n";
				}
				else {
					stream << "22\n";
				}
			}
			if (e.size() == 4) {
				if (z_coord != 0) {
					stream << "10\n";
				}
				else {
					stream << "9\n";
				}
			}
			if (e.size() == 3) {
				stream << "5\n";
			}
			if (e.size() == 10) {
				stream << "24\n";
			}
			if (e.size() == 15) {
				stream << "13\n";
			}
			if (e.size() == 20) {
				stream << "25\n";
			}
		}
		stream << "</DataArray>\n";
		stream << "</Cells>\n";

		stream << "<Points>\n";
		stream << "<DataArray type=\"Float64\" Name=\"coordinates\" NumberOfComponents=\"3\" format=\"ascii\" >\n";
		for (auto& n : model.nodes) {
			stream << std::format("{} {} {}", n.x, n.y, n.z) << '\n';
		}
		stream << "</DataArray>\n";
		stream << "</Points>\n";

		stream << "</Piece>\n";
		stream << "</UnstructuredGrid>\n";
		stream << "</VTKFile>\n";

		try {
			fedes::CloseOutputFileStream(stream);
		}
		catch (const std::ofstream::failure& e) {
			throw;
		}
	}

} //namespace end