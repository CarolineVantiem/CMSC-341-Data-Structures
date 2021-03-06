/* Caroline Vantiem
   Cvantie1@umbc.edu
   File: HashTable.cpp (FINAL COPY)
   CMSC 341 F18 - Proj5
*/

#include "HashTable.h"
#include "primes.cpp"
using namespace std;

char * const HashTable::DELETED = (char *) DELTED; 

unsigned int hashCode(const char *str) {
  
  unsigned int val = 0 ;
  const unsigned int thirtyThree = 33 ;  // magic number from textbook

  int i = 0 ;
  while (str[i] != '\0') {
    val = val * thirtyThree + str[i] ;
    i++ ;
  }
  return val ;
}

int getPrime(int n, int begin = 0, int end = numPrimes - 1) {
  // check HashTable bounds
  if (n <= 101)
    return 101;
  if (n >= 200000)
    throw out_of_range("Size out of range");
  
  int currIndex = (end - begin) / 2 + begin;
  int currPrime = primes[currIndex];
  
  // find prime num
  if (currPrime < n) {
    return getPrime(n, currIndex + 1, end);
  }
  
  // number is in range
  else {
    // found prime index
    if (primes[currIndex - 1] < n && currPrime >= n)
      return currPrime;
    // recurse thru - prime is found
    else {
      return getPrime(n, begin, currIndex - 1);
    }
  }
}

// default constructor
HashTable::HashTable(int n) {
  // set/create new HashTable 0
  int size = roundUpPrime(n);
  m_cap0 = getPrime(n);
  m_table0 = new char* [m_cap0]();
  m_size0 = 0;
  
  m_status = 0;
  
  // set/create other HashTables 1 & 2
  m_table1 = NULL;
  m_size1 = 0;
  
  m_table2 = NULL;
  m_size2 = 0;
}

// destructor
HashTable::~HashTable() {
  // destruct/delete all HashTables
  
  // destruct/delete HashTable 0
  if (m_table0 != NULL) {
    for (int i = 0; i < m_cap0; i++) {
      if (m_table0[i] != NULL && m_table0[i] != DELETED)
	free(m_table0[i]);      
    }
  }
  delete [] m_table0;

  // destruct/delete HashTable 1
  if (m_table1 != NULL) {
    for (int i = 0; i < m_cap1; i++) {
      if (m_table1[i] != NULL && m_table1[i] != DELETED)
	free(m_table1[i]);      
    }
  }
  delete [] m_table1;
  
  // destruct/delete HashTable 2
  if (m_table2 != NULL) {
    for (int i = 0; i < m_cap2; i++) {
      if (m_table2[i] != NULL && m_table2[i] != DELETED)
	free(m_table2[i]);      
    }
  }
  delete [] m_table2;
}

//Copy constructor
HashTable::HashTable(HashTable& other) {
  // if rehashing - call rehash func.
  if (other.m_status > 0)
    other.rehash();
  
  // copy HashTable 0
  m_cap0 = other.m_cap0;
  m_table0 = new char*[m_cap0]();
  
  for (int i = 0; i < m_cap0; i++) {
    if (other.m_table0[i] != NULL && other.m_table0[i] != DELETED) {
      m_table0[i] = strdup(other.m_table0[i]);
    }
  }
  
  m_size0 = other.m_size0;
  
  m_status = 0;
  
  // set/create other HashTables 1 & 2
  m_table1 = NULL;
  m_size1 = 0;
  
  m_table2 = NULL;
  m_size2 = 0;
}

