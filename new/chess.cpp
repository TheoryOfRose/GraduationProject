#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <regex>
#include <ctype.h>
#include <stdlib.h>
#include "chess.h"
//#include <emscripten/bind.h>

using namespace std;

// Initialize class static member data

string Chess::SYMBOLS = "pnbrqkPNBRQK";
string Chess::DEFAULT_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
string Chess::POSSIBLE_POSITION[4] = {"1-0", "0-1", "1/2-1/2", "*"};

map<string,vector<int>> Chess::PAWN_OFFSETS = {
    {"b", {16, 32, 17, 15} },
    {"w", {-16, -32, -17, -15} }
};

map<string,vector<int>> Chess::PIECE_OFFSETS = {
    {"n", {-18, -33, -31, -14, 18, 33, 31, 14} },
    {"b", {-17, -15, 17, 15} },
    {"r", {-16, 1, 16, -1} },
    {"q", {-17, -16, -15, 1, 17, 16, 15, -1} },
    {"k", {-17, -16, -15, 1, 17, 16, 15, -1} }
};

map<string,string> Chess::FLAGS = {
    {"NORMAL", "n"},
    {"CAPTURE","c"},
    {"BIG_PAWN","b"},
    {"EP_CAPTURE", "e"},
    {"PROMOTION","p"},
    {"KSIDE_CASTLE","k"},
    {"QSIDE_CASTLE","q"}
};

map<string,int> Chess::BITS = {    
    {"NORMAL", 1},
    {"CAPTURE", 2},
    {"BIG_PAWN", 4},
    {"EP_CAPTURE", 8},
    {"PROMOTION", 16},
    {"KSIDE_CASTLE", 32},
    {"QSIDE_CASTLE", 64}
};

map<string,int> Chess::mSQUARES = {
    {"a8", 0}, {"b8", 1}, {"c8", 2}, {"d8", 3},
    {"e8", 4}, {"f8", 5}, {"g8", 6}, {"h8", 7},
    {"a7", 16}, {"b7", 17}, {"c7", 18}, {"d7", 19},
    {"e7", 20}, {"f7", 21}, {"g7", 22}, {"h7", 23},
    {"a6", 32}, {"b6", 33}, {"c6", 34}, {"d6", 35},
    {"e6", 36}, {"f6", 37}, {"g6", 38}, {"h6", 39},
    {"a5", 48}, {"b5", 49}, {"c5", 50}, {"d5", 51},
    {"e5", 52}, {"f5", 53}, {"g5", 54}, {"h5", 55},
    {"a4", 64}, {"b4", 65}, {"c4", 66}, {"d4", 67},
    {"e4", 68}, {"f4", 69}, {"g4", 70}, {"h4", 71},
    {"a3", 80}, {"b3", 81}, {"c3", 82}, {"d3", 83},
    {"e3", 84}, {"f3", 85}, {"g3", 86}, {"h3", 87},
    {"a2", 96}, {"b2", 97}, {"c2", 98}, {"d2", 99},
    {"e2", 100}, {"f2", 101}, {"g2", 102}, {"h2", 103},
    {"a1", 112}, {"b1", 113}, {"c1", 114}, {"d1", 115},
    {"e1", 116}, {"f1", 117}, {"g1", 118}, {"h1", 119}
};

map<string, vector<map<string,int>>> Chess::ROOKS = {
    {"w", { { 
            {"square",Chess::mSQUARES["a1"]}, {"flag",Chess::BITS["QSIDE_CASTLE"]} 
        },
        {
            {"square",Chess::mSQUARES["h1"]}, {"flag",Chess::BITS["KSIDE_CASTLE"]} 
        } } },
    {"b", { { 
            {"square",Chess::mSQUARES["a8"]}, {"flag",Chess::BITS["QSIDE_CASTLE"]} 
        },
        {
            {"square",Chess::mSQUARES["h8"]}, {"flag",Chess::BITS["KSIDE_CASTLE"]} 
        } } }
};

