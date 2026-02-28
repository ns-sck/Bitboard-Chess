#include <iostream>
#include <string>
#include "Game.h"
#include "Move.h"
#include "BitUtil.h"
#include <map>

using namespace std;

int main() {
    init();
    Game game;
    game.reset_board();
    
    cout << "Chess Game" << endl;
    cout << "Enter moves in format 'e2e4'" << endl;
    cout << "Commands: 'quit' to exit, 'moves' to see legal moves" << endl;
    
    while (!game.is_game_over()) {
        game.print_board();
        
        string move_str;
        auto legal_moves = game.generate_legal_moves();
        map<std::string, Move> mp;
        for (auto move : legal_moves) {
            int from = get_mask(move.get_info(), 7, 12);
            int to = get_mask(move.get_info(), 13, 18);
            cout << square_to_algebraic[from] << ' ' << square_to_algebraic[to] << " | ";
            string s = square_to_algebraic[from] + square_to_algebraic[to];
            mp.insert({s, move});
        }
        cout << endl;
        cin >> move_str;
   
        if (move_str == "quit") {
            break;
        }

        try {
            if (mp.count(move_str)) {
                Move mv = game.parse_move_string(move_str);
                game.make_move(mv);
            } else {
                cout << "Illegal move. Try again." << endl;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    GameState finalState = game.get_state();
    if (finalState == GameState::CHECKMATE) {
        cout << (!game.is_white_to_move() ? "White" : "Black") << " wins by checkmate!" << endl;
    } else if (finalState == GameState::STALEMATE) {
        cout << "Game drawn by stalemate!" << endl;
    }
    
    return 0;
} 