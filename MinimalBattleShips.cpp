#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

// Global Variables
int grids[2][10][10]; // 2 grids. 0 = empty, 1 = boat, 2 = miss, 3 = hit
int hitgrids[2][10][10]; // 2 grids for player to keep track of hits
int rem[2]; // Remaining ships by player id


// Helper Functions
void invalid_input_notification() {
    cout << "Invalid input - retrying...\n";
}

bool cdtoid(int &i, int &j) {
// Convert raw input coordinates (e.g. a1) to two index i, j
// WARNING: Mutates given variables i, j
    string cd; // User given coordinates
    cin >> cd; // Reference: 'a' = 97. '1' = 49

    if (97 <= cd[0] && cd[0] <= 106) { // If alpha
        i = cd[0] - 97; // Index i given by (alpha) character manipulation
    } else {
        return false;
    }

    if (49 <= cd[1] && cd[1] <= 58) { // If numeric
        if (cd.length() == 3) {
            if (48 == cd[2]) {
                j = 9;
            } else {
                return false;
            }
        } else {
            j = cd[1] - 49; // Index j given by (numeric) character manipulation
        }
    } else {
        return false;
    }
    return true;
}

bool ship_ca(int grid[10][10], int i, int j, int axis, int pn, int length) {
// Ship final checker and adder for use in grid_setup() directional picker
// PRECONDITIONS: (axis == 0 || axis == 1) && (pn == -1 || pn == 1)
    if (axis == 0) {
        if (i + pn * (length - 1) < 10 && 0 <= i + pn * (length - 1)) {
            for (int adder = 0; adder < length; adder++) {
                if (grid[i+pn*adder][j] != 0) {
                    // invalid_input_notification();
                    return false;
                }
             }

            for (int adder = 0; adder < length; adder++){
                grid[i+pn*adder][j] = 1;
            }
        } else { 
            // invalid_input_notification(); 
            return false;
        }
    } else {
        if (j + pn * (length - 1) < 10 && 0 <= j + pn * (length - 1)) {
            for (int adder = 0; adder < length; adder++) {
                if (grid[i][j+pn*adder] != 0) {
                    // invalid_input_notification();
                    return false;
                }
             }

            for (int adder = 0; adder < length; adder++){
                grid[i][j+pn*adder] = 1;
            }
        } else { 
            // invalid_input_notification(); 
            return false;
        }
    }
    return true;
}

void grid_autosetup(int grid[10][10]) {
// Sets up a valid grid with random ship placements
    int ships = 9;
    int ships_remaining[] = {3, 3, 2, 1}; // 3 subs, 3 destroyers, 2 bs, 1 carr
    
    while (ships > 0) { // While there are ships left to choose
        bool cd_chosen = false, ship_chosen = false, valid;
        int ship, length; 
        int i, j;

        // Choose starting cd 
        i = rand() % 10; j = rand() % 10;

        // Choose ship
        int ships_i = 0;
while (ships_i < 4) {
    if (ships_remaining[ships_i] != 0) {
        ship = ships_i + 1;
        break;
    } else {
        ships_i++; 
    }
        }
        length = ship + 1;
            
        // Choose direction
        int dir;
        dir = rand() % 4;
        switch (dir) {
            case 0: valid = ship_ca(grid, i, j, 0, 1, length); break;
            case 1: valid = ship_ca(grid, i, j, 1, -1, length); break;
            case 2: valid = ship_ca(grid, i, j, 0, -1, length); break;
            case 3: valid = ship_ca(grid, i, j, 1, 1, length); break;
        }

        if (valid) {
            ships--;
            ships_remaining[ship-1]--;
        }
    }
}

void hit_notification(int id_self, int id_opp, bool hit) {
    string s = " missed!\n";
    if (hit) {
        s = " hit Player " + to_string(id_opp + 1) + "!\n";
    } cout << "Player " << to_string(id_self + 1) << s;
}


// Global Functions (Independent)
void grid_info(int grid[10][10]) {
// Prints grid at grid_id
// Precondition: 1 <= grid_id <= 2
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}


void grid_print(int grid[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (grid[i][j] == 0) {
                cout << "- ";
            } else if (grid[i][j] == 1) {
                cout << "1 ";
            } else if (grid[i][j] == 2) {
                cout << "O ";
            } else {
                cout << "X ";
            }
        }
        cout << endl;
    }
}


// Classes
class Player {
public:
    int id_self, id_opp;
    Player(int x, int y) {
    // id_self and id_opp must be unique integers either 1 or 2
        id_self = x;
        id_opp = y;
    }

    void grid_setup() {
    }

    void shoot() {
    }
};


class User : public Player { // Subclass of class Player
public:
    User(int x, int y) : Player(x, y) {} // Inherit the Player constructor

