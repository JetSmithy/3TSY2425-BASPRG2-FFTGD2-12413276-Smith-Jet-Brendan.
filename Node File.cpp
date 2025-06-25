#ifndef NODE_H
#define NODE_H

#include <string>

struct Node
{
	std::string name;
	Node* next = NULL;
	Node* previous = NULL;
};

#endif //NODE_H