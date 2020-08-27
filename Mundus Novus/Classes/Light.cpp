#include "Light.h"
#include "Shader.h"

Light::Light(LightType type, Shader* shader) {
	this->type = type;
	this->shader = shader;
}

void Light::setDirection(float PosX, float PosY, float PosZ) {
	if (this->type == LIGHT_DIRECTIONAL) {
		shader->setVec3("dirLight.direction", PosX, PosY, PosZ);
	}
}

void Light::setAmbient(float PosX, float PosY, float PosZ) {
	if (this->type == LIGHT_DIRECTIONAL) {
		shader->setVec3("dirLight.ambient", PosX, PosY, PosZ);
	}
}
void Light::setDiffuse(float PosX, float PosY, float PosZ) {
	if (this->type == LIGHT_DIRECTIONAL) {
		shader->setVec3("dirLight.diffuse", PosX, PosY, PosZ);
	}
}
void Light::setSpecular(float PosX, float PosY, float PosZ) {
	if (this->type == LIGHT_DIRECTIONAL) {
		shader->setVec3("dirLight.specular", PosX, PosY, PosZ);
	}
}
