
---

## Project 2: Parallel Histogram Computation for Grayscale Images using OpenMP

This project implements a parallel histogram calculation for grayscale images using OpenMP. The approach leverages multi-threading to accelerate processing by distributing the computation across multiple CPU cores. It aims to demonstrate performance improvements over traditional sequential methods by utilizing modern multi-core processors efficiently.

---

## 🔧 Parallelization Approach (OpenMP)

* Each thread computes a local histogram to avoid race conditions during concurrent updates.
* The image rows are distributed among threads using the `#pragma omp for` directive.
* After processing, local histograms from all threads are combined into a final global histogram.
* The number of threads is determined automatically at runtime by OpenMP based on the system’s available resources.



---

## 💻 Hardware Specifications

- **CPU:** Intel® Core™ i7-12700H
- **Physical Cores:** 14 (6 Performance + 8 Efficiency)
- **Logical Cores (Threads):** 20
- **RAM:** 16 GB DDR5

---

## 📊 Performance Results

| Version         | Execution Time (s) | Threads                                 | Total Pixels |
| --------------- | ------------------ | --------------------------------------- | ------------ |
| Sequential      | 0.0481             | 1                                       | 18,902,034   |
| OpenMP          | 0.0078             | Automatically chosen (20 detected)      | 18,902,034   |

### ✅ Speedup:

```text
Speedup = Sequential Time / OpenMP Time ≈ 6.16x
```
**Note:**
The number of threads used by OpenMP was **determined automatically at runtime** with the following code snippet:

```cpp
int num_threads = 0;
// Determine the number of threads to use
#pragma omp parallel
{
    #pragma omp single
    num_threads = omp_get_num_threads();
}
```

This enables OpenMP to select an optimal thread count based on the available hardware, which improves parallel efficiency.

---
---

## 🧩 Challenges and Solutions

* **Race Condition:** Solved using private/local histograms for each thread and merging results after.
  
![](Project%202/screenshots/local_his.PNG)

* **Incorrect Thread Handling:** Avoided hardcoding thread counts and used OpenMP's internal scheduler.

![](Project%202/screenshots/Dynamic_thread_selection.PNG)
  
  
* **Load Balancing:**Effective distribution of workload by dividing image rows evenly among threads via OpenMP’s for directive, minimizing idle time.

![](Project%202/screenshots/Load%20Balancing.PNG)


* **Complexity of 2D Data:** Converted the image data from 1D to 2D array format to better reflect real-world scenarios.  
  This increases indexing complexity and affects cache behavior, highlighting the true benefits of parallelization.

  ![](Project%202/screenshots/convertto2d.PNG)



* **Data Structure Choice:** To efficiently store pixel counts for values 0-255, a vector<int> was used instead of a map<int,int>.
                             This choice leverages constant-time indexing and improves cache locality.

```cpp
// Using vector<int> for histogram because pixel values are in [0..255]
// This allows O(1) access and better cache performance than a map
vector<int> histogram(256, 0);

// If map was used, access would be O(log n) and more overhead
// map<int, int> histogram_map;
```

---

## ✅ Validation

* The sum of all histogram values equals the total number of pixels.
* Output verified to match the sequential implementation exactly.

### 🐢 Sequential Result
![Sequential Result](Project%202/screenshots/Sequential_result.png)

### ⚡ OpenMP Result
![OpenMP Result](Project%202/screenshots/OpenMp_result.png)


**The parallel output is bitwise identical to the sequential output.**



---

---

## 🧠 Conclusion

This project demonstrated how parallelization using OpenMP can significantly accelerate histogram computation for large grayscale images. By avoiding race conditions, using efficient data structures, and relying on OpenMP's dynamic thread management, the program achieved over 6x speedup on a modern CPU.


---








