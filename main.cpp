// main.cpp
/*
* The main class accepts input, the number of files and then the file names, from the user and
* then either reads and hashes the file, or compares the two files for similar hashes and similar
* lines in each file. This class uses the FileHandler class to handle the reading, writing and
* hashing of files. The main also keeps track of run time of program execution
*
* David Terkula
* 12/4/16
*/

#include <string>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <chrono>
#include "FileHandler.h"

using namespace std;


int byteCounter = 0;
/*
* The main method, takes the number of arguemnts and the files input from the user.
* If the number of arguements input is 3, then the program will attempt to
* read and hash the file. If the number of arguements is equal to 4, then the
* program will attempt to compare the two data files for similar hashes.
*/
int main(int argc, char* argv[]){
  typedef chrono::steady_clock steady_clock;
  auto start = steady_clock::now();
  bool ran = false;

  if(argc == 2){ // create hash values and table
    string fileName = argv[1];
    FileHandler fileHandler;
    if(fileHandler.testFileValidty(fileName) == true){
      fileHandler.setReadFrom(fileName);
      byteCounter = fileHandler.readAndHash();
      ran = true;
    }
    else{
      cout << "you did not enter a valid file name, please try again" << endl;
    }
  }
  else if(argc == 3){ // read in output data files
    string file1 = argv[1];
    string file2 = argv[2];
    FileHandler fileHandler;
    byteCounter = fileHandler.compareFiles(file1, file2);
    ran = true;
  }

  else{
    cout << "you did not enter the proper number of arguements.";
    cout << "Enter the number of files, then 1 file to hash, or 2 to compare" << endl;
  }

  if(ran == true){ // give run time in milliseconds
    auto end = steady_clock::now();
    double elapsedSeconds = ((end - start).count()) * steady_clock::period::num / static_cast<double>(steady_clock::period::den) * 1000;
    cout << endl;
    cout << "time elapsed in milliseconds = " << elapsedSeconds << endl;
    cout << "num bytes = " << byteCounter << endl;
    double bytesPerMillisecond = byteCounter / elapsedSeconds;
    cout << "bytesPerMillisecond = " << bytesPerMillisecond << endl;
  }

}
