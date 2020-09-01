#pragma once
#include <vector>
class MidpointDisplacement {
public:
	std::vector<float> heightmap;

	MidpointDisplacement(unsigned int seed, float spread, float spreadReductionRate, int sizeX, int n);
	float getMinHeight();
private: 
	int sizeX;
	int n;
	int seed;
	float fHeight;
	float fRoughness;

	//Heightmap manipulation
	void clearHeightmap();
	void randomiseCorners();
	void midpointDisplacement();
	void normaliseHeightmap();
	void calculateMidpoints(int a, int b, int c, int d);	
};