#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <sstream>
#include <ctime> // 添加时间头文件
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

    // 从文件中读取数字到缓冲区
    while (input >> number)
    {
        numbers.push_back(number);

        // 达到缓冲区大小后，进行排序并写入临时文件
        if (numbers.size() == BUFFER_SIZE)
        {
            ioReadCount++;
            sort(numbers.begin(), numbers.end());
            ofstream out("temp" + to_string(chunkIndex++) + ".txt");
            for (size_t i = 0; i < numbers.size(); ++i)
            {
                out << numbers[i] << " ";
            }
            ioWriteCount++; // 每次写入文件增加写计数
            out.close();
            numbers.clear(); // 清空缓冲区
        }
    }

    // 如果有剩余数字，进行最后的排序和写入
    if (!numbers.empty())
    {
        sort(numbers.begin(), numbers.end());
        ofstream out("temp" + to_string(chunkIndex++) + ".txt");
        for (size_t i = 0; i < numbers.size(); ++i)
        {
            out << numbers[i] << " ";
        }
        ioWriteCount++; // 每次写入文件增加写计数
        out.close();      
    }
}

void mergeChunks(int numChunks, const string& outputFile, int& ioReadCount, int& ioWriteCount)
{
    auto compare = [](const pair<int, int>& a, const pair<int, int>& b)
    {
        return a.first > b.first; // 最小堆
    };
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(compare)> minHeap(compare);
    vector<ifstream> chunkFiles(numChunks);
    vector<int> currentNumbers(numChunks);

    // 打开所有临时文件
    for (int i = 0; i < numChunks; ++i)
    {
        chunkFiles[i].open("temp" + to_string(i) + ".txt");
        if (chunkFiles[i] >> currentNumbers[i])
        {
            minHeap.push({ currentNumbers[i], i }); // {数字, 文件索引}
        }
    }

    ofstream output(outputFile);

    // 归并过程
    while (!minHeap.empty())
    {
        auto topElement = minHeap.top();
        int value = topElement.first;
        int chunkIndex = topElement.second;
        minHeap.pop();
        output << value << " "; // 写入最终的有序文件，数字之间用空格分隔

        // 从相应的临时文件中读取下一个数字
        if (chunkFiles[chunkIndex] >> currentNumbers[chunkIndex])
        {
            minHeap.push({ currentNumbers[chunkIndex], chunkIndex });
        }
    }

    // 关闭文件
    for (int i = 0; i < numChunks; ++i)
    {
        chunkFiles[i].close();
    }
    output.close();
    ioWriteCount++; // 写入完成时增加写计数
}

int main()
{
    const string inputFileName = "input.txt";
    const int N = 10000;

    // 初始化 IO 计数器
    int ioReadCount = 0;
    int ioWriteCount = 0;

    generateInputFile(inputFileName, N);

    ifstream input(inputFileName);
    if (!input.is_open())
    {
        cerr << "无法打开文件!" << endl;
        return 1;
    }

    // 第一步：分割输入文件并排序
    int chunkIndex = 0;
    sortAndWriteChunk(input, chunkIndex, ioReadCount, ioWriteCount);
    input.close();

    // 第二步：归并所有已排序的部分
    mergeChunks(chunkIndex, "output.txt", ioReadCount, ioWriteCount);

    cout << "排序完成，结果已写入 output.txt" << endl;
    cout << "文件读取次数: " << ioReadCount << endl;
    cout << "文件写入次数: " << ioWriteCount << endl;
    cout << ioReadCount + ioWriteCount << endl;

    return 0;
}
