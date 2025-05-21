#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <pthread.h>

using namespace std;

const int NUM_THREADS = 14;

vector<vector<unsigned char>> image_data_2d;
int image_width = 0;
int image_height = 0;

vector<vector<int>> thread_histograms(NUM_THREADS, vector<int>(256, 0));
vector<int> final_histogram_result(256, 0);

struct ThreadData {
    int thread_id;
    int start_row;
    int end_row;
};

void* parallelHistogram(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    int start_r = data->start_row;
    int end_r = data->end_row;
    int id = data->thread_id;

    for (int r = start_r; r < end_r; ++r) {
        for (int c = 0; c < image_width; ++c) {
            thread_histograms[id][static_cast<int>(image_data_2d[r][c])]++;
        }
    }
    return NULL;
}

bool loadGrayscaleImage(const string& filename) {
    int channels;
    unsigned char* img_data = stbi_load(filename.c_str(), &image_width, &image_height, &channels, 1); 
    if (!img_data) {
        cerr << "Failed to load image: " << filename << endl;
        return false;
    }

    image_data_2d.assign(image_height, vector<unsigned char>(image_width));
    for (int r = 0; r < image_height; ++r) {
        for (int c = 0; c < image_width; ++c) {
            image_data_2d[r][c] = img_data[r * image_width + c];
        }
    }

    stbi_image_free(img_data);
    return true;
}

int main() {
    string image_path = "image_test.jpg"; 

    if (!loadGrayscaleImage(image_path)) {
        return 1;
    }

    cout << "--- Parallel Histogram Calculation for Grayscale Image ---" << endl;
    cout << "Image Size: " << image_width << "x" << image_height << endl;
    cout << "Number of Threads: " << NUM_THREADS << endl;

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    int rows_per_thread = image_height / NUM_THREADS;
    int remaining_rows = image_height % NUM_THREADS;
    int current_row = 0;

    auto start_time = chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; ++i) {
        int extra_row = (remaining_rows > 0) ? 1 : 0;
        thread_data[i] = {i, current_row, current_row + rows_per_thread + extra_row};
        pthread_create(&threads[i], NULL, parallelHistogram, &thread_data[i]);
        current_row += rows_per_thread + extra_row;
        if (remaining_rows > 0) remaining_rows--;
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        for (int value = 0; value <= 255; ++value) {
            final_histogram_result[value] += thread_histograms[i][value];
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> time_needed = end_time - start_time;

    cout << "\nExecution Time: " << time_needed.count() << " s\n\n";

    return 0;
}
