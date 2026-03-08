# Space Invaders - Multiprocess Version (C/Linux)

This version of the game implements a **Multiprocessing** architecture using Linux system calls to manage game logic, entities, and real-time rendering.

## 🏗️ Architecture

The project is structured as a tree of independent processes communicating through System V primitives:

* [cite_start]**Main Process (Parent):** Initializes the `ncurses` window, creates the **Pipe**, and runs the main rendering loop (`gestione_area_gioco`)[cite: 3].
* [cite_start]**Alien Process:** Manages horizontal movement and spawns child processes for bullets using `fork()`[cite: 2].
* [cite_start]**Defense Process:** Handles keyboard input (arrows/space) and creates laser processes[cite: 3].
* [cite_start]**Bullet/Laser Processes:** Independent tasks that calculate their own trajectories and send updates to the parent[cite: 2, 3].

## 🛠️ Technical Details

* [cite_start]**IPC (Inter-Process Communication):** Uses a unidirectional **Pipe** to send `Message` structures from children to the parent[cite: 3].
* [cite_start]**Process Management:** * `fork()` for dynamic entity creation[cite: 2, 3].
    * [cite_start]`waitpid()` with `WNOHANG` flag to monitor bullet termination without blocking the game flow[cite: 2, 3].
    * [cite_start]`kill()` signals to handle collisions and cleanup child processes upon game exit[cite: 3].
* [cite_start]**Graphics:** Built with the `ncurses` library for terminal-based rendering[cite: 3].

## 🚀 Build and Run

Ensure you have the `ncurses` development library installed on your Linux system.

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
[cite_start]**Note:** The collision system detects impacts between bullets and entities, reducing life counters and terminating the respective processes via system signals[cite: 3].