int Chess::ATTACKS[240] = {

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

int Chess::RAYS[240] = {

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

int Chess::RANK_1 = 7;
int Chess::RANK_2 = 6;
int Chess::RANK_3 = 5;
int Chess::RANK_4 = 4;
int Chess::RANK_5 = 3;
int Chess::RANK_6 = 2;
int Chess::RANK_7 = 1;
int Chess::RANK_8 = 0;

string Chess::BLACK = "b";
string Chess::WHITE = "w";
int  Chess::EMPTY = -1;
string Chess::PAWN = "p";
string Chess::KNIGHT = "n";
string Chess::BISHOP = "b";
string Chess::ROOK = "r";
string Chess::QUEEN = "q";
string Chess::KING = "k";



/////////////////////////////////////////////////////
/////////        private function       ////////////
////////////////////////////////////////////////////




/////////////////////////////////////////////////////
/////////        public function       ////////////
////////////////////////////////////////////////////

// constructor
Chess::Chess(){
    cout<<"Chess Default Constructor"<<endl;

    this->SQUARES = new int[64];
    int j = 0;
    for(int i = Chess::mSQUARES["a8"]; i < Chess::mSQUARES["h1"]; i++){
        if(i & 0x88){
            i += 7;
            continue;
        }
        this->SQUARES[j++] = this->algebraic(i);
    }

    this->board = new map<string,string>[128];
    for(int i = 0 ; i < 128 ; i++){
        board[i]["color"] = "";
        board[i]["type"] = "";
    }
    
    this->turn = Chess::WHITE;
    this->ep_square = Chess::EMPTY;
    this->half_moves = 0;
    this->move_number = 1;

    this->kings["w"] = EMPTY; 
    this->kings["b"] = EMPTY;
    this->castling["w"] = 0; 
    this->castling["b"] = 0;
 
    this->load(DEFAULT_POSITION);

}

Chess::Chess(string fen){

}

Chess::~Chess(){

}


void Chess::load(string fen){
/*
    cout<<"Call Chess->load("<<fen<<")"<<endl; 
    if(!validate_fen(fen)){ return; }
    
    vector<string> tokens = split(fen," ");
    string position = tokens[0];
    this->turn = tokens[1].at(0);

    this->clear();

    int sq = 0;
    for(int i = 0 ; i < position.length() ; i++){
        char piece = position.at(i);
        if(piece == '/'){
            sq += 8;
        } else if(isdigit(piece)){
            sq += piece-'0';
        }
        else{
            char color;
            if(piece < 'a'){ color = WHITE; }
            else{ color = BLACK; }
            this->put(tolower(piece),color,this->algebraic(sq));
            sq++;
        }
    }
    
    if( tokens[2].find("K") != string::npos ){
        this->castling['w'] |= this->BITS["KSIDE_CASTLE"];    
    }
    if( tokens[2].find("Q") != string::npos ){
        this->castling['w'] |= this->BITS["QSIDE_CASTLE"];    
    }
    if( tokens[2].find("k") != string::npos ){
        this->castling['b'] |= this->BITS["KSIDE_CASTLE"];    
    }
    if( tokens[2].find("q") != string::npos ){
        this->castling['b'] |= this->BITS["QSIDE_CASTLE"];    
    }

    if(tokens[3] == "-"){
        this->ep_square = EMPTY;
    }
    else{
        this->ep_square = mSQUARES[tokens[3]];
    }

    this->half_moves = stoi(tokens[4]);
    this->move_number = stoi(tokens[5]);

    this->update_setup(this->generate_fen());

    return;
*/
}



// Binding API
/*EMSCRIPTEN_BINDINGS(chess){
    emscripten::class_<Chess>("Chess")
        .constructor<>()
        //.function("incrementX", &Chess::incrementX)
        //.function("printX", &Chess::printX)
        //.function("strToIntY", &Chess::strToIntY)
    ;
}*/
