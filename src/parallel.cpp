#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"// Include the stb_image header for image loading

#include <iostream>
#include <vector>
#include <chrono>
#include <pthread.h>
#include <cassert> // for assert

using namespace std;

const int NUM_THREADS = 14;

vector<vector<unsigned char>> image_data_2d;
int image_width = 0;
int image_height = 0;

// Per-thread histograms (each thread has its own 256-bin histogram)
vector<vector<int>> thread_histograms(NUM_THREADS, vector<int>(256, 0));

// Final histogram result (sum of all thread histograms)
vector<int> final_histogram_result(256, 0);

struct ThreadData {
    int thread_id;
    int start_row;
    int end_row;
};

// Function to calculate histogram in parallel
// Each thread processes a portion of the image
// and updates its own histogram
// After all threads finish, the histograms are combined
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

// Function to load a grayscale image using stb_image
// The image is loaded into a 2D vector
// Each pixel value is stored in the vector
bool loadImage(const string& filename) {
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

// Function to print the histogram result
// This function prints the histogram values for each pixel intensity
// from 0 to 255
void printHistogram() {
    for (int i = 0; i < 256; ++i) {
        cout << "Pixel Value " << i << ": " << final_histogram_result[i] << endl;
    }
}


// Function to test the histogram sum
// This function calculates the sum of all pixel values in the histogram
// and compares it to the expected total pixel count
void testHistogramSum(const vector<int>& histogram, int expected_total_pixels) {
    int sum = 0;
    for (int value : histogram) {
        sum += value;
    }

    cout << "Histogram total pixel count: " << sum << endl;
    cout << "Expected pixel count: " << expected_total_pixels << endl;

    // Assert that the total matches expected
    assert(sum == expected_total_pixels && "Histogram sum does not match image size!");
}


int main() {
     // Load the image
    string image_filename = "image_test.jpg"; 
    loadImage(image_filename); 

    

    cout << "--- Parallel Histogram Calculation for Grayscale Image ---" << endl;
    cout << "Image Size: " << image_width << "x" << image_height << endl;
    cout << "Number of Threads: " << NUM_THREADS << endl;

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    // divide the image into rows for each thread
    int rows_per_thread = image_height / NUM_THREADS;

    // Handle any remaining rows
    // that don't fit evenly into the number of threads
    // This ensures that all rows are processed
    int remaining_rows = image_height % NUM_THREADS;
    int current_row = 0;

    
    auto start_time = chrono::high_resolution_clock::now();

// Create threads to calculate histograms
    for (int i = 0; i < NUM_THREADS; ++i) {
        int extra_row = (remaining_rows > 0) ? 1 : 0;
        thread_data[i] = {i, current_row, current_row + rows_per_thread + extra_row};
        pthread_create(&threads[i], NULL, parallelHistogram, &thread_data[i]);
        current_row += rows_per_thread + extra_row;
        if (remaining_rows > 0) remaining_rows--;
    }
    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Combine histograms from all threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        for (int value = 0; value <= 255; ++value) {
            final_histogram_result[value] += thread_histograms[i][value];
        }
    }

    auto end_time = chrono::high_resolution_clock::now();

    // Calculate execution time
    chrono::duration<double> time_needed = end_time - start_time;

    cout << "\nExecution Time: " << time_needed.count() << " s\n\n";

    
    // Test histogram sum
    testHistogramSum(final_histogram_result, image_width * image_height);

   printHistogram();
    return 0;
}
