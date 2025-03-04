#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>   // For Math Calculation Handling
#include <random> // For Random Number Generation
#include <thread> // Threading Library To Handle Threading 
#include <mutex> // Mutex Library To Apply Locks And Mutes 
#include <chrono> //Chrono Library Calculate Execution Time 

using namespace std;

// USER Player1 
int player1 = 1; // Current USER Player 1 Number 
char boardPlayer1[10][10]; // USER Player 1 Board 

// AI Player2 
int player2 = 2; // Current AI Player 2 Number
char boardPlayer2[10][10]; // AI Player 2 Board 

int moveAmount = 0; // Amount Of Moves To Win The Game

// Battleship Board States
const char WATER = '~';
const char SHIP  = 'S';
const char HIT   = 'X';
const char MISS  = 'O';

// Battleship Pieces 
// First Index Ships X Coordinate 
// Second Index Ships Y Coordinate
// Third Index Current Player 

// Player Battle Ships
int carrier1[5][2][2]; // USER Carrier Ship - 5 Holes
int battleship1[4][2][2]; // USER Battleship Ship - 4 Holes
int cruiser1[3][2][2]; // USER Cruiser Ship - 3 Holes
int submarine1[3][2][2]; // USER Submarine Ship - 3 Holes
int destroyer1[1][2][2]; // USER Destroyer Ship - 2 Holes

// AI Battle Ships
int carrier2[5][2][2]; // AI Carrier Ship -4 Holes
int battleship2[4][2][2]; // AI Battleship Ship - 4 Holes
int cruiser2[3][2][2]; // AI Cruiser Ship - 3 Holes
int submarine2[3][2][2]; // AI Submarine Ship - 3 Holes
int destroyer2[1][2][2]; // AI Destroyer Ship - 2 Holes

// A Node In The Monte Carlo Search Tree
// Each Node Represents A Specific Board Configuration 
struct MCTSNode {
    // MCTS Metrics
    double wins = 0.0;      // How Many "Wins" From Playout
    int visits = 0;         // How Many Times This Node Has Been Visited
    int moveRow = -1;       // Row of The Move Leading To This Node
    int moveCol = -1;       // Col of The Move Leading To This Node

    bool terminal = false;  // Is The Current State A Winned Game?
    
    char boardState[10][10]; // Board configuration for this node

    // Children Nodes
    vector<MCTSNode*> children;
    MCTSNode* parent = nullptr;
};

int MCTS_ITERATIONS = 200; // Amount Of Iterations To Run The MST

                                        //// Functions Declarations ////

// Board Functions 
void initializeBoard(char playerBoard[10][10]); // Initialize Player # Battleship Board
void printBoard(char board[10][10], int playerNumber); // Print Battleship Board

// Monte Carlos Search Tree Functions 
void monteCarloTreeSearch(char playerBoard[10][10]); // Monte Carlo Tree Search 
MCTSNode* runMCTSPhases(MCTSNode* root); // Run Monte Carlo 4 Phases Selection, Expansion, Simulation, and Backpropagation

// Monte Carlo Search Tree 4 Phases Functions  
MCTSNode* selection(MCTSNode* node); // Selection: Select A Child By The UCB Score Based On The Current Strategy
void expansion(MCTSNode* node); // Expand: Expand Create A New Child Node And Add It To The Tree 
double simulation(MCTSNode* node); // simulation: simulation Board States And Find The Most Optimal
void backpropagation(MCTSNode* node, double result); // backpropagation: backpropagation Through The Tree And Update It 

// Monte Carlo Search Tree Helper Functions 
bool isGameOver(const char board[10][10]); // Checks If The Board Has Any Ships 'S' Left
vector<pair<int,int>> getPossibleMoves(const char board[10][10]); // Get All Possible Moves
bool applyMove(char board[10][10], int row, int col); // Apply Possible Moves
void copyBoard(const char source[10][10], char dest[10][10]); // Copy The State Of The Board
void deleteTree(MCTSNode* root); // Clean Up Child Pointes
double UCB(MCTSNode* node); // Our UCB Score For Child Node Selection

                                        //// MAIN Function ////

