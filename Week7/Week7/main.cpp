#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

class Actor {
public:
    Actor(int _health) : health(_health), posX(0.0f), posY(0.0f), targetHealth(nullptr) {} // .text: Constructor code
    ~Actor() {} // .text: Destructor code
    void Attack(Actor* _target, float _damage) {
        _target->health -= _damage;
    }

    std::string name;  // Heap: String data on heap; Stack: Object management on stack
    int health;        // Stack: Part of Actor on stack
    float posX, posY;  // Stack: Part of Actor on stack
    int* targetHealth; // Stack: Pointer on stack, points to heap/stack
};


bool DidPlayerWin(int _playerChoice, int _enemyChoice) {
    // 1 = Fire, 2 = Grass, 3 = Water
    // Simple input validation
    // TODO: check for invalid choices
    // TODO: Tie: Same choice, enemy wins by default
    if (_playerChoice == _enemyChoice) {
        return false;
    }

    // Player wins: Fire > Grass, Grass > Water, Water > Fire
    if ((_playerChoice == 1 && _enemyChoice == 2) ||  // Fire beats Grass
        (_playerChoice == 2 && _enemyChoice == 3) ||  // Grass beats Water
        (_playerChoice == 3 && _enemyChoice == 1)) {  // Water beats Fire
        return true;
    }

    // Enemy wins by default
    return false;
}


int main()
{
    // Initialize random seed once
    srand(time(0));  // Safe cast for portability

    // Initialize Actors with 30 health
    Actor* player = new Actor(30);
    Actor* enemy = new Actor(30);

    int playerAttackChoice;
    constexpr int attackPower = 10;

    // Game Loop
    while (player->health > 0 && enemy->health > 0) {
        // Get player choice with basic validation
        std::cout << "Enter Attack (1 = Fire, 2 = Grass, 3 = Water): ";
        //std::cin >> playerAttackChoice;

        // TODO: Validate input
        while (!(std::cin >> playerAttackChoice) || playerAttackChoice < 1 || playerAttackChoice > 3) {
            std::cout << "Invalid Input. Please try again:\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        // Enemy choice
        int enemyChoice = rand() % 3 + 1;

        // Display choices
        std::cout << "Player used Attack: " << playerAttackChoice
            << ", Enemy used Attack: " << enemyChoice << "\n";

        // Determine winner and apply damage
        if (DidPlayerWin(playerAttackChoice, enemyChoice)) {
            std::cout << "Player wins the round!\n";
            player->Attack(enemy, attackPower);  // Player wins, enemy takes damage
        }
        else { // enemy win
            std::cout << "Enemy damages Player!\n";
            enemy->Attack(player, attackPower);
        }

        // Show health
        std::cout << "\n==========================================================\n"
            << "Player Health : " << player->health
            << " | Enemy Health: " << enemy->health
            << "\n=========================================================="
            << "\n\n";

    }

    // Declare winner
    if (player->health > 0 && enemy->health <= 0) {
        std::cout << "Player Wins!!\n";
    }
    else if (enemy->health > 0 && player->health <= 0) {
        std::cout << "Enemy Wins!!\n";
    }
    else {
        std::cout << "It's a draw!\n"; // Both hit 0
    }

    // Clean up
    delete enemy;  // Free enemy memory
    delete player; // Free player memory
}