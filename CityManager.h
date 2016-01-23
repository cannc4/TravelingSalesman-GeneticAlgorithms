#ifndef CityManager_H
#define CityManager_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class CityManager{

public:
	CityManager();

	void SetDistance(string city1, string city2, int distance); //sets distance between 2 cities
	int GetDistance(string city1, string city2); //returns distance between 2 cities
	vector<string> getCityNameList() const; //city names
	void setCityNameList(vector<string>& vec); 

private:
	
	map<string, map<string,int>> distanceMap;
	vector<string> cityNameList;

};

#endif