#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cassert>

using namespace std;

int image_width = 0;
int image_height = 0;
vector<vector<unsigned char>> image_data_2d;
vector<int> final_histogram_result(256, 0);

bool loadImage(const string& filename) {
    int channels;
    unsigned char* img_data = stbi_load(filename.c_str(), &image_width, &image_height, &channels, 1);
    if (!img_data) {
        cerr << "Failed to load image: " << filename << endl;
        return false;
    }

    image_data_2d.assign(image_height, vector<unsigned char>(image_width));
    for (int r = 0; r < image_height; ++r)
        for (int c = 0; c < image_width; ++c)
            image_data_2d[r][c] = img_data[r * image_width + c];

    stbi_image_free(img_data);
    return true;
}

void testHistogramSum(const vector<int>& histogram, int expected_total_pixels) {
    int sum = 0;
    for (int value : histogram) sum += value;

    cout << "Histogram total pixel count: " << sum << endl;
    cout << "Expected pixel count: " << expected_total_pixels << endl;
    assert(sum == expected_total_pixels && "Histogram sum does not match image size!");
}

int main() {

    string image_filename = "image_test.jpg";
    if (!loadImage(image_filename)) return -1;

    cout << "--- OpenMP Parallel Histogram ---" << endl;
    cout << "Image Size: " << image_width << "x" << image_height << endl;

    int num_threads = 0;
    // Determine the number of threads to use
    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }
    auto start_time = chrono::high_resolution_clock::now();

    vector<vector<int>> local_histograms(num_threads, vector<int>(256, 0));

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        // Each thread has its own local histogram
        vector<int>& local_hist = local_histograms[tid];

        #pragma omp for
        for (int r = 0; r < image_height; ++r) {
            for (int c = 0; c < image_width; ++c) {
                int pixel = static_cast<int>(image_data_2d[r][c]);
                local_hist[pixel]++;
            }
        }
    }

    // Combine local histograms into the final histogram
    for (int tid = 0; tid < num_threads; ++tid) {
        for (int i = 0; i < 256; ++i)
            final_histogram_result[i] += local_histograms[tid][i];
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> time_needed = end_time - start_time;

    cout << "Execution Time (OpenMP): " << time_needed.count() << " s\n";
    cout << "Number of Threads: " << num_threads << endl;

    testHistogramSum(final_histogram_result, image_width * image_height);
    return 0;
}