# Space Invaders - Multithreaded Version (C/Linux)

This version of the game implements a **Multithreading** architecture using the POSIX Threads library (`pthread`) to manage game entities within a shared memory space.

## 🏗️ Architecture

The project follows a **Producer-Consumer** pattern to handle game updates:

* **Main Thread:** Initializes the `ncurses` environment and the synchronization primitives (Mutex and Semaphores).
* **Producers (Entity Threads):** Alien, Defense, and Projectile threads calculate their positions and write updates into a shared circular buffer.
* **Consumer (Rendering Thread):** Reads from the shared buffer and updates the `ncurses` window in real-time.

## 🛠️ Technical Details

* **Synchronization:** * **Mutex (`pthread_mutex_t`):** Protects the critical section during buffer access to prevent race conditions.
    * **Semaphores (`sem_t`):** Manages the "Empty" and "Full" states of the circular buffer to coordinate producers and consumers.
* **Shared Memory:** Unlike the process version, all threads share the same address space, making data exchange faster but requiring careful locking.
* **Graphics:** Uses the `ncurses` library for terminal-based UI and input handling.



## 🚀 Build and Run

Ensure you have the `ncurses` development library installed.

1.  **Compile the project:**
    ```bash
    make
    ```
2.  **Run the game:**
    ```bash
    ./game
    ```
3.  **Clean build files:**
    ```bash
    make clean
    ```

---
**Note:** Proper cleanup is handled via `pthread_mutex_destroy` and `sem_destroy` to ensure no system resources are leaked upon exit.
