#include "ACommand.h"
#include "Manager.h"

void ACommand::Do(HDC hdc)
{
	manager->Command(command);
}