//Assignment operator
const HashTable& HashTable::operator=(HashTable& rhs) {
  // deallocate/delete memory in HashTables
  
  // delete/deallocate HashTable0
  if (m_table0 != NULL) {
    for (int i = 0; i < m_cap0; i++) {
      if (m_table0[i] != NULL && m_table0[i] != DELETED)
	free(m_table0[i]);      
    }
  }
  delete [] m_table0;
  
  // delete/deallocate HashTable1
  if (m_table1 != NULL) {
    for (int i = 0; i < m_cap1; i++) {
      if (m_table1[i] != NULL && m_table1[i] != DELETED)
	free (m_table1[i]);      
    }
  }
  delete [] m_table1;
  
  // delete/deallocate HashTable2
  if (m_table2 != NULL) {
    for (int i = 0; i < m_cap2; i++) {
      if (m_table2[i] != NULL && m_table2[i] != DELETED)
	free (m_table2[i]);      
    }
  }
  delete [] m_table2;
  
  // rhs copying
  // if rehashing - call rehash func.
  if (rhs.m_status > 0)
    rhs.rehash();
  
  // copy HashTable0
  m_cap0 = rhs.m_cap0;
  m_table0 = new char*[m_cap0]();
  
  for (int i = 0; i < m_cap0; i++) {
    if (rhs.m_table0[i] != NULL && rhs.m_table0[i] != DELETED) {
      m_table0[i] = strdup(rhs.m_table0[i]);
    }
  }
  m_size0 = rhs.m_size0;
  
  m_status = 0;
  
  // set other HashTables 1 & 2
  m_table1 = NULL;
  m_size1 = 0;
  
  m_table2 = NULL;
  m_size2 = 0;
  
  return *this;
}

// insert
void HashTable::insert(const char * str) {
  int counter1 = 1, counter2 = 1;
  unsigned int currIndex;
  unsigned int hashIndex = hashCode(str);
  unsigned int insertIndex = -1;
  bool dup = false;
  
  // if rehashing - call cluster func.
  if (m_table0[hashIndex % m_cap0] != NULL && m_status == 1) {
    clusterRehash(hashIndex % m_cap0);
  }

  // if not rehashing - insert to HashTable0
  if (m_status == 0) {
    currIndex = hashIndex % m_cap0;
     
    while(m_table0[currIndex] != NULL) {
      counter1++;
      
      // check for duplicates
      if (m_table0[currIndex] == DELETED) {
	if (insertIndex == -1)
	  insertIndex = currIndex;
      }
      // found duplicate
      else if ( strcmp(m_table0[currIndex], str) == 0) {
	dup = true;
      }

      currIndex = (currIndex + 1) % m_cap0;
    }
    
    // insert if NOT duplicate
    if (!dup) {
      if(insertIndex != -1)
	m_table0[insertIndex] = strdup(str);
      else
	m_table0[currIndex] = strdup(str);

      m_size0++;
    }
    
    // check for rehash conditions
    if (counter1 >= 10) {
      m_status = 1;
      clusterRehash(hashIndex % m_cap0);
    }
    
    if ( (m_size0/(double)m_cap0) > 0.5) {
      m_status = 1;
      clusterRehash(hashIndex % m_cap0);
    }
  }
  
  // if rehashing - HashTable1
  else if (m_status == 1) {
    currIndex = hashIndex % m_cap1;
    
    while(m_table1[currIndex] != NULL) {
      counter2++;
      
      // check for duplicates
      if (m_table1[currIndex] == DELETED) {
	if (insertIndex == -1)
	  insertIndex = currIndex;
      }
      
      // found duplicate
      else if ( strcmp(m_table1[currIndex], str) == 0)
	dup = true;
      
      currIndex = (currIndex + 1)% m_cap1;
    }
    
    // insert if NOT duplicate
    if (!dup) {
      if (insertIndex != -1)
	m_table1[insertIndex] = strdup(str);
      else
	m_table1[currIndex] = strdup(str);
      m_size1++;
    }
    
    // rehash
    if (counter2 >= 10) {
      m_status = 2;
      rehash();
    }
    
    if ( (m_size1/(double)m_cap1) > .5) { 
      m_status = 2;
      rehash();
    }
  }
  
  // if rehashing - HashTable2
  else if (m_status == 2) {
    currIndex = hashIndex % m_cap2;
    
    while(m_table2[currIndex] != NULL) {
      
      // look for duplicates
      if (m_table2[currIndex] == DELETED) {
	if (insertIndex == -1)
	  insertIndex = currIndex;
      }
  
      // found duplicate
      if ( strcmp(m_table2[currIndex], str) == 0)
	dup = true;
      
      currIndex = (currIndex + 1) % m_cap2;
    }
    
    // insert if NOT duplicate
    if (!dup) {
      if (insertIndex != -1)
	m_table2[insertIndex] = strdup(str);
      else
	m_table2[currIndex] = strdup(str);
      m_size2++;
    }
  }
}

