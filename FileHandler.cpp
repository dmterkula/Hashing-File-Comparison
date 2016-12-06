// FileHandler.cpp
/*
* Creates the implemetation of a class that reads and writes to files. This FileHandler also
* uses a Hasher object so that it can hash the strings it reads from a given file.
* This FileHandler will only read files that are valid. It also stores the current
* File name it is reding and writing to as an instance variable. This class will
* create and store a map of the hashes for the main class to retrieve and use easily. This class
* also counts the number of char or bytes in a file to aid in testing run time.
*
* David Terkula
* 12/4/2016
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>
#include <map>
#include "FileHandler.h"
#include "hashtab.h"

using namespace std;


/*
* The Constructor for the FileHandler class, does nothing extra.
*/

FileHandler::FileHandler(){

}

/*
* Sets the instance variable of readFrom to the passed string 'fileName'
*/
void FileHandler::setReadFrom(string fileName){
  readFrom = fileName;
}

/*
* Sets the instance variable of writeTo to the passed string 'fileName'
*/
void FileHandler::setWriteTo(string fileName){
  writeTo = fileName;
}

/*
* This method prints the lines in common between the two files based on their has values.
*/
void printFileComparison(map<vector<int>, vector<int>> linesInCommon, string file1, string file2){
  cout << "duplicate Hashes and lines between " << file1 << " and " << file2 << endl;
  cout << "file1: ";
  cout << "         file2: " << endl;
  for(map<vector<int>, vector<int>>::iterator value = linesInCommon.begin(); value != linesInCommon.end(); value++){
    vector<int> file1Lines = value->first;
    vector<int> file2Lines = value->second;


    for(int i = 0; i < file1Lines.size(); i++){
      cout << file1Lines[i] << ", ";
    }

    cout << "              ";
    for(int i = 0; i < file2Lines.size(); i++){
      cout << file2Lines[i] << ", "  << endl;
    }
  }
}

/*
* This method prints the hash values and starting lines for a given file in order of when they were hashed in the file.
*
*/
void FileHandler::printTable(map<hashval_t, vector<int>> hashMap, string tableName){
  cout << "Hashes and starting line locations from " << tableName << endl;
  cout << endl;
  for(int i = 1; i < hashMap.size()+1; i++){
    for(map<hashval_t, vector<int>>::iterator value = hashMap.begin(); value != hashMap.end(); value++){
      vector<int> startingLines = value->second;
      if(startingLines[0] == i){
        hashval_t hash = value->first;
        cout << "Hash value : " << hash << "    starting line location ";
        for(int i = 0; i < startingLines.size(); i++){
          cout << startingLines[i] << ", ";
        }
        cout<<endl;
      }
    }
  }
}

string FileHandler::standardizeText(string text){
  text.erase(remove( text.begin(), text.end(), ' ' ), text.end() );
  transform(text.begin(), text.end(), text.begin(), ::tolower);
  return text;
}

