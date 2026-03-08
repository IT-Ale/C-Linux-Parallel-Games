# C-Linux-Parallel-Games

A collection of two implementations of the same arcade game developed in **C** for **Linux**. This repository demonstrates two different low-level concurrency paradigms: **Multiprocessing** and **Multithreading**.

## 📂 Repository Structure

The project is divided into two main directories, each containing its own source code and Makefile:

### 1. [Multiprocess Version](./multiprocess_version/)
* [cite_start]**Core Logic:** Uses `fork()` to create independent processes for game entities (aliens, defenses, bullets). [cite: 2, 3]
* [cite_start]**Communication:** Implements **IPC (Inter-Process Communication)** via a unidirectional `pipe()`. [cite: 2, 3]
* [cite_start]**Signals:** Uses `kill()` to manage collisions and process termination. [cite: 2, 3]

### 2. [Multithread Version](./multithread_version/)
* [cite_start]**Core Logic:** Uses the `pthread` library to run game entities as threads within a single process. [cite: 1]
* [cite_start]**Synchronization:** Implements a **Producer-Consumer** pattern with a circular buffer. [cite: 1]
* [cite_start]**Primitives:** Uses `pthread_mutex_t` for mutual exclusion and `sem_t` (semaphores) for flow control. [cite: 1]



## 🛠️ Prerequisites

To compile and run these games, you need a Linux environment with:
* **GCC** (GNU Compiler Collection)
* **ncurses** library (for terminal graphics)
* **pthreads** library (usually included in glibc)

## 🚀 How to Run

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/YOUR_USERNAME/C-Linux-Parallel-Games.git](https://github.com/YOUR_USERNAME/C-Linux-Parallel-Games.git)
   cd C-Linux-Parallel-Games
