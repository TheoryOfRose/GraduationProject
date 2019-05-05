#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <regex>
#include <ctype.h>
#include <stdlib.h>
#include <emscripten/bind.h>

using namespace emscripten;

std::vector<std::string> split(const std::string& str, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    
    return tokens;
}

void print_vector_str(const std::vector<std::string>& vec){

   for(int i = 0 ; i < vec.size() ; i++){
        std::cout<<vec[i]<<' ';
   }
   std::cout<<std::endl;
}
std::string SYMBOLS = "pnbrqkPNBRQK";
std::string DEFAULT_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
std::string POSSIBLE_POSITION[4] = {"1-0", "0-1", "1/2-1/2", "*"};

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

// RANK
int RANK_1 = 7;
int RANK_2 = 6;
int RANK_3 = 5;
int RANK_4 = 4;
int RANK_5 = 3;
int RANK_6 = 2;
int RANK_7 = 1;
int RANK_8 = 0;

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
std::map<std::string,char> FLAGS; // Flags

class Chess
{

private:
   
   /****************************************************************************
   *****************************************************************************
        Member Data
   *****************************************************************************
   ****************************************************************************/

    // OFFSETS
    std::map<char, map<int,int> > PAWN_OFFSETS;
    std::map<char, map<int,int> > PIECE_OFFSETS;

    // SHIFTS
    std::map<char,int> SHIFTS;
    // BITS
    std::map<std::string,int> BITS;

    // SQUARE
    std::map<std::string,int> mSQUARES;

    // ROOKS
    std::map<char, map< int, map<std::string,int> > > ROOKS;

    // KINGS
    std::map<char,int> kings;

    // CASTLING
    std::map<char,int> castling;

    // BOARD
    std::map<std::string,char>* board;

    char turn;
    int ep_square;
    int half_moves;
    int move_number;

    std::map<std::string,string> header;
    std::vector< std::map<std::string, map<string,string> > > history;
    
    /****************************************************************************
     *****************************************************************************
         Member Function
     *****************************************************************************
     ****************************************************************************/
    std::string generate_fen(); 
    // set_header
    void update_setup(std::string fen);
    std::map<std::string,string> build_move(int from, int to, int flags, char promotion);
    void add_move(std::vector< std::map<std::string,string> > moves, int from, int to, int flags);
    std::vector< std::map<std::string,string> > generate_moves(map<string,string> options);
    std::string move_to_san(std::map<string,string> move, bool sloppy);
    // stripped_san
    bool attacked(char color, int square);
    bool king_attacked(char color);
    bool insufficient_material();
    bool in_threefold_repetition();
    void push(std::map<std::string,string> move);
    void make_move(std::map<std::string,string> move);
    std::map<std::string,string> undo_move();
    std::string get_disambiguator(std::map<string,string> move, bool sloppy);
    // ascii
    // move_from_san
    int rank(int i);
    int file(int i);
    std::string algebraic(int i);
    char swap_color(char c);
    // make_pretty
    // clone
    // trim
    // preft

public:

    Chess();    // Default Constructor
    Chess(std::string fen);  // Constructor with fen code
    ~Chess();   // Deconstructor

    // API

    // make board;
    int* SQUARES();
    void load(std::string fen);
    void reset();
    void clear();
    void put(char type, char color, std::string square); 
    std::map<std::string,char> get(string square);
    std::map<std::string,char> remove(string square);


    // get info
    std::string getFen();
    std::map<std::string,char>* getBoard();
    char getTurn();   
    // game
    
    //ugly_moves
    //moves
    bool in_check();
    bool in_checkmate();
    bool in_stalemate();
    //in_draw
    //insufficient_material
    //in_threefold_repetition
    //game_over
    bool validate_fen(std::string fen);
    //pgn
    //load_pgn
    //header
    //ascii
    //move
    //ugly_move
    //undo
    //perft
    //square_color
    //history
    void print();

};

