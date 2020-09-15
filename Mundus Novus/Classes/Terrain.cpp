#include "Terrain.h"
#include "MidpointDisplacement.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <random>

Terrain::Terrain(unsigned int n) {
	this->sizeN = n;
	this->sizeX = pow(2, n) + 1;
	//this->sizeX = pow(2, n);
	this->terrainHeightmap.resize(sizeX * sizeX);
	this->minHeight = INFINITY;
	this->maxHeight = -INFINITY;
}

void Terrain::setupTerrain() {
	unsigned int VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0); //unbind
}

void Terrain::drawTerrain(bool wireframe) {
	glBindVertexArray(VAO);
	
	// Generate a buffer for the indices
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);	
	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}	
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Set everything back to defaults once configured.
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

glm::vec3 Terrain::calculateFaceNormals(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
	glm::vec3 a = v1 - v0;
	glm::vec3 b = v2 - v0;
	glm::vec3 cross = glm::cross(a, b);	
	return glm::normalize(cross);
}

glm::vec3 Terrain::calculateVertexNormals(Vertex vertex) {
	return glm::vec3(1.0f, 1.0f, 1.0f);
}

void Terrain::generateTerrain() {
	vertices.clear();
	indices.clear();
	int size = sizeX * sizeX;
	Vertex vertex;
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeX; j++) {
			vertex.Position.x = (float)i;
			vertex.Position.z = (float)j;
			vertex.Position.y = this->terrainHeightmap[i * sizeX + j];
			vertex.Normal.x = 0.0f;
			vertex.Normal.y = 0.0f;
			vertex.Normal.z = 0.0f;
			vertex.TexCoords.x = float(i);
			vertex.TexCoords.y = float(j);
			vertex.adjacentFaceCount = 0;
			vertices.push_back(vertex);

			if (vertex.Position.y < minHeight) {
				minHeight = vertex.Position.y;
			}
			else if (vertex.Position.y > maxHeight) {
				maxHeight = vertex.Position.y;
			}
		}
	}
	glm::vec3 faceNormal;
	for (int i = 0; i < sizeX - 1; i++) {
		for (int j = 0; j < sizeX - 1; j++) {
			int topLeft = (i * sizeX) + j;
			int topRight = topLeft + 1;
			int bottomLeft = ((i + 1) * sizeX) + j;
			int bottomRight = bottomLeft + 1;
			//First triangle
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			faceNormal = calculateFaceNormals(vertices[topLeft].Position, vertices[bottomLeft].Position, vertices[topRight].Position);
			vertices[topLeft].Normal += faceNormal; vertices[topLeft].adjacentFaceCount += 1;
			vertices[bottomLeft].Normal += faceNormal; vertices[bottomLeft].adjacentFaceCount += 1;
			vertices[topRight].Normal += faceNormal; vertices[topRight].adjacentFaceCount += 1;

			//Second triangle
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);

			faceNormal = calculateFaceNormals(vertices[topRight].Position, vertices[bottomLeft].Position, vertices[bottomRight].Position);
			vertices[topRight].Normal += faceNormal; vertices[topRight].adjacentFaceCount += 1;
			vertices[bottomLeft].Normal += faceNormal; vertices[bottomLeft].adjacentFaceCount += 1;
			vertices[bottomRight].Normal += faceNormal; vertices[bottomRight].adjacentFaceCount += 1;
		}
	}
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].Normal = glm::normalize(vertices[i].Normal);
	}
}

void Terrain::genMidpointDisplacement(unsigned int seed, float spread, float spreadReductionRate) {
	MidpointDisplacement heightmapMD(seed, spread, spreadReductionRate, sizeX, sizeN);

	this->terrainHeightmap = heightmapMD.heightmap;
}

void Terrain::setSize(int n) {
	this->sizeN = n;
	this->sizeX = pow(2, n) + 1;
	this->terrainHeightmap.resize(sizeX * sizeX);
}