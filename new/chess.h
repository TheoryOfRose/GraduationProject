#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;


// util function
vector<string> split(const string& str, const string& delim) {
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    
    return tokens;
}

void print_vector_str(const vector<string>& vec){

   for(int i = 0 ; i < vec.size() ; i++){
        cout<<vec[i]<<" ";
   }
   cout<<endl;
}


class Chess{
private:
    // Private Member Data
    map<string, int> kings;
    map<string, int> castling;
    int* SQUARES;
    map<string, string>* board;

    string turn;
    int ep_square;
    int half_moves;
    int move_number;

    map<string,string> header;
    vector< map< string, map<string,string> > > history;

    // Private Member Function
     
public:
    // Static Data
    static string BLACK, WHITE;
    static int EMPTY;
    static string PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING;
    static string SYMBOLS;
    static string DEFAULT_POSITION;
    static string POSSIBLE_POSITION[4];
    static map<string, vector<int>> PAWN_OFFSETS;
    static map<string, vector<int>> PIECE_OFFSETS;
    static int ATTACKS[240];
    static int RAYS[240];
    static map<string,string> FLAGS;
    static map<string,int> BITS;
    static map<string,int> mSQUARES;
    static map<string, vector<map<string,int>>> ROOKS;
    static int RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8;
    // Public Member Data
    int* SQUARES;

    // Constructor
    Chess();
    ~Chess();

    // Public Member function
    void load();
};


