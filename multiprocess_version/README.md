# Space Invaders - Multiprocess Version (C/Linux)

This version of the game implements a **Multiprocessing** architecture using Linux system calls to manage game logic, entities, and real-time rendering.

## 🏗️ Architecture

The project is structured as a tree of independent processes communicating through System V primitives:

* **Main Process (Parent):** Initializes the `ncurses` window, creates the **Pipe**, and runs the main rendering loop (`gestione_area_gioco`).
* **Alien Process:** Manages horizontal movement and spawns child processes for bullets using `fork()`.
* **Defense Process:** Handles keyboard input (arrows/space) and creates laser processes.
* **Bullet/Laser Processes:** Independent tasks that calculate their own trajectories and send updates to the parent.

## 🛠️ Technical Details

* **IPC (Inter-Process Communication):** Uses a unidirectional **Pipe** to send `Message` structures from children to the parent.
* **Process Management:** * `fork()` for dynamic entity creation.
    * `waitpid()` with `WNOHANG` flag to monitor bullet termination without blocking the game flow.
    * `kill()` signals to handle collisions and cleanup child processes upon game exit.
* **Graphics:** Built with the `ncurses` library for terminal-based rendering.


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