/*
* Reads and hashes the file window by window, sliding down the file as it goes so each line is eventually
* considered as the first line of the window. will not hash the last lines in a file if it is smaller than
* the window size. This method also writes to a new file the hashes and starting lines of each window. This method
* returns the number of bytes (a char is stored as a byte) in order to tell the file size.
*/
int FileHandler::readAndHash(){
  infile.open(readFrom);
  ofstream outFile ("dataFrom" + readFrom);
  outFile << readFrom << endl;
  map<hashval_t, vector<int>> hashMap;
  // pre load window.
  array<string, windowSize> window;
  int counter = 0;
  while(infile.peek() && !infile.eof() && counter < windowSize-1){ // pre-load windowSize-1 lines into the window
    numLines++;
    string line;
    getline(infile, line);
    byteCounter += line.size();
    // cout << line << endl;
    window[counter] = line;
    counter++;
  }
  if(windowSize - window.size() > 1){ // window size greater than file size
    cout << "file has less lines than the window size, re-adjust window size " << endl;
  }
  else{ // run program as normal, window is currently
    int lineCounter = 1;
    while(infile.peek() && !infile.eof()){
      numLines++;
      vector<int> startLines;
      startLines.push_back(lineCounter);
      string nextLine;
      getline(infile, nextLine);
      byteCounter += nextLine.size();
      window[windowSize-1] = nextLine; // last line in window = next line in file.
      string toHash;
      for(int i = 0; i < window.size(); i++){
        toHash += window[i];
      }
      toHash = standardizeText(toHash);
      cout << "edited to hash = " << toHash << endl;
      hashval_t hash = hasher.hashString(toHash);
      cout << "hashval = " << hash << endl;

      if(hashMap.count(hash) > 0 ){ // has duplicate hash
        vector<int> temp = hashMap[hash];
        temp.push_back(lineCounter);
        hashMap[hash] = temp;
      }
      else{
        hashMap[hash] = startLines;
      }

      outFile << hash << " ";
      // loop over start lines and print to fileName
      vector<int> temp = hashMap[hash];
      for(int i = 0; i < temp.size(); i++){
        outFile << temp[i] << " ";
      }
      outFile << endl;

      for(int i = 0; i < window.size()-1; i++){ // shift window
        window[i] = window[i+1];
      }
      lineCounter ++;
    }
    infile.close();
  }
  printTable(hashMap, readFrom);
  byteCounter += numLines;
  return byteCounter;
}

/*
* Given a string fileName, this method returns true if the file exists in the current directory and can be opened,
* and false otherwise
*/
bool FileHandler::testFileValidty(string fileName){
  if(fileName == ""){
    return false;
  }
  else{
    fstream file(fileName.c_str());
    return (bool)file;
  }
}

/*
* Compares two files output from the readAndHash method. Tells the user which lines arein common
* between the files based on matching hash values. This method returns the byteCounter or byte
* size of the file
*/
int FileHandler::compareFiles(string file1, string file2){
  if(testFileValidty(file1) && testFileValidty(file2)){
    map<hashval_t, vector<int>> hashMapFile1;
    map<hashval_t, vector<int>> hashMapFile2;
    readFrom = file1;
    hashMapFile1 = readDataFile(hashMapFile1);
    readFrom = file2;
    hashMapFile2 = readDataFile(hashMapFile2);

    map<vector<int>, vector<int>> linesInCommon;
    for(map<hashval_t, vector<int>>::iterator value = hashMapFile1.begin(); value != hashMapFile1.end(); value++){
      if(hashMapFile2.count(value->first) > 0){ // is a duplicate hash
        linesInCommon[value->second] = hashMapFile2[value->first];
      }
    }

    printFileComparison(linesInCommon, file1, file2);

  }
  else{
    cout << "you did not enter 2 valid file names ... please try again" << endl;
  }
  return byteCounter;
}

/*
* reads the data file that was output from the readAndHash method, and parses the first part as divded by a space,
* a hashval_t and the next part as the starting lines of the hash value in the file. This method also counts number
* of bytes it reads
*/
map<hashval_t, vector<int>> FileHandler::readDataFile(map<hashval_t, vector<int>> hashMap){
  infile.open(readFrom);
  hashval_t nextHash;
  string nextLine;
  string hashText;
  string lineText;
  int splitIndex;

  getline(infile, nextLine); // captures file name from the file so that data can be parsed
  int lineCounter = 1;

  while(infile.peek() && !infile.eof()){ // split line into hash and lines at which hash starts
    numLines++;
    vector<int> startingLines;
    getline(infile, nextLine);
    byteCounter += nextLine.size();
    splitIndex = nextLine.find_first_of(" ");
    hashText = nextLine.substr(0, splitIndex);
    lineText = nextLine.substr(splitIndex+1);
    nextHash = stoull(hashText);
    for(int i = 0; i < lineText.size()-1; i++){ // parse start lines and store into vector
      if(lineText[i] != ' ' && lineText[i] != '\n'){ // parse as int
        string nextChar;
        nextChar.push_back(lineText[i]);
        //cout << nextChar << endl;
        startingLines.push_back(stoi(nextChar));
      }
    }
    hashMap[nextHash] = startingLines;
  }
  infile.close();
  byteCounter += numLines;
  return hashMap;
}
