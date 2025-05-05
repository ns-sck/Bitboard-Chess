# Chess Engine Project

## Overview
This project is a chess engine implemented in C++ that uses bitboards for efficient board representation. The engine employs object-oriented programming principles including inheritance and interfaces to create a modular and extensible design.

## Features
- Bitboard representation for fast move generation and position evaluation
- Object-oriented design with inheritance hierarchy for chess pieces
- Minimax algorithm with alpha-beta pruning for move calculation
- Complete rule implementation including special moves (castling, en passant, promotion)
- Position evaluation based on material balance and positional factors

## Architecture
The engine is structured around the following key components:

### Core Classes
- `Piece`: Abstract base class for all chess pieces
  - Derived classes: `Queen`, `Knight`, `Rook`, `Bishop`, `King`, `WhitePawn`, `BlackPawn`
- `BitUtil`: Utility functions for bitboard operations
- `Move`: Representation of chess moves
- `Game`: Game state management
- `Engine`: Move calculation and evaluation

### Bitboard Representation
Bitboards are 64-bit integers where each bit represents a square on the chess board. This representation allows for efficient move generation and board evaluation through bitwise operations.
