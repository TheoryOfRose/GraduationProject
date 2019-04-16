#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Chess
{

private:
   
   /****************************************************************************
   *****************************************************************************
        Member Data
   *****************************************************************************
   ****************************************************************************/
    char BLACK = 'b';
    char WHITE = 'w';
    int  EMPTY = -1;

    char PAWN = 'p';
    char KNIGHT = 'n';
    char BISHOP = 'b';
    char ROOK = 'r';
    char QUEEN = 'q';
    char KING = 'k';

    string SYMBOLS = "pnbrqkPNBRQK";
    string DEFAULT_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    string POSSIBLE_POSITION[4] = {"1-0", "0-1", "1/2-1/2", "*"};

    // OFFSETS
    map<char, int[4]> PAWN_OFFSETS;
    map<char, int[8]> PIECE_OFFSETS;

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
    map<char,int> SHIFTS;
    // FLAGS
    map<string,char> FLAGS;
    // BITS
    map<string,int> BITS;
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
    map<string,int> SQUARES;

    // ROOKS
    map<char,map<string,int>[2]> ROOKS;

    // KINGS
    map<char,int> kings;

    // CASTLING
    map<char,int> castling;
    // BOARD
    map<string,char>* board = new map<string,char>[128];

    // ETC
    char turn = WHITE;
    int ep_square = EMPTY;
    int half_moves = 0;
    int move_number = 1;
    
    // TODO var history = []; var header = {};
    /****************************************************************************
     *****************************************************************************
         Member Function
     *****************************************************************************
     ****************************************************************************/
    void clear();
    void reset();
    void load(string fen);
    bool validate_fen(string fen);
    string generate_fen();
    // set_header
    void update_setup(string fen);
    map<string,char> get(string square);
    void put(map<string,char> piece, string square);
    map<string,char> remove(string square);
    map<string,string> build_move(map<string,char>* board,string from, string to, int flags, int promotion);
    //generate_moves()
    // move_to_san
    // stripped_san
    // attacked
    // king_attacked
    // in_check
    // in_checkmate
    // in_stalemate
    // insufficient_material
    // in_threefold_repetition
    // push
    // make_move
    // undo_move
    // get_disambiguator
    // ascii
    // move_from_san
    int rank(int i);
    int file(int i);
    string algebraic(int i);
    char swap_color(char c);
    

public:

    Chess();    // Default Constructor
    Chess(string fen);  // Constructor with fen code


};

