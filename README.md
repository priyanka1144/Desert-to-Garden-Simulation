# 🌵 Desert to Garden - Ecosystem Evolution Simulation

A beautiful, interactive console-based simulation that transforms a barren desert into a thriving garden through careful water management, strategic planting, and time.

![Simulation Preview](https://via.placeholder.com/800x450/1a2a44/ffffff?text=Desert+→+Garden+Simulation+Preview)

## ✨ Features

- Realistic plant growth stages (seed → sprout → young → mature → blooming / tree)
- Two plant types: Flowers/Bushes (fast-growing) and Trees (long-lived & high-score)
- Dynamic water system with daily rainfall variation
- Smart planting placement (avoids overcrowding)
- Visual seasonal progression with moving sun & emerging clouds
- Colorful console interface (Windows-compatible ANSI fallback available)
- Score system based on plant growth and survival
- Death mechanics (neglect, over-age)
- Simple but engaging gameplay loop

## 🎮 Controls

| Key     | Action                        |
|---------|-------------------------------|
| `W`     | Water all plants              |
| `P`     | Plant a flower/bush seed      |
| `T`     | Plant a tree seed             |
| `Q`     | Quit the simulation           |

## 📋 Technical Details

```text
Language:      C++17
Platform:      Windows (primary) • Linux/macOS (with minor modifications)
Libraries:     Standard Library + Windows.h (for colors & cursor)
Rendering:     Console with ANSI fallback support
Data Structure: Vector<Plant>
Growth Model:   Time-based with probabilistic death
🖥️ System Requirements

Modern C++ compiler (g++ 9+, MSVC 2019+, clang 10+)
Windows terminal that supports colors (Windows Terminal recommended)
For Linux/macOS: terminal with UTF-8 and ANSI color support

🚀 Quick Start
Windows (Recommended)
Bash# Using MinGW / MSYS2 / Visual Studio
g++ -std=c++17 -o desert-to-garden main.cpp
./desert-to-garden.exe
Linux / macOS
Bash# Remove Windows.h and use ANSI colors instead
g++ -std=c++17 -o desert-to-garden main.cpp
./desert-to-garden
🎨 Visual Style (Windows)

Bright blue sky with moving sun
Emerging clouds after day 15
Sand dunes with subtle texture
Green → Dark Green → Magenta growth progression
Gold/yellow info panel

📈 Planned Features (Future Development)

Save / Load game state
Weather events (drought, rain bonus)
Plant interaction (pollination, shade effect)
Multiple biomes
Graphical version using SFML or Raylib
Sound effects (rain, growth sounds)

🤝 Contributing
Contributions are welcome!

Fork the repository
Create your feature branch (git checkout -b feature/rain-events)
Commit your changes (git commit -m 'Add dynamic rain system')
Push to the branch (git push origin feature/rain-events)
Open a Pull Request

📄 License
This project is licensed under the MIT License - see the LICENSE file for details.
❤️ Credits
Created with passion for simulation games and nature
Developed by: Priyanka Paul
GitHub: @priyanka1144

From wasteland to paradise — one drop at a time. 🌱
