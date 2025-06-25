#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

// Definition of the Node
struct Node {
    std::string name; // Soldier's name
    Node* next = nullptr;  // Next Node
    Node* previous = nullptr; // Previous Node
};

// Function to create circular linked list
Node* createMembers(int count) {
    Node* head = nullptr;
    Node* tail = nullptr;

    
    for (int i = 0; i < count; ++i) { // Loop through number of members
        cout << "What's your name soldier? ";
        string name;
        getline(cin, name);

        Node* newNode = new Node(); // Create new node dynamically
        newNode->name = name;

        if (head == nullptr) {
            head = newNode; // First node becomes head
            head->next = head;
            head->previous = head;
            tail = head;
        }
        else {
            newNode->previous = tail; // Link new node between tail and head
            newNode->next = head;
            tail->next = newNode;
            head->previous = newNode;
            tail = newNode;
        }
    }

    return head;
}
int countMembers(Node* start) { // Count how many nodes are currently active
    int count = 1;
    Node* temp = start->next;
    while (temp != start) {
        count++;
        temp = temp->next;
    }

    return count;
}

// Display members starting from current holder
void displayMembers(Node* start) {
    Node* temp = start;
    cout << "\t" << temp->name << endl;
    temp = temp->next;

    while (temp != start) {
        cout << "\t" << temp->name << endl;
        temp = temp->next;
    }
}

// Eliminate a node and return the next holder
Node* eliminate(Node* target) {
    Node* prev = target->previous;
    Node* next = target->next;

    // Remove target from the circle
    prev->next = next;
    next->previous = prev;

    delete target; // Free memory

    return next; // Cloak passes to next person
}

// Main game logic
int main() {
    srand(time(0)); // Initialize random number generator

    int numMembers;
    cout << "Enter number of Night's Watch members: ";
    cin >> numMembers;
    cin.ignore(); // Remove leftover newline

    Node* start = createMembers(numMembers); // Create member circle

    int round = 1;

    // Loop until one member remains
    while (countMembers(start) > 1) {
        cout << "\n==========================================" << endl;
        cout << "ROUND " << round++ << endl;
        cout << "==========================================" << endl;
        cout << "Remaining Members:\n";
        displayMembers(start);

        int membersLeft = countMembers(start);
        int roll = (rand() % membersLeft) + 1; // Roll a number between 1 and membersLeft

        cout << "\nResult:\n";
        cout << "   " << start->name << " has drawn " << roll << endl;

        Node* current = start; // Move roll-1 times to select the eliminated person
        for (int i = 0; i < roll - 1; ++i) {
            current = current->next;
        }

        cout << "   " << current->name << " was eliminated\n";

        start = eliminate(current);  // Eliminate current and update start to the next holder
    }

    // Final result: only one member left
    cout << "\n==========================================" << endl;
    cout << "FINAL RESULT" << endl;
    cout << "==========================================" << endl;
    cout << "   " << start->name << " will go to seek for reinforcements.\n";

    delete start; // Clean up last remaining node

    cout << "\nPress any key to continue . . .\n";
    cin.get();
    return 0;
}