///////////// private function
std::string Chess::generate_fen(){

    std::cout<<"Call Chess-generate_fen()"<<std::endl;
    int empty = 0;
    std::string fen = "";

    for(int i = mSQUARES["a8"]; i<= mSQUARES["h1"] ; i++){
        if(this->board[i]["color"] == '0' && this->board[i]["type"] == '0'){
            empty++;
        }
        else{
            if(empty > 0){
                fen += '0'+empty;
                empty = 0;
            }
            char color = this->board[i]["color"];
            char piece = this->board[i]["type"];

            if(color == WHITE){
                fen += toupper(piece);
            }
            else{
                fen += tolower(piece);
            }
        }

        if((i+1) & 0x88){
            if(empty>0){
                fen += '0'+empty;
            }
            if( i != mSQUARES["h1"] ){
                fen += "/";
            }
            empty = 0;
            i += 8;
        }
    }
        
    std::string cflags = "";
    if( castling[WHITE] & BITS["KSIDE_CASTLE"] ) { cflags += 'K'; }
    if( castling[WHITE] & BITS["QSIDE_CASTLE"] ) { cflags += 'Q'; }
    if( castling[BLACK] & BITS["KSIDE_CASTLE"] ) { cflags += 'k'; }
    if( castling[BLACK] & BITS["QSIDE_CASTLE"] ) { cflags += 'q'; }
    if( cflags == "" ) { cflags += '-'; }

    std::string epflags;
    if(ep_square == EMPTY){
        epflags = '-';
    }
    else{
        epflags = this->algebraic(ep_square);
    }

    fen += ' ';
    fen += turn;
    fen += ' ';
    fen += cflags;
    fen += ' ';
    fen += epflags;
    fen += ' ';
    fen += '0'+half_moves;
    fen += ' ';
    fen += '0'+move_number;
    std::cout<<fen<<std::endl;
    return fen;
}

void Chess::update_setup(std::string fen){
   
    std::cout<<"Call Chess->update_setup("<<fen<<")"<<std::endl;
    if( history.size() > 0 ) return;

    if( fen != DEFAULT_POSITION ){
        header["SetUp"] = "1";
        header["FEN"] = fen;
    }
    else{
        header.erase("SetUp");
        header.erase("FEN");
    }

}

std::map<std::string,string> Chess::build_move(int from, int to, int flags, char promotion){

    std::cout<<"Call Chess->build_move("<<from<<","<<to<<","<<flags<<","<<promotion<<")"<<std::endl;
    std::map<std::string,string> move;
    move["color"] = this->turn;
    move["from"] = from+'0';
    move["to"] = to+'0';
    move["flags"] = flags+'0';
    move["piece"] = this->board[from]["type"];
    
    move["promotion"] = '0';    //default promotion
    if(promotion != '0'){
        flags |= this->BITS["PROMOTION"];
        move["flags"] = flags+'0';
        move["promotion"] = promotion;
    }

    if( this->board[to]["type"] != '0' && this->board[to]["color"] != '0'){
        move["captured"] = this->board[to]["type"];
    }
    else if( flags & this->BITS["EP_CAPTURE"] ){
        move["captured"] = PAWN;
    }
    
    return move;

}

void Chess::add_move(std::vector<std::map<std::string,string> > moves, int from, int to, int flags){

    std::cout<<"Call Chess->add_move(moves,"<<from<<","<<to<<","<<flags<<")"<<std::endl;
    
    // if pawn promotion
    if( this->board[from]["type"] == PAWN && (this->rank(to) == RANK_8 || this->rank(to) == RANK_1)){
        moves.push_back(this->build_move(from,to,flags,QUEEN));
        moves.push_back(this->build_move(from,to,flags,ROOK));
        moves.push_back(this->build_move(from,to,flags,BISHOP));
        moves.push_back(this->build_move(from,to,flags,KNIGHT));
    }
    else{
        moves.push_back(this->build_move(from,to,flags,'0'));
    }

}


