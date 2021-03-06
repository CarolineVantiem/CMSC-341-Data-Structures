/*  Caroline Vantiem
    Cvantie1@umbc.edu
    CMSC 341 Fall 2018 - Proj4
*/

#ifndef MEDIAN_HEAP_H
#define MEDIAN_HEAP_H

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
using namespace std;

template <typename T>
class MedianHeap;

// Heap class
template <typename T>
class Heap {
  friend class MedianHeap<T>;
 public:
  Heap(int size, bool (*fpt) (const T&, const T&) ); // default constructor
  ~Heap();                                          // destructor
  Heap(const Heap<T> &other);                       // copy constructor
  
  // get size of array
  int getSize(); 

  // return min or max value 
  T getMinOrMax();

  // delete the first index - 0  
  void deleteFirstIndex(int data);  
  
  // insert value into array
  void insert(T data);
  
  // return true for empty
  bool isEmpty();
  
  // get the array
  T *getArray();
  
  // assignment operator
  const Heap<T>& operator=(const Heap<T> &rhs);
  
 private:
  // member variables 
  int m_heapSize;  
  int m_arraySize;

  // parent index
  int getParent(int parentValue);
  // left index
  int getLeft(int leftValue);
  // right index
  int getRight(int rightValue);

  // changeUp - change nodes
  void changeUp(int oldIndex);
  // changeDown
  void changeDown(int oldIndex);
  
  // variable for array
  T *m_array;
  
  // store function pointer
  bool (*compare) (const T&, const T&); 

};

// MedianHeap Class
template <typename T>
class MedianHeap {
  friend class Heap<T>;
  
 public:
  // default
  MedianHeap( bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), int cap=100) ;
  // destructor
  ~MedianHeap();
  // copy
  MedianHeap(const MedianHeap<T> &otherH);
  // overloaded
  const MedianHeap<T>& operator=(const MedianHeap<T>& rhs)  ;
  
  // number of items in medianheap
  int size();
  
  // max number of items that can be stored in medianheap
  int capacity();
  
  // insert item in medianheap
  void insert(const T& item);
  
  // returns median
  T getMedian();
  
  // returns min
  T getMin();

  // returns max
  T getMax();
  
  // delete item given in parameters 
  // true  - if value was found and removed
  // false - item was not found
  bool deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) );
  
  // dump for debugging
  void dump();

  // maxHeap size
  int maxHeapSize();
  
  // minHeap size
  int minHeapSize();

  // return item in position - maxHeap
  T locateInMaxHeap(int pos);
  
  // return item in position - minHeap
  T locateInMinHeap(int post);
  
 private:
  int m_size;
  int m_capacity;
  
  // template variables
  T m_median;
  T m_min;
  T m_max;
  
  // variables to store gt/lt funct. pointer
  bool (*grter)(const T&, const T&);
  bool (*less) (const T&, const T&);
  
  // min/max heap pointers
  Heap<T> *m_minHeap;
  Heap<T> *m_maxHeap;
  
};


// 
// HEAP FUNCTIONS HERE //
// 

// default constructor
template <typename T>
Heap<T>::Heap(int size, bool (*fpt) (const T&, const T&) ) {
  size = size + 2;
  m_array = new T[size];
  m_arraySize = size;
  m_heapSize = 0;
  compare = fpt;
}

// destructor
template <typename T>
Heap<T>::~Heap() { delete [] m_array; }

// copy constructor
template <typename T>
Heap<T>::Heap(const Heap<T> &other) {
  // copy variables
  m_arraySize = other.m_arraySize;
  m_heapSize = other.m_heapSize;
  compare = other.compare;
  
  // copy array contents
  m_array = new T[m_arraySize];
  for (int i = 1; i <= m_heapSize; i++) 
    m_array[i] = T(other.m_array[i]);
}

// getSize
template <typename T>
int Heap<T>::getSize() { return m_heapSize; }

// deleteFirst
template <typename T>
void Heap<T>::deleteFirstIndex(int data) {
  // if empty - dont remove 
  if ( isEmpty() == true) 
    throw out_of_range("EMPTY CANNOT REMOVE");
  
  else {
    // root = last element & fix heap
    m_array[data] = m_array[m_heapSize];
    m_heapSize--;
    
    // change if root was deleted
    if (m_heapSize > 0) 
      changeDown(data);
  }
}

