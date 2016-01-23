#include "GeneticAlgorithm.h"
#include <algorithm>
#include <iostream>
#include "randgen.h"
#include <ctime>


GeneticAlgorithm::GeneticAlgorithm()
{

}

void GeneticAlgorithm::GenerateChildren(int count){

	setPopSize(count);
	for(int i = 0; i<count; i++){
		GeneticChild c(cityMan);
		children.push_back(c);
	}
}

void GeneticAlgorithm::SetFileManager(FileManager& fm){
	fileMan = fm;
}

void GeneticAlgorithm::SetCityManager(CityManager& cm){
	cityMan = cm;
}

void GeneticAlgorithm::ApplyMutation(GeneticChild& c){
	/*
	Mutation mechanism
	To avoid duplication and missing cities
	Swap 2 locations on the same child.
	*/
	vector<string> vec = c.GetRoute();
	RandGen randgen;
	int loc1 = randgen.RandInt(0,vec.size()-1);
	int loc2 = randgen.RandInt(0,vec.size()-1);

	swap(vec[loc1], vec[loc2]);
	c.CalcScore(cityMan);
}

void GeneticAlgorithm::quickSort(vector<GeneticChild>& arr, int left, int right) {
	int i = left, j = right;
	GeneticChild tmp;
	GeneticChild pivot = arr[(left + right) / 2];

	/* partition */
	while (i <= j) {
		while (arr[i].getScore() < pivot.getScore())
			i++;
		while (arr[j].getScore() > pivot.getScore())
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};

	/* recursion */
	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}

int GeneticAlgorithm::TournamentSelection(vector<GeneticChild>& childVec)
{

	double totalScore=0;
	vector<double> scores(childVec.size());
	RandGen randgen;

	//little score = better score, so we 1/score to make little scores big for further calculation
	for(int i=0; i<childVec.size(); i++)
	{
		scores[i] = 1.0/childVec[i].getScore();
		totalScore += scores[i];
	}

	//setting up the scores vector
	for(int i=1; i<scores.size();i++)
	{
		scores[i] += scores[i-1];
	}
	
	//roll to see who won
	double roll = randgen.RandReal(0, totalScore);

	//find the winner
	for(int i=0; i<scores.size(); i++)
	{
		if(roll <= scores[i])
			return i;
	}

	//null should never happen, roll can't be greater or less then expected
	return NULL;
}


void GeneticAlgorithm::setTournamentSize(int i)
{
	tournamentSize = i;
}
void GeneticAlgorithm::setPopSize(int i)
{
	popSize = i;
}
void GeneticAlgorithm::setMutUpperRoll(int i)
{
	mutUpperRoll = i;
}
void GeneticAlgorithm::setElitismSize(int i)
{
	elitismSize = i;
}


