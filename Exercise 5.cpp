#include <iostream> 
#include <string> 
#include <vector>     
#include <memory>     
#include <cstdlib>       
#include <ctime>         
using namespace std;

// Base class for all Skills
class Unit; // Forward declaration of Unit class for use in Skill

// Abstract base class Skill
class Skill {
public:
    virtual ~Skill() {} // Virtual destructor for proper cleanup
    virtual void apply(Unit& unit) = 0; // Pure virtual function to apply skill effect
    virtual string name() const = 0;    // Returns skill name
};

// Unit class declaration
class Unit {
public:
    // Properties of the Unit
    string name;
    int hp;
    int power;
    int vitality;
    int dexterity;
    int agility;

    // Constructor to initialize a unit with base stats
    Unit(string n)
        : name(n), hp(100), power(5), vitality(5), dexterity(5), agility(5) {
    }

    // Adds a skill to the Unit's skill list
    void addSkill(shared_ptr<Skill> skill) {
        skills.push_back(skill);
    }

    // Apply a skill by index
    void useSkill(int index) {
        if (index >= 0 && index < skills.size()) {
            cout << skills[index]->name() << " used! ";
            skills[index]->apply(*this);
        }
    }

    // Display current stats
    void displayStats() const {
        cout << "\n--- " << name << "'s Stats ---\n";
        cout << "HP: " << hp << "\n";
        cout << "Power: " << power << "\n";
        cout << "Vitality: " << vitality << "\n";
        cout << "Dexterity: " << dexterity << "\n";
        cout << "Agility: " << agility << "\n";
        cout << "-------------------------\n";
    }

    // Get the number of skills
    int skillCount() const {
        return skills.size();
    }

private:
    vector<shared_ptr<Skill>> skills; // Holds all skills the Unit can use
};


// Derived Skill Classes

// Restores HP
class Heal : public Skill {
public:
    void apply(Unit& unit) override {
        unit.hp += 10; // Increase HP by 10
        cout << "HP healed by 10!\n";
    }

    string name() const override {
        return "Heal";
    }
};

// Might Skill - increases Power
class Might : public Skill {
public:
    void apply(Unit& unit) override {
        unit.power += 2; // Increase Power by 2
        cout << "Power increased by 2!\n";
    }

    string name() const override {
        return "Might";
    }
};

// increases protection
class IronSkin : public Skill {
public:
    void apply(Unit& unit) override {
        unit.vitality += 2; // Increase protection by 2
        cout << "Protection increased by 2!\n";
    }

    string name() const override {
        return "Protection Skin";
    }
};

// Increases Dexterity
class Concentration : public Skill {
public:
    void apply(Unit& unit) override {
        unit.dexterity += 2; // Increase Dexterity by 2
        cout << "Dexterity increased by 2!\n";
    }

    string name() const override {
        return "Concentration";
    }
};

// Increases Agility
class Haste : public Skill {
public:
    void apply(Unit& unit) override {
        unit.agility += 2; // Increase Stamina by 2
        cout << "Stamina increased by 2!\n";
    }

    string name() const override {
        return "Haste";
    }
};

int main() {
    srand(time(0)); //random number generator with current time

    Unit playerCharacter("Player Character");

    // Add all 5 skills to the hero
    playerCharacter.addSkill(make_shared<Heal>());
    playerCharacter.addSkill(make_shared<Might>());
    playerCharacter.addSkill(make_shared<IronSkin>());
    playerCharacter.addSkill(make_shared<Concentration>());
    playerCharacter.addSkill(make_shared<Haste>());

    // Infinite loop to randomly apply a skill
    while (true) {
        int randomIndex = rand() % playerCharacter.skillCount(); // Pick a random skill
        playerCharacter.useSkill(randomIndex); // Use the randomly selected skill
        playerCharacter.displayStats(); // Display current stats
        cout << "Press Enter to continue.\n";
        cin.get(); // Wait for user input to continue
    }

    return 0; // Program will never reach this point due to infinite loop
}