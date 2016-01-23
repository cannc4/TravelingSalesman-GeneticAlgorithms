#ifndef GeneticChild_H
#define GeneticChild_H

#include <vector>
#include "CityManager.h"


using namespace std;

class GeneticChild{

public:
	GeneticChild(); //default constructor, almost never used
	GeneticChild(CityManager& cm); //creates a random child
	vector<string>& GetRoute(); //returns route
	void CalcScore(CityManager& cm); //score calculation, lower score=better
	int getScore() const;
	void setRoute(vector<string>& vec); //override the route

private:
	int myScore;
	vector<string> myRoute;

};

#endif