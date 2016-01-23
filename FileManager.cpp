#include "FileManager.h"
#include "CityManager.h"
#include <iostream>
#include <string>
#include <sstream>

FileManager::FileManager()
{

}

CityManager FileManager::getCityManager() const
{
	return cm;
}

void FileManager::ReadFile()
{
	vector<string> cityList;
	ifstream reader;
	reader.open(infile);
	if(reader.fail())
	{
		cout << "File can't be found\n";
	}

	string word,line;
	int num;
	while(reader >> word)
	{
		if(word == "CITYEND")
			break;

		cityList.push_back(word);
	}

	cm.setCityNameList(cityList);

	for(int i = 0; getline(reader, line); i++)
	{
		istringstream iss(line);
		
		for(int j=0; iss>>num; j++)
		{
			cm.SetDistance(cityList[i], cityList[j], num);
		}
	}
}

void FileManager::setInFile(string s)
{
	infile = s;
}

void FileManager::setOutPrefix(string s)
{
	outPrefix = s;
}

void FileManager::PrintChild(GeneticChild& c)
{
	static int i = 1;
	ofstream writer;
	writer.open(outPrefix+to_string(i));
	
	writer << "Generation "<< i << ": ";
	writer << c.getScore();
	writer << "\n";
	vector<string> cVec = c.GetRoute();
	for(int i = 0; i<cVec.size(); i++)
		writer << cVec[i] << " ";
	i++;

}