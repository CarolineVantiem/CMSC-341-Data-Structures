/* Caroline Vantiem
   Cvantie1@umbc.edu
   File: Driver.cpp
   CMSC 341 F18 - Proj5
*/

#include "HashTable.h"
#include "words.h"

#include <iostream>
#include <cstdlib>
#include <set>
#include <string>
using namespace std;

// helper func.
bool isInSet(set<string>& S, const char * str) ;
void addToSet(set<string>& S, const char *str) ;
void removeFromSet(set<string>& S, const char *str) ;
void sanityCheck(HashTable& T, set<string> S) ;

// test func.
int test1();
void test2();
void test3();
int test4();
void test5();

// isInSet
bool isInSet(set<string>& S, const char * str) {
  set<string>::iterator it ;

  it = S.find(str) ;
  if (it != S.end() ) return true ;
  return false ;
}

// addToSet
void addToSet(set<string>& S, const char *str) {
  pair< set<string>::iterator, bool>  p ;

  p = S.insert(str) ;
}

// removeFromSet
void removeFromSet(set<string>& S, const char *str) {
  S.erase(str) ;
}

// sanityCheck
void sanityCheck(HashTable& T, set<string> S) {
   
  int Tsize ;
  const char * ONE = (char *) 1 ;

  // Check hash table size vs set size
  //
  if ( T.isRehashing() ) {
    Tsize = T.size(0) + T.size(1) ;
  } else {
    Tsize = T.size(0) ;
  }

  if ( Tsize == S.size() ) {
    cout << "Sets sizes are both " << Tsize << endl ;
  } else {
    cout << "Sets sizes are different:  " 
	 << "T size = " << Tsize << ", "
         << "S.size() = " << S.size() << endl ;
  }

  // Construct STL set<string> for all strings in the hash table
  //
  set<string> Tset ;
  const char *str ;

  int n = T.tableSize(0) ;
  for (int i=0 ; i < n ; i++) {
    str = T.at(i,0) ;
    if (str != NULL && str != ONE) addToSet(Tset,str) ;
  }
  if ( T.isRehashing() ) {
    n = T.tableSize(1) ;
    for (int i=0 ; i < n ; i++) {
      str = T.at(i,1) ;
      if (str != NULL && str != ONE) addToSet(Tset,str) ;
    }
  }

  set<string>::iterator it ; 

  int inSnotT = 0 ;
  int inTnotS = 0 ;

  for (it = S.begin() ; it != S.end() ; it++) {
    if (! isInSet(Tset, it->c_str()) ) inSnotT++ ;
  }

  for (it = Tset.begin() ; it != Tset.end() ; it++) {
    if (! isInSet(S, it->c_str()) ) inTnotS++ ;
  }

  if (inSnotT == 0 && inTnotS == 0) {
    cout << "Passed set equality test\n" ;
  } else {
    cout << "***Failed set equality test\n" ;
    cout << "   " << inSnotT << " words in set S but not in hash table T.\n" ;
    cout << "   " << inTnotS << " words in hash table T but not in set S.\n" ;
  }
}

