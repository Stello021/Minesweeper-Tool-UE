# Minesweeper-Tool-UE

## Setup Instructions
To set up the Minesweeper Tool as an Unreal Engine plugin, follow these steps:
1. **Clone this Repository**
2. Copy folder `Plugins/Minesweeper`
into your Unreal Engine project's `Plugins` folder
3. Open Project, if it prompts you to rebuild the plugin,
click "Yes"
4. Enable plugin to **Edit** -> **Plugins**

## Key Features
- **Dynamic Grid Generation:** The plugin generates a customizable Minesweeper grid with user-defined dimensions (width, height) and a specified number of mines.
- **Interactive UI:** Uses Unreal Engine's Slate UI framework to create buttons that represent grid cells.
- **Gameplay Mechanics:** Implements core Minesweeper mechanics, including mine placement, neighbor detection, and mine revealing.
- **Reset and Validation:** Features for resetting the game state and validating user inputs for grid dimensions and mine count.

### Core Components:
1. **Grid and Mines Management:**
    - The `FMinesweeperModule::GenerateGrid` function dynamically creates the Minesweeper grid by initializing buttons (`SMinesweeperButton`) for each cell.
    - Mines are randomly placed using the Fisher-Yates shuffle algorithm in the `FMinesweeperModule::PlaceMines` function.
    - Each button interacts with its neighbors to count and display nearby mines.

2. **Game Logic:**
    - The `MineClicked` function reveals a clicked mine and ends the game.
    - `RevealNeighbours` uncovers adjacent cells recursively when a non-mine cell is clicked.
    - `ResetGameState` clears the grid and resets game variables for a fresh start.

3. **UI Integration:**
    - The plugin adds a custom dockable tab to the Unreal Editor using `FMinesweeperModule::OnSpawnPluginTab`.
    - Text inputs and buttons allow users to specify grid dimensions and start a new game.

4. **Custom Button Class:**
    - `SMinesweeperButton` represents each cell in the grid and defines its behavior when clicked or revealed.
    - The button changes appearance based on its state (e.g., mine, neighbor count).

[Registrazione 2025-05-15 154555.mp4](Resorces/Registrazione%202025-05-15%20154555.mp4)

## AI Usage
As my first experiment using Unreal Engine's Slate framework, 
I utilized Claude AI to provide practical examples, 
explain the framework, and assist me in building this plugin. 
The AI provided valuable guidance in understanding Slate's layout and widget system,
making the development process much smoother.