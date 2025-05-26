#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>

using namespace std;

// image data in 2D vector
vector<vector<unsigned char>> image_data_2d;

// The histogram data for pixel values from 0 to 255
vector<int> histogram_Algorithm_data(256, 0);
int image_width = 0;
int image_height = 0;


// Function to load an image using stb_image
// and convert it to grayscale
void loadImage(const string &filename) {
    int channels;
    unsigned char *data = stbi_load(filename.c_str(), &image_width, &image_height, &channels, 1); 
    if (!data) {
        cerr << "Error loading image!" << endl;
        exit(1);
    }

    image_data_2d.assign(image_height, vector<unsigned char>(image_width));
    for (int r = 0; r < image_height; ++r) {
        for (int c = 0; c < image_width; ++c) {
            image_data_2d[r][c] = data[r * image_width + c];
        }
    }

    stbi_image_free(data);
}

// Function to calculate histogram sequentially
// This function iterates through the image data and counts pixel values
void sequentialHistogram() {
    for (int r = 0; r < image_height; ++r) {
        for (int c = 0; c < image_width; ++c) {
            histogram_Algorithm_data[image_data_2d[r][c]]++;
        }
    }
}

// Function to print the histogram
void printHistogram() {
    for (int i = 0; i < 256; ++i) {
        cout << "Pixel Value " << i << ": " << histogram_Algorithm_data[i] << endl;
    }
}

// Function to test the histogram sum
// This function checks if the sum of histogram values matches the total number of pixels
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

    cout << "--- Sequential Histogram Calculation for Grayscale Image ---" << endl;
    cout << "Image Size: " << image_width << "x" << image_height << endl;
    cout << "Number of Threads: 1 (Sequential)\n";

    auto start_time = chrono::high_resolution_clock::now();

    // Calculate histogram sequentially
    sequentialHistogram();

    auto end_time = chrono::high_resolution_clock::now();

    // Calculate execution time
    chrono::duration<double> time_needed = end_time - start_time;

    cout << "\nExecution Time: " << time_needed.count() << " s\n" << endl;


    // Test histogram sum
    testHistogramSum(histogram_Algorithm_data, image_width * image_height);


  
 printHistogram();
 

    return 0;
}