    void grid_setup() {
        string autoset;
            cout << "Auto set ships? [y/n]: "; cin >> autoset;
        if (autoset == "y") {
            grid_autosetup(grids[id_self]);
            grid_print(grids[id_self]); cout << endl;
            return;
        }

        int ships = 9;
        int ships_remaining[] = {3, 3, 2, 1}; // 3 subs, 3 destroyers, 2 bs, 1 carr
        cout << "Ship sizes - Submarine: 2 | Destroyer: 3 | Battleship: 4 | Carrier: 5\n";
    
        while (ships > 0) { // While there are ships left to choose
            bool cd_chosen = false, ship_chosen = false, valid;
            int ship, length; 
            int i, j;

            // Choose starting cd 
            while (!cd_chosen) { // Loop to ensure valid coordinates
                cout << "Enter coordinates for front of ship [a1-j10]: ";
                cd_chosen = cdtoid(i, j);
                if (!cd_chosen) {
                    invalid_input_notification();
                }
            }

            // Choose ship
            while (!ship_chosen) {
        cout << "Remaining choices - Submarines: " << 
        ships_remaining[0] << ", Destroyers: " << 
        ships_remaining[1] << ", Battleships: " << 
        ships_remaining[2] << ", Carriers: " << 
        ships_remaining[3] << endl;

        cout << "Choose a ship to place on your grid\n";
        cout << "[1] Submarine, [2] Destroyer, [3] battleship, [4] Carrier: "; 
    
        string ship_input; // To avoid non-integer user input
        cin >> ship_input;

        // Check input and complete if input is valid
        try {
            ship = stoi(ship_input);
        } catch (const exception e) {
            ship = 0; // Deliberate OOB to restart the loop
        }
    
        if (1 <= ship && ship <= 4 && ships_remaining[ship-1] != 0) {
                    length = ship + 1;
                    ship_chosen = true;
                } else {
                    invalid_input_notification();
                }
            }

            // Choose direction
            char dir;
            cout << "Direction [N/E/S/W]: ";
            cin >> dir;
            switch (dir) {
                case 'N':
                case 'n': valid = ship_ca(grids[id_self], i, j, 0, 1, length); break;
                case 'E':
                case 'e': valid = ship_ca(grids[id_self], i, j, 1, -1, length); break;
                case 'S':
                case 's': valid = ship_ca(grids[id_self], i, j, 0, -1, length); break;
                case 'W':
                case 'w': valid = ship_ca(grids[id_self], i, j, 1, 1, length); break;
                default: valid = false; invalid_input_notification(); break;
            }

            if (valid) {
                ships--;
                ships_remaining[ship-1]--;
            } else {
                invalid_input_notification();
            }
            grid_info(grids[id_self]);
        }
    }

    void shoot() {
    // shoot a player decided coordinate of the opponent's grid
    // 0 = empty, 1 = boat, 2 = miss, 3 = hit
        bool valid = false;
        int i, j;
        while (!valid) {
            cout << "\nEnemy Information Grid:\n";
            grid_print(hitgrids[id_opp]); cout << endl;

            cout << "Enter coordinates for attack: ";
            bool cd_chosen = cdtoid(i, j);
            if (cd_chosen && grids[id_opp][i][j] != 3) {
                if (grids[id_opp][i][j] == 0) {
                    hitgrids[id_opp][i][j] = 2;
                    hit_notification(id_self, id_opp, false);
                } else {
                    grids[id_opp][i][j] = 3;
                    hitgrids[id_opp][i][j] = 3;
                    rem[id_opp]--;
                    hit_notification(id_self, id_opp, true);
                }
                valid = true;
            } else {
                invalid_input_notification();
            }
        }
    }
};


class Ai : public Player {
public:
    int diff;
    int last_i = 0;
    int last_j = 0; // For more advanced difficulty
    int last_or = 5; // 0 = none hit, 1 = north, 2 = east, etc. 5 = none miss

    Ai(int x, int y) : Player(x, y) {
        diff_choose();
    } // Inherit the Player constructor and choose difficulty

    void diff_choose(){
        while (diff < 1 || diff > 3) {
            cout << "Enter difficulty of Player " << id_self + 1 << " [1-3]: ";
            cin >> diff;
            if (diff > 3 || diff < 1){
                invalid_input_notification();
            }
        }
    }

    void grid_setup() {
        grid_autosetup(grids[id_self]);
    }

    void shoot_calculation(bool result, int i, int j) {
        if (result) {
            grids[id_opp][i][j] = 3;
            hitgrids[id_opp][i][j] = 3;
            rem[id_opp]--;
        } else {
            hitgrids[id_opp][i][j] = 2;
        }

    hit_notification(id_self, id_opp, result);
    }

    void grid_print_opp() {
        cout << "\nPlayer " << to_string(id_opp+1) << " Grid:\n";
        grid_print(grids[id_opp]);
    }

