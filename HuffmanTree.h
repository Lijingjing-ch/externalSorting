#include<iostream>
#include<vector>
#include <fstream>
#include <string>
#include <algorithm>
#include"HuffmanNode.h"

const int BUFFER_SIZE = 3;

class HuffmanTree
{
public:
    vector<HuffmanNode> tree;
    int ioReadCount = 0;  // To count read operations
    int ioWriteCount = 0; // To count write operations

    static bool compareByAttribute1(const HuffmanNode& a, const HuffmanNode& b)
    {
        return a.length < b.length;
    }

    // Function to count the number of data entries in the file
    int countData(const string& filename)
    {
        ifstream input(filename);
        int count = 0;
        int num;
        while (input >> num)
        {
            count++;
        }
        return count;
    }

    HuffmanTree(int num)
    {
        tree.resize(num);
        for (int i = 0; i < num; i++)
        {
            string file = to_string(0) + "temp" + to_string(i) + ".txt";
            int length = countData(file);
            HuffmanNode node(length, file);
            tree[i] = node;
        }
        sort(tree.begin(), tree.end(), compareByAttribute1);
    }

    // Function to merge two chunks into an output file
    void mergeChunks(const string& file1, const string& file2, const string& outputFile)
    {
        ifstream input1(file1);
        ifstream input2(file2);
        ofstream output(outputFile);

        vector<int> buffer1(BUFFER_SIZE);
        vector<int> buffer2(BUFFER_SIZE);
        size_t index1 = 0, index2 = 0;

        while (index1 < BUFFER_SIZE && input1 >> buffer1[index1])
        {
            index1++;
        }
        ioReadCount++;
        while (index2 < BUFFER_SIZE && input2 >> buffer2[index2])
        {
            index2++;
        }
        ioReadCount++;

        while (index1 > 0 || index2 > 0)
        {
            if (index1 == 0)
            {
                output << buffer2[0] << " ";
                copy(buffer2.begin() + 1, buffer2.begin() + index2, buffer2.begin());
                index2--;
                if (input2 >> buffer2[index2])
                {
                    index2++;
                }
            }
            else if (index2 == 0)
            {
                output << buffer1[0] << " ";
                copy(buffer1.begin() + 1, buffer1.begin() + index1, buffer1.begin());
                index1--;
                if (input1 >> buffer1[index1])
                {
                    index1++;
                }
            }
            else if (buffer1[0] < buffer2[0])
            {
                output << buffer1[0] << " ";
                copy(buffer1.begin() + 1, buffer1.begin() + index1, buffer1.begin());
                index1--;
                if (input1 >> buffer1[index1])
                {
                    index1++;
                }
            }
            else
            {
                output << buffer2[0] << " ";
                copy(buffer2.begin() + 1, buffer2.begin() + index2, buffer2.begin());
                index2--;
                if (input2 >> buffer2[index2])
                {
                    index2++;
                }
            }
        }

        input1.close();
        input2.close();
        output.close();
    }

    int merge()
    {
        int fileIndex = 1;
        while (tree.size() > 1)
        {
            string file1 = tree[0].filename;
            string file2 = tree[1].filename;
            string file = "temp" + to_string(fileIndex) + ".txt";
            mergeChunks(file1, file2, file);

            HuffmanNode node(tree[0].length + tree[1].length, file);
            tree = vector<HuffmanNode>(tree.begin() + 2, tree.end());
            tree.push_back(node);
            sort(tree.begin(), tree.end(), compareByAttribute1);
            fileIndex++;
        }
        return fileIndex;
    }

};