std::vector< std::map<std::string,string> > Chess::generate_moves(map<string,string> options){

    std::cout<<"Call Chess->generate_move"<<std::endl;
    
    std::vector<std::map<std::string,string> > moves;
    char us = this->turn;
    char them = this->swap_color(us);
    std::map<char,int> second_rank;
    second_rank['b'] = RANK_7;
    second_rank['w'] = RANK_2;

    int first_sq = this->mSQUARES["a8"];
    int last_sq = this->mSQUARES["h1"];
    bool single_square = false;
    bool legal;
   
    // do we want legal moves?
    if(options.size() != 0 && options["legal"] == "true") legal = true;
    else legal = false;

    // are we generating moves for a single square?
    if(options.size() != 0 && options["square"] != "0"){
        if(this->mSQUARES.find(options["square"]) != this->mSQUARES.end() ){
            first_sq = this->mSQUARES[options["square"]];
            last_sq = this->mSQUARES[options["square"]];
            single_square = false;
        }
        else{
            std::vector< std::map<std::string,string> > temp;
            return temp;
        }
    }

    for(int i = first_sq; i<= last_sq ; i++){
        if(i & 0x88){
            i += 7;
            continue;
        }

        std::map<std::string,char> piece = this->board[i];
        if( piece["type"] == '0' || piece["color"] != us ) continue;

        if( piece["type"] == PAWN ){
            // single square, non-capturing
            int square = i + PAWN_OFFSETS[us][0];
            if(this->board[square]["type"] == '0'){
                this->add_move(moves,i,square,BITS["NORMAL"]);
                // double square
                int double_square = i + PAWN_OFFSETS[us][1];
                if( second_rank[us] == this->rank(i) && this->board[double_square]["type"] == '0'){
                    this->add_move(moves,i,double_square,BITS["BIG_PAWN"]);
                }
            }
            // pawm captures
            for(int j=2; j < 4 ; j++){
                int capture_square = i + PAWN_OFFSETS[us][j];
                if( capture_square & 0x88 ) continue;

                if(this->board[square]["type"] != '0' &&
                    this->board[square]["color"] == them){
                    this->add_move(moves,i,capture_square,BITS["CAPTURE"]);
                }
                else if( capture_square == this->ep_square ){
                    this->add_move(moves,i,ep_square,BITS["EP_CAPTURE"]);
                }
            }
        }
        else{
            int len = PIECE_OFFSETS[piece["type"]].size();
            for( int j = 0; j < len ; j++ ){
                int offset = PIECE_OFFSETS[piece["type"]][j];
                int square = i;

                while(true){
                    square += offset;
                    if( square & 0x88 ) break;

                    if(this->board[square]["type"] == '0'){
                        this->add_move(moves,i,square,BITS["NORMAL"]);
                    }
                    else{
                        if(this->board[square]["color"] == us) break;
                        this->add_move(moves,i,square,BITS["CAPTURE"]);
                        break;
                    }

                    if(piece["type"] == 'n' || piece["type"] == 'k') break;
                }
            }
        }
    }

    if((!single_square) || last_sq == kings[us]){
        // king-side castling
        if(castling[us] & BITS["KSIDE_CASTLE"]){
            int castling_from = kings[us];
            int castling_to = castling_from + 2;

            if(this->board[castling_from+1]["type"] == '0' &&
                this->board[castling_to]["type"] == '0' &&
                !this->attacked(them,kings[us]) &&
                !this->attacked(them,castling_from+1) &&
                !this->attacked(them,castling_to)){
                this->add_move(moves,kings[us],castling_to,BITS["KSIDE_CASTLE"]);
            }
        }
        
        // queen-side castling 
        if(castling[us] & BITS["QSIDE_CASTLE"]){
            int castling_from = kings[us];
            int castling_to = castling_from - 2;

            if(this->board[castling_from-1]["type"] == '0' &&
                this->board[castling_from-2]["type"] == '0' &&
                this->board[castling_from-3]["type"] == '0' &&
                !this->attacked(them,kings[us]) &&
                !this->attacked(them,castling_from-1) &&
                !this->attacked(them,castling_to)){
                this->add_move(moves,kings[us],castling_to,BITS["QSIDE_CASTLE"]);
            }
        }
    }

    if(!legal) return moves;

    // filter out illegal moves
    std::vector< std::map<std::string,string> > legal_moves;
    int len = moves.size();
    for( int i = 0 ; i < len ; i++){
        this->make_move(moves[i]);
        if(!this->king_attacked(us)){
            legal_moves.push_back(moves[i]);
        }
        this->undo_move();
    }
    return legal_moves;
}

