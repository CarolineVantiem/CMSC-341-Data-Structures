#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include <stdlib.h>
#include <stdexcept>
using namespace std;

template <typename T>
class Heap {
 public:
  Heap(int size, bool (*fpt) (const T&, const T&));
  ~Heap();
  T getFirst(); // return min or max value 
  bool isEmpty();
  void insert(T value);
  void deleteFirst();
  T getValue(int index); 
  T *getArray();
  
 private:
  T *data; 
  int heapSize;
  int arraySize;
  bool (*compare)(const T&, const T&);
  
  int getLeftChildIndex(int nodeIndex);
  int getRightChildIndex(int nodeIndex);
  int getParentIndex(int nodeIndex);
  void siftUp(int nodeIndex);
  void siftDown(int nodeIndex);
};

//
// FUNCTIONS HERE //
//

// constructor
template <typename T>
Heap<T>::Heap(int size, bool (*fpt) (const T&, const T&)) {
  data = new int[size+2];
  heapSize = 0;
  arraySize = size;
  compare = fpt;
}

// destructor
template <typename T>
Heap<T>::~Heap() {
  delete[] data;
}

// getfirst 
template <typename T>
T Heap<T>::getFirst() {
  if (isEmpty())
    throw out_of_range("nothing there");
  
  else
    return data[1];
}

// isEmpty
template <typename T>
bool Heap<T>::isEmpty() {
  return (heapSize == 0);
}

// insert
template <typename T>
void Heap<T>::insert(T value) {
  if ((heapSize+1) == arraySize)
    throw out_of_range("nothing here");
  
  else {
    heapSize++;
    data[heapSize] = value;
    siftUp(heapSize);
  }
}

// deleteFirst
template <typename T>
void Heap<T>::deleteFirst() {
  if (isEmpty())
    throw out_of_range("nothing there");
  
  else {
    data[1] = data[heapSize];
    heapSize--;
    if (heapSize > 0)
      siftDown(1);
  }
}

// get value                                                                                       
template<typename T>
T Heap<T>::getValue(int index){
  return data[index];
}

// getArray
template <typename T>
T* Heap<T>::getArray(){
  return data;
}

// getLeftChildIndex
template <typename T>
int Heap<T>::getLeftChildIndex(int nodeIndex) {
  return 2 * nodeIndex;
}

// getRightChildIndex
template <typename T>
int Heap<T>::getRightChildIndex(int nodeIndex) {
  return 2 * nodeIndex + 1;
}

// getParentIndex
template <typename T>
int Heap<T>::getParentIndex(int nodeIndex) {
  return nodeIndex / 2;
}

// siftUp
template <typename T>
void Heap<T>::siftUp(int nodeIndex) {
  int parentIndex, tmp;
  
  if (nodeIndex != 1) { //if the value we just inserted is not the root
    parentIndex = getParentIndex(nodeIndex);
    
    if ( compare(data[nodeIndex], data[parentIndex]) ) {
      tmp = data[parentIndex];
      data[parentIndex] = data[nodeIndex];
      data[nodeIndex] = tmp;
      siftUp(parentIndex);
    }
  }
}

// siftDown
template <typename T>
void Heap<T>::siftDown(int nodeIndex) {
  int leftChildIndex, rightChildIndex, compareIndex, tmp;
  
  leftChildIndex = getLeftChildIndex(nodeIndex);
  rightChildIndex = getRightChildIndex(nodeIndex);
  
  if (rightChildIndex > heapSize) {
    if (leftChildIndex > heapSize)
      return;
    else
      compareIndex = leftChildIndex;
  } 
  else {
    if ( compare(data[rightChildIndex], data[leftChildIndex]) )
      compareIndex = rightChildIndex;
    else
      compareIndex = leftChildIndex;
  }
  
  if ( compare(data[compareIndex], data[nodeIndex]) ) {
    tmp = data[compareIndex];
    data[compareIndex] = data[nodeIndex];
    data[nodeIndex] = tmp;
    siftDown(compareIndex);
  }
}



























#endif
