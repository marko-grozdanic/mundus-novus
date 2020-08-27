#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Terrain {
public:
	unsigned int sizeX;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> terrainHeightmap;

	Terrain(unsigned int sizeX);
	void generateTerrain();
	void setupTerrain();
	void drawTerrain(bool wireframe);
	void calculateVertexNormals();
	void calculateFaceNormals();

	//Different heightmap generation algorithms
	void genMidpointDisplacement(unsigned int seed, float spread, float spreadReductionRate);

private:
	//  render data
	unsigned int VAO;
	float minHeightOffset;
};