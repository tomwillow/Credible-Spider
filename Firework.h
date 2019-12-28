#pragma once

#include <Windows.h>
#include <chrono>
#include <random>
#include <vector>

#include "resource.h"

struct HSB
{
	float h, s, b;
};
HSB operator+(const HSB& l, const HSB& r);
HSB operator-(const HSB& l, const HSB& r);
HSB operator*(const HSB& l, float f);
COLORREF HSB2RGB(HSB hsb);

class Firework
{
private:
	//e为静态函数，避免调用时时间一样产生一样的随机数
	static std::default_random_engine e;

	//整体死亡：火箭、烟花均已显示完
	bool dead;

	//true:显示粒子动画
	//false:显示火箭动画
	bool boom;

	//火箭参数
	float W, H;//屏幕宽高
	float mr = 5;//火箭半径
	float r = 50;//火箭顶点距屏幕距离
	float x0,y0;//发射起始点
	float vx0,vy0;//初速度
	float g = -40;//重力加速度
	HSB rocketHSB;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;//火箭发射时刻

	//粒子参数
	struct Ball
	{
		float vx0, vy0;//初速度
		float x0, y0;//初始点
		float life;//寿命 秒
		float rb;//最大半径
		HSB startHSB, endHSB;//终末颜色
		
	};
	std::vector<Ball> balls;
	std::chrono::time_point<std::chrono::high_resolution_clock> boomTime;//粒子爆炸时刻
public:
	Firework(float width, float height) :W(width), H(height), boom(false), dead(false)
	{
		using namespace std::chrono;
		startTime = high_resolution_clock::now();
		e.seed(e() + GetTickCount64());

		//发射起点
		x0 = randRange(r, W - r);
		y0 = 0;

		//x方向初速度
		vx0 = randRange(-50.0, 50.0);

		if (vx0<2 * r || vx0>W - 2 * r)
			vx0 = -vx0;

		//使y方向终点位于 [H/2,H-r] 需要的初速度范围
		float vy0min = sqrt(2 * g * y0 - g * H);
		float vy0max = sqrt(2 * g * y0 - 2 * g * (H - r));

		float ymin = (2 * g * y0 - vy0min * vy0min) / (2.0 * g);
		float ymax = (2 * g * y0 - vy0max * vy0max) / (2.0 * g);

		//在范围内产生y初速度
		vy0 = randRange(vy0min, vy0max);

		float h = randRange(0, 360);
		float s = 1.0;
		float b = randRange(0.7, 1.0);//b越大饱和度越高
		rocketHSB = { h,s,b };
		//PlaySound((LPCSTR)IDR_WAVE_LAUNCH, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	}

	float randRange(float a, float b)
	{
		std::uniform_real_distribution<> dis(a, b);
		return dis(e);
	}

	bool IsDead()
	{
		return dead;
	}



	void Draw(HDC hdc)
	{
		using namespace std;
		using namespace std::chrono;

		auto now = high_resolution_clock::now();
		if (boom == false)
		{
			//从发射起算时间
			float t = duration_cast<milliseconds>(now - startTime).count() / 1000.0;

			//火箭y方向速度
			float vy = vy0 + g * t;

			//火箭位置
			float x = x0 + vx0 * t;
			float y = y0 + vy0 * t + 1.0 / 2.0 * g * t * t;

			//画火箭
			HBRUSH hBrush = CreateSolidBrush(HSB2RGB(rocketHSB));
			SelectObject(hdc, hBrush);

			HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
			SelectObject(hdc, hPen);

			int sy = H - y;
			Ellipse(hdc, x - mr, sy - mr, x + mr, sy + mr);

			DeleteObject(hPen);
			DeleteObject(hBrush);

			//火箭已经到达顶点
			if (vy <= 0)
			{
				//爆炸音效
				PlaySound((LPCSTR)IDR_WAVE_FIREWORK, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

				//设置开始爆炸
				boom = true;

				//记录爆炸起算时间
				boomTime = now;
				balls.clear();
				
				//粒子数量
				int i = randRange(40,80);
				while (i--)
				{
					Ball ball;
					ball.rb = randRange(3,5);
					ball.life = randRange(0.1,4);

					float v0 = randRange(20, 80);
					float pi = 3.1415926;
					float angle = randRange(-pi,pi);//初速度角度在360度范围内
					ball.vx0 = v0 * cos(angle);
					ball.vy0 = v0 * sin(angle);
					ball.x0 = x;
					ball.y0 = y;

					ball.startHSB = rocketHSB;

					//最终色相左右偏移各45度
					//s越小越白，所以粒子亮度会提高
					//b越大饱和度越高
					ball.endHSB = { rocketHSB.h+randRange(-45,45),rocketHSB.s+randRange(-0.7,-0.2),rocketHSB.b };
					balls.push_back(ball);
				}
			}
		}
		else
		{
			//绘制粒子

			bool allIsDead = true;
			float t = duration_cast<milliseconds>(now - boomTime).count() / 1000.0;
			for (auto& ball : balls)
			{
				if (t >= ball.life)
					continue;

				//任一粒子没有死亡都不能算死亡
				allIsDead = false;

				//rbnow为t的二次函数，顶点为rb
				float a = -4.0 * ball.rb / (ball.life * ball.life);
				float b = 4.0 * ball.rb / ball.life;
				float rbnow = a * t * t + b * t;

				//x方向匀速直线
				float xb = ball.x0 + ball.vx0 * t;

				//y方向受到重力
				float yb = ball.y0 + ball.vy0 * t + 1.0 / 2.0 * g * t * t;

				//计算当前颜色
				HSB hsb = (ball.endHSB - ball.startHSB) * (t / ball.life)+ball.startHSB;

				//绘制粒子
				HBRUSH hBrush = CreateSolidBrush(HSB2RGB(hsb));
				SelectObject(hdc, hBrush); 

				HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
				SelectObject(hdc, hPen);

				int drawY = H - yb;
				Ellipse(hdc, xb - rbnow, drawY - rbnow, xb + rbnow, drawY + rbnow);

				DeleteObject(hPen);
				DeleteObject(hBrush);
			}

			//设置死亡标记
			if (allIsDead)
				dead = true;
		}
	}
};

