# Chess Engine (C++)

A fully playable chess engine built from scratch in C++ using object-oriented design principles and modular game-state architecture.

This project implements legal chess gameplay within the console, including move validation, piece-specific movement logic, check/checkmate detection, and game-state management. The engine was designed with extensibility in mind, allowing future expansion into GUI and online multiplayer implementations.

## Features

### Implemented

* Legal move generation
* Piece-specific movement rules
* Check and checkmate detection
* Castling
* En passant
* Pawn promotion
* Move history
* Undo/redo support
* Console-based gameplay

## Architecture

The engine separates game-state management from piece behaviour to maintain modularity and extensibility.

### Design Overview

* **Pieces** determine how they move based on movement rules
* **Board logic** validates move legality and maintains overall game state
* **Game management** handles turn flow, move execution, and rule enforcement

This separation simplifies validation of more complex rules such as checks, pins, castling restrictions, and special moves.

## Project Structure

```text
chess/
├── src/        # implementation (.cc)
├── include/    # headers (.h)
├── build/      # compilation output
```

## How to Run

Compile:

```bash
make
```

Run:

```bash
./chess
```

## Future Development

Planned expansions include:

* Graphical user interface (GUI)
* AI opponent
* Online multiplayer implementation
* Additional gameplay and interface improvements

## Development Notes

Additional design notes and roadmap planning are available in the `/docs` directory.
