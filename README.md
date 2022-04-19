# 可靠蜘蛛纸牌 Credible-Spider

## 视频 Video

Bilibili: [用C++验证蜘蛛纸牌1、2、4花色有多难（编程自动打蜘蛛纸牌）](https://www.bilibili.com/video/BV19g4y1i7W6/)

## 简介 Introduce

保证每一把都是可解问题的蜘蛛纸牌。
The spider solitaire game that each game is certain to be solved.

本项目的目的是：
The purpose of this project is in below:

1. 完成蜘蛛纸牌内核，实现生成牌局、发牌、移牌、胜利、撤销等操作。（已完成）
   Realize the core of spider solitaire, include the dealing, moving, win-judging, redoing operation, etc. (finished)

2. 完成蜘蛛纸牌的自动翻牌，使用博弈树及估值函数实现。自动翻牌是为了证明当前牌局一定可解出/大概率无法解出。（已完成）
   Realize the auto-play function, by using the game tree and the valuation function. Auto-play is for proving the current game can be solved or almost not. (finished)

3. 实现仿 Windows 的蜘蛛纸牌，增加“可靠牌局”，“自动翻牌”功能。（已完成）
   Realize a like-Windows spider solitaire, added the "Credible Game", "Auto-Play" function. (finished)

## 可靠牌局 Credible game
多线程寻找可解牌局 Find one game that can be solved by using multi-thread
![](https://github.com/tomwillow/Credible-Spider/blob/master/snap/credible%20game.PNG)
![](https://github.com/tomwillow/Credible-Spider/blob/master/snap/found%20credible%20game.PNG)

## 自动翻牌 Auto-play
自动翻牌功能目前能解出90%以上的1花色牌局，20%左右的2花色牌局，4花色不能解出。在解决方案内另有一个控制台版蜘蛛纸牌，我在控制台版本中使用项目方法求解4花色蜘蛛纸牌，最大尝试步骤设置为100,000，单把牌局大约耗时10min，仍然不能解出。
Currently Auto-play function can solve more than 90% game of 1 suit number, and about 20% game of 2 suit number, can not solve 4 suit-number game. Here is a console-version spider solitaire game, I have used this method to solve 4 suit-number game, in the case of that maximum trying-step-number set be 100,000 and one game spend 10min, the 4 suit-number game is still not solved.
![](https://github.com/tomwillow/Credible-Spider/blob/master/snap/auto-play.PNG)

## 胜利效果 Win effect
![](https://github.com/tomwillow/Credible-Spider/blob/master/snap/firework.PNG)