// isEmpty
template <typename T>
bool Heap<T>::isEmpty() { return (m_heapSize == 0); }

// insert
template <typename T>
void Heap<T>::insert(T data) {
  int newHeapSize = m_heapSize + 1;
  // array full - no room to insert
  if (newHeapSize == m_arraySize) 
    throw out_of_range("CANNOT INSERT");
  
  else {
    // insert at index (+1)
    m_heapSize++;
    m_array[m_heapSize] = data;
  }
}

// getFirst
template <typename T>
T Heap<T>::getMinOrMax() {
  // if empty - no elements to return
  if (isEmpty() == true) 
    throw out_of_range("NO VALUE TO RETURN");
  // return at 1 - heap starts at 1
  else 
    return m_array[1];
}

// getArray
template <typename T>
T *Heap<T>::getArray() { return m_array; }

// const operator=
template <typename T>
const Heap<T>& Heap<T>::operator=(const Heap<T> &rhs) {
  delete [] m_array;
  
  // copy variables
  m_heapSize = rhs.m_heapSize;
  m_arraySize = rhs.m_arraySize;

  // copy array contents
  m_array = new T[m_arraySize];
  for (int i = 1; i <= m_heapSize; i++) {
    m_array[i] = T(rhs.m_array[i]);
  }
  
  return *this;
}
  
// get parent
template <typename T>
int Heap<T>::getParent(int parentValue) { return parentValue / 2; }

// get left
template <typename T>
int Heap<T>::getLeft(int leftValue) { return 2 * leftValue; }

// get right
template <typename T>
int Heap<T>::getRight(int rightValue) { return 2 * rightValue + 1; }

// changeUp
template <typename T>
void Heap<T>::changeUp(int oldIndex) {
  T temp;
  int parentValue;
  
  // not the root
  if (oldIndex != 1) {
    parentValue = getParent(oldIndex);
    
    // compare cur to parent - recursive
    if ( compare(m_array[oldIndex], m_array[parentValue]) ) {
      // swap m_array values
      temp = m_array[parentValue];
      m_array[parentValue] = m_array[oldIndex];
      m_array[oldIndex] = temp;
      changeUp(parentValue);
    }
  }
}

// changeDown
template <typename T>
void Heap<T>::changeDown(int oldIndex) {
  int leftIndex = getLeft(oldIndex), rightIndex = getRight(oldIndex);
  int compareIndex;
  T temp;
  
  // compare values
  if (rightIndex > m_heapSize) {
    
    // dont need to change
    if (leftIndex > m_heapSize) 
      return;
    else
      compareIndex = leftIndex;
  }

  else {
    if ( compare(m_array[rightIndex], m_array[leftIndex]) )
      compareIndex = rightIndex;
    else
      compareIndex = leftIndex;
  }

  if ( compare(m_array[compareIndex], m_array[oldIndex]) ) {
    temp = m_array[compareIndex];
    m_array[compareIndex] = m_array[oldIndex];
    m_array[oldIndex] = temp;
    changeDown(compareIndex);
  }
}
   
//
// MEDIANHEAP FUNCTIONS HERE //
//

// default constructor
template <typename T>
MedianHeap<T>::MedianHeap( bool (*lt) (const T&, const T&), bool (*gt) (const T&, const T&), \
			   int cap) {
  // set other default variables
  m_capacity = cap;  
  m_size = 0;
  
  grter = gt;
  less = lt;

  // create & set min/max heap values
  m_minHeap = new Heap<T>(cap/2, lt);
  m_maxHeap = new Heap<T>(cap/2, gt);
}

// destructor
template <typename T>
MedianHeap<T>::~MedianHeap() { delete m_minHeap; delete m_maxHeap; }

// copy constructor
template <typename T>
MedianHeap<T>::MedianHeap(const MedianHeap<T> &other) {  
  m_capacity = other.m_capacity;

  m_minHeap = new Heap<T> ( *(other.m_minHeap) );
  m_maxHeap = new Heap<T> ( *(other.m_maxHeap) );
  
  less = other.less;
  grter = other.grter;
}

// overloaded assignment operator =
template <typename T>
const MedianHeap<T>& MedianHeap<T>::operator=(const MedianHeap<T>& rhs) {
  // delete memory
  delete m_minHeap;
  delete m_maxHeap;
  
  m_capacity = rhs.m_capacity;
  
  grter = rhs.grter;
  less = rhs.less;

  m_minHeap = new Heap<T> ( *(rhs.m_minHeap) );
  m_maxHeap = new Heap<T> ( *(rhs.m_maxHeap) );
  
  return *this;
}
  
