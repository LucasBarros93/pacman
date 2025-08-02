# Pacman Terminal ASCII

This is a **Pacman** game written in **C**, fully rendered in the **terminal** using **ASCII characters** and the **ncurses** library.

## 🎮 About the Game

The game runs directly in the terminal, without any graphical interface. It reads the map from a `.txt` file and uses the `ncurses` library to control the cursor, handle key inputs, and draw the game's interface.

## 📁 Project Structure

```

pacman/
├── bin/            # Compiled binary
│   └── game
├── map/            # Game map file
│   └── map.txt
├── src/            # Source code
│   └── pacman.c
├── Makefile        # Compilation and execution instructions
└── .gitignore

````

## ⚙️ How to Compile

Make sure the `ncurses` library is installed on your system.

To compile the game, run:

```bash
make
````

This will create the executable at `./bin/game`.

## ▶️ How to Run

After compiling, run:

```bash
make run
```

Or execute directly:

```bash
./bin/game
```

## 🧹 How to Clean

To remove the generated files, run:

```bash
make clean
```

This will delete everything inside `./bin` and the file `map/result.txt`.

## 🗺️ Map Editing

The game map is stored in `map/map.txt`. You can customize it with walls, paths, and items using ASCII characters, following the expected format defined in the source code.

## 🧱 Requirements

* `gcc` compiler
* `ncurses` library
  (on Debian/Ubuntu: `sudo apt install libncurses5-dev`)

---

> This project was made for educational purposes to practice C programming and terminal handling with `ncurses`.
