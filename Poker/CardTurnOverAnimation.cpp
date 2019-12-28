#include "CardTurnOverAnimation.h"

#include "ValueAnimation.h"
#include "SettingAnimation.h"

namespace CardTurnOverAnimation
{
	std::vector<AbstractAnimation*> AddBackToFrontAnimation(Card& card)
	{
		std::vector<AbstractAnimation*> ret;
		//显示背面
		card.SetShow(false);

		//背面翻到不显示
		ret.push_back(new ValueAnimation<TImage, double>(&card.GetBackImage(), 25, &TImage::SetIWidth, 1.0, 0.0));

		//动画：显示牌正面
		ret.push_back(new SettingAnimation<Card, bool>(&card, 0, &Card::SetShow, true));

		//正面翻出来
		ret.push_back(new ValueAnimation<TImage, double>(&card.GetImage(), 25, &TImage::SetIWidth, 0.0, 1.0));

		//正面已经显示，可以恢复背面的iWidth
		ret.push_back(new SettingAnimation<TImage, double>(&card.GetBackImage(), 0, &TImage::SetIWidth, 1.0));

		return ret;
	}

	std::vector<AbstractAnimation*> AddFrontToBackAnimation(Card& card)
	{
		std::vector<AbstractAnimation*> ret;
		//显示正面
		card.SetShow(true);

		//正面翻到不显示
		ret.push_back(new ValueAnimation<TImage, double>(&card.GetImage(), 25, &TImage::SetIWidth, 1.0, 0.0));

		//动画：显示牌背面
		ret.push_back(new SettingAnimation<Card, bool>(&card, 0, &Card::SetShow, false));

		//背面翻出来
		ret.push_back(new ValueAnimation<TImage, double>(&card.GetBackImage(), 25, &TImage::SetIWidth, 0.0, 1.0));

		//背面已经显示，可以恢复正面的iWidth
		ret.push_back(new SettingAnimation<TImage, double>(&card.GetImage(), 0, &TImage::SetIWidth, 1.0));

		return ret;

	}
}