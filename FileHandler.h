// FileHandler.h
/*
* Creates the outline for a class that reads and writes to files. This FileHandler also
* stores a Hasher object so that it can hash the strings it reads from a given file.
* This FileHandler will only read files that are valid. It also stores the current
* File name it is reding and writing to as an instance variable. This class will
* create and store a map of the hashes for the main class to retrieve and use easily.
* In addition, this class also counts the number of bytes it reads.
*
* David Terkula
* 12/4/2016
*/


#ifndef FileHandler_H
#define FileHandler_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Hasher.h"
#include <map>

using namespace std;

class FileHandler{
private:
  string readFrom;
  string writeTo;
  ifstream infile;
  Hasher hasher;
  static const int windowSize = 3;
  int byteCounter = 0;
  int numLines = 0;

public:

  /*
  * Constructor for FileHandler, takes no arguements
  */
  FileHandler();

  /*
  * Sets the instance variable of readFrom to the passed string 'fileName'
  */
  void setReadFrom(string fileName);

  /*
  * Sets the instance variable of writeTo to the passed string 'fileName'
  */
  void setWriteTo(string fileName);

  /*
  * Reads and hashes the file window by window, sliding down the file as it goes so each line is eventually
  * considered as the first line of the window. will not hash the last lines in a file if it is smaller than
  * the window size. This method also writes to a new file the hashes and starting lines of each window
  */
  int readAndHash();

  /*
  * Compares two files output from the readAndHash method. Tells the user which lines arein common
  * between the files based on matching hash values. returns the number of bytes it reads
  */
  int compareFiles(string file1, string file2);

  /*
  * reads the data file that was output from the readAndHash method, and parses the first part as divded by a space,
  * a hashval_t and the next part as the starting lines of the hash value in the file.
  * counts the number of bytes it reads
  */
  map<hashval_t, vector<int>> readDataFile(map<hashval_t, vector<int>> hashMap);

  /*
  * Given a string fileName, this method returns true if the file exists in the current directory and can be opened,
  * and false otherwise
  */
  bool testFileValidty(string fileName);

  /*
  * this method prints the file comparison between two data files showing which lines are the same from the two files.
  */
  void printTable(map<hashval_t, vector<int>> hashMap, string tableName);

  /*
  * this method returns the given string without any white spaces, or
  */
  string standardizeText(string text);


};
#endif
