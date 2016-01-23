#ifndef FileManager_H
#define FileManager_H

#include <string>
#include <vector>
#include <fstream>
#include "GeneticChild.h"

using namespace std;

class FileManager{
	// Is used to read/print data
public:
	FileManager();
	
	void ReadFile();

	void PrintChild(GeneticChild& printMe);
	void setInFile(string s); // sets input file
	void setOutPrefix(string s); //sets output prefix -> out1 out2 ...
	CityManager getCityManager() const;

private:
	CityManager cm;
	string infile;
	string outPrefix;


};

#endif