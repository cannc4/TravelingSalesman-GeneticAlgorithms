#include "CityManager.h"

CityManager::CityManager()
{

}

void CityManager::SetDistance(string s1, string s2, int dist)
{
	distanceMap[s1][s2] = dist;
}

int CityManager::GetDistance(string s1, string s2)
{
	return distanceMap[s1][s2];
}

vector<string> CityManager::getCityNameList() const
{
	return cityNameList;
}

void CityManager::setCityNameList(vector<string>& list)
{
	cityNameList = list;
}

