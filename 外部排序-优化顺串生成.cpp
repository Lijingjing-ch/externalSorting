#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <queue>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cstdlib> 
#include <ctime> 
#include <algorithm>
#include "LoserTree.h"
#include"HuffmanTree.h"

const int leaf = 8;

int ioReadCount = 0;  // To count read operations
int ioWriteCount = 0; // To count write operations


// Function to generate an input file with N random numbers
void generateInputFile(const string& filename, int N)
{
    ofstream output(filename);
    if (!output.is_open())
    {
        cerr << "Error creating input file." << endl;
        return;
    }
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    for (int i = 0; i < N; ++i)
    {
        output << rand() % 100 << " ";
    }
    output.close();
}

int generateRun(ifstream& input)
{
    vector<int> inputbuffer;
    vector<int> outputbuffer;
    int num;

    while (inputbuffer.size() < leaf && input >> num)
    {
        inputbuffer.push_back(num);
    }
    ioReadCount++;

    LoserTree lt(leaf, inputbuffer);
    Node win = lt.build();
    outputbuffer.push_back(win.value);

    inputbuffer.clear();

    int i = 0;// runIndex

    while (true)
    {
        while (lt.checkVaild())
        {
            if (inputbuffer.empty())
            {
                while (inputbuffer.size() < leaf && input >> num)
                {
                    inputbuffer.push_back(num);
                }
                ioReadCount++;
                if (input.eof())
                {
                    goto label;
                }
            }
            if (!inputbuffer.empty())
            {
                num = inputbuffer.back();
                inputbuffer.pop_back();
                win = lt.update(win.index, num);
                outputbuffer.push_back(win.value);
            }

        } // One run is generated
        outputbuffer.pop_back();

        ofstream tempFile(to_string(0) + "temp" + to_string(i) + ".txt");
        for (int val : outputbuffer)
        {
            tempFile << val << " ";
        }
        ioWriteCount++;
        tempFile.close();
        outputbuffer.clear();
        lt.max = 0;


        lt.turntrue();
        win = lt.build();
        outputbuffer.push_back(win.value);
        i++;

    }

label:
    int index = win.index;
    for (int i = leaf - 1; i < 2 * leaf - 1; i++)
    {
        if (lt.tree[i].vaild == true && i != index)
        {
            outputbuffer.push_back(lt.tree[i].value);
        }
    }
    sort(outputbuffer.begin(), outputbuffer.end());

    ofstream tempFile(to_string(0) + "temp" + to_string(i) + ".txt");

    for (int val : outputbuffer)
    {
        tempFile << val << " ";
    }
    ioWriteCount++;
    tempFile.close();
    i++;

    vector<int> remain;
    for (int i = leaf - 1; i < 2 * leaf - 1; i++)
    {
        if (lt.tree[i].vaild == false)
        {
            remain.push_back(lt.tree[i].value);
        }
    }
    sort(remain.begin(), remain.end());


    ofstream tempFile1(to_string(0) + "temp" + to_string(i) + ".txt");
    for (int val : remain)
    {
        tempFile1 << val << " ";
    }
    ioWriteCount++;
    tempFile1.close();

    return i;
}

void huffmanTreeMerge(int totalChunks)
{
    HuffmanTree ht(totalChunks);
    int num = ht.merge();
    string oldFileName = "temp" + to_string(num - 1) + ".txt";
    string newFileName = "output.txt";

    ioReadCount += ht.ioReadCount;
    ioWriteCount += ht.ioWriteCount;

    filesystem::rename(oldFileName, newFileName);

}

int main()
{
    const string inputFileName = "input.txt";
    const int N = 50;

    generateInputFile(inputFileName, N);

    ifstream input("input.txt");
    input.clear();
    input.seekg(0);
    int num = generateRun(input);

    huffmanTreeMerge(num + 1);

    cout << "ioRead count " << ioReadCount << endl;
    cout << "ioWrite count " << ioWriteCount << endl;
    cout << ioReadCount + ioWriteCount << endl;
}