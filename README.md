
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

| Version         | Execution Time (s) | Threads | Total Pixels |
| --------------- | ------------------ | ------- | ------------ |
| Sequential      | 0.0481             | 1       | 18,902,034   |
| OpenMP          | 0.0078             | 20      | 18,902,034   |

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
  
* **Load Balancing:** Achieving load balancing by parallelizing over image rows using OpenMP’s `for` directive.

![](Project%202/screenshots/Load%20Balancing.png)

---

## ✅ Validation

* The sum of all histogram values equals the total number of pixels.
* Output verified to match the sequential implementation exactly.

### 🐢 Sequential Result
![Sequential Result](Project%202/screenshots/Sequential_result.png)

### ⚡ OpenMP Result
![OpenMP Result](Project%202/screenshots/OpenMp_result.png)

---







