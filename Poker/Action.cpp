#include "Action.h"


std::ostream& operator<<(std::ostream& out, const Action& action)
{
	return out << action.GetCommand();
}