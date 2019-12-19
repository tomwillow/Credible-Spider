#include "Deal.h"
#include "Poker.h"

#include <algorithm>
#include <random>
#include <Windows.h>

using namespace std;

//返回1维数组，各花色依次1-13点，共8*13=104张
vector<Card> Deal::genInitCard() const
{
	vector<Card> result;
	switch (suitNum)
	{
	case 1:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ 4, j });//1个花色：黑桃
		break;
	case 2:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ (i>3) ? 3 : 4, j });//2个花色：红桃，黑桃
		break;
	case 4:
		for (int i = 0; i < 8; ++i)
			for (int j = 1; j <= 13; ++j)
				result.push_back({ i % 4 + 1, j });//4个花色
		break;
	default:
		throw string("Error:'genInitCard(" + to_string(suitNum) + ")");
	}
	return result;
}

bool Deal::Do(Poker* inpoker)
{
	poker = inpoker;

	//先清理
	poker->desk.clear();
	poker->corner.clear();
	poker->finished.clear();

	std::default_random_engine e;

	//生成整齐牌
	auto cards = genInitCard();

	//打乱
	e.seed(seed);
	random_shuffle(cards.begin(), cards.end(), [&](int i){return e() % i; });

	//发牌
	int pos = 0;

	//4摞6张的=24
	for (int i = 0; i < 4; ++i)
	{
		vector<Card> deskOne;
		for (int j = 0; j < 6; ++j)
			deskOne.push_back(cards[pos++]);
		poker->desk.push_back(deskOne);
	}

	//6摞5张的=30
	for (int i = 0; i < 6; ++i)
	{
		vector<Card> deskOne;
		for (int j = 0; j < 5; ++j)
			deskOne.push_back(cards[pos++]);
		poker->desk.push_back(deskOne);
	}

	//5摞 待发区=50
	for (int i = 0; i < 5; ++i)
	{
		vector<Card> cornerOne;
		for (int j = 0; j < 10; ++j)
			cornerOne.push_back(cards[pos++]);
		poker->corner.push_back(cornerOne);
	}

	//每摞最外的牌亮牌
	for (auto &deskOne : poker->desk)
		deskOne.back().show = true;

	poker->score = 500;
	poker->operation = 0;

	return true;
}

bool Deal::Redo(Poker* inpoker)
{
	poker = inpoker;

	poker->desk.clear();
	poker->corner.clear();
	poker->finished.clear();

	return true;
}