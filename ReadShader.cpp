#include "ReadShader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

std::string ReadVertexShader(const std::string &filename) {
	std::ifstream file(filename.c_str());
	std::string code = "";

	if (!file.is_open()) {
		std::cout << "File not open" << filename << std::endl;
	}
	else {
		std::string buffer = "";
		while (std::getline(file, buffer)) {
			code += buffer + '\n';
		}
		file.close();
	}

	return code;

}