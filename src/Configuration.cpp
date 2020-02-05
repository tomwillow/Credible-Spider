#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <sstream>
#include <ctime>

#include "Configuration.h"

using namespace std;

std::string time_tToString(time_t t)
{
	tm* timer = localtime(&t);
	char temp[128];
	strftime(temp, 128, "%Y年%m月%d日 %T", timer);
	return string(temp);
}

std::vector<std::string> Configuration::Record::ToVecString()
{
	std::vector<std::string> ret;
	ret.push_back(time_tToString(this->time));
	ret.push_back(to_string(seed));
	ret.push_back((diff == 0) ? "未评估" : (prefixDiff ? prefixDiff + to_string(diff) : to_string(diff)));
	ret.push_back(to_string(highScore));
	ret.push_back(solved ? "已解决" : "未解决");
	return ret;
}

void Configuration::Record::SaveToFile(FILE* fp)
{
	fwrite(&time, sizeof(time), 1, fp);
	fwrite(&seed, sizeof(seed), 1, fp);
	fwrite(&highScore, sizeof(highScore), 1, fp);
	fwrite(&prefixDiff, sizeof(prefixDiff), 1, fp);
	fwrite(&diff, sizeof(diff), 1, fp);
	fwrite(&solved, sizeof(solved), 1, fp);
}

void Configuration::Record::ReadFromFile(FILE* fp)
{
	fread(&time, sizeof(time), 1, fp);
	fread(&seed, sizeof(seed), 1, fp);
	fread(&highScore, sizeof(highScore), 1, fp);
	fread(&prefixDiff, sizeof(prefixDiff), 1, fp);
	fread(&diff, sizeof(diff), 1, fp);
	fread(&solved, sizeof(solved), 1, fp);
}

bool Configuration::ReadFromFile(std::string fileName)
{
	FILE* fp = fopen(fileName.c_str(), "rb");
	if (fp)
	{
		fread(&enableAnimation, 1, 1, fp);
		fread(&enableSound, 1, 1, fp);

		ReadRecord(fp);

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

		SaveRecord(fp);

		fclose(fp);
		return true;
	}
	return false;
}

void Configuration::ReadRecord(FILE* fp)
{

	for (int i = 0; i < 3; ++i)
	{
		int sz;
		fread(&sz, sizeof(sz), 1, fp);

		for (int j = 0; j < sz; ++j)
		{
			shared_ptr<Record> temp = make_shared<Record>();
			temp->ReadFromFile(fp);
			record[i].push_back(temp);
			seedMap[i][temp->seed] = temp;
		}
	}
}

void Configuration::SaveRecord(FILE* fp)
{

	for (int i = 0; i < 3; ++i)
	{
		int sz = record[i].size();
		fwrite(&sz, sizeof(sz), 1, fp);

		for (int j = 0; j < sz; ++j)
		{
			record[i][j]->SaveToFile(fp);
		}
	}
}

void Configuration::UpdateRecord(int suitNum, unsigned int seed, int highScore, bool solved, int calc,bool hasPrefix)
{
	int index = -1;
	switch (suitNum)
	{
	case 1:index = 0; break;
	case 2:index = 1; break;
	case 4:index = 2; break;
	}

	auto it = seedMap[index].find(seed);
	if (it == seedMap[index].end())
	{
		record[index].push_back(std::make_shared<Record>(time(0), seed, highScore, 0, calc, false));
		seedMap[index][seed] = record[index].back();
	}
	else
	{
		if (highScore > it->second->highScore)
		{
			it->second->time = time(0);
			it->second->highScore = highScore;
		}
		if (calc)
		{
			//之前已经有评估值，未解决，新评估值小于原评估值
			if (it->second->diff && solved == false && calc <= it->second->diff)
				;
			else
			{
				//更新评估值
				//it->second->time = time(0);
				it->second->diff = calc;
				it->second->prefixDiff = hasPrefix ? '>' : 0;
			}
		}
		if (solved)
		{
			it->second->time = time(0);
			it->second->solved = solved;
		}
	}
}

void Configuration::ClearRecord()
{
	for (int i = 0; i < 3; ++i)
	{
		record[i].clear();
		seedMap[i].clear();
	}
}