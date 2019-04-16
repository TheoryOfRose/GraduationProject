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
    map<string,int> mSQUARES;

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
    // set_header
    //void update_setup(string fen);
    //map<string,char> get(string square);
    //void put(map<string,char> piece, string square);
    //map<string,char> remove(string square);
    //map<string,string> build_move(map<string,char>* board,string from, string to, int flags, int promotion);
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
    // make_pretty
    // clone
    // trim
    // preft

public:

    Chess();    // Default Constructor
    Chess(string fen);  // Constructor with fen code
    ~Chess();   // Deconstructor

    // Public Constants
    char BLACK = 'b';
    char WHITE = 'w';
    int  EMPTY = -1;
    char PAWN = 'p';
    char KNIGHT = 'n';
    char BISHOP = 'b';
    char ROOK = 'r';
    char QUEEN = 'q';
    char KING = 'k';
    map<string,char> FLAGS; // Flags

    // API
    int* SQUARES();
    void load(string fen);
    void reset();
    //moves
    //ugly_moves
    //in_check
    //in_checkmate
    //in_stalemate
    //in_draw
    //insufficient_material
    //in_threefold_repetition
    //game_over
    //validate_fen
    //fen
    //board
    //pgn
    //load_pgn
    //header
    //ascii
    //turn
    //move
    //ugly_move
    //undo
    void clear();
    //put
    //get
    //remove
    //perft
    //square_color
    //history

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
    mSQUARES["a8"] =0; mSQUARES["b8"] = 1; mSQUARES["c8"] = 2; mSQUARES["d8"] = 3;
    mSQUARES["e8"] =4; mSQUARES["f8"] = 5; mSQUARES["g8"] = 6; mSQUARES["h8"] = 7;
    mSQUARES["a7"] =16; mSQUARES["b7"] = 17; mSQUARES["c7"] = 18; mSQUARES["d7"] = 19;
    mSQUARES["e7"] =20; mSQUARES["f7"] = 21; mSQUARES["g7"] = 22; mSQUARES["h7"] = 23;
    mSQUARES["a6"] =32; mSQUARES["b6"] = 33; mSQUARES["c6"] = 34; mSQUARES["d6"] = 35;
    mSQUARES["e6"] =36; mSQUARES["f6"] = 37; mSQUARES["g6"] = 38; mSQUARES["h6"] = 39;
    mSQUARES["a5"] =48; mSQUARES["b5"] = 49; mSQUARES["c5"] = 50; mSQUARES["d5"] = 51;
    mSQUARES["e5"] =52; mSQUARES["f5"] = 53; mSQUARES["g5"] = 54; mSQUARES["h5"] = 55;
    mSQUARES["a4"] =64; mSQUARES["b4"] = 65; mSQUARES["c4"] = 66; mSQUARES["d4"] = 67;
    mSQUARES["e4"] =68; mSQUARES["f4"] = 69; mSQUARES["g4"] = 70; mSQUARES["h4"] = 71;
    mSQUARES["a3"] =80; mSQUARES["b3"] = 81; mSQUARES["c3"] = 82; mSQUARES["d3"] = 83;
    mSQUARES["e3"] =84; mSQUARES["f3"] = 85; mSQUARES["g3"] = 86; mSQUARES["h3"] = 87;
    mSQUARES["a2"] =96; mSQUARES["b2"] = 97; mSQUARES["c2"] = 98; mSQUARES["d2"] = 99;
    mSQUARES["e2"] =100;mSQUARES["f2"] = 101;mSQUARES["g2"] = 102;mSQUARES["h2"] = 103;
    mSQUARES["a1"] =112;mSQUARES["b1"] = 113;mSQUARES["c1"] = 114;mSQUARES["d1"] = 115;
    mSQUARES["e1"] =116;mSQUARES["f1"] = 117;mSQUARES["g1"] = 118;mSQUARES["h1"] = 119;

    // ROOKS
    ROOKS['w'][0]["square"] = mSQUARES["a1"];
    ROOKS['w'][0]["flag"] = BITS["QSIDE_CASTLE"];
    ROOKS['w'][1]["square"] = mSQUARES["h1"];
    ROOKS['w'][1]["flag"] = BITS["KSIDE_CASTLE"];
    ROOKS['b'][0]["square"] = mSQUARES["a8"];
    ROOKS['b'][0]["flag"] = BITS["QSIDE_CASTLE"];
    ROOKS['b'][1]["square"] = mSQUARES["h8"];
    ROOKS['b'][1]["flag"] = BITS["KSIDE_CASTLE"];
    
    kings['w'] = EMPTY; kings['b'] = EMPTY;
    castling['w'] = 0; castling['b'] = 0;

    //this->clear();
}