// insertT1
void HashTable::insertT1(const char * str) {
  int counter = 1;
  unsigned int hashIndex = hashCode(str);
  unsigned int currIndex = hashIndex % m_cap1;
  unsigned int insertIndex = -1;
  bool dup = false;
  
  // loop thru HashTable1
  while (m_table1[currIndex] != NULL) {
    counter++;
    
    // look for DELETED
    if (m_table1[currIndex] == DELETED) {
      if (insertIndex == -1)
	insertIndex = currIndex;
    }
    
    // found duplicate
    else if ( strcmp(m_table1[currIndex], str) == 0)
      dup = true;
    

    currIndex = (currIndex + 1) % m_cap1;
  }
  
  // insert if NOT duplicate
  if (!dup) {
    if (insertIndex != -1)
      m_table1[insertIndex] = strdup(str);
    else
      m_table1[currIndex] = strdup(str);
    m_size1++;
  }

  // check rehash conditions
  if (counter >= 10) {
    m_status = 2;
    rehash();
  }
  
  if ( (m_size1/(double)m_cap1) > .5) {
    m_status = 2;
    rehash();
  }
}

// insertT2
void HashTable::insertT2(const char * str) {
  unsigned int hashIndex = hashCode(str);
  unsigned int currIndex = hashIndex % m_cap2; 
  unsigned int insertIndex = -1;
  bool dup = false;
  
  // loop thru HashTable2
  while (m_table2[currIndex] != NULL) {
    
    // check for duplicates
    if (m_table2[currIndex] == DELETED) {
      if (insertIndex == -1)
	insertIndex = currIndex;
    }
    // found duplicate
    else if ( strcmp(m_table2[currIndex], str) == 0)
      dup = true;
    
    currIndex = (currIndex + 1) % m_cap2;
  }
    
  // insert if NOT duplicate
  if (!dup) {
    if (insertIndex != -1)
      m_table2[insertIndex] = strdup(str);
    else
      m_table2[currIndex] = strdup(str);
    m_size2++;
  }
}
  
// find
bool HashTable::find(const char * str) {
  int counter1 = 1, counter2 = 1;
  unsigned int hashIndex = hashCode(str);
  unsigned int currIndex;
  bool found = false;
  
  // if rehashing - call cluste func.
  if (m_table0[hashIndex % m_cap0] != NULL && m_status == 1) {
    clusterRehash(hashIndex % m_cap0);
  }
  
  // if not rehashing
  if (m_status == 0) {
    currIndex = hashIndex % m_cap0;
    
    // check for DELETED
    while (m_table0[currIndex] != NULL) {
      counter1++;
      // don't find deleted
      if (m_table0[currIndex] == DELETED) {
      }
      // found deleted
      else if( strcmp(m_table0[currIndex], str) == 0) {
	found = true;
	break;
      }
      
      currIndex = (currIndex + 1) % m_cap0;
    }
    
    // check for rehash conditions
    if (counter1 >= 10) {
      m_status = 1;
      clusterRehash(hashIndex % m_cap0);
    }
    
    if ( (m_size0/(double)m_cap0) > .5) {
      m_status = 1;
      clusterRehash(hashIndex % m_cap0);
    }
  }
  
  // if rehashing
  else if (m_status == 1) {
    currIndex = hashIndex % m_cap1;
    
    // check for DELETED
    while (m_table1[currIndex] != NULL) {
      counter2++;
      
      // don't find deleted
      if (m_table1[currIndex] == DELETED) {}
      // found deleted
      else if ( strcmp(m_table1[currIndex], str) == 0) {
	found = true;
	break;
      }
      
      currIndex = (currIndex + 1) % m_cap1;
    }
    
    // check for rehash conditions
    if (counter2 >= 10) {
      m_status = 2;
      rehash();
      
    }
    
    if ( (m_size1/(double)m_cap1) > .5) {
      m_status = 2;
      rehash();
    }
  }
  
  return found;
}

