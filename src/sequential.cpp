#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

vector<vector<unsigned char>> image_data_2d;
vector<int> histogram_Algorithm_data(256, 0);
int image_width = 0;
int image_height = 0;

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

void sequentialHistogram() {
    for (int r = 0; r < image_height; ++r) {
        for (int c = 0; c < image_width; ++c) {
            histogram_Algorithm_data[image_data_2d[r][c]]++;
        }
    }
}

int main() {
    string image_filename = "image_test.jpg"; 
    loadImage(image_filename);

    cout << "--- Sequential Histogram Calculation for Grayscale Image ---" << endl;
    cout << "Image Size: " << image_width << "x" << image_height << endl;
    cout << "Number of Threads: 1 (Sequential)\n";

    auto start_time = chrono::high_resolution_clock::now();

    sequentialHistogram();

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> time_needed = end_time - start_time;

    cout << "\nExecution Time: " << time_needed.count() << " s\n" << endl;

 

    return 0;
}