int main() {
    // initializeBoard(boardPlayer1); // Initialize All Empty Spaces From Player 1 Board To Water Spaces '~'
    // initializeBoard(boardPlayer2); // Initialize All Empty Spaces From Player 2 Board To Water Spaces '~'

    // For Now We Are Trying To Solve This Current Case Board With The AI Monte Carlo Search Tree
    char boardPlayer1[10][10] = {
        {'~', '~', '~', '~', '~', '~', '~', 'S', 'S', '~'},
        {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
        {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
        {'~', '~', 'S', 'S', 'S', 'S', 'S', '~', '~', '~'},
        {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'},
        {'~', '~', '~', '~', '~', '~', 'S', '~', '~', '~'},
        {'~', 'S', '~', '~', '~', '~', 'S', '~', '~', 'S'},
        {'~', 'S', '~', '~', '~', '~', 'S', '~', '~', 'S'},
        {'~', 'S', '~', '~', '~', '~', 'S', '~', '~', 'S'},
        {'~', '~', '~', '~', '~', '~', '~', '~', '~', '~'}
    };

    printBoard(boardPlayer1, player1); // Print Initial Board

    // Now We Run The Monte Carlo Function
    auto start = chrono::high_resolution_clock::now(); // Start Timing For Showroom Guests
    monteCarloTreeSearch(boardPlayer1); // Mpnte Carlo Battleship Algorithm
    auto end = chrono::high_resolution_clock::now(); // End Timing For Showroom Guests
    auto result = chrono::duration_cast<chrono::milliseconds>(end - start); //Stop Timer

    cout << "MCTS AI has finished searching for ships!" << endl;
    cout << "All ships found. AI wins!" << endl;
    cout << "Amount of moves: " << moveAmount << endl; // Display Total Amount Of Movements For The Algorithm To Win The Game
    cout << "Time Result: " << result.count() << " ms" << endl; // Display Total Amount Of For The Algorithm To Run In Milliseconds
    cout << "Time Result: " << (result.count() / 1000) << " s" << endl << endl;  // Display Total Amount Of For The Algorithm To Run In Seconds

    printBoard(boardPlayer1, player1); // Print The Finalized Board Again To See All Hits

    return 0;
}

                                        //// Functions Creation ////

// Initialize The Board With WATER Characters
void initializeBoard(char playerBoard[10][10]) {
    // All Board Equal to '~' Water Spaces
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            playerBoard[i][j] = WATER;
        }
    }
}

// Print Battleship Board
void printBoard(char board[10][10], int playerNumber) {
    cout << "\tPlayer " <<  playerNumber << " Board\n\n";

    // Column Letters A - J
    cout << "   ";
    for (int col = 0; col < 10; col++) {
        cout << (char)('A' + col) << "  ";
    }
    cout << endl;

    // Rows 1 - 10
    for (int row = 0; row < 10; row++) {
        if (row < 9) {
            cout << row+1 << "  ";
        } else {
            // row = 9 => "10"
            cout << row + 1 << " ";
        }
        for (int col = 0; col < 10; col++) {
            cout << board[row][col] << "  ";
        }
        cout << endl;
    }

    cout << endl;
}

                                // --------- Main MCTS ---------- //

void monteCarloTreeSearch(char playerBoard[10][10]) {
    // We will run MCST and Run Multiple Times Until We Find An Optimal Move 
    // We Run The 4 Phases Slection, Expansion, Simulation, Backpropagation 
    // We do This Until We Win The Game Meaning No 'S' Are Left On The Board
    while (!isGameOver(playerBoard)) {
        // Build Root Node For Current Board
        MCTSNode* root = new MCTSNode();
        copyBoard(playerBoard, root->boardState);
        root->terminal = isGameOver(root->boardState);

        // If The Board Is Slready Done Then Break
        if(root->terminal) {
            delete root;
            break;
        }

        // Expand Root So We Can Have Children Nodes 
        expansion(root);

        // Run MCTS From Root To Check For Best Move
        MCTSNode* bestChild = runMCTSPhases(root);
        
        // If There Is No Best Child Node Then Break To Avoid An Infinite Loop
        if(!bestChild) {
            deleteTree(root);
            break;
        }

        // Apply The Current Selected Move To The Player Board To Give The AI Input
        applyMove(playerBoard, bestChild->moveRow, bestChild->moveCol);

        // Delete Current Tree 
        deleteTree(root);

        printBoard(playerBoard, player1); // Print Current Selected Move For DEBBUGING PURPOSES 

        moveAmount++; // Increse Current Amount Of Moves 

        // If The Game Is Not Over Repeat Process 
    }
}

// Run MCTS Iterations From A Given Root And Then Pick The Best Child For The Best Move
MCTSNode* runMCTSPhases(MCTSNode* root) {
    // Run Trough All The 4 MCST Phases
    for(int i = 0; i < MCTS_ITERATIONS; i++){
        // 1) SELECTION
        MCTSNode* current = root;
        // Traverse The Tree In Descending It 
        while(!current->children.empty() && !current->terminal) {
            current = selection(current); // Pick The Best Child By Its UCB Score
        }

        // 2) EXPANSION
        if(!current->terminal) {
            // Expand Children If Needed 
            if(current->children.empty()) {
                expansion(current);
            }
            // If There Are Children Pick One At Random
            if(!current->children.empty()) {
                static std::mt19937 rng((unsigned)std::random_device{}());
                std::uniform_int_distribution<int> dist(0, (int)current->children.size() - 1);
                current = current->children[dist(rng)];
            }
        }

        // 3) SIMULATION
        double result = simulation(current);

        // 4) BACKPROPAGATION
        backpropagation(current, result);
    }

    
    double bestRate = -1.0;

    MCTSNode* bestChild = nullptr;

    // After All  The Iterations We Have To Pick The Child Node With The Highest Win Rate That is wins/visits
    for (auto* child : root->children) {
        if(child->visits > 0) {
            double rate = child->wins / (double)child->visits;
            if(rate > bestRate) {
                bestRate = rate;
                bestChild = child;
            }
        }
    }

    return bestChild; // We Return The Best Child Posible That Is Our Current Best Move
}

                                // --------- MCTS Phases ---------- //

// Select Child By Its UCB Score
MCTSNode* selection(MCTSNode* node) {
    MCTSNode* bestChild = nullptr;
    double bestValue = -1e9;

    // Search Each Children Node And Compare Who Has The Best UCB Score
    for (auto* child : node->children) {
        double ucbValue = UCB(child);
        if (ucbValue > bestValue) {
            bestValue = ucbValue;
            bestChild = child;
        }
    }

    return bestChild; // We Return The Best Child Posible That Is Our Current Best Move Based On Its UCB Score
}

// Expansion: Create A New Child For Each Possible Move From This Current Node's Board
void expansion(MCTSNode* node) {
    if(node->terminal) return; // No Expansion If Game Is Over

    vector<pair<int,int>> moves = getPossibleMoves(node->boardState); // Vector Pair To Check For All Possible Moves 

    // Check For The Amount Of Moves That can Be Performed 
    for (auto& m : moves) {
        int r = m.first;
        int c = m.second;

        // Create A Child Node From Applying This Move
        MCTSNode* child = new MCTSNode();
        child->parent = node;
        copyBoard(node->boardState, child->boardState); // Copy The Board State

        applyMove(child->boardState, r, c); // Apply The Move

        child->moveRow = r;
        child->moveCol = c;
        child->terminal = isGameOver(child->boardState); // Check If The Game Is Over 

        node->children.push_back(child); // Push The Node Back
    }
}

// Simulation: Simualte A Random Playout Starting From The Current Node's Board State
double simulation(MCTSNode* node) {
    // Make A Copy Of The Board To Not Overwrite The Current Node's Boards State
    char simBoard[10][10];
    copyBoard(node->boardState, simBoard); // Copy Board State

    // Check For Game Over To See If We Already Arrived To A Winning State
    if(isGameOver(simBoard)) {
        return 1.0; 
    }

    static std::mt19937 rng((unsigned)std::random_device{}());
    int moveLimit = 100; // Limit Amount Of Moves To Avoid Going Overboard 
    
    // We Apply Strategic move Until We Hit All Ships 'S' Spaces
    for(int i = 0; i < moveLimit; i++){
        // Check For Game Over To See If We Already Arrived To A Winning State
        if(isGameOver(simBoard)) {
            return 1.0;
        }

        auto possible = getPossibleMoves(simBoard);
        
        // If There Is No Moves That Exisit We Are Stuck And We Have To Break
        if(possible.empty()) {
            break; 
        } 
        
        // Pick A Random Move For Now We Will Implement Our Startegy Later 
        std::uniform_int_distribution<int> dist(0, (int)possible.size() - 1);
        int idx = dist(rng);

        int rr = possible[idx].first;
        int cc = possible[idx].second;

        applyMove(simBoard, rr, cc); // Apply Our Current Selected Move 
    }

    // Check If We Won The Game And Ended With No 'S' Left
    if(isGameOver(simBoard)) {
        return 1.0;
    }

    return 0.0;
}

// Backpropagation Backporpagate The Result Up The Search Tree
void backpropagation(MCTSNode* node, double result) {
    MCTSNode* current = node;
    while (current != nullptr) {
        current->visits += 1;
        current->wins += result; 
        current = current->parent;
    }
}

                                // --------- MCTS Helper Functions ---------- //

// Checks If The Board Has Any Ship 'S' Spaces Left
// For Now We Will use This Implementation But Ther Is A Better One 
bool isGameOver(const char board[10][10]) {
    for(int r = 0; r < 10; r++) {
        for(int c = 0; c < 10; c++) {
            if(board[r][c] == SHIP) return false;
        }
    }

    return true;
}

// Get The Best Possible Move 
// Possible Moves Are Any Cell That Is Not HIT 'X' And Not MISS 'O' That Means It Could Be Water Space '~' Or Ship Space 'S'
vector<pair<int,int>> getPossibleMoves(const char board[10][10]) {
    vector<pair<int,int>> moves;
    for(int r = 0; r < 10; r++){
        for(int c = 0; c < 10; c++){
            if(board[r][c] != HIT && board[r][c] != MISS){
                moves.push_back({r, c});
            }
        }
    }

    return moves;
}

// Apply The Best Move 
// Apply A Move To The Board Return Whether The Applied Move Was A Hit Or Not
bool applyMove(char board[10][10], int row, int col) {
    if (board[row][col] == SHIP) {
        board[row][col] = HIT;
        return true; // Hit
    } else {
        board[row][col] = MISS;
        return false; // Miss
    }
}

// Copy The Board Current State To A New One 
void copyBoard(const char source[10][10], char dest[10][10]) {
    for(int r = 0; r < 10; r++){
        for(int c = 0; c < 10; c++){
            dest[r][c] = source[r][c];
        }
    }
}

                                // --------- MCTS Struct Helper Functions ---------- //

// Clean Up Child Pointers To Delete The Tree
void deleteTree(MCTSNode* root) {
    for(MCTSNode* child : root->children) {
        deleteTree(child);
    }
    
    delete root;
}

// UCB Score
double UCB(MCTSNode* node) {
    // Our UCB Score Will Be Handled As Follows = (wins / visits) + sqrt(2 * ln(parent->visits) / visits)
    if (node->visits == 0) return std::numeric_limits<double>::infinity();
    double exploitation = node->wins / (double)node->visits;
    double exploration  = sqrt(2.0 * log((double)node->parent->visits) / (double)node->visits);
    return exploitation + exploration;
}