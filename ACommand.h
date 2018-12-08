#pragma once
#include "Animation.h"

class Manager;
class ACommand :public Animation
{
private:
	std::string command;
	Manager *manager;
public:
	ACommand(std::string command,Manager *manager) :command(command),manager(manager){};
	void Do(HDC hdc)override;
};

