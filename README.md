# Chess Engine (C++)

A fully playable chess engine built from scratch in C++ using object-oriented design principles, polymorphism, and modular game-state architecture.

The engine supports all major chess rules, including castling, en passant, pawn promotion, check/checkmate detection, stalemate detection, threefold repetition, and the fifty-move rule. The project was designed with extensibility in mind and serves as the foundation for future GUI, AI, and online multiplayer implementations.

---

## Version

**Current Release:** v1.0

The console-based chess engine is feature complete. Future development will focus on graphical interfaces, AI integration, and additional quality-of-life improvements.

---

## Features

### Gameplay Rules

* Legal move validation
* Piece-specific movement rules
* Check detection
* Checkmate detection
* Stalemate detection
* Castling
* En passant
* Pawn promotion

### Draw Detection

* Threefold repetition detection
* Fifty-move rule detection

### Engine Functionality

* Move history tracking
* Undo support
* Position fingerprinting
* Turn management and rule enforcement
* Console-based gameplay

---

## Architecture

The engine separates game-state management from piece behavior to maintain modularity, extensibility, and clean responsibility boundaries.

### Design Overview

* **Pieces** inherit from a common abstract base class and implement their own movement rules through polymorphism.
* **Board logic** validates move legality and manages overall game state.
* **Move history** stores state required for undo functionality and position tracking.
* **Game management** handles turn flow, move execution, draw detection, and rule enforcement.

This architecture simplifies the implementation of complex chess mechanics such as checks, castling restrictions, en passant captures, repetition detection, and move simulation.

---

## Technical Highlights

* Object-Oriented Design (OOP)
* Polymorphism through inheritance
* Modular game-state management
* Undo system using move history records
* Position fingerprinting for repetition detection
* Simulation-based move validation for check detection
* Dynamic memory management for piece ownership

---

## Project Structure

```text
chess/
├── src/        # implementation (.cc)
├── include/    # headers (.h)
├── build/      # compilation output
└── docs/       # design notes and development planning
```

---

## Technologies

* C++
* STL Containers (`vector`, `stack`, `string`)
* Make
* Object-Oriented Programming
* Polymorphism

---

## Build & Run

Compile:

```bash
make
```

Run:

```bash
./chess
```

---

## Future Development

### V2 — Graphical User Interface

Planned improvements include:

* Interactive chess board
* Click-to-move controls
* Move highlighting
* Promotion selection menu
* Visual game-state indicators

### V3 — Advanced Features

Potential future additions:

* AI opponent
* Position loading and saving
* PGN/FEN support
* Online multiplayer
* Web-based deployment

---

## Learning Outcomes

This project provided practical experience with:

* Object-oriented software design
* Polymorphism and inheritance
* State management
* Undo/redo architecture
* Rule-based systems
* Refactoring and code organization
* Testing and debugging complex edge cases

---

## Author

Developed as a personal software engineering project to strengthen C++ programming skills, object-oriented design principles, and large-scale application architecture.
