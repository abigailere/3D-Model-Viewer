#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
struct Normal {
	float x, y, z;
};
struct Vertex {
	float x, y, z;
	Normal normal;
	void print()
	{
		cout << x << " " << y << " " << z << endl;
	}
	float magnitude()
	{
		float result = sqrt(x * x + y * y + z * z);
		return result;
	}
};

struct Face {
	int vertexIndex, texCoordIndex, normalIndex;
	void print()
	{
		cout << vertexIndex << " " << texCoordIndex << " " << normalIndex;
	}
};

void readObjFile(const char* filename, vector<Vertex>& vertices, vector<Normal>& normals, vector<Face>& faces) {

	ifstream objFile(filename);
	if (!objFile) {
		cerr << "Failed to open file: " << filename << endl;
		return;
	}

	string line;
	Vertex maxMagnitude;
	maxMagnitude.x = 0;
	maxMagnitude.y = 0;
	maxMagnitude.z = 0;
	while (getline(objFile, line)) {
		istringstream iss(line);
		string token;
		iss >> token;

		if (token == "v") {
			Vertex vertex;
			iss >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
			if (vertex.magnitude() > maxMagnitude.magnitude())
			{
				maxMagnitude = vertex;
			}
		}
		else if (token == "f") {
			Face face;
			for (int i = 0; i < 3; i++) {
				string faceValue;
				iss >> faceValue;
				istringstream faceValueStream(faceValue);
				string vertexIndexStr, texCoordIndexStr, normalIndexStr;
				getline(faceValueStream, vertexIndexStr, '/');
				getline(faceValueStream, texCoordIndexStr, '/');
				getline(faceValueStream, normalIndexStr, '/');

				face.vertexIndex = stoi(vertexIndexStr) - 1; // OBJ indices are 1-based
				if (isdigit(normalIndexStr[0]))
				{
					face.normalIndex = stoi(normalIndexStr) - 1;
				}
				else
				{
					cout << "Not a digit";
				}
				faces.push_back(face);
			}


		}
		else if (token == "vn")
		{
			Normal nor;
			iss >> nor.x >> nor.y >> nor.z;
			normals.push_back(nor);
		}

	}

}