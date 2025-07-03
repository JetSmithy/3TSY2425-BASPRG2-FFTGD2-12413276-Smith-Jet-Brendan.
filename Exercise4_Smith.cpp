#include <iostream>      
#include <string>        
#include <cstdlib>       
#include <ctime>         
#include <windows.h>     

using namespace std;

class Wizard;

// Spell Class 
class Spell {
private:
    string name; // Name of the spell
    int minDamage; // Minimum damage the spell can do
    int maxDamage; // Maximum damage the spell can do
    int mpCost; // MP cost to cast the spell

public:
    // Constructor to initialize spell properties
    Spell(string name, int minDmg, int maxDmg, int cost)
        : name(name), minDamage(minDmg), maxDamage(maxDmg), mpCost(cost) {
    }

    // Function to cast the spell, requires caster and target
    void activate(Wizard* caster, Wizard* target);

    // Destructor to confirm when spell object is destroyed
    ~Spell() {
        cout << "[Spell \"" << name << "\" destroyed]" << endl;
    }
};

// Wizard Class 
class Wizard {
private:
    string name; // Wizard's name
    int hp; // Wizard's health points
    int mp; // Wizard's mana points
    int minDamage;// Minimum damage for physical attack
    int maxDamage;// Maximum damage for physical attack
    Spell* spell; // Pointer to a Spell object

public:
    // Constructor to initialize Wizard's stats and their Spell
    Wizard(string n, int minDmg = 10, int maxDmg = 15)
        : name(n), hp(250), mp(0), minDamage(minDmg), maxDamage(maxDmg) {
        // Each wizard gets their own spell instance
        spell = new Spell("Fireball", 40, 60, 50);
    }

    // Accessor to get wizard's name
    string getName() const { return name; }

    // Accessor to get wizard's HP
    int getHP() const { return hp; }

    // Accessor to get wizard's MP
    int getMP() const { return mp; }

    // Check if wizard is alive (HP > 0)
    bool isAlive() const { return hp > 0; }

    // Reduce wizard's HP when taking damage
    void takeDamage(int dmg) {
        hp -= dmg;
        if (hp < 0) hp = 0; // Avoid negative HP
    }

    // Increase wizard's MP
    void gainMP(int mpGain) {
        mp += mpGain;
    }

    // Decrease wizard's MP (e.g., when casting spells)
    void reduceMP(int amount) {
        mp -= amount;
        if (mp < 0) mp = 0;
    }
    // Perform a normal attack on another wizard
    void attack(Wizard* target) {
        // Calculate random damage between min and max
        int damage = rand() % (maxDamage - minDamage + 1) + minDamage;

        // Generate random MP gain between 10 and 20
        int mpGain = rand() % 11 + 10;

        // Output attack message
        cout << name << " attacks " << target->getName() << " for " << damage << " damage!" << endl;

        // Apply damage to target
        target->takeDamage(damage);

        // Gain MP and output message
        cout << name << " gains " << mpGain << " MP." << endl;
        gainMP(mpGain);
    }

    // Cast a spell on the target
    void castSpell(Wizard* target) {
        spell->activate(this, target); // Pass self and target to spell
    }

    // Take a turn — prioritize spell if MP allows
    void takeTurn(Wizard* target) {
        if (mp >= 50) {
            castSpell(target); // Use spell if enough MP
        }
        else {
            attack(target); // Otherwise perform normal attack
        }

        displayStats();
    }

    void displayStats() { // Display current HP and MP
        cout << name << " | HP: " << hp << " | MP: " << mp << endl << endl;
    }

    ~Wizard() {     // Destructor to clean up allocated spell
        delete spell; 
        cout << "[Wizard \"" << name << "\" destroyed]" << endl;
    }

    friend class Spell; // Grant Spell access to Wizard internals (e.g., MP)
};

// Spell Implementation
void Spell::activate(Wizard* caster, Wizard* target) {  // Check if caster has enough MP to cast the spell

    if (caster->mp >= mpCost) {
        // Calculate spell damage randomly between min and max
        int damage = rand() % (maxDamage - minDamage + 1) + minDamage;

        cout << caster->getName() << " casts " << name << " on " << target->getName()
            << " for " << damage << " damage!" << endl;   // Output casting message

        target->takeDamage(damage); // Apply damage to target

        caster->reduceMP(mpCost); // Deduct MP from caster
    }
    else { // Not enough MP to cast the spell
        cout << caster->getName() << " tried to cast " << name << " but didn't have enough MP!" << endl;
    }
}

// Main Game Loop
int main() {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Create two wizards dynamically
    Wizard* wiz1 = new Wizard("Gandalf");
    Wizard* wiz2 = new Wizard("Saruman");

    // Display game start
    cout << "==== WIZARD BATTLE BEGINS ====\n\n";

    int turn = 0; // Track turn count

    // Game loop until one wizard dies
    while (wiz1->isAlive() && wiz2->isAlive()) {
        cout << ">> Turn " << ++turn << " <<" << endl;

        // Wizard 1 takes a turn against Wizard 2
        wiz1->takeTurn(wiz2);
        if (!wiz2->isAlive()) break; // Check if Wizard 2 died

        // Wizard 2 takes a turn against Wizard 1
        wiz2->takeTurn(wiz1);
        if (!wiz1->isAlive()) break; // Check if Wizard 1 died

        // Optional delay between turns for readability
        Sleep(1000);
    }

    // Display winner
    cout << "==== GAME OVER ====" << endl;

    if (wiz1->isAlive()) {
        cout << wiz1->getName() << " wins!" << endl;
    }
    else if (wiz2->isAlive()) {
        cout << wiz2->getName() << " wins!" << endl;
    }
    else {
        cout << "Both wizards fell at the same time!" << endl;
    }

    // Delete dynamically allocated wizards
    delete wiz1;
    delete wiz2;

    return 0;
}
