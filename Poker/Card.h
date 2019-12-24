#pragma once

#include <string>
#include <iostream>
#include <memory>
#include "TImage.h"

class Card
{
private:
	//
	int z_index;
	bool visible;
	std::shared_ptr<TImage> img, imgBack;
public:

	int suit;//花色 1 2 3 4

	int point;//点数 1-13

	bool show;//是否已翻开

	Card(int suit, int point, bool show) :
		suit(suit), point(point), show(show),
		visible(true),z_index(0),img(nullptr),imgBack(nullptr) {}
	Card(int suit, int point) :Card(suit,point,false) {}

	Card(const Card& card):suit(card.suit),point(card.point),show(card.show),z_index(card.z_index),visible(card.visible),img(card.img),imgBack(card.imgBack)
	{

	}

	~Card()
	{
	}

	void SetVisible(bool visible)
	{
		this->visible = visible;
	}

	void SetZIndex(int z)
	{
		z_index = z;
	}

	int GetZIndex() const
	{
		return z_index;
	}

	void SetShow(bool show)
	{
		this->show = show;
	}

	void SetPos(POINT pt)
	{
		img->pt = pt;
		imgBack->pt = pt;
	}

	POINT GetPos() const
	{
		return img->pt;
	}

	bool HasImage()
	{
		return img && imgBack;
	}

	void SetImage(std::shared_ptr<TImage> img, std::shared_ptr<TImage> imgBack)
	{
		this->img = img;
		this->imgBack = imgBack;
	}

	TImage& GetBackImage()
	{
		return *imgBack;
	}

	TImage& GetImage()
	{
		return *img;
	}

	void Draw(HDC hdc)
	{
		if (visible)
		if (show)
			img->Draw(hdc);
		else
			imgBack->Draw(hdc);
	}

	//返回花色 C D H S
	char getSuit() const
	{
		switch (suit)
		{
		case 1:return 'C';//梅花
		case 2:return 'D';//方块
		case 3:return 'H';//红桃
		case 4:return 'S';//黑桃
		default:
			throw std::string("Error:'getSuit():' Undefined suit");
		}
	}

	//打印牌
	//eg. [S1]
	friend std::ostream& operator<<(std::ostream& out, const Card& card);
};

bool operator==(const Card& lhs, const Card& rhs);

std::ostream& operator<<(std::ostream& out, const Card& card);