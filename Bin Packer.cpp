#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Best stores the optimal number of bins for the weights.
int best = 0;
// The bins vector stores the bins that are used for weights.
vector<float> bins;
// The weights vector stores the elements' weights that are meant to be packaged optimally in unit capacity bins.
vector<float> weights;

/*
Author: Collin Ennis
Date: 12/7/2022

Brute Force approach to bin packing problem
Complexity: O(N!)

Note that bruteForce with 11 elements will take ~45 minutes. Larger than 11 elements is not recommended as it will theoretically take hours

ASSUMPTIONS: bins are max unit capacity, 1, and any elements in weights should be <= 1 while still being positive.
*/
void bruteForce(int index) {
    int count = 0;
    for (int i = 0; i < (int)bins.size(); i++) {
        if (bins[i] != 0) {
            count++;
        }
    }
    if (index >= (int)weights.size()) {
        if (count < best) {
            best = count;
        }
        return;
    }
    for (int i = 0; i < (int)bins.size(); i++) {
        if (bins[i] + weights[index] <= 1) {
            bins[i] += weights[index];
            bruteForce(index + 1);
            bins[i] -= weights[index];
        }
    }
}
/*
Next Fit heuristic

Time Complexity: O(N)

Modified by: Collin Ennis
Date: 12/7/2022

ASSUMPTIONS: Bins are unit capacity, weights should be positive and <= 1.
*/
void nextFit() {
    int currentBin = 0;
    for (int i = 0; i < (int)weights.size(); i++) {
        if (bins[currentBin] + weights[i] <= 1) {
            bins[currentBin] += weights[i];
        }
        else {
            bins[++currentBin] += weights[i];
        }

    }
    int count = 0;
    for (int i = 0; i < (int)bins.size(); i++) {
        if (bins[i] != 0.0) {
            count++;
        }
    }
    best = min(best, count);
}
/*
Author: Collin Ennis
Date: 12/7/2022

Function to reset the global variables representing weights and bins with random numbers generated up until size.

The random numbers generated are from 0.00 to 1.00, always to 2 decimal places.
*/
void constructDataset(int size) {
    vector<float> tempBins(size, 0.0);
    vector<float> tempWeights;
    for (int i = 0; i < size; i++) {
        float random = static_cast<float>((rand() % 101)) / static_cast<float>(100);
        tempWeights.push_back(random);
    }
    bins = tempBins;
    weights = tempWeights;
    best = (int)weights.size();
}
int main()
{
    srand(time(NULL));

    // Testing 10 different randomly generated data sets for both the bruteforce algorithm and the heuristic. Also prints the execution time (wall time) of each.
    for (int i = 0; i < 10; i++) {
        constructDataset(i);
        cout << endl << "weights: " << endl;
        for (int i = 0; i < (int)weights.size(); i++) {
            cout << weights[i] << " ";
        }
        cout << endl;
        vector<float> tempBins = bins;
        vector<float> tempWeights = weights;
        auto startTime = high_resolution_clock::now();
        bruteForce(0);
        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(endTime - startTime);
        cout << "bruteForce optimal = " << best << endl << "Execution time: " << duration.count() << " microseconds" << endl;

        bins = tempBins;
        weights = tempWeights;
        best = (int)weights.size();
        startTime = high_resolution_clock::now();
        nextFit();
        endTime = high_resolution_clock::now();
        duration = duration_cast<microseconds>(endTime - startTime);
        cout << "nextFit optimal = " << best << endl << "Execution time: " << duration.count() << " microseconds" << endl;
    }

    // Test the heuristic for very large N, just to show the execution time.
    cout << endl << endl << "Testing heuristic for N = 10000" << endl;
    constructDataset(10000);
    auto startTime = high_resolution_clock::now();
    nextFit();
    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(endTime - startTime);
    cout << "nextFit optimal = " << best << endl << "Execution time: " << duration.count() << " microseconds" << endl << endl;

    cout << "Testing heuristic for N = 20000" << endl;
    constructDataset(20000);
    startTime = high_resolution_clock::now();
    nextFit();
    endTime = high_resolution_clock::now();
    duration = duration_cast<microseconds>(endTime - startTime);
    cout << "nextFit optimal = " << best << endl << "Execution time: " << duration.count() << " microseconds" << endl;

}