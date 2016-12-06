//Hasher.h
/*
* Creates the outline for a class to hash a string to a hashval_t as defined by the
* hashtab class
*
* David Terkula
* 12/4/16
*/
#ifndef Hasher_H
#define Hasher_h

#include <iostream>
#include <string>
#include "hashtab.h"

using namespace std;

class Hasher{
public:
  hashval_t hashString(string text);


};
#endif
