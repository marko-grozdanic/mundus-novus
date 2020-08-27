#pragma once
#include <string>

class Texture {
public: 
	unsigned int ID;
	std::string fileName;
	std::string type;

	Texture(const std::string name, const bool alphaChannel);
};