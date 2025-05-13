#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <pthread.h>
#include <ctime>

using namespace std;

const int MIN_RANGE_VAL = 0;
const int MAX_RANGE_VAL = 10;
const int NUM_THREADS = 2;



vector<int> data_array;

vector<vector<int>> thread_histograms(NUM_THREADS, vector<int>(MAX_RANGE_VAL + 1, 0));

vector<int> final_histogram_result(MAX_RANGE_VAL + 1, 0);

struct ThreadData {
    int thread_id;
    int start_index;
    int end_index;
};



void* parallelHistogram(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    int start = data->start_index;
    int end = data->end_index;
    int id = data->thread_id;


    for (int i = start; i < end; ++i) {
        thread_histograms[id][data_array[i]]++;
    }

return NULL;
}

void printFinalHistogram(const vector<int>& histogram) {
    for (int value = MIN_RANGE_VAL; value <= MAX_RANGE_VAL; ++value) {
        if (histogram[value] > 0) {
            cout << "Number " << value << " repeat " << histogram[value] << " times\n";
        }
    }
}
void loadArrayFromFile(const string& filename, vector<int>& arr) {
    ifstream infile(filename); 
    arr.clear(); 
    int number;
    while (infile >> number) { 
        arr.push_back(number);
    }
    infile.close(); 
}

int main() {


    // string input_filename = "data_100k.txt";
    // string input_filename = "data_1M.txt";
    string input_filename = "data_50M.txt";
    // string input_filename = "data_100M.txt";


    loadArrayFromFile(input_filename, data_array);
    int ARRAY_SIZE = data_array.size();



    cout << "--- Parallel Histogram Calculation ---" << endl;
    cout << "Array Size: " << ARRAY_SIZE << "  , Value Range: [" << MIN_RANGE_VAL << ", " << MAX_RANGE_VAL << "]" << endl;
    cout << "Number of Threads: " << NUM_THREADS << endl;
    cout << "Generating array..." << endl;


    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    int chunk_size = ARRAY_SIZE / NUM_THREADS;
    int remaining =ARRAY_SIZE % NUM_THREADS;
    int current_index = 0;

    auto start_time = chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        int extra = (remaining > 0) ? 1 : 0;
        thread_data[i] = {i, current_index, current_index + chunk_size + extra};
        pthread_create(&threads[i], NULL, parallelHistogram, &thread_data[i]);
        current_index += chunk_size + extra;
        if (remaining > 0) remaining--;
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Merge thread histograms into final histogram
    for (int i = 0; i < NUM_THREADS; ++i) {
        for (int value = MIN_RANGE_VAL; value <= MAX_RANGE_VAL; ++value) {
            final_histogram_result[value] += thread_histograms[i][value];
        }
    }

    auto end_time = chrono::high_resolution_clock::now();

    chrono::duration<double> time_nedeed = end_time - start_time;

    cout << "\nExecution Time: " << time_nedeed.count() << " seconds\n\n";

    printFinalHistogram(final_histogram_result);


    return 0;
}
