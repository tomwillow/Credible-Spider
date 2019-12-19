#include "Poker.h"

#include <iostream>
#include <algorithm>

using namespace std;


void Poker::printCard(const vector<Card> &cards) const
{
	for (auto &card : cards)
	{
		cout << card << " ";
	}
}

void Poker::printCard(const vector<vector<Card>> &vvcards) const
{
	for (auto &vcards : vvcards)
	{
		printCard(vcards);
		cout << endl;
	}
}

std::ostream& operator<<(std::ostream& out, const Poker& poker)
{
	out << "score:" << poker.score << " operation:" << poker.operation << endl;

	for (size_t i = 0; i < poker.desk.size(); ++i)
	{
		out << "desk " << i << ":";
		poker.printCard(poker.desk[i]);
		out << endl;
	}
	out << endl;

	for (size_t i = 0; i < poker.corner.size(); ++i)
	{
		out << "corner " << i << ":";
		poker.printCard(poker.corner[i]);
		out << endl;
	}
	out << endl;

	for (size_t i = 0; i < poker.finished.size(); ++i)
	{
		out << "finished " << i << ":";
		poker.printCard(poker.finished[i]);
		out << endl;
	}

	out << "value:" << poker.GetValue() << endl;
	out << "hash:" << hash<Poker>{}(poker) << endl;
	return out;
}

void Poker::printCard(int deskIndex, int num) const
{
	for (auto it=desk[deskIndex].end()-num;it!=desk[deskIndex].end();++it)
	{
		cout << *it<<" ";
	}
	cout << endl;
}

bool Poker::isFinished()
{
	return finished.size() == 8;
}

int Poker::GetValue() const
{
	int value = finished.size()*200;

	//
	for (auto& cards : desk)
	{
		if (cards.empty())
			value += 0;
		else
		{
			//一摞牌且非空

			auto AddValue = [&value](int num,int topPoint)
			{
				if (num)
					value += topPoint * num;
			};

			int num = 0;
			const Card* pTop = &cards.back();
			const Card* pDown = &cards.back();

			//从下数第2张到顶部
			for (int i = cards.size() - 2; i >= 0; --i)
			{
				//已经不显示了则跳出
				if (cards[i].show == false)
					break;

				pTop = &cards[i];
				pDown = &cards[i + 1];

				//点数相差1
				if (pTop->point == pDown->point + 1)
				{
					if (pTop->suit == pDown->suit)
					{
						num++;
					}
					else
					{
						AddValue(num, pDown->point);
						AddValue(1, pTop->point / 2);
						num = 0;
					}
				}
				else
				{
					//储存之前的序列
					AddValue(num, pDown->point);
					num = 0;

					//一个乱序组
					//eg. 7 1 -> -7
					//eg. 1 7 -> -14
					//eg. 5 5 -> -10
					int dv = -max(pTop->point, pDown->point);
					if (pTop->point < pDown->point)
						dv *= 2;
					AddValue(1, dv);
				}
			}

			AddValue(num, pTop->point);

		}

		//hide card
		int num = 10;
		for (auto& card : cards)
		{
			if (card.show == false)
			{
				value -= num;
				num--;
			}
			else
				break;
		}
	}

	return value;
}


bool operator==(const Poker& lhs, const Poker& rhs)
{
	if (lhs.desk != rhs.desk) return false;
	if (lhs.corner != rhs.corner) return false;
	if (lhs.finished != rhs.finished) return false;
	return true;
}

size_t std::hash<Poker>::operator()(const Poker& poker) const
{
	size_t ret = 0;
	std::ostringstream oss;
	auto vvv = { &poker.desk,&poker.corner,&poker.finished };

	for (auto pvv : vvv)
		for (auto& cards : *pvv)
		{
			for (auto& card : cards)
				oss << card;
			ret ^= std::hash<std::string>{}(oss.str());
			oss.clear();
		}
	return ret;
}