std::string Chess::move_to_san(std::map<string,string> move, bool sloppy){
    
    std::cout<<"Call Chess->move_to_san"<<std::endl;
    std::string output = "";
    int flags = stoi(move["flags"]);
    int from = stoi(move["from"]);
    int to = stoi(move["to"]);
    if( flags & BITS["KSIDE_CASTLE"] ){ output = "O-O"; }
    else if( flags & BITS["QSIDE_CASTLE"] ){ output = "O-O-O"; }
    else{
        std::string disambiguator = this->get_disambiguator(move,sloppy);

        if(move["piece"].at(0) != PAWN){
            output += toupper(move["piece"].at(0));
            output += disambiguator;
        }

        if( flags & (BITS["CAPTURE"] | BITS["EP_CAPTURE"]) ){
            if( move["piece"].at(0) == PAWN ){
                output += this->algebraic(from).at(0);
            }
            output += 'x';
        }

        output += this->algebraic(to);

        if( flags & BITS["PROMOTION"] ){
            output += '=';
            output += toupper(move["promotion"].at(0));
        }
    }

    this->make_move(move);
    if(this->in_check()){
        if(this->in_checkmate())    output += '#';
        else output += '+';
    }
    this->undo_move();

    return output;
}
    // stripped_san

bool Chess::attacked(char color, int square){
    
    std::cout<<"Call Chess->attacked"<<std::endl;
    for(int i = mSQUARES["a8"]; i <= mSQUARES["h1"]; i++){
        if( i & 0x88 ){
            i += 7;
            continue;
        }

        if(this->board[i]["type"] == '0' || this->board[i]["color"] != color){
            continue;
        }

        std::map<std::string,char> piece = this->board[i];
        int difference = i - square;
        int index = difference + 119;

        if(ATTACKS[index] & ( 1 << SHIFTS[piece["type"]] )){
            if(piece["type"] == PAWN){
                if( difference > 0 ){
                    if( piece["type"] == WHITE ) return true;
                }
                else{
                    if( piece["type"] == BLACK ) return true;
                }
                continue;
            }

            if( piece["type"] == 'n' || piece["type"] == 'k' )  return true;

            int offset = RAYS[index];
            int j = i + offset;

            bool blocked = false;
            while( j != square ){
                if( this->board[j]["type"] != '0' ) {
                    blocked = true;
                    break;
                }
                j += offset;
            }

            if( !blocked ) return true;
        }
    }

    return false;
}

bool Chess::king_attacked(char color){
    std::cout<<"Call Chess->king_attacked"<<std::endl;
    return this->attacked(this->swap_color(color),kings[color]);
}

