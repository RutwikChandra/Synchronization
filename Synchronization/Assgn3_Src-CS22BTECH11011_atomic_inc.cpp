#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <atomic>
#include <unistd.h>

using namespace std;

int n, k, rowInc;

// Shared counter
atomic<int> counter(0);

// Structure to hold thread parameters
struct thread_param {
    int thread_no;
    vector<vector<int>> *matrix;
    vector<vector<int>> *result;
};

void *runner(void *params) {
    thread_param *thread_params = (thread_param *)params;
    vector<vector<int>> &matrix = *(thread_params->matrix);
    vector<vector<int>> &result = *(thread_params->result);

    while (1) {
        int start = counter.fetch_add(rowInc); // Atomically increment counter and get the previous value
        if (start >= n) {
            pthread_exit(NULL);
        }

        int end = start + rowInc - 1 ;

        // Compute matrix multiplication for the assigned rows
        for (int i = start; i <= end && i<=n-1 ; i++) {
            for (int j = 0; j < n; j++) {
                int sum = 0;
                for (int l = 0; l < n; l++) {
                    sum += matrix[i][l] * matrix[l][j];
                }
                result[i][j] = sum;
            }
        }
    }
}

int main() {

    ifstream input_file("inp.txt");
    ofstream output_file("out_atomic_inc.txt");

    input_file >> n >> k >> rowInc;

    vector<vector<int>> matrix(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            input_file >> matrix[i][j];
        }
    }

    vector<vector<int>> result(n, vector<int>(n));

    // Initialize pthread variables
    pthread_t threads[k];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Create thread parameters
    struct thread_param thread_param_arr[k];

    // Create and execute threads for chunked matrix multiplication
    auto start_time = chrono::high_resolution_clock::now();

    for (int i = 0; i < k; i++) {
        thread_param_arr[i].thread_no = i;
        thread_param_arr[i].matrix = &matrix;
        thread_param_arr[i].result = &result;

        pthread_create(&threads[i], &attr, runner, &thread_param_arr[i]);
    }

    // Join threads
    for (int i = 0; i < k; i++) {
        pthread_join(threads[i], NULL);
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

    // Output the resulting square matrix and time taken
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            output_file << result[i][j] << " ";
        }
        output_file << endl;
    }

    output_file << "Time taken Using Atomic Increment: " << duration << " microseconds" << endl;

    input_file.close();
    output_file.close();

    return 0;
}