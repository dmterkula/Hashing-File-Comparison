//Hasher.cpp
/*
* Creates the implementation for a class to hash a string to a hashval_t as defined by the
* hashtab class.
*
* David Terkula
* 12/4/16
*/
#include <string>
#include "hashtab.h"
#include "Hasher.h"

using namespace std;

/*
* Given a string of text, this method hashes the text to a hashval_t,
* or an unsigned long long as outlined by the hashtab.h file
*
*/
hashval_t Hasher::hashString(string text){
  hashval_t h;
  for(int i = 0; i < text.size(); i++){
      h = h ^ magicbits[text[i]][i];
  }
  return h;
}
