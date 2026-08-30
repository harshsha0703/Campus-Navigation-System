#include "canvaslist.h"

#include <iostream>

using namespace std;

// Default constructor that initializes an empty list.
CanvasList::CanvasList() : list_size(0), list_front(nullptr) {
}

// Copy constructor: copies all nodes and shapes from another list.
CanvasList::CanvasList(const CanvasList& other)
    : list_size(0), list_front(nullptr) {
  // Handle empty list case.
  if (other.list_front == nullptr) {
    return;
  }
  // Copy the first node and set up pointers.
  ShapeNode* current_other_node = other.list_front;
  ShapeNode* new_node = new ShapeNode();
  new_node->value = current_other_node->value->copy();
  new_node->next = nullptr;
  list_front = new_node;
  ShapeNode* tail = new_node;
  list_size++;
  current_other_node = current_other_node->next;

  // Copy the remaining nodes.
  while (current_other_node != nullptr) {
    ShapeNode* new_node = new ShapeNode();
    new_node->value = current_other_node->value->copy();
    new_node->next = nullptr;
    tail->next = new_node;
    tail = new_node;
    list_size++;
    current_other_node = current_other_node->next;
  }
}

// Assignment operator: clears current list and copies from another list.
CanvasList& CanvasList::operator=(const CanvasList& other) {
  if (this != &other) {
    clear();
    if (other.list_front == nullptr) {
      return *this;
    }

    // Copy the first node and initialize pointers.
    list_front = new ShapeNode();
    list_front->value = other.list_front->value->copy();
    list_front->next = nullptr;
    ShapeNode* tail = list_front;

    ShapeNode* current_node = other.list_front->next;

    // Copy remaining nodes.
    while (current_node != nullptr) {
      ShapeNode* new_node = new ShapeNode();
      new_node->value = current_node->value->copy();
      new_node->next = nullptr;
      tail->next = new_node;
      tail = new_node;
      current_node = current_node->next;
    }
    // Set the size of the copied list.
    list_size = other.list_size;
  }
  return *this;
}

// Destructor: frees all dynamically allocated memory.
CanvasList::~CanvasList() {
  clear();
}

// Deletes all nodes and their shapes from the list.
void CanvasList::clear() {
  while (list_front != nullptr) {
    ShapeNode* temp = list_front;
    list_front = list_front->next;
    delete temp->value;
    delete temp;
  }
  list_size = 0;
}

// Inserts a new shape after the given index in the list.
void CanvasList::insert_after(int index, Shape* shape) {
  if (index < 0 || index >= list_size) {
    return;
  }
  ShapeNode* new_node = new ShapeNode();
  new_node->value = shape;

  // If index is 0, insert after the first node.
  if (index == 0) {
    new_node->next = list_front->next;
    list_front->next = new_node;
  } else {
    ShapeNode* current_node = list_front;
    for (int i = 0; i < index; i++) {
      current_node = current_node->next;
    }
    new_node->next = current_node->next;
    current_node->next = new_node;
  }
  list_size++;
}

// Adds a new shape to the front of the list.
void CanvasList::push_front(Shape* newShape) {
  ShapeNode* new_Node = new ShapeNode();
  new_Node->value = newShape;
  new_Node->next = list_front;
  list_front = new_Node;
  list_size++;
}

// Adds a new shape to the back of the list.
void CanvasList::push_back(Shape* newShape) {
  ShapeNode* new_Node = new ShapeNode();
  new_Node->value = newShape;
  new_Node->next = nullptr;

  if (list_front == nullptr) {
    list_front = new_Node;
  } else {
    ShapeNode* current_node = list_front;
    while (current_node->next != nullptr) {
      current_node = current_node->next;
    }
    // Append new node at the end.
    current_node->next = new_Node;
  }
  list_size++;
}

// Removes the node at a given index and deletes its shape.
void CanvasList::remove_at(int index) {
  if (index < 0 || index >= list_size) {
    return;
  }
  if (index == 0) {
    ShapeNode* temp = list_front;
    list_front = list_front->next;
    delete temp->value;
    delete temp;
  } else {
    ShapeNode* current_node = list_front;
    for (int i = 0; i < index - 1; i++) {
      current_node = current_node->next;
    }
    ShapeNode* temp = current_node->next;
    current_node->next = temp->next;
    delete temp->value;
    delete temp;
  }
  list_size--;
}

// Removes every other node in the list.
void CanvasList::remove_every_other() {
  // If 0 or 1 node, nothing to remove.
  if (list_size <= 1) {
    return;
  }
  ShapeNode* current_node = list_front;
  while (current_node != nullptr && current_node->next != nullptr) {
    ShapeNode* temp = current_node->next;
    current_node->next = temp->next;
    delete temp->value;
    delete temp;
    // Move to the next node.
    current_node = current_node->next;
  }
  list_size = (list_size + 1) / 2;
}

// Removes and returns the shape at the front of the list.
Shape* CanvasList::pop_front() {
  if (list_size == 0) {
    return nullptr;
  }
  ShapeNode* temp = list_front;
  list_front = list_front->next;
  Shape* shape_to_return = temp->value;
  delete temp;
  list_size--;
  return shape_to_return;
}

// Removes and returns the shape at the back of the list.
Shape* CanvasList::pop_back() {
  if (list_size == 0) {
    return nullptr;
  }
  if (list_size == 1) {
    Shape* shape_to_return = list_front->value;
    delete list_front;
    list_front = nullptr;
    list_size--;
    return shape_to_return;
  }

  ShapeNode* current_node = list_front;
  while (current_node->next->next != nullptr) {
    current_node = current_node->next;
  }
  Shape* shape_to_return = current_node->next->value;
  delete current_node->next;
  current_node->next = nullptr;
  list_size--;
  return shape_to_return;
}

// Returns the first node in the list.
ShapeNode* CanvasList::front() const {
  return list_front;
}

// Checks if the list is empty.
bool CanvasList::empty() const {
  return list_size == 0;
}

// Returns the number of nodes in the list.
int CanvasList::size() const {
  return list_size;
}

// Finds the index of the shape at a given position (x, y).
int CanvasList::find(int x, int y) const {
  ShapeNode* current_node = list_front;
  int index = 0;
  while (current_node != nullptr) {
    if (current_node->value->getX() == x && current_node->value->getY() == y) {
      return index;
    }
    current_node = current_node->next;
    index++;
  }
  return -1;
}

// Returns the shape at the specified index.
Shape* CanvasList::shape_at(int index) const {
  if (index < 0 || index >= list_size) {
    return nullptr;
  }
  ShapeNode* current = list_front;
  for (int i = 0; i < index; i++) {
    current = current->next;
  }
  return current->value;
}

// Prints each shape in the list.
void CanvasList::draw() const {
  ShapeNode* current_node = list_front;
  while (current_node != nullptr) {
    cout << current_node->value->as_string() << endl;
    current_node = current_node->next;
  }
}
// Print Node and Shape Addresses
void CanvasList::print_addresses() const {
  ShapeNode* current_node = list_front;
  while (current_node != nullptr) {
    cout << "Node Address: " << current_node
         << "\tShape Address: " << current_node->value << endl;
    current_node = current_node->next;
  }
}
