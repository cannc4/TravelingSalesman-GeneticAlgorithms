#ifndef GeneticAlgorithm_H
#define GeneticAlgorithm_H

#include <vector>
#include "GeneticChild.h"
#include "CityManager.h"
#include "FileManager.h"


using namespace std;

class GeneticAlgorithm{

public:
	GeneticAlgorithm();

	void GenerateChildren(int count);  //generates random children
	void ApplyMutation(GeneticChild& c);
	void ApplyCrossover(GeneticChild& c1, GeneticChild& c2);
	void SetFileManager(FileManager& fm);
	void SetCityManager(CityManager& fm);
	void execute(); //run the generation
	void quickSort(vector<GeneticChild>& arr, int left, int right);
	bool existsCityName(vector<string>& vec, string city); //city name check
	int TournamentSelection(vector<GeneticChild>& childVec); //selection mechanism for genetic algorithm
	
	void setTournamentSize(int i);
	void setPopSize(int i);
	void setMutUpperRoll(int i);
	void setElitismSize(int i);


private:
	FileManager fileMan;
	CityManager cityMan;
	vector<GeneticChild> children;
	int tournamentSize; //5
	int popSize; //50
	int mutUpperRoll; //9
	int elitismSize; //1
};

#endif