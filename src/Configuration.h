#pragma once

#include <string>

class Configuration
{
public:
	bool enableAnimation;
	bool enableSound;

	bool LoadFromFile(std::string fileName);
	bool SaveToFile(std::string fileName);
};