// remove
char * HashTable::remove(const char *str) {
  int counter1 = 1, counter2 = 1;
  unsigned int hashIndex = hashCode(str);
  unsigned int currIndex;
  char * returnString = NULL;
  bool found = false;
  
  // if rehashing - call cluster func.
  if (m_table0[hashIndex % m_cap0] != NULL && m_status == 1) {
    clusterRehash(hashIndex % m_cap0);
  }
  
  // not rehashing - remove from HashTable0
  if (m_status == 0) { 
    currIndex = hashIndex % m_cap0;
    
    // loop thru to find elem.
    while(m_table0[currIndex] != NULL) {
      counter1++;
      if (m_table0[currIndex] == DELETED) {}
      // found elem.
      else if ( strcmp(m_table0[currIndex], str) == 0) {
	found = true;
	break;
      }
      
      currIndex = (currIndex + 1) % m_cap0;
    }

    // delete if found
    if (found) {
      returnString = m_table0[currIndex];
      m_table0[currIndex] = DELETED;
      m_size0--;
    }

    // check for rehash conditions
    if (counter1 >= 10) {
      m_status = 1;
      clusterRehash(hashIndex % m_cap0);
    }
    
    if ( (m_size0/(double)m_cap0) > .5) {
      m_status = 1;
      clusterRehash(hashIndex % m_cap0);
    }
  }
  
  // rehashing - remove from HashTable1
  else if (m_status == 1) {
    currIndex = hashIndex % m_cap1;
    
    // loop thru to find elem.
    while (m_table1[currIndex] != NULL) {
      counter2++;
      // don't find elem.
      if (m_table1[currIndex] == DELETED) {}
      // found elem.
      else if ( strcmp(m_table1[currIndex], str) == 0) {
	found = true;
	break;
      }
      
      currIndex = (currIndex + 1) % m_cap1;
    }
    
    // delete if found
    if (found) {
      returnString = m_table1[currIndex];
      m_table1[currIndex] = DELETED;
      m_size1--;
    }

    // check for rehash conditions
    if (counter2 >= 10) {
      m_status = 2;
      rehash();
    }
    
    if ( (m_size1/(double)m_cap1) > .5) {
      m_status = 2;
      rehash();
    }
  }
  
  return returnString;
}

// clusterRehash
void HashTable::clusterRehash(unsigned int index) {
  unsigned int newIndex = index;
  
  // create HashTable1
  if (m_table1 == NULL) {
    m_cap1 = getPrime(m_size0 * 4);
    m_size1 = 0;
    m_table1 = new char *[m_cap1]();
  }
  
  // move left rehash 
  while (m_table0[(newIndex - 1) % m_cap0] != NULL) {
    newIndex = (newIndex - 1) % m_cap0;
  }

  // move cluster to HashTable1
  while (m_table0[newIndex] != NULL && m_table1 != NULL) {
    if (m_table0[newIndex] != DELETED) {
      insertT1(m_table0[newIndex]);
      
      // second hash
      if (m_table1 != NULL) {
	free(m_table0[newIndex]);
	m_table0[newIndex] = NULL;
	
	m_size0--;
      }
    }
    
    newIndex = (newIndex + 1) % m_cap0;
  }
  
  // move left of elems.
  if ( (m_size0/(double)m_cap0) < .03) {
    while (m_size0 > 0) {
      
      if (m_table0[newIndex] != NULL && m_table0[newIndex] != DELETED && \
	  m_table1 != NULL) {
	insertT1(m_table0[newIndex]);
	
	if (m_table1 != NULL) {
	  free(m_table0[newIndex]);
	  m_table0[newIndex] = NULL;
	    
	  m_size0--;
	}
      }
      
      newIndex = (newIndex + 1) % m_cap0;
    }
    
    // set/create other HashTables
    m_status = 0;
    
    delete [] m_table0;
    m_table0 = m_table1;
    
    m_table1 = NULL;
    
    m_size0 = m_size1;
    m_cap0 = m_cap1; 
  }
}