void GeneticAlgorithm::execute()//run the population
{
	static int counter = 1;

	vector<GeneticChild> elitism(elitismSize);

	quickSort(children,0,children.size()-1);
	for(int i=0; i<elitism.size(); i++)
	{
		elitism[i] = children[i];
	}
	
	RandGen randgen;
	GeneticChild c1,c2;

	//copy the array because we have to breed twice
	vector<GeneticChild> save = children;
	children.clear();

	srand(time(0));
	random_shuffle(save.begin(),save.end());

	while(children.size() < popSize)
	{
		vector<GeneticChild> temp;
		RandGen randgen;
		for(int i=0;i<tournamentSize;i++)
		{
			temp.push_back(save[i]);
		}
		
		quickSort(temp,0,temp.size()-1);
		reverse(temp.begin(), temp.end());
		int loc1 = TournamentSelection(temp);
		int loc2 = TournamentSelection(temp);
		while(loc1 == loc2)
			loc2 = TournamentSelection(temp);

		c1 = save[loc1];
		c2 = save[loc2];

		ApplyCrossover(c1,c2);

		//mutation prob is %10
		int mutRand = randgen.RandInt(0,mutUpperRoll);
		if(mutRand == 0)
			ApplyMutation(c1);

		mutRand = randgen.RandInt(0,mutUpperRoll);
		if(mutRand == 0)
			ApplyMutation(c2);
		
		//save those children
		children.push_back(c1);
		children.push_back(c2);

		//no need to empty the vector if we're done
		if(save.size()>tournamentSize)
		{
			for(int i=0;i<tournamentSize;i++)
			{
				save[i] = save[save.size()-1];
				save.pop_back();
			}
		}
	}



	quickSort(children,0,children.size()-1);
	//apply elitism on the worst child
	for(int i=0; i<elitism.size(); i++)
	{
		children[children.size()-i-1] = elitism[i];
	}



	//find best and print
	int bestScore = INT_MAX;
	GeneticChild c;

	for(int i=0; i<children.size(); i++)
	{
		if(children[i].getScore() < bestScore)
		{
			bestScore = children[i].getScore();
			c = children[i];
		}
	}

	
	fileMan.PrintChild(c);
	counter++;





	/*  //for cout
	//find best and print
	int bestScore = INT_MAX;
	vector<string> bestRoute;

	for(int i=0; i<children.size(); i++)
	{
		if(children[i].getScore() < bestScore)
		{
			bestScore = children[i].getScore();
			bestRoute = children[i].GetRoute();
		}
	}

	
	cout << "Generation " << counter << ": Score " << bestScore << "\n";
	counter++;
	for(int i = 0; i<bestRoute.size(); i++)
	{
		cout << bestRoute[i] << " ";
	}
	cout << "\n\n";
	*/
}
void GeneticAlgorithm::ApplyCrossover(GeneticChild& c1, GeneticChild& c2)
{
	/*
	Crossover Mechanics
	1 - Find sites
	2 - Copy them elsewhere(site1, site2)
	3 - Fill the remaining of site1 and site2 based on the order of the children genes.
	why? -> avoid duplication/missing cities
	*/


	vector<string> firstRoute = c1.GetRoute();
	vector<string> secondRoute = c2.GetRoute();
	vector<int> vecBoolSecond(firstRoute.size(), 0);
	vector<int> vecBoolFirst(firstRoute.size(), 0);
	vector<string> finalRouteSecond(firstRoute.size());
	vector<string> finalRouteFirst(firstRoute.size());
	RandGen randgen;

	//find crossover sites
	int loc1 = randgen.RandInt(0,firstRoute.size()-1);
	int loc2 = randgen.RandInt(0,firstRoute.size()-1);
	while(loc1>loc2)
	{
		loc1 = randgen.RandInt(0,firstRoute.size()-1);
		loc2 = randgen.RandInt(0,firstRoute.size()-1);
	}

	//save the children as legacy
	vector<string> legacySecondRouteCrossSite(loc2-loc1+1);
	for(int i = loc1, j=0; i<=loc2; i++, j++)
	{
		legacySecondRouteCrossSite[j] = secondRoute[i];
	}

	//bool array to see which sites are not crossover sites
	for(;loc1<=loc2; loc1++)
	{
		finalRouteSecond[loc1] = firstRoute[loc1];
		vecBoolSecond[loc1] = 1;
		finalRouteFirst[loc1] = secondRoute[loc1];
		vecBoolFirst[loc1] = 1;
	}
	
	//crossover for the second child
	for(int j=0,i=0; i<secondRoute.size() && j<secondRoute.size();)
		{
			if(vecBoolSecond[j] == 0)
			{
				if(!existsCityName(finalRouteSecond,secondRoute[i])) //to avoid duplication
				{
					finalRouteSecond[j] = secondRoute[i];
					j++;
					i++;
				}
				else
				{
					i++;
					continue;
				}

			}
			else
			{
				j++;
			}
		}

	c2.setRoute(finalRouteSecond);
	c2.CalcScore(cityMan);

	//same of the above but for first children
	for(int j=0,i=0;  i<secondRoute.size() && j<secondRoute.size();)
	{
		if(vecBoolFirst[j] == 0)
		{
			if(!existsCityName(finalRouteFirst,firstRoute[i]))
			{
				finalRouteFirst[j] = firstRoute[i];
				j++;
				i++;
			}
			else
			{
				i++;
				continue;
			}

		}
		else
		{
			j++;
		}
	}

	c1.setRoute(finalRouteFirst);
	c1.CalcScore(cityMan);
	
}

