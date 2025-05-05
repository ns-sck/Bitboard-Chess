#include <iostream>
#include <string>
#include "Game.h"
#include "Move.h"
#include "BitUtil.h"

using namespace std;

Move parseMove(const string& moveStr) {
    if (moveStr.length() < 4) {
        throw invalid_argument("Invalid move format. Expected format: e2e4");
    }
    
    int sourceFile = moveStr[0] - 'a';
    int sourceRank = moveStr[1] - '1';
    
    int destFile = moveStr[2] - 'a';
    int destRank = moveStr[3] - '1';
    
    if (sourceFile < 0 || sourceFile > 7 || sourceRank < 0 || sourceRank > 7 ||
        destFile < 0 || destFile > 7 || destRank < 0 || destRank > 7) {
        throw invalid_argument("Invalid square coordinates");
    }
    
    int from = sourceRank * 8 + sourceFile;
    int to = destRank * 8 + destFile;
    
    return Move(from | (to << 6));
}

int main() {
    init();
    Game game;
    game.resetBoard();
    
    cout << "Chess Game" << endl;
    cout << "Enter moves in format 'e2e4'" << endl;
    cout << "Commands: 'quit' to exit, 'moves' to see legal moves" << endl;
    
    while (!game.isGameOver()) {
        game.printBoard();
        
        cout << (game.isWhiteToMove() ? "White" : "Black") << " to move:\n";
        string moveStr;
        auto legalMoves = game.generateLegalMoves();
        for (auto move : legalMoves) {
            int from = move.getInfo() & 63;
            int to = (move.getInfo() >> 6) & 63;
            cout << square_to_algebraic[from] << ' ' << square_to_algebraic[to] << " | ";
        }
        cin >> moveStr;
        
        if (moveStr == "quit") {
            break;
        }
        
        try {
            Move move = parseMove(moveStr);
            if (game.makeSimpleMove(move)) {
            } else {
                cout << "Illegal move. Try again." << endl;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    GameState finalState = game.getState();
    if (finalState == GameState::CHECKMATE) {
        cout << (!game.isWhiteToMove() ? "White" : "Black") << " wins by checkmate!" << endl;
    } else if (finalState == GameState::STALEMATE) {
        cout << "Game drawn by stalemate!" << endl;
    }
    
    return 0;
} 