// size
template <typename T>
int MedianHeap<T>::size() {
  return m_size = m_minHeap->getSize() + m_maxHeap->getSize();
}

// capacity
template <typename T>
int MedianHeap<T>::capacity() { return m_capacity; }

// insert
template <typename T>
void MedianHeap<T>::insert(const T& item){
  m_size = size();
  
  // if medianHeap is full
  if(m_size == m_capacity){
    throw out_of_range("FULL");
  }
  
  // no elements - add to default
  if(m_size == 0){
    m_maxHeap->insert(item);
    m_maxHeap->changeUp(m_maxHeap->m_heapSize);
    m_min = item;
    m_max = item;
  }
  
  // compare item to median
  else{
    // item < median
    if( less(item, getMedian() ) ){
      m_maxHeap->insert(item);
      m_maxHeap->changeUp(m_maxHeap->m_heapSize);
    }
    
    // item >= median
    else {
      m_minHeap->insert(item);
      m_minHeap->changeUp(m_minHeap->m_heapSize);
    }
    
    // check for min or max
    if( less(item, m_min)){
      m_min = item;
    }
    else if( grter(item, m_max)){
      m_max = item;
    }
    
    // rebalance check
    
    // max has more than 2 elements
    if( (m_maxHeap->getSize() - m_minHeap->getSize() ) == 2) {
      T temp = m_maxHeap->getMinOrMax(); 
      m_maxHeap->deleteFirstIndex(1); 
      m_minHeap->insert(temp); 
      m_minHeap->changeUp(m_minHeap->m_heapSize);
    }
    
    // min has more than 2 elements
    else if( (m_minHeap->getSize() - m_maxHeap->getSize() ) == 2) { 
      //same thing as above
      T temp = m_minHeap->getMinOrMax();
      m_minHeap->deleteFirstIndex(1);
      m_maxHeap->insert(temp);
      m_maxHeap->changeUp(m_maxHeap->m_heapSize);
    }
  }
}

// getMedian
template <typename T>
T MedianHeap<T>::getMedian() {
  // if heap is empty
  if (m_size == 0) {
    throw out_of_range("EMPTY");
  }

  // if m_minHeap greater
  if (m_minHeap->m_heapSize > m_maxHeap->m_heapSize) {
    return m_minHeap->getMinOrMax();
  }

  else {
    return m_maxHeap->getMinOrMax();
  }
}

// getMin
template <typename T>
T MedianHeap<T>::getMin() {
  m_size = size();
  
  // if heap is empty
  if (m_size == 0) {
    throw out_of_range("EMPTY");
  }
  return m_min;
}
    
// getMax
template <typename T>
T MedianHeap<T>::getMax() {
  m_size = size();
  
  // if heap is empty
  if (m_size == 0) {
    throw out_of_range("EMPTY");
  }
  return m_max;
}

