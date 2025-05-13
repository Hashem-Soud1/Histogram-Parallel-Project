#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <map>
#include <fstream>

using namespace std;

const int MIN_RANGE_VAL = 0;
const int MAX_RANGE_VAL = 10;
const int VALUE_RANGE = MAX_RANGE_VAL - MIN_RANGE_VAL + 1;

vector<int> data_array;

vector<int> histogram_Algorithm_data(VALUE_RANGE, 0);

void sequentialHistogram(const vector<int> &arr)
{

    for (auto value : arr)
    {
        histogram_Algorithm_data[value - MIN_RANGE_VAL]++;
    }
}

void printHistogram()
{

    for (int i = 0; i < VALUE_RANGE; ++i)
    {
        cout << "number " << (i + MIN_RANGE_VAL) << " : " << histogram_Algorithm_data[i] << " times" << endl;
    }
}

void loadArrayFromFile(const string &filename, vector<int> &arr)
{
    ifstream infile(filename);
    arr.clear();
    int number;
    while (infile >> number)
    {
        arr.push_back(number);
    }
    infile.close();
}

int main()
{

    // string input_filename = "data_10.txt";
    // string input_filename = "data_1k.txt";
    // string input_filename = "data_100k.txt";
    // string input_filename = "data_1M.txt";
    // string input_filename = "data_50M.txt";
    string input_filename = "data_100M.txt";

    loadArrayFromFile(input_filename, data_array);


    cout << endl
         << "--- Sequential Histogram Calculation ---" << endl;
    cout << "Array Size: " << data_array.size() << ", Value Range: [" << MIN_RANGE_VAL << ", " << MAX_RANGE_VAL << "]" << endl;
    cout << "Number of Threads: " << 1 << endl;
    auto start_time = chrono::high_resolution_clock::now();

    sequentialHistogram(data_array);

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> time_nedeed = end_time - start_time;

    cout << endl
         << "Execution Time: " << time_nedeed.count() << "seconds" << endl
         << endl;

    printHistogram();

    return 0;
}