    void shoot_1() {
        int i, j;
        do {
            i = rand() % 10; j = rand() % 10;
        } while (grids[id_opp][i][j] == 3);

        bool result;
        if (grids[id_opp][i][j] == 0) {
            result = false;
        } else {
            result = true;
        }
        shoot_calculation(result, i, j);
        grid_print_opp();
    }

    bool check_surrounded(int &ui, int &uj) {
    // Check if all previously hit spots have been surrounded. If so, return true. If not, mutate
    // i and j to the surrounding spot not checked
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (hitgrids[id_opp][i][j] == 3) {
                    if (i - 1 >= 0 && hitgrids[id_opp][i-1][j] < 2) { // North has not been shot
                        ui = i - 1;
                        uj = j;
                        return false; // May want to look into optimizing this code.
                    } else if (j + 1 < 10 && hitgrids[id_opp][i][j+1] < 2) { // East has not been shot
                        ui = i;
                        uj = j + 1;
                        return false;
                    } else if (i + 1 < 10 && hitgrids[id_opp][i+1][j] < 2) { // South has not been shot
                        ui = i + 1;
                        uj = j;
                        return false;
                    } else if (j - 1 >= 0 && hitgrids[id_opp][i][j-1] < 2) { // West has not been shot
                        ui = i;
                        uj = j - 1;
                        return false;
                    }
                }
            }
        }

        return true;
    }

    void scan_shoot() {
        for (int i = 0; i < 10; i++) {
            bool odd_displaced = (i % 2 == 0);
            for (int j = 0; j < 10; j += 2) {
                if (!odd_displaced) {
                    j++;
                    odd_displaced = true;
                }

                bool result;
                if (hitgrids[id_opp][i][j] == 0) {
                    if (grids[id_opp][i][j] == 1) {
                        result = true;
                    } else {
                        result = false;
                    }

                    shoot_calculation(result, i, j);
                    grid_print_opp();
                    return;
                }
            }
        }
    }

    void shoot_2() {
        int i, j;
        if (check_surrounded(i, j)) { // Check if all hits have been previously surrounded by hits/miss
            scan_shoot(); // If so, do the scan_shoot algorithm and reassign i, j
        } else {
            bool result;
            if (grids[id_opp][i][j] == 1) {
                result = true;
            } else { // As we have already checked that the given coords have not been hit
                result = false;
            }

            shoot_calculation(result, i, j);
            grid_print_opp();
        }
    }

    void shoot_3() {
    // This one cheats and looks directly at the other player's board
        for (int i = last_i; i < 10; i++) {
            for (int j = last_j; j < 10; j++) {
                if (grids[id_opp][i][j] == 1) {
                    shoot_calculation(true, i, j);
                    last_i = i; last_j = j + 1;
                    grid_print_opp();
                    return;
                }
            }

            last_j = 0;
        }
    }

    void shoot() {
        switch (diff) {
            case 1: shoot_1(); break;
            case 2: shoot_2(); break;
            case 3: shoot_3(); break; // So tempted to pull a YandereDev right here
            default: cout << "This should never happen due to an earlier check.\n";
        }
    }
};

class Game {
public:
    Player* p[2]; // Array of pointers to objects of class Player allows subclass ptrs
    int p_type[2]; // Integer representation of types
    Game() {
        cout << "Player Types: [1] User, [2] Bot\n";
        cout << "Choose Player 1 [1-2]: "; cin >> p_type[0];
        cout << "Choose Player 2 [1-2]: "; cin >> p_type[1];
        
        for (int i = 0; i < 2; i++) {
            if (p_type[i] == 1) {
                p[i] = new User(0+i, 1-i); // We must only assign pointers to new objects
            } else {
                p[i] = new Ai(0+i, 1-i);
            }
        }

        for (int i = 0; i < 2; i++) {
            if (p_type[i] == 1) {
                ((User*) p[i]) -> grid_setup(); // We use arrow to access ptr obj's method
            } else {
                ((Ai*) p[i]) -> grid_setup(); // Note the bracketed type cast of Player* to Ai*
            }
        }

        rem[0] = 28; rem[1] = 28; // In case we want to implement a game continue function
    }

    void play() {
        while (rem[0] > 0 && rem[1] > 0) {
            for (int i = 0; i < 2; i++) {
                if (p_type[i] == 1) {
                    ((User*) p[i]) -> shoot();
                } else {
                    ((Ai*) p[i]) -> shoot();
                }
            }
        }

        if (rem[1] == 0) {
            cout << "Player 1 Wins!\n";
        } else {
            cout << "Player 2 Wins!\n";
        }
    }
};


// Global Functions (Dependent)
int main() {
    srand((unsigned) time(0));
    Game g;
    g.play();
}