// deleteItem
template <typename T>
bool MedianHeap<T>::deleteItem(T& givenItem, bool (*equalTo) (const T&, const T&) ) {
  int deleteIndex = 1;
  
  m_size = size();
  
  // if empty
  if (m_size == 0) {
    throw out_of_range("EMPTY");
  }
  
  // iterate through max heap first
  for (int i = 1; i <= m_maxHeap->getSize(); i++) {
    if ( equalTo(givenItem, (m_maxHeap->m_array[i]) ) ) {
      givenItem = m_maxHeap->m_array[i];
      m_maxHeap->deleteFirstIndex(i);

      m_maxHeap->changeDown(i);
      m_maxHeap->changeUp(i);

      // rebalance check
    
      // max has more than 2 elements
      if ( ( m_maxHeap->getSize() - m_minHeap->getSize() ) == 2) {
	T temp = m_maxHeap->getMinOrMax();
	m_maxHeap->deleteFirstIndex(deleteIndex);   // remove max 
	m_minHeap->insert(temp);                    // insert removed value to min
	m_minHeap->changeUp(m_minHeap->m_heapSize); // rebalance
      }
      
      // min has 2 more than 2 elements
      else if ( ( m_minHeap->getSize() - m_maxHeap->getSize() ) == 2) {
	T temp = m_minHeap->getMinOrMax();
	m_minHeap->deleteFirstIndex(deleteIndex);
	m_maxHeap->insert(temp);
	m_maxHeap->changeUp(m_maxHeap->m_heapSize);
      }
      
      // check for deleted min
      if ( equalTo(givenItem, m_min) ) {
	m_min = m_maxHeap->m_array[deleteIndex];
	// search whole max heap for new min
	for (int i = 2; i < m_maxHeap->m_heapSize; i++) {
	  if ( less(m_maxHeap->m_array[i], m_min) ) {
	    m_min = m_maxHeap->m_array[i];
	  }
	}
      }
    
      return true;
  
    }
  }
 
  // iterate through min heap
  for (int i = 1; i < m_minHeap->getSize(); i++) {
    if ( equalTo(givenItem, (m_minHeap->m_array[i]) ) ) {
      givenItem = m_minHeap->m_array[i];
      m_minHeap->deleteFirstIndex(i);
      
      m_minHeap->changeDown(i);
      m_minHeap->changeUp(i);
      
      // rebalance check
      
      // max has more than 2 elements
      if ( ( m_maxHeap->getSize() - m_minHeap->getSize() ) == 2) {
	T temp = m_maxHeap->getMinOrMax();
	m_maxHeap->deleteFirstIndex(deleteIndex);   // remove max 
        m_minHeap->insert(temp);                    // insert removed value to min
        m_minHeap->changeUp(m_minHeap->m_heapSize); // rebalance
      }

      // min has 2 more than 2 elements 
      else if ( ( m_minHeap->getSize() - m_maxHeap->getSize() ) == 2) {
        T temp = m_minHeap->getMinOrMax();
        m_minHeap->deleteFirstIndex(deleteIndex);
        m_maxHeap->insert(temp);
        m_maxHeap->changeUp(m_maxHeap->m_heapSize);
      }
      
      // check for deleted max
      if ( equalTo(givenItem, m_max) ) {
	m_max = m_minHeap->m_array[deleteIndex];
	// search whole min heap for new max
	for (int i = 2; i < m_minHeap->m_heapSize; i++) {
	  if ( grter(m_minHeap->m_array[i], m_max) ) {
	    m_max = m_minHeap->m_array[i];
	  }
	}
      }
      
      return true;
      
    }
  }
  
  return false;
}
 
// dump
template <typename T>
void MedianHeap<T>::dump() {
  cout << "\n----MedianHeap::dump()----\n" << endl;
  cout << "-------Max Heap-------\n";
  
  cout << "size = " << " " << m_maxHeap->m_heapSize << ", capacity = " << m_maxHeap->m_arraySize << endl;
  
  for(int i = 1; i < m_maxHeap->m_heapSize + 1; i++) {
    cout << "Heap[" << i << "] =  (" << m_maxHeap->m_array[i] << ")" << endl;
  }

  cout << "\n-------Min Heap-------\n" << endl;

  cout << "size = " << m_minHeap->m_heapSize << ", capacity = " << m_minHeap->m_arraySize << endl;
  
  for(int i = 1; i < m_minHeap->m_heapSize + 1; i++) {
    cout << "Heap[" << i << "] = (" << m_minHeap->m_array[i] << ")" << endl; 
  }

  cout << "\n-----------------------\n\n";

  cout << "min    = " << getMin() << endl;
  cout << "median = " << getMedian() << endl;
  cout << "max    = " << getMax() << endl;

}

// maxHeapSize
template <typename T>
int MedianHeap<T>::maxHeapSize() { return m_maxHeap->m_heapSize; }

// minHeapSize
template <typename T>
int MedianHeap<T>::minHeapSize() { return m_minHeap->m_heapSize; }

// locateMax
template <typename T>
T MedianHeap<T>::locateInMaxHeap(int pos) {
  // if index is out of range
  if (pos < 1 || pos > m_maxHeap->m_heapSize) {
    throw out_of_range("INDEX OUT OF RANGE");
  }
  return m_maxHeap->m_array[pos];
}

// locateMin
template <typename T>
T MedianHeap<T>::locateInMinHeap(int pos){
  // if pos is out of range
  if(pos < 1 || pos > m_minHeap->m_heapSize){
    throw out_of_range("INDEX OUT OF RANGE");
  }
  return m_minHeap->m_array[pos];
}

#endif
