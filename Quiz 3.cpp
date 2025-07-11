#include <iostream> // for input and output
#include <string>  // for using string data type
#include <cstdlib>  // for rand and srand
#include <ctime> // to seed random numbers
#include <iomanip>  // for formatting
using namespace std;


// Unit Class Definition
class Unit {
public:
    string name; // Unit name
    string unitClass; // Class: Warrior, Assassin, Mage
    int maxHP, hp;  // Max and current health
    int pow, vit, agi, dex; // Stats: power, vitality, agility, dexterity

    Unit(string n, string c) {  // Constructor for creating unit
        name = n;
        unitClass = c;

        // Base stats per class
        if (c == "Warrior") {
            maxHP = hp = 100;
            pow = 10; vit = 10; agi = 5; dex = 5;
        }
        else if (c == "Assassin") {
            maxHP = hp = 80;
            pow = 8; vit = 5; agi = 10; dex = 10;
        }
        else if (c == "Mage") {
            maxHP = hp = 70;
            pow = 12; vit = 3; agi = 6; dex = 8;
        }
    }

    bool isAlive() { return hp > 0; }  // Check if unit is alive
};

// Class Advantage Logic
float getBonusMultiplier(string attacker, string defender) {
    if (attacker == "Warrior" && defender == "Assassin") return 1.5f;
    if (attacker == "Assassin" && defender == "Mage") return 1.5f;
    if (attacker == "Mage" && defender == "Warrior") return 1.5f;
    return 1.0f;
}

// Damage Calculation
int calculateDamage(Unit* attacker, Unit* defender) {
    int baseDamage = attacker->pow - defender->vit; // POW - VIT
    if (baseDamage < 1) baseDamage = 1;// Minimum 1
    float bonus = getBonusMultiplier(attacker->unitClass, defender->unitClass); // Bonus if class is strong
    int total = baseDamage * bonus;  // Apply bonus
    return total;
}

// Hit Rate Logic
bool attackHits(Unit* attacker, Unit* defender) {
    float hitChance = ((float)attacker->dex / defender->agi) * 100; // Calculate %
    if (hitChance < 20) hitChance = 20; // Clamp min
    if (hitChance > 80) hitChance = 80; // Clamp max
    int roll = rand() % 100 + 1; // Random 1-100
    return roll <= hitChance;
}

// Display Stats Function
void showStats(Unit* unit) {
    cout << "Name: " << unit->name << " | Class: " << unit->unitClass
        << " | HP: " << unit->hp << "/" << unit->maxHP
        << " | POW: " << unit->pow << " | VIT: " << unit->vit
        << " | AGI: " << unit->agi << " | DEX: " << unit->dex << endl;
}

// Generate Random Enemy
Unit* spawnEnemy(int round) {
    string classes[3] = { "Warrior", "Assassin", "Mage" }; // Possible classes
    string chosenClass = classes[rand() % 3]; // Randomly choose class
    Unit* enemy = new Unit("Enemy", chosenClass); // Create enemy

    // Increase stats by 3 per round
    enemy->maxHP += round * 3;
    enemy->hp = enemy->maxHP;
    enemy->pow += round * 3;
    enemy->vit += round * 3;
    enemy->agi += round * 3;
    enemy->dex += round * 3;
    return enemy;
}

// Victory Stat Bonus
void applyVictoryBonus(Unit* player, string enemyClass) {
    if (enemyClass == "Warrior") {
        player->pow += 3;
        player->vit += 3;
    }
    else if (enemyClass == "Assassin") {
        player->agi += 3;
        player->dex += 3;
    }
    else if (enemyClass == "Mage") {
        player->pow += 5;
    }
}

// Game Start & Main Loop
int main() {
    srand(time(0));  // Seed random

    string playerName, playerClass;
    cout << "Enter your name: ";
    getline(cin, playerName);// Get name
    cout << "Choose class (Warrior / Assassin / Mage): ";
    cin >> playerClass; // Get class
    Unit* player = new Unit(playerName, playerClass);// Create player
    int round = 1;

    while (player->isAlive()) {
        cout << "\n=== ROUND " << round << " ===" << endl;
        showStats(player);// Show player stats

        Unit* enemy = spawnEnemy(round);// Create enemy
        cout << "\nEnemy Appears!\n";
        showStats(enemy);// Show enemy stats

        // Determine turn order
        Unit* first = player, * second = enemy;
        if (enemy->agi > player->agi) {
            first = enemy;
            second = player;
        }

        cout << "\n--- BATTLE START ---" << endl;
        while (player->isAlive() && enemy->isAlive()) {
            // First attack
            if (first->isAlive()) {
                if (attackHits(first, second)) {
                    int dmg = calculateDamage(first, second);
                    second->hp -= dmg;
                    if (second->hp < 0) second->hp = 0;
                    cout << first->name << " hits " << second->name << " for " << dmg << " damage.\n";
                }
                else {
                    cout << first->name << " missed!\n";
                }
            }
            if (!second->isAlive()) break;

            // Second attack
            if (second->isAlive()) {
                if (attackHits(second, first)) {
                    int dmg = calculateDamage(second, first);
                    first->hp -= dmg;
                    if (first->hp < 0) first->hp = 0;
                    cout << second->name << " hits " << first->name << " for " << dmg << " damage.\n";
                }
                else {
                    cout << second->name << " missed!\n";
                }
            }

            system("pause");  // Pause to show each turn
        }

        // Check outcome
        if (player->isAlive()) {
            cout << "\nYou won Round " << round << "!\n";
            applyVictoryBonus(player, enemy->unitClass); // Apply stat bonus
            int heal = player->maxHP * 0.3;
            player->hp += heal;
            if (player->hp > player->maxHP) player->hp = player->maxHP;
            cout << "You healed " << heal << " HP.\n";
        }
        else {
            cout << "\nYou died on Round " << round << "!\n";
            delete enemy;
            break;
        }

        delete enemy;// Deallocate memory
        round++;// Next round
    }

    delete player;  // Free player memory
    cout << "Game Over.\n";
    return 0;
}