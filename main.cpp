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
            int promote = get_mask(move.get_info(), 0, 3);
            if (promote > 6) promote -= 6;
            string pcs[6] = {"", "", "N", "B", "R", "Q"}; 
            cout << square_to_algebraic[from]  << square_to_algebraic[to] << pcs[promote];
            cout << " | ";
            string s = square_to_algebraic[from] + square_to_algebraic[to] + pcs[promote];
            mp.insert({s, move});
        }
        cout << endl;
        cin >> move_str;

        
        try {
            auto it = mp.find(move_str);
            if (it != mp.end()) {
                Move mv = it->second;
                game.make_move(mv);
            } else {
                cout << "Illegal move. Try again." << endl;
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }

        if (move_str == "reset") {
            game.reset_board();
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