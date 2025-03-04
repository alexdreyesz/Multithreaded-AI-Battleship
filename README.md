# Multithreaded-AI-Battleship: Enhancing Game AI Through Parallelism

# Battleship Game - Compilation Instructions

## Prerequisites

Ensure you have a C++ compiler installed. If you don't have one, follow these steps:

### Windows (MinGW)
- Download and install [MinGW-w64](https://www.mingw-w64.org/).
- Add `g++` to your system `PATH` (check by running `g++ --version` in Command Prompt).
- Alternatively, use [WSL](https://docs.microsoft.com/en-us/windows/wsl/install) with Ubuntu.

---

## Compilation

Navigate to the directory src containing `battleship.cpp` and compile using:

```sh
g++ -o battleship.exe battleship.cpp -std=c++17
```

## Running the Program

After successful compilation, run the program:

```sh
./battleship.exe
```

## Cleaning Up

To remove the compiled binary:

```sh
del battleship.exe
```

## Additional Notes

- Ensure all dependencies and required files are in the same directory as `battleship.cpp`.
- If you encounter errors, verify your compiler version with:

```sh
g++ --version
```
