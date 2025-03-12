#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <filesystem>
#include <cstdlib> 
#include <ctime>  
using namespace std;

const int BUFFER_SIZE = 5;  
const int RUN_SIZE = 350;


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

// Function to sort and write a chunk to a temporary file
void sortAndWriteChunk(ifstream& input, int chunkIndex)
{
    vector<int> buffer;
    int num;

    // Read a chunk of data
    while (buffer.size() < RUN_SIZE && input >> num) 
    {
        buffer.push_back(num);
    }
    ioReadCount++;

    // If no data was read, return
    if (buffer.empty()) return;

    // Sort the chunk
    sort(buffer.begin(), buffer.end());

    // Write sorted data to a temporary file
    ofstream tempFile(to_string(0) + "temp" + to_string(chunkIndex) + ".txt");
    for (int val : buffer) 
    {
        tempFile << val << " ";
    }
    ioWriteCount++;
    tempFile.close();
}

void mergeChunks(const string& file1, const std::string& outputFile) 
{
    filesystem::copy(file1, outputFile);
    ioReadCount++;  
    ioWriteCount++; 
    return;
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

// Function to perform two-way merge of sorted chunks
int twoWayMerge(int totalChunks) 
{
    int currentChunks = totalChunks;  
    int round = 1;  

    while (currentChunks > 1) 
    {
        int newChunks = (currentChunks + 1) / 2;  
        for (int i = 0; i < currentChunks; i += 2) 
        {
            string file1 = to_string(round - 1) + "temp" + to_string(i) + ".txt";
            if (i + 1 >= currentChunks) 
            {
                string outputFile = to_string(round) + "temp" + to_string(i / 2) + ".txt";
                mergeChunks(file1, outputFile);
            }
            else
            {
                string file2 = to_string(round - 1) + "temp" + to_string(i + 1) + ".txt";
                string outputFile = to_string(round) + "temp" + to_string(i / 2) + ".txt";
                mergeChunks(file1, file2, outputFile);
            }
        }
        currentChunks = newChunks; 
        round++;  
    }
    return round - 1;
}

int main() 
{
    const string inputFileName = "input.txt";
    const int N = 8000; 

    generateInputFile(inputFileName, N);

    ifstream input(inputFileName);
    int totalData = countData(inputFileName);
    int totalChunks = (totalData + RUN_SIZE - 1) / RUN_SIZE;

    input.clear(); 
    input.seekg(0); 

    // Sort and write each chunk
    for (int i = 0; i < totalChunks; i++) 
    {
        sortAndWriteChunk(input, i);
    }

    // Merge all chunks
    int num = twoWayMerge(totalChunks);

    // Build output file name
    string tempFileName = to_string(num) + "temp0.txt";
    string outputFileName = "output.txt";

    // Rename the temporary file to output file
    filesystem::rename(tempFileName, outputFileName);

    cout << "Sorting and merging completed. Output written to " << outputFileName << endl;
    cout << "Total I/O Read Count: " << ioReadCount << endl;
    cout << "Total I/O Write Count: " << ioWriteCount << endl;
    cout << ioReadCount + ioWriteCount << endl;
}