// rehash
void HashTable::rehash() {
  unsigned int counter = 0;
  
  m_cap2 = getPrime((m_size0 + m_size1) * 4);
  m_size2 = 0;
  m_table2 = new char *[m_cap2]();
  
  // HashTable0 = HashTable2
  while (m_size0 > 0) {
    if (m_table0[counter] != NULL && m_table0[counter] != DELETED) {
      insertT2(m_table0[counter]);
      free(m_table0[counter]);
      m_table0[counter] = NULL;
      
      m_size0--;
    }
    
    counter = (counter + 1) % m_cap0;
  }
  delete [] m_table0;
  
  // HashTable1 = HashTable2
  if (m_table1 != NULL) {
    unsigned int counter1 = 0;
    
    while (m_size1 > 0) {
      if (m_table1[counter1] != NULL && m_table1[counter1] != DELETED) {
	insertT2(m_table1[counter1]);
	free(m_table1[counter1]);
	m_table1[counter1] = NULL;
	
	m_size1--;
      }
      
      counter1 = (counter1 + 1) % m_cap1;
    }
    delete [] m_table1;
  }
  
  // set/create other HashTables 
  m_table0 = m_table2;

  m_table1 = NULL;
  m_table2 = NULL;

  m_size0 = m_size2;
  m_cap0 = m_cap2;
  m_status = 0;
}

// at
const char * HashTable::at(int index, int table) {
  // HashTable0
  if (table == 0) {
    if (index >= m_cap0)
      throw out_of_range("Index out of range");
    else
      return m_table0[index];
  }
  
  // HashTable1
  else if (table == 1 ) {
    if (index >= m_cap1)
      throw out_of_range("Index out of range");
    else
      return m_table1[index];
  }

  // HashTable2
  else if (table == 2) {
    if (index >= m_cap2)
      throw out_of_range("Index out of range");
    else
      return m_table2[index];
  }
  
  else if (table < 0 || table > 2) {
    throw out_of_range("out of range");
  }
}

// dump 
void HashTable::dump() {
  cout << "HashTable #1: size = " << m_size0 \
       << ", tableSize = " << m_cap0 << endl;
  
  for (int i = 0; i < m_cap0 ; i++) {
    cout << "H1[" << i << "] = " ;
    
    if (m_table0[i] == DELETED)
      cout << "DELETED";
    
    else if (m_table0[i] != NULL)
      cout << m_table0[i] << " (" << hashCode(m_table0[i]) % m_cap0 << ")";
    cout << endl;
  }
  
  if (m_status) {
    cout << endl << endl << endl << "HashTable #2: size = " << m_size1 \
	 << ", tableSize = " << m_cap1 << endl;
    
    for(int i = 0; i < m_cap1 ; i++) {
      cout << "H2[" << i << "] = ";
      
      if (m_table1[i] == DELETED)
	cout << "DELETED" ;
      
      else if (m_table1[i] != NULL)
	cout << m_table1[i]  << " (" << hashCode(m_table1[i]) % m_cap1 << ")";
      cout << endl;
    }
  }
}

// isRehashing
bool HashTable::isRehashing() { return (m_status > 0); }

// tableSize
int HashTable::tableSize(int table) {
  if(table == 0)
    return m_cap0;
  else if(table == 1 )
    return m_cap1;
  else if(table == 2)
    return m_cap2;
  else
    throw out_of_range("out of range");
}

// size 
int HashTable::size(int table) {
  if(table == 0)
    return m_size0;
  else if(table == 1 )
    return m_size1;
  else if(table == 2)
    return m_size2;
  else
    throw out_of_range("out of range");
}