bool GeneticAlgorithm::existsCityName(vector<string>& vec, string city){ //checks if city name exists
	for(int i = 0; i<vec.size(); i++)
	{
		if(vec[i] == city)
			return true;
	}
	return false;
}


/* random //old code, dont mind.
void GeneticAlgorithm::execute()
{

	vector<int> crVec(children.size());
	for(int i = 0; i<crVec.size();i++)
		crVec[i] = i;

	vector<GeneticChild> elitism(1);

	quickSort(children,0,children.size()-1);
	for(int i=0; i<elitism.size(); i++)
	{
		elitism[i] = children[i];
	}

	RandGen randgen;
	int loc1,loc2;
	while(crVec.size() > 0)
	{
		loc1 = randgen.RandInt(0,crVec.size()-1);
		loc2 = randgen.RandInt(0,crVec.size()-1);
		while(loc2 == loc1)
			loc2 = randgen.RandInt(0,crVec.size()-1);

		ApplyCrossover(children[loc1],children[loc2]);

		int mutRand = randgen.RandInt(0,9);
		if(mutRand == 0)
			ApplyMutation(children[loc1]);

		mutRand = randgen.RandInt(0,9);
		if(mutRand == 0)
			ApplyMutation(children[loc2]);

		if(loc1 > loc2)
		{
			crVec.erase(crVec.begin()+loc1);
			crVec.erase(crVec.begin()+loc2);
		}
		else
		{
			crVec.erase(crVec.begin()+loc2);
			crVec.erase(crVec.begin()+loc1);
		}
	}



	quickSort(children,0,children.size()-1);
	for(int i=0; i<elitism.size(); i++)
	{
		children[children.size()-i-1] = elitism[i];
	}


	quickSort(children,0,children.size()-1);
	int bestScore = children[0].getScore();
	vector<string> bestRoute = children[0].GetRoute();

	cout << bestScore << "\n";
	for(int i = 0; i<bestRoute.size(); i++)
	{
		cout << bestRoute[i] << " ";
	}
	cout << "\n\n";

}
*/


/* //old code again
void GeneticAlgorithm::execute()
{
	vector<GeneticChild> newChildren;

	int popSize = 50;

	vector<GeneticChild> elitism(1);

	quickSort(children,0,children.size()-1);
	for(int i=0; i<elitism.size(); i++)
	{
		elitism[i] = children[i];
	}
	
	newChildren.push_back(elitism[0]);
	RandGen randgen;
	GeneticChild c1,c2;

	while(children.size() > 1)
	{
		c1 = TournamentSelection();
		c2 = TournamentSelection();

		ApplyCrossover(c1,c2);

		int mutRand = randgen.RandInt(0,9);
		if(mutRand == 0)
			ApplyMutation(c1);

		mutRand = randgen.RandInt(0,9);
		if(mutRand == 0)
			ApplyMutation(c2);
		
		newChildren.push_back(c1);
		newChildren.push_back(c2);
	}



	quickSort(children,0,children.size()-1);
	for(int i=0; i<elitism.size(); i++)
	{
		children[children.size()-i-1] = elitism[i];
	}

	int bestScore = INT_MAX;
	vector<string> bestRoute;
	for(int i=0; i<children.size(); i++)
	{
		if(children[i].getScore() < bestScore)
		{
			bestScore = children[i].getScore();
			bestRoute = children[i].GetRoute();
		}
	}

	cout << bestScore << "\n";
	for(int i = 0; i<bestRoute.size(); i++)
	{
		cout << bestRoute[i] << " ";
	}
	cout << "\n\n";
	children = newChildren;
}
*/