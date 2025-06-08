# ♟️ Chess Game Engine (C++)

Welcome to the **Chess Game Engine**, a modern C++ implementation of the classic strategy game. This project is a modular, object-oriented approach to simulating chess, built with clarity, extensibility, and AI experimentation in mind.

---

## 🧠 Overview

This engine includes:

- Complete chessboard representation
- Move validation based on piece rules
- Support for algebraic notation
- A basic chess-playing bot
- Robust error handling via custom exceptions

It's designed as a console-based engine, ideal for backend logic in more complex projects like GUI applications or online multiplayer platforms.

---

## 📂 File Structure

```
.
├── main.cpp                         # Entry point of the application
├── Board.hpp                        # Board state management and piece positions
├── chess_board.hpp                  # Piece behavior and interaction logic
├── chess_board_constants.hpp        # Constants for board setup and piece types
├── chess_bot.hpp                    # AI logic for basic move decisions
├── chess_bot_constants.hpp          # Constants for bot evaluation and behavior
├── chess_notation.hpp               # Parsing and generating chess notation
├── Exception.hpp                    # Custom exception classes
└── README.md                        # Project documentation
```

---

## 🎯 Project Goals

- ✅ Build a fully functional chessboard with legal move validation
- ✅ Implement a basic AI bot capable of responding to user moves
- ✅ Develop a clean, well-documented codebase for portfolio showcasing
- ⬜ Add support for GUI or web integration (planned future extension)

---

## 🚀 Getting Started

### Prerequisites

- A C++ compiler that supports C++17 or later (e.g. GCC, Clang, MSVC)

### Compilation

Use a terminal in the project directory:

```bash
g++ -std=c++17 main.cpp -o chess
./chess
```

> Ensure all header files are in the same directory, or adjust include paths as needed.

---

## 🧪 Example Usage

```
Welcome to Chess!
White's move: e2e4
Black (Bot) plays: e7e5
White's move: g1f3
Black (Bot) plays: b8c6
...
```

You can input moves using algebraic notation (e.g., `e2e4`, `g1f3`).

---

## 🤖 Bot Logic

The bot operates using a simple heuristic-based evaluation of legal moves. While it's not built for competitive strength, it provides a foundational framework that can be expanded with:

- Minimax or Alpha-Beta pruning
- Piece-square tables
- Opening books and endgame databases

---

## 🛠 Skills Demonstrated

- Advanced C++ (OOP, header management, custom exceptions)
- Game logic design
- AI fundamentals (move generation & evaluation)
- Code modularity and reusability
- CLI interface design

---

## 📌 Future Improvements

- [ ] Add unit tests with Google Test
- [ ] Develop a graphical user interface with SFML or Qt
- [ ] Integrate a stronger chess engine backend
- [ ] Implement multiplayer support (LAN/online)

---

## 📜 License

This project is open-source and available under the [MIT License](LICENSE).

---

## 🙋 About Me

Hi! I'm a passionate C++ developer interested in backend programming. This project showcases my love for clean architecture and strategic logic. If you'd like to collaborate, feel free to reach out!

[🔗 LinkedIn](https://www.linkedin.com/in/anton-burtasenkov-1a44492b1/) | [📧 Email](mailto:aburtasenkov@gmail.com)

---
