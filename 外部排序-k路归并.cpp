#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <sstream>
#include <ctime> // ���ʱ��ͷ�ļ�
using namespace std;

const int BUFFER_SIZE = 10;

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

void sortAndWriteChunk(ifstream& input, int& chunkIndex, int& ioReadCount, int& ioWriteCount)
{
    vector<int> numbers;
    int number;

    // ���ļ��ж�ȡ���ֵ�������
    while (input >> number)
    {
        numbers.push_back(number);

        // �ﵽ��������С�󣬽�������д����ʱ�ļ�
        if (numbers.size() == BUFFER_SIZE)
        {
            ioReadCount++;
            sort(numbers.begin(), numbers.end());
            ofstream out("temp" + to_string(chunkIndex++) + ".txt");
            for (size_t i = 0; i < numbers.size(); ++i)
            {
                out << numbers[i] << " ";
            }
            ioWriteCount++; // ÿ��д���ļ�����д����
            out.close();
            numbers.clear(); // ��ջ�����
        }
    }

    // �����ʣ�����֣��������������д��
    if (!numbers.empty())
    {
        sort(numbers.begin(), numbers.end());
        ofstream out("temp" + to_string(chunkIndex++) + ".txt");
        for (size_t i = 0; i < numbers.size(); ++i)
        {
            out << numbers[i] << " ";
        }
        ioWriteCount++; // ÿ��д���ļ�����д����
        out.close();      
    }
}

void mergeChunks(int numChunks, const string& outputFile, int& ioReadCount, int& ioWriteCount)
{
    auto compare = [](const pair<int, int>& a, const pair<int, int>& b)
    {
        return a.first > b.first; // ��С��
    };
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(compare)> minHeap(compare);
    vector<ifstream> chunkFiles(numChunks);
    vector<int> currentNumbers(numChunks);

    // ��������ʱ�ļ�
    for (int i = 0; i < numChunks; ++i)
    {
        chunkFiles[i].open("temp" + to_string(i) + ".txt");
        if (chunkFiles[i] >> currentNumbers[i])
        {
            minHeap.push({ currentNumbers[i], i }); // {����, �ļ�����}
        }
    }

    ofstream output(outputFile);

    // �鲢����
    while (!minHeap.empty())
    {
        auto topElement = minHeap.top();
        int value = topElement.first;
        int chunkIndex = topElement.second;
        minHeap.pop();
        output << value << " "; // д�����յ������ļ�������֮���ÿո�ָ�

        // ����Ӧ����ʱ�ļ��ж�ȡ��һ������
        if (chunkFiles[chunkIndex] >> currentNumbers[chunkIndex])
        {
            minHeap.push({ currentNumbers[chunkIndex], chunkIndex });
        }
    }

    // �ر��ļ�
    for (int i = 0; i < numChunks; ++i)
    {
        chunkFiles[i].close();
    }
    output.close();
    ioWriteCount++; // д�����ʱ����д����
}

int main()
{
    const string inputFileName = "input.txt";
    const int N = 10000;

    // ��ʼ�� IO ������
    int ioReadCount = 0;
    int ioWriteCount = 0;

    generateInputFile(inputFileName, N);

    ifstream input(inputFileName);
    if (!input.is_open())
    {
        cerr << "�޷����ļ�!" << endl;
        return 1;
    }

    // ��һ�����ָ������ļ�������
    int chunkIndex = 0;
    sortAndWriteChunk(input, chunkIndex, ioReadCount, ioWriteCount);
    input.close();

    // �ڶ������鲢����������Ĳ���
    mergeChunks(chunkIndex, "output.txt", ioReadCount, ioWriteCount);

    cout << "������ɣ������д�� output.txt" << endl;
    cout << "�ļ���ȡ����: " << ioReadCount << endl;
    cout << "�ļ�д�����: " << ioWriteCount << endl;
    cout << ioReadCount + ioWriteCount << endl;

    return 0;
}
