#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Chess
{

private:
    
    char BLACK = 'b';
    char WHITE = 'w';
    
    bool EMPTY = -1;

    char PAWN = 'p';
    char KNIGHT = 'n';
    char BISHOP = 'b';
    char ROOK = 'r';
    char QUEEN = 'q';
    char KING = 'k';

    string SYMBOLS = "pnbrqkPNBRQK";
    string DEFAULT_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    string POSSIBLE_POSITION[4] = {"1-0", "0-1", "1/2-1/2", "*"};

    // TODO
    int PAWN_OFFSETS[8] = { -16, -32, -17, -15, 16, 32, 17, 15 };
    int KNIGHT_OFFSETS[8] = { -18, -33, -31, -14, 18, 33, 31, 14 };
    int BISHOP_OFFSETS[4] = { -17, -15, 17, -15 };
    int ROOK_OFFSETS[4] = { -16, -1, 16, 1 };
    int QUEEN_OFFSETS[8] = { -17, -16, -15, 1, 17, 16, 15, -1 };
    int KING_OFFSETS[8] = { -17, -16, -15, 1, 17, 16, 15, -1 };

    int ATTACKS[240] = {
        
        20, 0, 0, 0, 0, 0, 0, 24,  0, 0, 0, 0, 0, 0,20, 0,
        0,20, 0, 0, 0, 0, 0, 24,  0, 0, 0, 0, 0,20, 0, 0,
        0, 0,20, 0, 0, 0, 0, 24,  0, 0, 0, 0,20, 0, 0, 0,
        0, 0, 0,20, 0, 0, 0, 24,  0, 0, 0,20, 0, 0, 0, 0,
        0, 0, 0, 0,20, 0, 0, 24,  0, 0,20, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,20, 2, 24,  2,20, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 2,53, 56, 53, 2, 0, 0, 0, 0, 0, 0,
        24,24,24,24,24,24,56,  0, 56,24,24,24,24,24,24, 0,
        0, 0, 0, 0, 0, 2,53, 56, 53, 2, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,20, 2, 24,  2,20, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0,20, 0, 0, 24,  0, 0,20, 0, 0, 0, 0, 0,
        0, 0, 0,20, 0, 0, 0, 24,  0, 0, 0,20, 0, 0, 0, 0,
        0, 0,20, 0, 0, 0, 0, 24,  0, 0, 0, 0,20, 0, 0, 0,
        0,20, 0, 0, 0, 0, 0, 24,  0, 0, 0, 0, 0,20, 0, 0,
        20, 0, 0, 0, 0, 0, 0, 24,  0, 0, 0, 0, 0, 0,20

    };

    int RAYS[240] = {

        17,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0, 15, 0,
        0, 17,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0, 15,  0, 0,
        0,  0, 17,  0,  0,  0,  0, 16,  0,  0,  0,  0, 15,  0,  0, 0,
        0,  0,  0, 17,  0,  0,  0, 16,  0,  0,  0, 15,  0,  0,  0, 0,
        0,  0,  0,  0, 17,  0,  0, 16,  0,  0, 15,  0,  0,  0,  0, 0,
        0,  0,  0,  0,  0, 17,  0, 16,  0, 15,  0,  0,  0,  0,  0, 0,
        0,  0,  0,  0,  0,  0, 17, 16, 15,  0,  0,  0,  0,  0,  0, 0,
        1,  1,  1,  1,  1,  1,  1,  0, -1, -1,  -1,-1, -1, -1, -1, 0,
        0,  0,  0,  0,  0,  0,-15,-16,-17,  0,  0,  0,  0,  0,  0, 0,
        0,  0,  0,  0,  0,-15,  0,-16,  0,-17,  0,  0,  0,  0,  0, 0,
        0,  0,  0,  0,-15,  0,  0,-16,  0,  0,-17,  0,  0,  0,  0, 0,
        0,  0,  0,-15,  0,  0,  0,-16,  0,  0,  0,-17,  0,  0,  0, 0,
        0,  0,-15,  0,  0,  0,  0,-16,  0,  0,  0,  0,-17,  0,  0, 0,
        0,-15,  0,  0,  0,  0,  0,-16,  0,  0,  0,  0,  0,-17,  0, 0,
        -15,  0,  0,  0,  0,  0,  0,-16,  0,  0,  0,  0,  0,  0,-17

    };

    // SHIFTS
    int PAWN_SHIFT = 0;
    int KNIGHT_SHIFT = 1;
    int ROOK_SHIFT = 2;
    int QUEEN_SHIFT = 3;
    int KING_SHIFT = 4;

    // FLAGS
    char NORMAL_FLAG = 'n';
    char CAPTURE_FLAG = 'c';
    char BIG_PAWN_FLAG = 'b';
    char EP_CAPTURE_FLAG = 'e';
    char PROMOTION_FLAG = 'p';
    char KSIDE_CASTLE_FLAG = 'k';
    char QSIDE_CASTLE_FLAG = 'q';

    // BITS
    int NORMAL_BIT = 1;
    int CAPTURE_BIT = 2;
    int BIG_PAWN_BIT = 3;
    int EP_CAPTURE_BIT = 8;
    int PROMOTION_BIT = 16;
    int KSIDE_CASTLE_BIT = 32;
    int QSIDE_CASTLE_BIT = 64;

    // RANK
    int RANK_1 = 7;
    int RANK_2 = 6;
    int RANK_3 = 5;
    int RANK_4 = 4;
    int RANK_5 = 3;
    int RANK_6 = 2;
    int RANK_7 = 1;
    int RANK_8 = 0;

    // SQUARE
    int SQUARE_a8 = 0;
    int SQUARE_b8 = 1;
    int SQUARE_c8 = 2;
    int SQUARE_d8 = 3;
    int SQUARE_e8 = 4;
    int SQUARE_f8 = 5;
    int SQUARE_g8 = 6;
    int SQUARE_h8 = 7;
    int SQUARE_a7 = 16;
    int SQUARE_b7 = 17;
    int SQUARE_c7 = 18;
    int SQUARE_d7 = 19;
    int SQUARE_e7 = 20;
    int SQUARE_f7 = 21;
    int SQUARE_g7 = 22;
    int SQUARE_h7 = 23;
    int SQUARE_a6 = 32;
    int SQUARE_b6 = 33;
    int SQUARE_c6 = 34;
    int SQUARE_d6 = 35;
    int SQUARE_e6 = 36;
    int SQUARE_f6 = 37;
    int SQUARE_g6 = 38;
    int SQUARE_h6 = 39;
    int SQUARE_a5 = 48;
    int SQUARE_b5 = 49;
    int SQUARE_c5 = 50;
    int SQUARE_d5 = 51;
    int SQUARE_e5 = 52;
    int SQUARE_f5 = 53;
    int SQUARE_g5 = 54;
    int SQUARE_h5 = 55;
    int SQUARE_a4 = 64;
    int SQUARE_b4 = 65;
    int SQUARE_c4 = 66;
    int SQUARE_d4 = 67;
    int SQUARE_e4 = 68;
    int SQUARE_f4 = 69;
    int SQUARE_g4 = 70;
    int SQUARE_h4 = 71;
    int SQUARE_a3 = 80;
    int SQUARE_b3 = 81;
    int SQUARE_c3 = 82;
    int SQUARE_d3 = 83;
    int SQUARE_e3 = 84;
    int SQUARE_f3 = 85;
    int SQUARE_g3 = 86;
    int SQUARE_h3 = 87;
    int SQUARE_a2 = 96;
    int SQUARE_b2 = 97;
    int SQUARE_c2 = 98;
    int SQUARE_d2 = 99;
    int SQUARE_e2 = 100;
    int SQUARE_f2 = 101;
    int SQUARE_g2 = 102;
    int SQUARE_h2 = 103;
    int SQUARE_a1 = 112;
    int SQUARE_b1 = 113;
    int SQUARE_c1 = 114;
    int SQUARE_d1 = 115;
    int SQUARE_e1 = 116;
    int SQUARE_f1 = 117;
    int SQUARE_g1 = 118;
    int SQUARE_h1 = 119;

    // ROOKS
    int ROOKS_W_SQUARE[2] = { SQUARE_a1, SQUARE_h1 };
    int ROOKS_W_FLAG[2] = { QSIDE_CASTLE_BIT, KSIDE_CASTLE_BIT };
    int ROOKS_B_SQUARE[2] = { SQUARE_a8, SQUARE_h8 };
    int ROOKS_B_FLAG[2] = { QSIDE_CASTLE_BIT, KSIDE_CASTLE_BIT };

    // TODO var board = new Array(128);

    // ETC
    // TODO var kings = {w: EMPTY, b: EMPTY};
    int kings_w;
    int kings_b;


};

