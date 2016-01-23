#include "City.h"
#include <stdlib.h>

City::City(){
	this->xPos = rand() %10 + 1;
	this->yPos = rand() %10 + 1;
}

City::City(int x, int y){
	this->xPos = x;
	this->yPos = y;
}