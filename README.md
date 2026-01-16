# C Chess Computer

A chess computer implementation written in C with a pygame-based graphical frontend. This project combines the performance of C for the chess engine with the ease of Python/pygame for the user interface.

## Features

- **Chess Engine in C**: High-performance chess engine written in C
  - Board representation and move generation
  - Search algorithms for move evaluation
  - Position evaluation functions
  - Legal move validation
  
- **Pygame Frontend**: User-friendly graphical interface
  - Interactive chess board
  - Drag-and-drop piece movement
  - Visual feedback for valid moves
  - Game state display

## Project Structure

```
c-chess-computer/
├── src/              # C source files for chess engine
├── include/          # C header files
├── python/           # Pygame frontend implementation
├── docs/             # Documentation
├── tests/            # Test files
├── examples/         # Example code and usage demonstrations
├── LICENSE           # GPL-3.0 License
└── README.md         # This file
```

## Prerequisites

### For C Chess Engine
- GCC or compatible C compiler
- Make (optional, for build automation)
- Standard C library

### For Pygame Frontend
- Python 3.7+
- pygame library

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/funnykayan/c-chess-computer.git
cd c-chess-computer
```

### 2. Build the C Chess Engine

```bash
# Compile the chess engine
cd src
gcc -o chess_engine *.c -I../include

# Or use make if Makefile is provided
make
```

### 3. Install Python Dependencies

```bash
# Install pygame
pip install pygame

# Or using requirements.txt (if available)
pip install -r requirements.txt
```

## Usage

### Running the Chess Computer

```bash
# Start the pygame frontend
python python/main.py
```

### Using the Chess Engine Directly

```bash
# Run the compiled engine
./src/chess_engine
```

## Development

### Building from Source

```bash
# Compile with debugging symbols
gcc -g -o chess_engine src/*.c -I include

# Compile with optimizations
gcc -O3 -o chess_engine src/*.c -I include
```

### Running Tests

```bash
# Run test suite (if implemented)
cd tests
make test
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## Roadmap

- [ ] Implement core chess engine
- [ ] Add move generation
- [ ] Implement search algorithms (Minimax, Alpha-Beta pruning)
- [ ] Add position evaluation
- [ ] Create pygame frontend
- [ ] Implement AI difficulty levels
- [ ] Add opening book
- [ ] Add endgame tablebase support
- [ ] Save/load game functionality

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Chess programming community
- Pygame development team
- Contributors and testers

## Contact

Project Link: [https://github.com/funnykayan/c-chess-computer](https://github.com/funnykayan/c-chess-computer)

---

**Note**: This project is currently in development. Features and documentation will be updated as the project progresses.