Chess::Chess(){

    cout<<"Chess Default Constructor"<<endl;

    // OFFSETS
    PAWN_OFFSETS['b'][0] = 16;PAWN_OFFSETS['b'][1] = 32;
    PAWN_OFFSETS['b'][2] = 17;PAWN_OFFSETS['b'][3] = 15;
    PAWN_OFFSETS['w'][0] = -16;PAWN_OFFSETS['w'][1] = -32;
    PAWN_OFFSETS['w'][2] = -17;PAWN_OFFSETS['w'][3] = -15;

    PIECE_OFFSETS['n'][0]=-18;PIECE_OFFSETS['n'][1]=-33;
    PIECE_OFFSETS['n'][2]=-31;PIECE_OFFSETS['n'][3]=-14;
    PIECE_OFFSETS['n'][4]=18;PIECE_OFFSETS['n'][5]=33;
    PIECE_OFFSETS['n'][6]=31;PIECE_OFFSETS['n'][7]=14;
    
    PIECE_OFFSETS['b'][0]=-17;PIECE_OFFSETS['b'][1]=-15;
    PIECE_OFFSETS['b'][2]=-17;PIECE_OFFSETS['b'][4]=15;
    
    PIECE_OFFSETS['r'][0]=-16;PIECE_OFFSETS['r'][1]=1;
    PIECE_OFFSETS['r'][2]=16;PIECE_OFFSETS['r'][3]=-1;
    
    PIECE_OFFSETS['q'][0]=-17;PIECE_OFFSETS['q'][1]=-16;
    PIECE_OFFSETS['q'][2]=-15;PIECE_OFFSETS['q'][3]=1;
    PIECE_OFFSETS['q'][4]=17;PIECE_OFFSETS['q'][5]=16;
    PIECE_OFFSETS['q'][6]=15;PIECE_OFFSETS['q'][7]=-1;
    
    PIECE_OFFSETS['k'][0]=-17;PIECE_OFFSETS['k'][1]=-16;
    PIECE_OFFSETS['k'][2]=-15;PIECE_OFFSETS['k'][3]=1;
    PIECE_OFFSETS['k'][4]=17;PIECE_OFFSETS['k'][5]=16;
    PIECE_OFFSETS['k'][6]=15;PIECE_OFFSETS['k'][7]=-1;

    // FLAGS
    FLAGS["NORMAL"]='n';
    FLAGS["CAPTURE"]='c';
    FLAGS["BIG_PAWN"]='b';
    FLAGS["EP_CAPTURE"]='e';
    FLAGS["PROMOTION"]='p';
    FLAGS["KSIDE_CASTLE"]='k';
    FLAGS["QSIDE_CASTLE"]='q';

    // BITS
    BITS["NORMAL"]=1;
    BITS["CAPTURE"]=2;
    BITS["BIG_PAWN"]=4;
    BITS["EP_CAPTURE"]=8;
    BITS["PROMOTION"]=16;
    BITS["KSIDE_CASTLE"]=32;
    BITS["QSIDE_CASTLE"]=64;

    // SQUARES
    SQUARES["a8"] = 0; SQUARES["b8"] = 1; SQUARES["c8"] = 2; SQUARES["d8"] = 3;
    SQUARES["e8"] = 4; SQUARES["f8"] = 5; SQUARES["g8"] = 6; SQUARES["h8"] = 7;
    SQUARES["a7"] = 16; SQUARES["b7"] = 17; SQUARES["c7"] = 18; SQUARES["d7"] = 19;
    SQUARES["e7"] = 20; SQUARES["f7"] = 21; SQUARES["g7"] = 22; SQUARES["h7"] = 23;
    SQUARES["a6"] = 32; SQUARES["b6"] = 33; SQUARES["c6"] = 34; SQUARES["d6"] = 35;
    SQUARES["e6"] = 36; SQUARES["f6"] = 37; SQUARES["g6"] = 38; SQUARES["h6"] = 39;
    SQUARES["a5"] = 48; SQUARES["b5"] = 49; SQUARES["c5"] = 50; SQUARES["d5"] = 51;
    SQUARES["e5"] = 52; SQUARES["f5"] = 53; SQUARES["g5"] = 54; SQUARES["h5"] = 55;
    SQUARES["a4"] = 64; SQUARES["b4"] = 65; SQUARES["c4"] = 66; SQUARES["d4"] = 67;
    SQUARES["e4"] = 68; SQUARES["f4"] = 69; SQUARES["g4"] = 70; SQUARES["h4"] = 71;
    SQUARES["a3"] = 80; SQUARES["b3"] = 81; SQUARES["c3"] = 82; SQUARES["d3"] = 83;
    SQUARES["e3"] = 84; SQUARES["f3"] = 85; SQUARES["g3"] = 86; SQUARES["h3"] = 87;
    SQUARES["a2"] = 96; SQUARES["b2"] = 97; SQUARES["c2"] = 98; SQUARES["d2"] = 99;
    SQUARES["e2"] =100;SQUARES["f2"] = 101;SQUARES["g2"] = 102;SQUARES["h2"] = 103;
    SQUARES["a1"] =112;SQUARES["b1"] = 113;SQUARES["c1"] = 114;SQUARES["d1"] = 115;
    SQUARES["e1"] =116;SQUARES["f1"] = 117;SQUARES["g1"] = 118;SQUARES["h1"] = 119;

    // ROOKS
    ROOKS['w'][0]["square"] = SQUARES["a1"];
    ROOKS['w'][0]["flag"] = BITS["QSIDE_CASTLE"];
    ROOKS['w'][1]["square"] = SQUARES["h1"];
    ROOKS['w'][1]["flag"] = BITS["KSIDE_CASTLE"];
    ROOKS['b'][0]["square"] = SQUARES["a8"];
    ROOKS['b'][0]["flag"] = BITS["QSIDE_CASTLE"];
    ROOKS['b'][1]["square"] = SQUARES["h8"];
    ROOKS['b'][1]["flag"] = BITS["KSIDE_CASTLE"];
    
    kings['w'] = EMPTY; kings['b'] = EMPTY;
    castling['w'] = 0; castling['b'] = 0;

    this->clear();
}

void Chess::clear(){
    cout<<"aa"<<endl;

}

int main(int argc, char *argv[])
{
    Chess* chess = new Chess();
    return 0;
}
