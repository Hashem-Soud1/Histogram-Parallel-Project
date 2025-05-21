#ifndef HISTOGRAM_TEST_H
#define HISTOGRAM_TEST_H

#include <vector>
#include <iostream>
#include <cassert>
using namespace std;

void testHistogramSum(const vector<int>& histogram, int expected_total_pixels) {
    int sum = 0;
    for (int value : histogram) {
        sum += value;
    }

    cout << "Histogram total pixel count: " << sum << endl;
    cout << "Expected pixel count: " << expected_total_pixels << endl;

    assert(sum == expected_total_pixels && "Histogram sum does not match image size!");
}

#endif
