# Platformer Game Engine 🎮

A 2D platformer-style game engine written in C++. Features include level loading from text files, basic game physics like gravity and jumping, and object interactions like players, enemies, and power-ups.

---

## ✅ Features
- Custom level loading via `Level.txt`
- Player movement, jumping, and gravity
- Goodies and enemy interaction
- Win/loss conditions
- Clean object-oriented design using polymorphism and inheritance

---

## 🛠 How to Compile

If using g++:

```bash
g++ -std=c++11 main.cpp Actor.cpp StudentWorld.cpp Level.cpp -o game
./game
