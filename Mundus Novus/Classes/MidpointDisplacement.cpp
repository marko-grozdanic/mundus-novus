#include "MidpointDisplacement.h"
#include <iostream>

MidpointDisplacement::MidpointDisplacement(unsigned int seed, float fHeight, float fRoughness, int sizeX, int n) {
	this->sizeX = sizeX;
	this->n;
	this->fHeight = fHeight;
	//this->fRoughness = pow(2, -fRoughness);
	this->fRoughness = fRoughness;
	this->heightmap.resize(sizeX * sizeX);

	srand(seed);

	clearHeightmap();
	randomiseCorners();
	midpointDisplacement();
}

float GetRandom(float fHeight)
{
	int min = -fHeight/2;
	int max = fHeight/2;
	return (min + rand() % ((max + 1) - min));
}

int calculateMidpoint(int a, int b)
{
	return ((b - a) / 2) + a;
}

float twoPointAverage(float a, float b)
{
	return (a + b) / 2.0f;
}

float fourPointAverage(float a, float b, float c, float d)
{
	return (a + b + c + d) / 4.0f;
}


void MidpointDisplacement::clearHeightmap()
{
	for (int y = 0; y < sizeX; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			heightmap[y * x + y] = 0.0f;
		}
	}
}

void MidpointDisplacement::randomiseCorners()
{
	heightmap[0] = GetRandom(fHeight); //bottomLeft
	heightmap[sizeX - 1] = GetRandom(fHeight); //bottomRight
	heightmap[sizeX * (sizeX - 1)] = GetRandom(fHeight); //topLeft
	heightmap[sizeX * sizeX - 1] = GetRandom(fHeight); //topRight
}

int returnNLogValue(int n)
{
	if (n == 0)
		return false;

	return (ceil(log2(n)));
}

void MidpointDisplacement::midpointDisplacement()
{
	int i = 0;
	while (i < returnNLogValue(sizeX))
	{
		int numberOfQuads = pow(4, i);
		int quadsPerRow = sqrt(numberOfQuads);
		int quadLength = (sizeX - 1) / quadsPerRow;

		for (int y = 0; y < quadsPerRow; y++)
		{
			for (int x = 0; x < quadsPerRow; x++)
			{
				calculateMidpoints(quadLength * x, quadLength * (x + 1), quadLength * y, quadLength * (y + 1));
			}
		}
		fHeight *= fRoughness; //Lower the spread
		i++;
	}
}

void MidpointDisplacement::calculateMidpoints(int a, int b, int c, int d)
{
	int midpointX = calculateMidpoint(a, b);
	int midpointY = calculateMidpoint(c, d);
	//Square step
	float bottomEdge, topEdge, leftEdge, rightEdge; //Quad edges
	heightmap[midpointX * sizeX + c] = twoPointAverage(heightmap[a * sizeX + c], heightmap[b * sizeX + c]) + GetRandom(fHeight); //Bottom edge
	heightmap[midpointX * sizeX + d] = twoPointAverage(heightmap[a * sizeX + d], heightmap[b * sizeX + d]) + GetRandom(fHeight); //Top Edge
	heightmap[a * sizeX + midpointY] = twoPointAverage(heightmap[a * sizeX + c], heightmap[a * sizeX + d]) + GetRandom(fHeight); //Left Edge
	heightmap[b * sizeX + midpointY] = twoPointAverage(heightmap[b * sizeX + c], heightmap[b * sizeX + d]) + GetRandom(fHeight); //Right Edge
	bottomEdge = heightmap[midpointX * sizeX + c];
	topEdge = heightmap[midpointX * sizeX + d];
	leftEdge = heightmap[a * sizeX + midpointY];
	rightEdge = heightmap[b * sizeX + midpointY];
	//Diamond step
	heightmap[midpointX * sizeX + midpointY] = fourPointAverage(bottomEdge, topEdge, leftEdge, rightEdge) + GetRandom(fHeight);
}