bool Chess::insufficient_material(){
    
    std::cout<<"Call Chess->insufficient_material"<<std::endl;
    std::map<char,int> pieces;
    std::vector<int> bishops;
    int num_pieces = 0;
    int sq_color = 0;

    for(int i = mSQUARES["a8"]; i <= mSQUARES["h1"]; i++){
        sq_color = (sq_color + 1) % 2;
        if( i & 0x88 ){
            i += 7;
            continue;
        }
        std::map<std::string,char> piece = this->board[i];
        if( piece["type"] != '0' ){
            if(pieces.find(piece["type"]) != pieces.end()){
                pieces[piece["type"]] += 1;
            }
            else{
                pieces[piece["type"]] = 1;
            }
            if(piece["type"] == BISHOP) bishops.push_back(sq_color);
            num_pieces++;
        }
    }

    if(num_pieces == 2){ return true; }
    else if(num_pieces == 3 && (pieces[BISHOP] == 1 || pieces[KNIGHT] == 1)){
        return true;
    }
    else if(num_pieces == pieces[BISHOP] + 2){
        int sum = 0;
        int len = bishops.size();
        for( int i = 0 ; i < len ; i++ ){
            sum += bishops[i];
        }
        if(sum == 0 || sum == len)  return true;
    }

    return false;
}
/*
bool Chess::in_threefold_repetition(){
    
    std::cout<<"Call Chess->insufficient_material"<<std::endl;
    std::vector< std::map<std::string,string> > moves;
    std::map<std::string,int> positions;
    bool repetition = false;

    while(true){
        std::map<std::string,string> move = this->undo_move();
        if(!move.size())    break;
        moves.push_back(move);
    }

    while(true){
        std::string fen = this->generate_fen()
    }
}*/

void Chess::push(std::map<std::string,string> move){

    std::cout<<"Call Chess->push"<<std::endl;
    std::map<std::string, map<string,string> > history;
    history["move"] = move;
    history["kings"]["b"] = kings['b']+'0';
    history["kings"]["w"] = kings['w']+'0';
    history["turn"]["0"] = this->turn;
    history["castling"]["b"] = castling['b']+'0';
    history["castling"]["w"] = castling['w']+'0';
    history["ep_square"]["0"] = this->ep_square+'0';
    history["half_moves"]["0"] = this->half_moves+'0';
    history["move_number"]["0"] = this->move_number+'0';

    this->history.push_back(history);
}

void Chess::make_move(std::map<std::string,string> move){
    
    std::cout<<"Call Chess->make_move"<<std::endl;
    char us = this->turn;
    char them = this->swap_color(us);
    this->push(move);

    int to = stoi(move["to"]);
    int from = stoi(move["from"]);
    int flags = stoi(move["flags"]);
    char piece = move["piece"].at(0);

    this->board[to] = this->board[from];
    this->board[from]["type"] = '0';
    this->board[from]["color"] = '0';

    if(flags & BITS["EP_CAPTURE"]){
        if( us == BLACK ){
            this->board[to - 16]["type"] = '0';
            this->board[to - 16]["color"] = '0';
        }
        else{
            this->board[to + 16]["type"] = '0';
            this->board[to + 16]["color"] = '0';
        }
    }

    if( flags & BITS["PROMOTION"] ){
        this->board[to]["type"] = move["promotion"].at(0);
        this->board[to]["color"] = us;
    }

    // move king
    if(this->board[to]["type"] == KING){
        kings[this->board[to]["color"]] = to;

        if( flags & BITS["KSIDE_CASTLE"] ){
            int castling_to = to - 1;
            int castling_from = to + 1;
            this->board[castling_to] = this->board[castling_from];
            this->board[castling_from]["type"] = '0';
            this->board[castling_from]["color"] = '0';
        }
        else if ( flags & BITS["QSIDE_CASTLE"] ){
            int castling_to = to + 1;
            int castling_from = to - 2;
            this->board[castling_to] = this->board[castling_from];
            this->board[castling_from]["type"] = '0';
            this->board[castling_from]["color"] = '0';
        }
        castling[us] = 0; // turn off castling 
    }

    // turn off castling if move rook
    if(castling[us]){
        for(int i = 0 ; i < ROOKS[us].size() ; i++){
            if(from == ROOKS[us][i]["square"] && castling[us] & ROOKS[us][i]["flag"]){
                castling[us] ^= ROOKS[us][i]["flag"];
                break;
            }
        }
    }

    // turn off castling if capture rook
    if(castling[them]){
        for(int i = 0 ; i < ROOKS[them].size() ; i++){
            if(to == ROOKS[them][i]["square"] &&
                castling[them] & ROOKS[them][i]["flag"]){
                castling[them] ^= ROOKS[them][i]["flag"]; 
                break;
            }
        }
    }

    if(flags & BITS["BIG_PAWN"]){
        if(us == 'b'){
            this->ep_square = to - 16;
        }
        else{
            this->ep_square = to + 16;
        }
    }
    else{
        this->ep_square = EMPTY;
    }

    if(piece == PAWN){
        this->half_moves = 0;
    }
    else if(flags & (BITS["CAPTURE"] | BITS["EP_CAPTURE"])){
        this->half_moves = 0;
    }
    else{
        this->half_moves++;
    }

    if(us == BLACK) move_number++;
    this->turn = this->swap_color(this->turn);

}

