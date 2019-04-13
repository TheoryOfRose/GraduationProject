#include <iostream>
#include <string>
#include <vector>
#include <regex>

//-----------------------------------------------------------------------------
// Chess C++ Functions
//-----------------------------------------------------------------------------
using namespace std;

string COLUMNS; // TODO

// split string function
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

//-----------------------------------------------------------------------------
// Chess Util Types
//-----------------------------------------------------------------------------

struct position            
 {
    string square;         
    string code;
};


//-----------------------------------------------------------------------------
// Chess Util Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Validation
//-----------------------------------------------------------------------------

// check square validation of move (regular expression)
bool validSquare(string square) {
    cout<<"FUNC: validSquare"<<endl;
    
    // move should be in the form of "e2-e4", "f6-d5"
    regex reg("[a-h][1-8]");
    return regex_match(square,reg);
}

// check validation of move
bool validMove(string move) {
    cout<<"FUNC: validMove"<<endl;

    // split with delimeter '-'
    vector<string> temp = split(move,"-");
    if(temp.size() != 2) return false;

    // square validation
    return (validSquare(temp[0]) && validSquare(temp[1]));
}

// check validation of piece code
bool validPieceCode(string code) {
    cout<<"FUNC: validPieceCode"<<endl;

    regex reg("[bw][KQRNBP]");
    return regex_match(code,reg);
}

// check validation of position
bool validPositionObject(vector<position> pos) {
    cout<<"FUNC: validPositionObject"<<endl;
    
    // check validation of pos's square and code
    for (int i = 0; i < pos.size(); ++i) {
        if(!validSquare(pos[i].square) || !validPieceCode(pos[i].code)) return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
// FEN Code
//-----------------------------------------------------------------------------

// convert FEN piece code to bP, wK, etc
/*string fenToPieceCode(string fen) {
    string temp = fen;
    // black piece
    if (fen == tolower(temp)) return 'b' + toupper(temp);
    // white piece
    return 'w'+toupper(temp);
}
*/
/*
int main(int argc, char *argv[])
{
    int i = 0;
    string a = "a1";
    i = validSquare(a);
    cout<<i<<endl;
    return 0;
}
*/
