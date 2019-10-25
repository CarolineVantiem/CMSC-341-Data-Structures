#include <iostream>
#include <stdexcept>

#include "CBofCB.h"

using namespace std;

// default
CBofCB::CBofCB() {
  m_buffers[0] = new InnerCB();
  m_obSize = 1;
  m_oldest = 0;
  m_newest = 0;
}

// copy
CBofCB::CBofCB(const CBofCB& other) {
  int index = other.m_oldest;
  for (int i = 0; i < other.m_obSize; i++) {
    m_buffers[index] = new InnerCB(*other.m_buffers[index]);
    index++;
    
    if (index == m_obCapacity)
      index = 0;
  }

  m_obSize = other.m_obSize;
  m_oldest = other.m_oldest;
  m_newest = other.m_newest;
}

CBofCB::~CBofCB() {
  int index = m_oldest;
  for (int i = 0; i < m_obSize; i++) {
    delete m_buffers[index];
    m_buffers[index] = NULL;
    index++;
    
    if (index == m_obCapacity)
      index = 0;
  }
  
  m_obSize = NULL;
  m_oldest = NULL;
  m_newest = NULL;
}

void CBofCB::enqueue(int data) {
  if (isFull())
    throw overflow_error("Cannot add, buffer full");
  else {
    if (m_buffers[m_newest]->isFull()) {
      
      int oldNewest = m_newest;
      if (m_newest == (m_obCapacity-1))
        m_newest = 0;
      else
        m_newest++;
      m_buffers[m_newest] = new InnerCB(m_buffers[oldNewest]->capacity()*2);

      m_obSize++;
    }
    m_buffers[m_newest]->enqueue(data);
  }
}

int CBofCB::dequeue() {
  if (isEmpty())
    throw underflow_error("Cannot remove, buffer empty");
  else {
    if (m_obSize == 1)
      return m_buffers[m_oldest]->dequeue();
    if (m_buffers[m_oldest]->size() == 1) {
      int data = m_buffers[m_oldest]->dequeue();
      delete m_buffers[m_oldest];
      m_buffers[m_oldest] = NULL;
      m_obSize--;
      if (m_oldest == m_obCapacity-1)
        m_oldest = 0;
      else
        m_oldest++;
      
      return data;
    }
    return m_buffers[m_oldest]->dequeue();
  }
}

bool CBofCB::isFull() {
  if (m_obSize == m_obCapacity) {

    //checks if last circular buffer is full
    return m_buffers[m_newest]->isFull();
  }
  return false;
}

bool CBofCB::isEmpty() {
  if (m_obSize == 1) {
    
    //checks if inner buffer is empty 
    return m_buffers[m_oldest]->isEmpty();
  }
  
  return false;
}

int CBofCB::size() {
  int size = 0;
  int index = m_oldest;
  for (int i = 0; i < m_obSize; i++) {
    size += m_buffers[index]->size();
    index++;
    if (index == m_obCapacity)
      index = 0;
  }
  return size;
}

const CBofCB& CBofCB::operator=(const CBofCB& rhs) {
  if (&rhs == this)
    return *this;
  int index = m_oldest;
  for (int j = 0; j < m_obSize; j++) {
    delete m_buffers[index];
    m_buffers[index] = NULL;
    index++;
    if (index == m_obCapacity)
      index = 0;
  }
  m_obSize = NULL;
  m_oldest = NULL;
  m_newest = NULL;
  
  index = rhs.m_oldest;
  for (int i = 0; i < rhs.m_obSize; i++) {
    m_buffers[index] = new InnerCB(*rhs.m_buffers[index]);
    index++;
    if (index == m_obCapacity)
      index = 0;
  }
  m_obSize = rhs.m_obSize;
  m_oldest = rhs.m_oldest;
  m_newest = rhs.m_newest;

  return *this;
}

void CBofCB::dump() {
  cout << "Outer Circular buffer dump(), m_obSize = " << m_obSize << ":"  << endl;
  
  int index = m_oldest;
  for (int i = 0; i < m_obSize; i++) {
    m_buffers[index]->dump();
    index++;

    if (index == m_obCapacity)
      index = 0;
  }
}
    
