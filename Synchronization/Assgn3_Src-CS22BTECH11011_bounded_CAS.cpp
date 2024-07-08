#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <atomic>
#include <unistd.h>

using namespace std;

int n, k, rowInc ;

// Shared counter
int counter = 0;

// Shared lock variable
atomic<int> lock(0);

// Array to track waiting status of threads
vector<bool> waiting;

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

    int i = thread_params->thread_no;

    while(1){
        int expected=0;
        int j;
        
        waiting[i] = true;
        int key = 1;
        while (waiting[i] && key == 1)
        key = lock.compare_exchange_strong(expected, 1);
        waiting[i] = false;

        if(counter >= n){
            // Release the lock
            lock.store(0);
            waiting[i]=false;
            pthread_exit(NULL);
        }

        int start = counter; 
        counter += rowInc;
        int end = counter - 1; 


        j = (i + 1) % k;
        while ((j != i) && !waiting[j])
        j = (j + 1) % k;
        if (j == i)
        lock = 0;
        else
        waiting[j] = false;


        // Compute matrix multiplication for the assigned rows
        for (int i = start; i <= end && i <= n - 1; i++) {
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

int main(){

    ifstream input_file("inp.txt");
    ofstream output_file("out_bounded_CAS.txt");

    input_file >> n >> k >> rowInc ;
    
    vector<vector<int>> matrix(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            input_file >> matrix[i][j];
        }
    }

    vector<vector<int>> result(n, vector<int>(n)) ;

    // Initialize pthread variables
    pthread_t threads[k];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Create thread parameters
    struct thread_param thread_param_arr[k];

    // Create and execute threads for chunked matrix multiplication
    auto start_time = chrono::high_resolution_clock::now();

    // Initialize waiting array
    waiting.resize(k);
    for (int i = 0; i < k; ++i) {
        waiting[i] = false;
    }

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

    output_file << "Time taken Using Bounded CAS : " << duration << " microseconds" << endl;

    input_file.close();
    output_file.close();

    return 0;
}