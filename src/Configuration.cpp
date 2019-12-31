#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "Configuration.h"

bool Configuration::LoadFromFile(std::string fileName)
{
	FILE *fp = fopen(fileName.c_str(), "rb");
	if (fp)
	{
		fread(&enableAnimation, 1, 1, fp);
		fread(&enableSound, 1, 1, fp);

		fclose(fp);
		return true;
	}
	return false;
}

bool Configuration::SaveToFile(std::string fileName)
{
	FILE* fp = fopen(fileName.c_str(), "wb");
	if (fp)
	{
		fwrite(&enableAnimation, 1, 1, fp);
		fwrite(&enableSound, 1, 1, fp);

		fclose(fp);
		return true;
	}
	return false;
}