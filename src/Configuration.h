#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

class Configuration
{
private:
public:
	struct Record
	{
		time_t time;
		unsigned int seed;
		int highScore;
		char prefixDiff;
		int diff;
		bool solved;
		Record() = default;
		Record(time_t t,unsigned int s,int hs,char pre,int diff,bool sv):time(t),seed(s),highScore(hs),prefixDiff(pre),diff(diff),solved(sv)
		{}

		std::vector<std::string> ToVecString();

		void SaveToFile(FILE* fp);

		void ReadFromFile(FILE* fp);
	};
	bool enableAnimation;
	bool enableSound;
	std::vector<std::shared_ptr<Record>> record1,record2,record4,*record;
	std::unordered_map<unsigned int, std::shared_ptr<Record>> seedMap1,seedMap2,seedMap4,*seedMap;

	bool ReadFromFile(std::string fileName);
	bool SaveToFile(std::string fileName);

	void UpdateRecord(int suitNum,unsigned int seed, int highScore,bool solved=false,int calc=0);
	void ReadRecord(FILE* fp);
	void SaveRecord(FILE* fp);
};

std::string time_tToString(time_t t);