std::map<std::string,string> Chess::undo_move(){
    
    std::cout<<"Call Chess->unde_move"<<std::endl;
    if(this->history.empty()){
        std::map<std::string,string>  temp;
        temp["0"]="0";
        return temp;
    }
    std::map<std::string, map<string,string> > old = this->history.back();
    this->history.pop_back();

    std::map<std::string,string> move = old["move"];
    int from = stoi(move["from"]);
    int to = stoi(move["to"]);
    int flags = stoi(move["flags"]);
    //std::map<std::string,string> kings = old["kings"];
    char turn = old["turn"]["0"].at(0);
    //std::map<std::string,string> castling = old["castling"];
    int ep_square = stoi(old["ep_square"]["0"]);
    int half_moves = stoi(old["half_moves"]["0"]);
    int move_number = stoi(old["move_number"]["0"]);

    char us = turn;
    char them = this->swap_color(turn);

    this->board[from] = this->board[to];
    this->board[from]["type"] = move["piece"].at(0);
    this->board[to]["type"]='0';
    this->board[to]["color"]='0';

    if(flags & BITS["CAPTURE"]){
        this->board[to]["type"] = move["captured"].at(0);
        this->board[to]["color"] = them;
    }
    else if( flags & BITS["EP_CAPTURE"] ){
        int index;
        if(us == BLACK){
            index = to - 16;
        }
        else{
            index = to + 16;
        }
        this->board[index]["type"] = PAWN;
        this->board[index]["color"] = them;
    }

    if( flags & (BITS["KSIDE_CASTLE"] | BITS["QSIDE_CASTLE"]) ){
        int castling_to;
        int castling_from;
        if( flags & BITS["KSIDE_CASTLE"] ){
            castling_to = to + 1;
            castling_from = to - 1;
        }
        else if ( flags & BITS["QSIDE_CASTLE"] ){
            castling_to = to - 2;
            castling_from = to + 1;
        }
        this->board[castling_to] = this->board[castling_from];
        this->board[castling_from]["type"] = '0';
        this->board[castling_from]["color"] = '0';
    }

    return move;
}
    // ascii
    // move_from_san


///////public function

