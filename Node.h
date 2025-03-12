#include<iostream>
#include<vector>
using namespace std;

class Node
{
public:
	int value;
	int index;
	bool vaild = true;

	Node(int v, int i) :value(v), index(i) {}
	Node() {}
};

