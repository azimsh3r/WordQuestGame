# WordQuest

WordQuest is an interactive challenge-based game where players face a series of puzzles or word challenges. The game tracks the player's progress, allowing them to master a set of challenges while testing their logical thinking and problem-solving abilities. Ideal for players looking to improve their response times and cognitive skills in a fun and competitive environment.

## Features
- **Challenge-based gameplay**: Players are presented with different challenges that test their knowledge and problem-solving abilities.
- **Word puzzles**: Solve word-based challenges to advance in the game.
- **Real-time responses**: Players must respond to challenges within a set time limit to succeed.
- **Leaderboard**: Track your progress and compete with others to see who can master the most challenges.

## Installation

### Prerequisites
Make sure you have the following installed:
- C compiler (like GCC)
- Make (if using a Makefile)
- Linux-based OS (or any system with a compatible socket library)

### Steps
1. Clone the repository:
    ```bash
    git clone https://github.com/azimsh3r/WordQuestGame.git
    cd ChallenQuest
    ```

2. Compile the game:
    ```bash
    make
    ```

3. Run the game:
    ```bash
    ./challenquest
    ```

   By default, the game will start a server and wait for players to connect.

## Usage

### Start the Game
To start the game, simply run the compiled program. The game can run in different modes:
- **Single-player mode**: The game will present challenges to a single player.
- **Multiplayer mode**: Multiple players can connect to the server and compete to solve challenges.

### Command-line Arguments
You can control some aspects of the game using command-line arguments:
```bash
./challenquest [option]
```
Options:
- `-p [port]`: Set the server's port for multiplayer mode (default is `8080`).

### Game Mechanics
- Players are presented with a series of challenges, and they must respond to each one. 
- The game tracks the number of challenges completed and the number solved.
- Incorrect responses will result in a failure message, while correct responses will award points.

### Example Interaction
```
Server: Welcome to ChallenQuest!
Player: What is 5 + 5?
Player Response: R: 10
Server: O: Correct!
Player: What is the capital of France?
Player Response: R: Paris
Server: O: Correct!
```

## Contribution

We welcome contributions to improve the game! Here are some ways you can help:
1. **Fork the repo** and make changes in your own branch.
2. **Create a pull request** with a detailed explanation of what changes you've made.

### Contribution Guidelines
- Write clear commit messages.
- Ensure that the code follows the existing coding style and guidelines.
- Add tests for new features or bug fixes.
- Provide a brief description of your changes in the pull request.

## License

ChallenQuest is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Contact

If you have any questions, feel free to open an issue or contact the author at azimjon.works@gmail.com
---

This README provides a clear overview of the game and outlines how others can get involved or contribute. It also helps users get started quickly by providing installation and usage instructions.