void Chess::load(std::string fen){

    std::cout<<"Call Chess->load("<<fen<<")"<<std::endl; 
    if(!validate_fen(fen)){ return; }
    
    std::vector<std::string> tokens = split(fen," ");
    std::string position = tokens[0];
    this->turn = tokens[1].at(0);

    this->clear();

    int sq = 0;
    for(int i = 0 ; i < position.length() ; i++){
        char piece = position.at(i);
        std::cout<<piece<<std::endl;
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
        std::cout<<sq<<std::endl;
    }
    
    if( tokens[2].find("K") != std::string::npos ){
        this->castling['w'] |= this->BITS["KSIDE_CASTLE"];    
    }
    if( tokens[2].find("Q") != std::string::npos ){
        this->castling['w'] |= this->BITS["QSIDE_CASTLE"];    
    }
    if( tokens[2].find("k") != std::string::npos ){
        this->castling['b'] |= this->BITS["KSIDE_CASTLE"];    
    }
    if( tokens[2].find("q") != std::string::npos ){
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
}

void Chess::reset(){
    this->load(DEFAULT_POSITION);
}

void Chess::clear(){
    std::cout<<"Call Chess->clear()"<<std::endl;
    delete[] this->board; 
    this->board = new std::map<std::string,char>[128];
    for(int i = 0 ; i < 128 ; i++){
        board[i]["color"] = '0';
        board[i]["type"] = '0';
    }

    this->kings['w'] = EMPTY; this->kings['b'] = EMPTY;
    this->turn = WHITE;
    this->castling['w'] = 0; this->castling['b'] = 0;
    this->ep_square = EMPTY;
    this->half_moves = 0;
    this->move_number = 1;
    //this->history
    //this->header
    this->update_setup(this->generate_fen());
}

void Chess::put(char type, char color, std::string square){

    std::cout<<"Call Chess->put("<<type<<","<<color<<","<<square<<")"<<std::endl;

    int sq = mSQUARES[square];
    board[sq]["type"] = type;
    board[sq]["color"] = color;
    if(type == KING){ kings[color] = sq; }
    this->update_setup(generate_fen());

}

std::map<std::string,char> Chess::get(string square){

    std::cout<<"Call Chess->get("<<square<<")"<<std::endl;

    std::map<std::string,char> piece = this->board[this->mSQUARES[square]];
    return piece;

}

std::map<std::string,char> Chess::remove(string square){

    std::cout<<"Call Chess->remove("<<square<<")"<<std::endl;

    std::map<std::string,char> piece = this->get(square);
    if(piece["type"] == KING)   this->kings[piece["color"]] = EMPTY;
    this->update_setup(this->generate_fen());
    return piece;

}

    /****************************************************************************
     *****************************************************************************
         Get Info
     *****************************************************************************
     ****************************************************************************/

std::string Chess::getFen(){
    return this->generate_fen();
}

std::map<std::string,char>* Chess::getBoard(){
    return this->board;
}
char Chess::getTurn(){
    return this->turn;
}
bool Chess::in_check(){
    std::cout<<"Call Chess->in_check"<<std::endl;
    return this->king_attacked(this->turn);
}

bool Chess::in_checkmate(){
    std::cout<<"Call Chess->in_checkmate"<<std::endl;
    std::map<std::string,string> options;
    return this->in_check() && this->generate_moves(options).size() == 0;
}

bool Chess::in_stalemate(){
    std::cout<<"Call Chess->in_stalemate"<<std::endl;
    std::map<std::string,string> options;
    return !this->in_check() && this->generate_moves(options).size() == 0;
}
 
    /****************************************************************************
     *****************************************************************************
         Util Function
     *****************************************************************************
     ****************************************************************************/
bool Chess::validate_fen(std::string fen){

    std::cout<<"call Chess->validate_fen("<<fen<<")"<<std::endl;
    std::vector<std::string> tokens = split(fen," ");
    std::cout<<"print fen"<<std::endl;
    print_std::vector_str(tokens);
    if(tokens.size() != 6){ return false; }

    return true;

}

int Chess::rank(int i){
    return i >> 4;
}

int Chess::file(int i){
    return i & 15;
}

std::string Chess::algebraic(int i){
    int f = this->file(i);
    int r = this->rank(i);
    std::string file = "abcdefgh";
    std::string rank = "87654321";
    std::string sq = "";
    sq += file.at(f);
    sq += rank.at(r);
    return sq;
}

char Chess::swap_color(char c){ 
    if( c == WHITE ) return BLACK;
    else return WHITE;
}

std::string Chess::get_disambiguator(std::map<string,string> move, bool sloppy){
    std::map<std::string,string> temp;
    if(sloppy) temp["legal"] = "false";
    else temp["legal"] = "true";

    std::vector< std::map<std::string,string> > moves = this->generate_moves(temp);

    int from = stoi(move["from"]);
    int to = stoi(move["to"]);
    char piece = move["piece"].at(0);

    int ambiguities = 0;
    int same_rank = 0;
    int same_file = 0;

    int len = moves.size();
    for(int i = 0 ; i < len ; i++){
        int ambig_from = stoi(moves[i]["from"]);
        int ambig_to = stoi(moves[i]["to"]);
        char ambig_piece = moves[i]["piece"].at(0);

        if(piece == ambig_piece && from != ambig_from && to == ambig_to){
            ambiguities++;
            if(this->rank(from) == this->rank(ambig_from))  same_rank++;
            if(this->file(from) == this->file(ambig_from))  same_file++;
        }
    }

    if(ambiguities > 0){
        if(same_rank > 0 && same_file > 0){  return this->algebraic(from); }
        else if(same_file > 0){
            std::string s;
            s += (this->algebraic(from)).at(1);
            return s;
        }
        else{
            std::string s;
            s += (this->algebraic(from)).at(0);
            return s;
        } 
    }

    return "";
}


/// constructor

Chess::Chess(){

    std::cout<<"Chess Default Constructor"<<std::endl;

    this->board = new std::map<std::string,char>[128];
    for(int i = 0 ; i < 128 ; i++){
        board[i]["color"] = '0';
        board[i]["type"] = '0';
    }
    
    turn = WHITE;
    ep_square = EMPTY;
    half_moves = 0;
    move_number = 1;

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

    this->load(DEFAULT_POSITION);
}

Chess::Chess(std::string fen){
    std::cout<<"Chess Default Constructor"<<std::endl;

    this->board = new std::map<std::string,char>[128];
    for(int i = 0 ; i < 128 ; i++){
        board[i]["color"] = '0';
        board[i]["type"] = '0';
    }

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

    this->load(fen);

}

Chess::~Chess(){
    delete[] board;
}

void Chess::print(){

    //board
    std::cout<<"===== board ====="<<std::endl;
    for(int i = 0 ; i < 128 ; i++){
        for(std::map<std::string,char>::iterator j = board[i].begin(); j != board[i].end(); j++){
        std::cout<<"i: "<<i<<"  ["<<j->first<<","<<j->second<<"]"<<std::endl;
        }
    }

    //ROOKS
    std::cout<<"===== ROOKS ====="<<std::endl;
    for(std::map<char,map<int,map<std::string,int> > >::iterator i = ROOKS.begin(); i != ROOKS.end(); i++){
        for(std::map<int,map<std::string,int> >::iterator j = i->second.begin(); j != i->second.end(); j++){
            for(std::map<std::string,int>::iterator k = j->second.begin() ; k != j->second.end() ; k++){
            std::cout<<i->first<<"   "<<j->first<<" "<<k->first<<"  "<<k->second<<std::endl;
            }
        }
    }

    //kings
    std::cout<<"===== kings ====="<<std::endl;
    for(std::map<char,int>::iterator i = kings.begin(); i != kings.end(); i++){
        std::cout<<i->first<<"   "<<i->second<<std::endl;
    }

    //castling
    std::cout<<"===== castling ====="<<std::endl;
    for(std::map<char,int>::iterator i = castling.begin(); i != castling.end(); i++){
        std::cout<<i->first<<"   "<<i->second<<std::endl;
    }

    //turn
    std::cout<<"turn :   "<<turn<<std::endl;
    //ep_square
    std::cout<<"ep_square    :   "<<ep_square<<std::endl;
    //half_moves
    std::cout<<"half_moves   :   "<<half_moves<<std::endl;
    //move_number
    std::cout<<"move_number  :   "<<move_number<<std::endl;
}

/*
int main(int argc, char *argv[])
{
    Chess* chess = new Chess();
    chess->print();
    return 0;
}*/
