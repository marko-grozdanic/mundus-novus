#pragma once
#include "Shader.h"

enum LightType
{
	LIGHT_DIRECTIONAL,
};

class Light {
public:
	LightType type;
	Shader* shader;

	Light(LightType type, Shader* shader);
	//general
	void setDirection(float PosX, float PosY, float PosZ);
	void setAmbient(float PosX, float PosY, float PosZ);
	void setDiffuse(float PosX, float PosY, float PosZ);
	void setSpecular(float PosX, float PosY, float PosZ);
};

