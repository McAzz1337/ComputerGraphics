#include "fileio.h"
#include <ios>
#include <iostream>
#include <fstream>
#include <filesystem>



	void readFile(const std::string& path, std::string& dst, bool log) {

		std::ifstream in(path);

		if (!in.is_open()) {
			
			if (log)
				printf("failed to open file: %s\n", path.c_str());
			
			dst = "";
			return;
		}

		std::string line;
		while (getline(in, line)) {
			dst += line + "\n";
		}

		in.close();
	}

	void readFileSplit(const std::string& path, std::vector<std::string>& dst, bool log) {

		std::ifstream in(path);
		
		if (!in.is_open()) {
			
			if (log)
				printf("failed to open file: %s\n", path.c_str());
			
			return;
		}
		

		std::string line;
		while (getline(in, line)) {
			dst.push_back(line);
		}

		in.close();
	}


	void extractFileName(const std::string& path, std::string& dst, const char del) {
		size_t start = path.find_last_of(del);

		if (start != std::string::npos)
			dst = path.substr(start + 1);
	}

	void splitPath(const std::string& path, std::string& pathDst, std::string& fileDst) {
		size_t delimeter = path.find_last_of('/');

		if (delimeter == std::string::npos) {
			pathDst = "";
			fileDst = "";
		}

		pathDst = path.substr(0, delimeter);
		fileDst = path.substr(delimeter + 1);
	}

	void split(const std::string& src, const char delimeter, std::string* tokens, int size) {
		
		size_t start = 0;
		for (int i = 0; i < size; i++) {

			size_t end = src.find(delimeter, start);
			if (end == std::string::npos) {
				tokens[i] = src.substr(start);
				break;
			}

			tokens[i] = src.substr(start, end);
			start = end + 1;
		}
	}

	void split(const std::string& src, std::vector<std::string>& dst, const char delimeter) {
		
		size_t end = src.find(delimeter);
		size_t  start = 0;

		while (end != std::string::npos) {

			dst.push_back(src.substr(start, end - start));

			start = end + 1;
			end = src.find(delimeter, start);
		}

		dst.push_back(src.substr(start));

	}



	uint64_t requestFileSize(const std::string& path) {
		
		uint64_t begin;
		uint64_t end;

		std::ifstream in(path);
		
		if (!in.is_open())
			return 0;

		begin = in.tellg();
		
		in.seekg(0, std::ios::end);
		end = in.tellg();
		
		in.close();

		return end - begin;
	}

	void listDirectoryEntries(const std::string& dir, std::vector<std::string>& dst) {

		for (const auto entry : std::filesystem::directory_iterator(dir)) {
			dst.push_back(entry.path().filename().string());
		}

	}






	void writeFile(const std::string& path, const std::string& src) {

		std::ofstream out(path);

		out << src;

		out.close();
	}


