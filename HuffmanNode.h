#include<iostream>
#include<vector>
using namespace std;

class HuffmanNode
{
public:
	int length;
	string filename;

	HuffmanNode(int l, string f) :length(l), filename(f) {}
	HuffmanNode() {}

};