Chess::Chess(string fen){
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
    mSQUARES["a8"] =0; mSQUARES["b8"] = 1; mSQUARES["c8"] = 2; mSQUARES["d8"] = 3;
    mSQUARES["e8"] =4; mSQUARES["f8"] = 5; mSQUARES["g8"] = 6; mSQUARES["h8"] = 7;
    mSQUARES["a7"] =16; mSQUARES["b7"] = 17; mSQUARES["c7"] = 18; mSQUARES["d7"] = 19;
    mSQUARES["e7"] =20; mSQUARES["f7"] = 21; mSQUARES["g7"] = 22; mSQUARES["h7"] = 23;
    mSQUARES["a6"] =32; mSQUARES["b6"] = 33; mSQUARES["c6"] = 34; mSQUARES["d6"] = 35;
    mSQUARES["e6"] =36; mSQUARES["f6"] = 37; mSQUARES["g6"] = 38; mSQUARES["h6"] = 39;
    mSQUARES["a5"] =48; mSQUARES["b5"] = 49; mSQUARES["c5"] = 50; mSQUARES["d5"] = 51;
    mSQUARES["e5"] =52; mSQUARES["f5"] = 53; mSQUARES["g5"] = 54; mSQUARES["h5"] = 55;
    mSQUARES["a4"] =64; mSQUARES["b4"] = 65; mSQUARES["c4"] = 66; mSQUARES["d4"] = 67;
    mSQUARES["e4"] =68; mSQUARES["f4"] = 69; mSQUARES["g4"] = 70; mSQUARES["h4"] = 71;
    mSQUARES["a3"] =80; mSQUARES["b3"] = 81; mSQUARES["c3"] = 82; mSQUARES["d3"] = 83;
    mSQUARES["e3"] =84; mSQUARES["f3"] = 85; mSQUARES["g3"] = 86; mSQUARES["h3"] = 87;
    mSQUARES["a2"] =96; mSQUARES["b2"] = 97; mSQUARES["c2"] = 98; mSQUARES["d2"] = 99;
    mSQUARES["e2"] =100;mSQUARES["f2"] = 101;mSQUARES["g2"] = 102;mSQUARES["h2"] = 103;
    mSQUARES["a1"] =112;mSQUARES["b1"] = 113;mSQUARES["c1"] = 114;mSQUARES["d1"] = 115;
    mSQUARES["e1"] =116;mSQUARES["f1"] = 117;mSQUARES["g1"] = 118;mSQUARES["h1"] = 119;

    // ROOKS
    ROOKS['w'][0]["square"] = mSQUARES["a1"];
    ROOKS['w'][0]["flag"] = BITS["QSIDE_CASTLE"];
    ROOKS['w'][1]["square"] = mSQUARES["h1"];
    ROOKS['w'][1]["flag"] = BITS["KSIDE_CASTLE"];
    ROOKS['b'][0]["square"] = mSQUARES["a8"];
    ROOKS['b'][0]["flag"] = BITS["QSIDE_CASTLE"];
    ROOKS['b'][1]["square"] = mSQUARES["h8"];
    ROOKS['b'][1]["flag"] = BITS["KSIDE_CASTLE"];
    
    kings['w'] = EMPTY; kings['b'] = EMPTY;
    castling['w'] = 0; castling['b'] = 0;

    //this->clear();


}

void Chess::clear(){
    cout<<"aa"<<endl;

}

int main(int argc, char *argv[])
{
    Chess* chess = new Chess();
    return 0;
}
