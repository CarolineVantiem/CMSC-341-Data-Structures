#include <iostream>
#include "InnerCB.h"
#include <stdexcept>

using namespace std;

InnerCB::InnerCB(int n) {
  m_buffer = new int[n];
  m_capacity = n;
  m_size = 0;
  m_start = 0;
  m_end = 0;
}

InnerCB::InnerCB(const InnerCB& other) {

  //dynamically allocates memory      
  m_buffer = new int[other.m_capacity];

  int index = other.m_start;
  for (int i = 0; i < other.m_size; i++) {

    m_buffer[index] = other.m_buffer[index];

    index++;
    if (index == other.m_capacity)
      index = 0;
  }
  m_capacity = other.m_capacity;
  m_size = other.m_size;
  m_start = other.m_start;
  m_end = other.m_end;
}

InnerCB::~InnerCB() {
  delete[] m_buffer;
  m_buffer = NULL;
  m_capacity = NULL;
  m_size = NULL;
  m_start = NULL;
  m_end = NULL;
}

void InnerCB::enqueue(int data) {
  if (isFull())
    throw overflow_error("Buffer is full, cannot add");
  else {
    if (isEmpty())
      m_buffer[m_start] = data;
    else {
      if (m_end == (m_capacity-1))
        m_end = 0;
      else
        m_end++;
      m_buffer[m_end] = data;
    }
    m_size++;
  }
}

int InnerCB::dequeue() {
  if (isEmpty())
    throw underflow_error("Buffer is empty, cannot remove");
  else {
    int data = m_buffer[m_start];
    m_buffer[m_start] = NULL;
    if (m_start == (m_capacity-1))
      m_start = 0;
    else
      m_start++;
    m_size--;
    return data;
  }
}

bool InnerCB::isFull() {
  return m_capacity == m_size;
}

bool InnerCB::isEmpty() {
  return m_size == 0;
}

int InnerCB::capacity() {
  return m_capacity;
}

int InnerCB::size() {
  return m_size;
}

const InnerCB& InnerCB::operator=(const InnerCB& rhs) {
  if (&rhs == this)
    return *this;
  delete[] m_buffer;
  m_buffer = NULL;
  m_capacity = NULL;
  m_size = NULL;
  m_start = NULL;
  m_end = NULL;
  m_buffer = new int[rhs.m_capacity];
  int index = rhs.m_start;

  for (int i = 0; i < rhs.m_size; i++) {
    m_buffer[index] = rhs.m_buffer[index];

    index++;
    if (index == rhs.m_capacity)
      index = 0;
  }
  m_capacity = rhs.m_capacity;
  m_size = rhs.m_size;
  m_start = rhs.m_start;
  m_end = rhs.m_end;

  return *this;
}

void InnerCB::dump() {
  cout << "InnerCB dump(): m_size =  " << m_size << endl;
  int index = m_start;
  for (int i = 0; i < m_size; i++) {
    cout << "[" << index << "] " << m_buffer[index] << ", ";
    index++;
    if (index == m_capacity)
      index = 0;
  }
  cout << endl;
}



