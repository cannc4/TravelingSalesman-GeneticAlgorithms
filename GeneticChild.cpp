#include "GeneticChild.h"
#include <algorithm>
#include "randgen.h"

GeneticChild::GeneticChild()
{}

GeneticChild::GeneticChild(CityManager& cm){

	myRoute = cm.getCityNameList();

	int i1,i2;
	RandGen randgen;
	string temp;
	//shuffles the child.
	for(int i=0; i<myRoute.size(); i++)
	{
		i1 = randgen.RandInt(0,myRoute.size()-1);
		i2 = randgen.RandInt(0,myRoute.size()-1);
		temp = myRoute[i1];
		myRoute[i1] = myRoute[i2];
		myRoute[i2] = temp;
	}

	CalcScore(cm);
}

void GeneticChild::CalcScore(CityManager& cm)
{
	/*
	Score mechanics
	Score = distance. So lesser score = better result!
	*/
	myScore = 0;
	for(int i = 0; i<myRoute.size()-1; i++)
	{
		myScore += cm.GetDistance(myRoute[i],myRoute[i+1]);
	}
}

vector<string>& GeneticChild::GetRoute() 
{
	return myRoute;
}

int GeneticChild::getScore() const
{
	return myScore;
}

void GeneticChild::setRoute(vector<string>& vec)
{
	myRoute = vec;
}