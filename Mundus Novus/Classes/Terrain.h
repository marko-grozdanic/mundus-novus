#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	int adjacentFaceCount;
};

class Terrain {
public:
	unsigned int sizeX;
	unsigned int sizeN;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> terrainHeightmap;

	Terrain(unsigned int sizeX);
	void generateTerrain();
	void setupTerrain();
	void drawTerrain(bool wireframe);
	void setSize(int n);
	glm::vec3 calculateVertexNormals(Vertex vertex);
	glm::vec3 calculateFaceNormals(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);

	//Different heightmap generation algorithms
	void genMidpointDisplacement(unsigned int seed, float spread, float spreadReductionRate);

private:
	//  render data
	unsigned int VAO;
	float minHeightOffset;
};