// test1
int test1() {
  HashTable T(107) ;

  T.insert("tributes") ;// 21
  T.insert("skulduggery") ;// 22
  T.insert("convulse") ;// 23
  T.insert("frothed") ;// 24
  T.insert("horrify") ;// 25
  T.insert("blackmailers") ;// 26
  T.insert("defenestrated") ;// 27
  T.insert("garrison") ;// 23 -> 28
  T.insert("lidless") ;// 22 -> 29
  // T.insert("eye") ;            // 21 -> 30

  cout << "----------------------------------------------------\n" ;
  cout << "Original hash table\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;


  cout << "\n\nDo some find() and remove() operations...\n\n" ;

  const char *str ;
  char *ptr ;
  bool found ;

  if( T.find(str="skulduggery") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  if( T.find(str="lidless") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  if( T.find(str="defenestrated") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  if( T.find(str="peircing") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  ptr = T.remove(str="garrison") ;
  if (ptr == NULL) {
    cout << "String " << str << " not found, not removed\n" ;
  } else {
    cout << "Removed string = " << ptr << endl ;
    free(ptr) ;
  }

  ptr = T.remove(str="infractions") ;
  if (ptr == NULL) {
    cout << "String " << str << " not found, not removed\n" ;
  } else {
    cout << "Removed string = " << ptr << endl ;
    free(ptr) ;
  }

  if( T.find(str="garrison") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  if( T.find(str="lidless") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  cout << "\n\n" ;
  cout << "----------------------------------------------------\n" ;
  cout << "Hash table after finds and removes\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;
   
  cout << "\n\nNext insert should reuse DELETED slots...\n\n" ;

  T.insert("undying") ;        // 25 -> 28 

  cout << "\n\n" ;
  cout << "----------------------------------------------------\n" ;
  cout << "Final hash table\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;
  return 0 ;
}

// test2
void test2() {
  HashTable T(107) ;

  T.insert("undulation") ;     //  9
  T.insert("impertinent") ;    // 10
  T.insert("maladies") ;       // 10 -> 11
  T.insert("dominions") ;// 12

  T.insert("waspish") ;        // 52
  T.insert("wildebeest") ;     // 52 -> 53
  T.insert("reaction") ;       // 52 -> 54

  T.insert("pawns") ;          // 43
  T.insert("vacuously") ;      // 43 -> 44
  T.insert("firth") ;          // 43 -> 45
  T.insert("manipulator") ;    // 43 -> 46
  T.insert("dreariness") ;     // 43 -> 47

  T.insert("insanity") ;       // 105
  T.insert("enthronement") ;   // 105 -> 106
  T.insert("falsifiers") ;     // 105 -> 0
  T.insert("ignominious") ;    // 105 -> 1
  T.insert("mummified") ;      // 105 -> 2

  T.insert("tributes") ;// 21
  T.insert("skulduggery") ;// 22
  T.insert("convulse") ;// 23
  T.insert("frothed") ;// 24
  T.insert("horrify") ;// 25
  T.insert("blackmailers") ;// 26
  T.insert("defenestrated") ;// 27
  T.insert("garrison") ;// 23 -> 28
  T.insert("lidless") ;// 22 -> 29

  cout << "----------------------------------------------------\n" ;
  cout << "Original hash table\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;

  cout << "\n\nInserting \"eye\" should trigger rehash\n\n" ;
  T.insert("eye") ;            // 21 -> 30, should trigger inc. rehash


  cout << "----------------------------------------------------\n" ;
  cout << "Hash table after rehash triggered\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;

  cout << "\n\nSearch for \"manipulator\" should move cluster in slots 43-47.\n\n" ;
  T.find("manipulator") ;            // 21 -> 30, should trigger inc. rehash

  cout << "----------------------------------------------------\n" ;
  cout << "Hash table after cluster 43-47 moved.\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;

  cout << "\n\nDo some finds, inserts and removes\n\n" ;

  const char *str ;
  char *ptr ;

  if( T.find(str="zip") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  if( T.find(str="spaceflight") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }
  
  if( T.find(str="frothed") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  T.insert("wildcat") ;        // 18 (new table)
  T.insert("weightlessness") ; // 69 (new table)
  T.insert("sorceress") ;      // 33 (new table)
  T.insert("enchantress") ;    // 33 (new table) really.


  cout << "----------------------------------------------------\n" ;
  cout << "Hash table after more insertions.\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;

  cout << "\n\nA find on \"ignominious\" + 1 more operation should cause the tables to consolidate down to one table. \n\n" ;

  if( T.find(str="ignominious") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }

  if( T.find(str="reaction") ) {
    cout << "Found " << str << endl ;
  } else {
    cout << "Did not find " << str << endl ;
  }


  cout << "----------------------------------------------------\n" ;
  cout << "Hash table after wrap up.\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;
}

// test3
void test3() {
  HashTable T(107) ;

  T.insert("undulation") ;     //  9
  T.insert("impertinent") ;    // 10
  T.insert("maladies") ;       // 10 -> 11
  T.insert("dominions") ;// 12

  T.insert("waspish") ;        // 52
  T.insert("wildebeest") ;     // 52 -> 53
  T.insert("reaction") ;       // 52 -> 54

  T.insert("pawns") ;          // 43
  T.insert("vacuously") ;      // 43 -> 44
  T.insert("firth") ;          // 43 -> 45
  T.insert("manipulator") ;    // 43 -> 46
  T.insert("dreariness") ;     // 43 -> 47

  T.insert("insanity") ;       // 105
  T.insert("enthronement") ;   // 105 -> 106
  T.insert("falsifiers") ;     // 105 -> 0
  T.insert("ignominious") ;    // 105 -> 1
  T.insert("mummified") ;      // 105 -> 2

  T.insert("tributes") ;// 21
  T.insert("skulduggery") ;// 22
  T.insert("convulse") ;// 23
  T.insert("frothed") ;// 24
  T.insert("horrify") ;// 25
  T.insert("blackmailers") ;// 26
  T.insert("defenestrated") ;// 27
  T.insert("garrison") ;// 23 -> 28
  T.insert("lidless") ;// 22 -> 29
  T.insert("eye") ;            // 21 -> 30, should trigger inc. rehash


  cout << "----------------------------------------------------\n" ;
  cout << "Hash table after rehash triggered\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;

  cout << "\n\nDo some insertions to make long linear probe sequence in the second table.\n\n" ;

  T.insert("wildcat") ;        // 18 (new table)
  T.insert("weightlessness") ; // 69 (new table)
  T.insert("sorceress") ;      // 33 (new table)
  T.insert("enchantress") ;    // 33 (new table) really.
  T.insert("witchery") ;       // 67 -> 68 (new table)
  T.insert("heliosphere") ;    // 67 -> 72 (new table)
  T.insert("obstruct") ;       // 67 -> 73 (new table)

  cout << "----------------------------------------------------\n" ;
  cout << "Hash table insertions.\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;

  cout << "\n\nOne more insertion in slot 67 should make us give up on rehashing.\n\n" ;
  T.insert("peripatetic") ;    // 67 -> 77 (new table)

  cout << "----------------------------------------------------\n" ;
  cout << "Hash table giving up on rehashing.\n" ;
  cout << "----------------------------------------------------\n" ;
  T.dump() ;
  cout << "----------------------------------------------------\n" ;
}

// test4
int test4() {
  HashTable T(883) ;
  set<string> S ;
  char *str ;

  // insert 900 words
  for (int i=100 ; i < 1000 ; i++) {
    T.insert(words[i]) ;  addToSet(S,words[i]) ;
  }

  // remove 100 words
  for (int i=400 ; i < 500 ; i++) {
    str = T.remove(words[i]) ;  
    free(str) ;
    removeFromSet(S,words[i]) ;
  }

  // insert 10,000 words
  for (int i=30000 ; i < 40000 ; i++) { 
    T.insert(words[i]) ;  addToSet(S,words[i]) ;
  }

  // remove 2000 words + try to remove 2000 not in table
  for (int i=28000 ; i < 32000 ; i++) {
    str = T.remove(words[i]) ;  
    free(str) ;
    removeFromSet(S,words[i]) ;
  }

  // now do 10,000 random finds
  int r ;
  int Tcount = 0 ;
  int Scount = 0 ;
  srand(843150) ;
  for (int i=0 ; i<10000 ; i++) {
    r = rand() % numWords ;
    if ( T.find(words[r]) ) Tcount++ ;
    if ( isInSet(S,words[r]) ) Scount++ ;
  }

  if (Tcount == Scount) {
    cout << "Passed random find() tests: "
	 << "Tcount = " << Tcount << ", "
         << "Scount = " << Scount << endl ;
  } else {
    cout << "***Failed random find() tests: "
	 << "Tcount = " << Tcount << ", "
         << "Scount = " << Scount << endl ;
  }

  sanityCheck(T,S) ;

  return 0 ;
}

// test5
void test5() {
  HashTable T(107) ;

  T.insert("undulation") ;     //  9
  T.insert("impertinent") ;    // 10
  T.insert("maladies") ;       // 10 -> 11
  T.insert("dominions") ;// 12

  T.insert("waspish") ;        // 52
  T.insert("wildebeest") ;     // 52 -> 53
  T.insert("reaction") ;       // 52 -> 54

  T.insert("pawns") ;          // 43
  T.insert("vacuously") ;      // 43 -> 44
  T.insert("firth") ;          // 43 -> 45
  T.insert("manipulator") ;    // 43 -> 46
  T.insert("dreariness") ;     // 43 -> 47

  T.insert("insanity") ;       // 105
  T.insert("enthronement") ;   // 105 -> 106
  T.insert("falsifiers") ;     // 105 -> 0
  T.insert("ignominious") ;    // 105 -> 1
  T.insert("mummified") ;      // 105 -> 2

  T.insert("tributes") ;// 21
  T.insert("skulduggery") ;// 22
  T.insert("convulse") ;// 23
  T.insert("frothed") ;// 24
  T.insert("horrify") ;// 25
  T.insert("blackmailers") ;// 26
  T.insert("defenestrated") ;// 27
  T.insert("garrison") ;// 23 -> 28
  T.insert("lidless") ;// 22 -> 29
  T.insert("eye") ;            // 21 -> 30, should trigger inc. rehash

  set<string> S ;

  addToSet(S,"undulation") ;
  addToSet(S,"impertinent") ;
  addToSet(S,"maladies") ;
  addToSet(S,"dominions") ;
  addToSet(S,"waspish") ;
  addToSet(S,"wildebeest") ;
  addToSet(S,"reaction") ;
  addToSet(S,"pawns") ;
  addToSet(S,"vacuously") ;
  addToSet(S,"firth") ;
  addToSet(S,"manipulator") ;
  addToSet(S,"dreariness") ;
  addToSet(S,"insanity") ;
  addToSet(S,"enthronement") ;
  addToSet(S,"falsifiers") ;
  addToSet(S,"ignominious") ;
  addToSet(S,"mummified") ;
  addToSet(S,"tributes") ;
  addToSet(S,"skulduggery") ;
  addToSet(S,"convulse") ;
  addToSet(S,"frothed") ;
  addToSet(S,"horrify") ;
  addToSet(S,"blackmailers") ;
  addToSet(S,"defenestrated") ;
  addToSet(S,"garrison") ;
  addToSet(S,"lidless") ;
  addToSet(S,"eye") ;

  cout << "\nCheck original hash table T...\n" ;
  sanityCheck(T,S) ;

  HashTable *T2ptr = new HashTable(T) ;
  HashTable T3(*T2ptr) ;

  cout << "\nCheck hash tables T, *T2ptr & T3 after copying...\n" ;
  sanityCheck(T,S) ;
  sanityCheck(*T2ptr,S) ;
  sanityCheck(T3,S) ;

  delete T2ptr ;
  cout << "\nCheck hash tables T & T3 after deleting T2ptr...\n" ;
  sanityCheck(T,S) ;
  sanityCheck(T3,S) ;


  HashTable X(107) ;

  X.insert("undulation") ;     //  9
  X.insert("impertinent") ;    // 10
  X.insert("maladies") ;       // 10 -> 11
  X.insert("dominions") ;// 12

  X.insert("waspish") ;        // 52
  X.insert("wildebeest") ;     // 52 -> 53
  X.insert("reaction") ;       // 52 -> 54

  X.insert("pawns") ;          // 43
  X.insert("vacuously") ;      // 43 -> 44
  X.insert("firth") ;          // 43 -> 45
  X.insert("manipulator") ;    // 43 -> 46
  X.insert("dreariness") ;     // 43 -> 47

  X.insert("insanity") ;       // 105
  X.insert("enthronement") ;   // 105 -> 106
  X.insert("falsifiers") ;     // 105 -> 0
  X.insert("ignominious") ;    // 105 -> 1
  X.insert("mummified") ;      // 105 -> 2

  X.insert("tributes") ;// 21
  X.insert("skulduggery") ;// 22
  X.insert("convulse") ;// 23
  X.insert("frothed") ;// 24
  X.insert("horrify") ;// 25
  X.insert("blackmailers") ;// 26
  X.insert("defenestrated") ;// 27
  X.insert("garrison") ;// 23 -> 28
  X.insert("lidless") ;// 22 -> 29
  X.insert("eye") ;            // 21 -> 30, should trigger inc. rehash

  HashTable X2, X3 ;

  cout << "\nCheck original hash table X...\n" ;
  sanityCheck(X,S) ;

  X2 = X ;
  X3 = X2 ;

  cout << "\nCheck hash tables X, X2 & X3 after assignment...\n" ;
  sanityCheck(X,S) ;
  sanityCheck(X2,S) ;
  sanityCheck(X3,S) ;

  X2.insert("wildcat") ;
  X2.insert("weightlessness") ;
  X2.insert("sorceress") ;
  X2.insert("enchantress") ;
  X2.insert("witchery") ;
  X2.insert("heliosphere") ;
  X2.insert("obstruct") ;

  cout << "\nCheck hash tables X & X3 after modifying X2...\n" ;
  sanityCheck(X,S) ;
  sanityCheck(X3,S) ;
}

// main
int main() {
  int input;
  
  do {
    
    cout << endl << "Enter Test # to Run 1-5" << endl;
    cout << " 0 - to quit" << endl;
    
    for (int i = 1; i <= 5; i++) 
      cout << " " << i << ". " << "test" << i << ".cpp" << endl;
    
    cout << endl;
    cin >> input;

    // check bounds
    if (input < 0 || input > 5) {
      
    }
    
    else {
      
      switch (input) {
	
	// test 1
      case 1:
	test1();
	break;
	
	// test 2
      case 2:
	test2();
	break;
	
	// test 3
      case 3:
	test3();
	break;
	
	// test 4
      case 4:
	test4();
	break;
	
	// test 5
      case 5:
	test5();
	break;
      }
    }
  }
  
  while (input != 0);
  cout << endl;
}
