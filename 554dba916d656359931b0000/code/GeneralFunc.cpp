#include "GeneralFunc.h"

int transformOrder(int ord)
{
	if (ord % 3 == 2)
	{
		return -1;
	}
	else
	{
		return ord % 3;
	}
}
