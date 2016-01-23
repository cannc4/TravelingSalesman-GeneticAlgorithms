#include "FileManager.h"
#include "GeneticAlgorithm.h"
#include "CityManager.h"
#include <iostream>

int main()
{
	FileManager fm;
	fm.setOutPrefix("out"); //name prefix of output files -> out1 out2 out3...
	fm.setInFile("hue.txt"); //input file
	fm.ReadFile();

	CityManager cm = fm.getCityManager(); //passing around objects

	int tournamentSize;
	int mutUpperRoll;
	int elitismSize;
	int populationSize;

	cout << "Enter Tournament Size(5): ";
	cin >> tournamentSize;
	cout << "\nEnter Mutation Upper Roll(9): ";
	cin >> mutUpperRoll;
	cout << "\n Enter Elitism Size(1): ";
	cin >> elitismSize;
	cout << "\n Enter Population Size(50): ";
	cin >> populationSize;

	GeneticAlgorithm gene;
	gene.setTournamentSize(tournamentSize);
	gene.setMutUpperRoll(mutUpperRoll);
	gene.setElitismSize(elitismSize);
	gene.SetFileManager(fm);
	gene.SetCityManager(cm);
	gene.GenerateChildren(populationSize); //population = 50
	
	
	while(true)
	{

	gene.execute();
	
	}

	


}