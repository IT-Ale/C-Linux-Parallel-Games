# C-Linux-Parallel-Games

A collection of two implementations of the same arcade game developed in **C** for **Linux**. This repository demonstrates two different low-level concurrency paradigms: **Multiprocessing** and **Multithreading**.

## 📂 Repository Structure

The project is divided into two main directories, each containing its own source code and Makefile:

### 1. [Multiprocess Version](./multiprocess_version/)
* **Core Logic:** Uses `fork()` to create independent processes for game entities (aliens, defenses, bullets).
* **Communication:** Implements **IPC (Inter-Process Communication)** via a unidirectional `pipe()`.
* **Signals:** Uses `kill()` to manage collisions and process termination.

### 2. [Multithread Version](./multithread_version/)
* **Core Logic:** Uses the `pthread` library to run game entities as threads within a single process.
* **Synchronization:** Implements a **Producer-Consumer** pattern with a circular buffer.
* **Primitives:** Uses `pthread_mutex_t` for mutual exclusion and `sem_t` (semaphores) for flow control.



## 🛠️ Prerequisites

To compile and run these games, you need a Linux environment with:
* **GCC** (GNU Compiler Collection)
* **ncurses** library (for terminal graphics)
* **pthreads** library (usually included in glibc)


