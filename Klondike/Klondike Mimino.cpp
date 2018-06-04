#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
using namespace std; 

char suit[]={'C','D','H','S'};
char rank[]={'A','2','3','4','5','6','7','8','9','T','J','Q','K'};

int c2n (string C) {
    int ret=0;
    for (int i=0; i<13; i++)
        if (C[0]==rank[i]) { ret+=i; break; }
    for (int i=0; i<4; i++)
        if (C[1]==suit[i]) { ret+=i*13; break; }
    return ret;
}

string n2c (int N) {
    string ret="";
    ret+=rank[N%13];
    ret+=suit[N/13];
    return ret;
}

int suitcheck (int s,int d) {
    if ((s/13)%3==(d/13)%3) return 0;
    if ((s/13)%3!=0 && (d/13)%3!=0) return 0;
    return 1;
}

#define DEBUG(x) cerr << '>' << #x << ':' << x << endl;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define FOR2(i, a, b) for (int i = (a); i > (b); --i)
const int INF = 1 << 29;

inline int rand_int(int a) { return rand() % a; }
inline int rand_int(int a, int b) { return rand() % (b - a + 1) + a; }
inline double rand_double() { return (double)rand() / (double)RAND_MAX; }
inline double rand_double(double a, double b) { return rand_double() * (b-a) + a; }
////////////////////////////////////////////////////////////////////////////////////////////

const int CLUBS = 0, DIAMONDS = 1, HEARTS = 2, SPADES = 3;
const int RED = 0, BLACK = 1;
const int STACKS = 7;

struct Card
{
    //konstruktor(y)
    Card() { }
    Card(int rrank, int ssuit): rank(rrank), suit(ssuit) { }
    Card(const string & str) { from_string(str); }

    //vnutorne premenne
    int rank, suit; //cislo / znak

    //hlavne funkcie
    bool operator==(const Card & c) const { return rank == c.rank && suit == c.suit; }
    bool operator!=(const Card & c) const { return rank != c.rank || suit != c.suit; }
    int colour() const { if (suit == CLUBS || suit == SPADES) return BLACK; return RED; }

    //pomocne funkcie
    void from_string(const string & str)
    {
        //nastavi cislo
        if (str[0] == 'A') rank = 0;
        else if (str[0] >= '2' && str[0] <= '9') rank = str[0] - '1';
        else if (str[0] == 'T') rank = 9;
        else if (str[0] == 'J') rank = 10;
        else if (str[0] == 'Q') rank = 11;
        else if (str[0] == 'K') rank = 12;

        //nastavi znak
        if (str[1] == 'C') suit = CLUBS;
        else if (str[1] == 'D') suit = DIAMONDS;
        else if (str[1] == 'H') suit = HEARTS;
        else if (str[1] == 'S') suit = SPADES;
    }

    string to_string() const
    {
        string res;

        //nastavi cislo
        if (rank == 0) res += "A";
        else if (rank >= 1 && rank <= 8) res += (char)(rank + '1');
        else if (rank == 9) res += "T";
        else if (rank == 10) res += "J";
        else if (rank == 11) res += "Q";
        else if (rank == 12) res += "K";

        //nastavi znak
        if (suit == CLUBS) res += "C";
        else if (suit == DIAMONDS) res += "D";
        else if (suit == HEARTS) res += "H";
        else if (suit == SPADES) res += "S";

        return res;
    }
};

struct Move
{
    Move(): primary(0), secondary(0) { }
    Move(int src, int dest, Card c, bool llast = true): source(src), destination(dest), card(c), last(llast), primary(0), secondary(0) { }

    int source, destination; //0-3 = C1-C4   4-10 = S1-S7   11 = DECK
    Card card; //zdrojova karta, ktorou sa hybe (ak ich je viac, tak najspodnejsia)
    bool last; //ci sa jedna o najspodnejsiu kartu (co sa tyka kopiek)
    int deck_pos, advances; //pozicia karty v balicku / pocet nutnych posunov

    int primary, secondary; //primarne a sekundarne skore tahu
    bool operator<(const Move & m) const { return primary < m.primary || (primary == m.primary && secondary < m.secondary); }

    string to_string() const
    {
        string res;
        
        FOR(i, 0, 2)
        {
            if (i) res += " ";
            int temp = (i == 0 ? source : destination);
            if (temp < 4) { res += "C"; res += (char)(temp + '1'); }
            else if (temp < 11) { res += "S"; res += (char)(temp - 4 + '1'); }
            else res += "DECK";
        }

        return res;
    }
};

struct GameState
{
    vector<Card> deck; //balicek kariet
    int position; //pozicia v balicku kariet
    vector<pair<Card, int> > from_deck; //ktore karty viem zahrat z balicka v tomto tahu (karta/pozicia v balicku)

    int downcards[STACKS]; //pocet neotocenych kariet v kopkach
    vector<Card> stacks[STACKS]; //otocene karty v kopkach
    int completed[4]; //hotove kopky kariet

    //hlavne funkcie
    void init_from_deck(); //inicializuje pole aktivnych kariet z balicku
    vector<Move> get_moves(); //vrati mnozinu moznych tahov
    void make_move(Move move); //vykona zvoleny tah

    bool win() const { return completed[0] + completed[1] + completed[2] + completed[3] == 52; }
    bool playable(Card c) const; //ci je dana karta hratelna
    double count_prob() const; //vyrata pravdepodobnost dalsieho pokracovania v hre
};

void GameState::init_from_deck()
{
    from_deck.clear();

    if (!deck.empty())
    {
        vector<bool> visited(deck.size(), false);
        int pos = position;
        while (!visited[pos])
        {
            visited[pos] = true;
            from_deck.push_back(make_pair(deck[pos], pos));
            if (pos == deck.size() - 1) pos = 2;
            else pos += 3;
            if (pos >= deck.size()) pos = deck.size()-1;
        }
    }
}

vector<Move> GameState::get_moves()
{
    vector<Move> moves;

    //tahy z balicka do hotovych
    FOR(i, 0, from_deck.size())
        if (from_deck[i].first.rank == completed[from_deck[i].first.suit])
        {
            moves.push_back(Move(11, from_deck[i].first.suit, from_deck[i].first));
            moves.back().deck_pos = from_deck[i].second;
            moves.back().advances = i;
        }

    //tahy z balicka do kopiek
    FOR(i, 0, from_deck.size())
        FOR(j, 0, STACKS)
            //budto hybeme kralom na prazdnu poziciu alebo umiestnujeme dalsiu kartu na vrch
            if ((stacks[j].empty() && !downcards[j] && from_deck[i].first.rank == 12) || 
                (!stacks[j].empty() && from_deck[i].first.rank + 1 == stacks[j].back().rank && from_deck[i].first.colour() != stacks[j].back().colour()))
            {
                moves.push_back(Move(11, j+4, from_deck[i].first));
                moves.back().deck_pos = from_deck[i].second;
                moves.back().advances = i;
            }

    //tahy z kopiek do hotovych
    FOR(i, 0, STACKS)
        if (!stacks[i].empty() && stacks[i].back().rank == completed[stacks[i].back().suit])
            moves.push_back(Move(i+4, stacks[i].back().suit, stacks[i].back(), stacks[i].size()==1));

    //tahy z kopiek do kopiek
    FOR(i, 0, STACKS)
        if (!stacks[i].empty())
            FOR(j, 0, STACKS)
                if (i != j)
                {
                    if (stacks[j].empty() && !downcards[j]) //tahame na prazdnu kopku
                    {
                        if (stacks[i][0].rank == 12 && downcards[i]) //tahame kralom a uvolnujeme pri to karty
                            moves.push_back(Move(i+4, j+4, stacks[i][0], true));
                    }
                    else if (!stacks[j].empty()) //umiestnujeme dalej na kopku
                    {
                        //zistime, co mozme tahat
                        FOR(k, 0, stacks[i].size())
                            if (stacks[i][k].rank + 1 == stacks[j].back().rank && stacks[i][k].colour() != stacks[j].back().colour())
                                moves.push_back(Move(i+4, j+4, stacks[i][k], k == 0));
                    }
                }

    //tahy z hotovych do kopiek
    FOR(i, 0, 4)
        if (completed[i])
        {
            FOR(j, 0, STACKS)
                if (!stacks[j].empty() && completed[i] == stacks[j].back().rank && (i == 0 || i == 3 ? BLACK : RED) != stacks[j].back().colour())
                    moves.push_back(Move(i, j+4, Card(completed[i]-1, i)));
        }

    return moves;
}
void GameState::make_move(Move move)
{
    //tahame z balicka
    if (move.source == 11)
    {
        position = move.deck_pos;

        if (move.destination < 4) //na hotove
            ++completed[move.destination];
        else //na kopky
            stacks[move.destination-4].push_back(deck[position]);
        deck.erase(deck.begin() + position);
        if (--position < 0 && !deck.empty())
        {
            position = 2;
            if (position >= deck.size()) position = deck.size()-1;
        }
    }
    else if (move.source < 4) //z hotovych
    {
        stacks[move.destination-4].push_back(move.card);
        --completed[move.source];
    }
    else //z kopiek
    {
        //na hotovu
        if (move.destination < 4)
        {
            ++completed[move.destination];
            stacks[move.source-4].pop_back();
        }
        else //na inu kopku
        {
            int from = -1;
            FOR(i, 0, stacks[move.source-4].size())
                if (stacks[move.source-4][i] == move.card)
                {
                    from = i; 
                    break; 
                }
            FOR(i, from, stacks[move.source-4].size())
                stacks[move.destination-4].push_back(stacks[move.source-4][i]);
            stacks[move.source-4].erase(stacks[move.source-4].begin() + from, stacks[move.source-4].end());
        }
    }
}
bool GameState::playable(Card c) const
{
    //ci sa da zahrat na inu kartu
    if (c.rank == completed[c.suit]) return true; //do hotovych
    FOR(i, 0, STACKS) //na inu kopku
        if ((stacks[i].empty() && !downcards[i] && c.rank == 12) || (!stacks[i].empty() && c.rank + 1 == stacks[i].back().rank && c.colour() != stacks[i].back().colour()))
            return true;
    //ci sa ina karta da zahrat na tuto
    FOR(i, 0, from_deck.size()) //z balicka
        if (from_deck[i].first.rank + 1 == c.rank && from_deck[i].first.colour() != c.colour())
            return true;
    FOR(i, 0, STACKS) //z inej kopky
        if (!stacks[i].empty() && stacks[i][0].rank + 1 == c.rank && stacks[i][0].colour() != c.colour())
            return true;
    return false;
}
double GameState::count_prob() const
{
    int turns = 0; //kolko kariet sa ma otocit
    FOR(i, 0, STACKS)
        if (stacks[i].empty() && downcards[i]) ++turns;
    if (turns == 0) return 0.0;

    bool used[4][13] = {false}; //ktore karty uz odkryte su
    FOR(i, 0, 4)
        FOR(j, 0, completed[i]) used[i][j] = true;
    FOR(i, 0, deck.size())
        used[deck[i].suit][deck[i].rank] = true;
    FOR(i, 0, STACKS)
        FOR(j, 0, stacks[i].size())
            used[stacks[i][j].suit][stacks[i][j].rank] = true;

    int total = 0, pl = 0; //kolko kariet zostava neodkrytych a kolko z nich je hratelnych
    FOR(i, 0, 4)
        FOR(j, 0, 13)
            if (!used[i][j])
            {
                ++total;
                if (playable(Card(j, i))) ++pl;
            }

    //vyrata konecnu pravdepodobnost
    double res = 1.0;
    FOR(i, 0, turns)
    {
        res *= (double)(total-pl) / (double)total;
        --total;
    }
    return 1.0 - res;
}
class Klondike
{
private:

    //vnutorne premenne
    set<vector<int> > game_states; //uz navstivene herne stavy

    int index; //index na najblizsi tah
    vector<string> next_moves; //najblizsie tahy, ktore je treba vykonat

public:
    //hlavne funkcie
    string move(vector<string> sstacks, vector<string> ddeck, vector<int> ccompleted, int pposition);

    void score_move(Move & move, const GameState & gs); //ohodnoti dany tah

    double backtrack(GameState & gs, int steps, Move & next_move); //backtrackuje priebeh hry a vyberie najlepsi tah
    double simulate(GameState & gs); //odsimuluje zvysok hry pomocou heuristiky a vrati pravdepodobnost uspechu
};

//hlavne funkcie
string Klondike::move(vector<string> sstacks, vector<string> ddeck, vector<int> ccompleted, int pposition)
{
    //ak mame dalsi tah predratany
    if (!next_moves.empty() && index < next_moves.size())
        return next_moves[index++];
    next_moves.clear();
    index = 0;

    //nacita stav hry
    GameState gs;

    FOR(i, 0, STACKS)
    {
        istringstream is(sstacks[i]);    
        is >> gs.downcards[i];

        string temp;
        while (is >> temp) gs.stacks[i].push_back(Card(temp));
    }

    FOR(i, 0, ddeck.size())
        gs.deck.push_back(Card(ddeck[i]));
    gs.position = pposition;
    
    FOR(i, 0, 4) gs.completed[i] = ccompleted[i];

    gs.init_from_deck();

    //zisti, ci sa dany tah uz neopakuje
    vector<int> state;
    FOR(i, 0, gs.deck.size()) state.push_back(gs.deck[i].rank * 10 + gs.deck[i].suit);
    state.push_back(-10);
    FOR(i, 0, STACKS)
    {
        state.push_back(-gs.downcards[i]);
        FOR(j, 0, gs.stacks[i].size()) state.push_back(gs.stacks[i][j].rank * 10 + gs.stacks[i][j].suit);
        state.push_back(-10);
    }
    FOR(i, 0, 4)
        state.push_back(gs.downcards[i]);
    state.push_back(gs.position);
    if (game_states.find(state) != game_states.end()) return "DONE";
    game_states.insert(state);

    //vezme a ohodnoti mozne tahy
    /*vector<Move> moves = gs.get_moves();
    if (moves.empty()) return "DONE";

    FOR(i, 0, moves.size()) score_move(moves[i], gs);
    sort(moves.begin(), moves.end());

    Move temp = moves.back();
    if (temp.primary == 0) return "DONE";*/

    int sum = 0;
    FOR(i, 0, 4) sum += gs.completed[i];
    int sum2 = 0;
    FOR(i, 0, STACKS) sum2 += gs.downcards[i];

    Move temp;
//    double res = backtrack(gs, sum > 10 ? 2 : 3, temp);
    double res = backtrack(gs, sum2 == 0 ? 4 : (sum > 12 ? 1 : 2), temp); //3635
//    double res = backtrack(gs, sum2 == 0 ? 4 : (sum > 12 ? 2 : 3), temp);
    if (res < 0.0) return "DONE";
    //DEBUG(res)

    if (res == 1.5)
    {
        return next_moves[index++];
    }

    if (temp.source == 11)
    {
        FOR(i, 0, temp.advances) next_moves.push_back("ADVANCE");
    }
    next_moves.push_back(temp.to_string());

    return next_moves[index++];
}

void Klondike::score_move(Move & move, const GameState & gs)
{    
    /*
    3 - vyznamny uzitok z tahu
    2 - znacny uzitok z tahu
    1 - minimalny uzitok z tahu
    0 - ziaden uzitok z tahu
    -1 - tah spet

    (K) - z kopiek
    (B) - z balicka
    (H) - z hotovych

    3 - (K) (B) -> (H) zahratie esa alebo dvojky (0)
      - (K) (B) -> (H) zahratie karty, ktorej nizsie varianty su uz zahrate (0)

    2 - (K) -> (K) (H) dojde k okamzitemu otoceniu kariet (pocet otocenych kariet + 1)
      - (K) -> (K) uvolnuje pole kralovi (0)
      - (B) -> (K) zahratie karty z balicku, s tym ze v dalsom tahu dojde k otoceniu kariet (1)
      - (B) -> (K) (H) zahratie karty z balicku, bez akehokolvek dalsieho vyznamu (-1)

    1 - (K) -> (H) bez akehokolvek dalsieho vyznamu (2)
      - (H) -> (K) zahratie karty, s tym ze v dalsom tahu dojde k otoceniu kariet alebo zahratia z balicku (1)
      - (K) -> (K) karta podo mnou sa uvolni na zahratie do hotovych (0)

    0 - (K) -> (K) zahratie karty len tak
      - (H) -> (K) zahratie karty spet do hry, bez akehokolvek dalsieho vyznamu

    (K) (B) -> (K) zahratie krala na volne miesto bez jeho nasledovnika (--2)
    */

    //trojky
    //(K) (B) -> (H) zahratie esa alebo dvojky (0)
    //(K) (B) -> (H) zahratie karty, ktorej nizsie varianty su uz zahrate (0)
    if (move.destination < 4 && (move.card.rank < 2 || (move.card.colour() == BLACK && gs.completed[1] >= move.card.rank && gs.completed[2] >= move.card.rank) ||
                (move.card.colour() == RED && gs.completed[0] >= move.card.rank && gs.completed[3] >= move.card.rank))) //obe nizsie su uz zahrate
    {
            move.primary = 3;
    }
    //dvojky
    //(K) -> (K) (H) dojde k okamzitemu otoceniu kariet (pocet otocenych kariet + 1)
    else if (move.source >= 4 && move.source < 11 && move.last && gs.downcards[move.source-4])
    {
        move.primary = 2;
        move.secondary = gs.downcards[move.source-4]+1;
    }
    //(K) -> (K) uvolnuje pole kralovi (0)
    else if (move.source >= 4 && move.source < 11 && move.destination >= 4 && move.destination < 11 && move.last && !gs.downcards[move.source-4] && move.card.rank != 12)
    {
        bool r = false;
        for (int i = 0; i < STACKS && !r; ++i)
            if (!gs.stacks[i].empty() && gs.downcards[i] && gs.stacks[i][0].rank == 12) 
                r = true;
        for (int i = 0; i < gs.from_deck.size() && !r; ++i)
            if (gs.from_deck[i].first.rank == 12) 
                r = true; 

        if (r)
        {
            move.primary = 2;
        }
    }
    //(B) -> (K) zahratie karty z balicku, s tym ze v dalsom tahu dojde k otoceniu kariet (1)
    //(B) -> (K) (H) zahratie karty z balicku, bez akehokolvek dalsieho vyznamu (-1)
    else if (move.source == 11)
    {
        move.primary = 2;
        move.secondary = 1;

        bool r = false;
        if (move.destination >= 4 && move.destination < 11)
        {
            for (int i = 0; i < STACKS && !r; ++i)
                if (!gs.stacks[i].empty() && gs.stacks[i][0].rank + 1 == move.card.rank && gs.stacks[i][0].colour() != move.card.colour() && gs.downcards[i])
                    r = true;
        }
        if (!r) move.secondary = -1;
    }
    //jednotky
    //(K) -> (H) bez akehokolvek dalsieho vyznamu (2)
    else if (move.destination < 4)
    {
        move.primary = 1;
        move.secondary = 2;
    }
    else if (move.source < 4)
    {
        //(H) -> (K) zahratie karty, s tym ze v dalsom tahu dojde k otoceniu kariet alebo zahratia z balicku (1)
        bool r = false;
        for (int i = 0; i < STACKS && !r; ++i)
            if (!gs.stacks[i].empty() && gs.stacks[i][0].rank + 1 == move.card.rank && gs.stacks[i][0].colour() != move.card.colour() && gs.downcards[i])
                r = true;
        for (int i = 0; i < gs.from_deck.size() && !r; ++i)
            if (gs.from_deck[i].first.rank + 1 == move.card.rank && gs.from_deck[i].first.colour() != move.card.colour())
                r = true;
        if (r)
        {
            move.primary = 1;
            move.secondary = 1;
        }
    }
    //(K) -> (K) karta podo mnou sa uvolni na zahratie do hotovych (0)
    else if (move.source >= 4 && move.source < 11 && move.destination >= 4 && move.destination < 11 && !move.last)
    {
        bool r = false;
        FOR(i, 0, gs.stacks[move.source-4].size())
            if (gs.stacks[move.source-4][i] == move.card && gs.stacks[move.source-4][i-1].rank == gs.completed[gs.stacks[move.source-4][i-1].suit])
                r = true;
        if (r)
        {
            move.primary = 1;
        }
    }

    //(K) (B) -> (K) zahratie krala na volne miesto bez jeho nasledovnika (--2)
    if (move.card.rank == 12 && move.destination >= 4 && move.destination < 11)
    {
        if (move.source == 11)
        {
            bool r = false;
            for (int i = 0; i < STACKS && !r; ++i)
                if (!gs.stacks[i].empty() && gs.stacks[i][0].rank + 1 == move.card.rank && gs.stacks[i][0].colour() != move.card.colour() && gs.downcards[i])
                    r = true;
            for (int i = 0; i < gs.from_deck.size() && !r; ++i)
                if (gs.from_deck[i].first.rank + 1 == move.card.rank && gs.from_deck[i].first.colour() != move.card.colour())
                    r = true;
            if (!r) move.secondary -= 2;
        }
        else if (move.source >= 4 && move.source < 11)
        {
            bool r = false;
            for (int i = 0; i < STACKS && !r; ++i)
                if (!gs.stacks[i].empty() && gs.stacks[i][0].rank + 1 == gs.stacks[move.source-4].back().rank && gs.stacks[i][0].colour() != gs.stacks[move.source-4].back().colour() && gs.downcards[i])
                    r = true;
            for (int i = 0; i < gs.from_deck.size() && !r; ++i)
                if (gs.from_deck[i].first.rank + 1 == gs.stacks[move.source-4].back().rank && gs.from_deck[i].first.colour() != gs.stacks[move.source-4].back().colour())
                    r = true;
            if (!r) move.secondary -= 2;
        }
    }
}

double Klondike::backtrack(GameState & gs, int steps, Move & next_move)
{
    if (!steps) return simulate(gs);

    if (gs.win()) return 1.5;

    gs.init_from_deck();
    vector<Move> moves = gs.get_moves();
    if (moves.empty()) return -1.0;

    FOR(i, 0, moves.size()) score_move(moves[i], gs);

    double best = -2.0;
    Move temp;
    FOR(i, 0, moves.size())
    {
        GameState gs_next = gs;
        gs_next.make_move(moves[i]);
        double act = backtrack(gs_next, steps-1, temp);
        if (act == 1.5)
        {
            next_moves.insert(next_moves.begin(), moves[i].to_string());
            if (moves[i].source == 11)
                FOR(j, 0, moves[i].advances) next_moves.insert(next_moves.begin(), "ADVANCE");
            return 1.5;
        }

        if (act > best || (act == best && next_move < moves[i]))
        {
            best = act;
            next_move = moves[i];
        }
    }
    return best;
}
double Klondike::simulate(GameState & gs)
{
    gs.init_from_deck();
    vector<Move> moves = gs.get_moves();

    vector<Move> used_moves;

    double res = 0.0;
    bool r = false;
    FOR(i, 0, STACKS) if (gs.stacks[i].empty() && gs.downcards[i]) r = true;
    if (r) res = max(res, gs.count_prob());

    while (!moves.empty())
    {
        FOR(i, 0, moves.size())
            score_move(moves[i], gs);
        sort(moves.begin(), moves.end());
        if (moves.back().primary == 0)
            break;

        gs.make_move(moves.back());
        used_moves.push_back(moves.back());

        gs.init_from_deck();
        moves = gs.get_moves();

        bool r = false;
        FOR(i, 0, STACKS) if (gs.stacks[i].empty() && gs.downcards[i]) r = true;
        if (r) res = max(res, gs.count_prob());
    }
    if (gs.win()) 
    {
        FOR(i, 0, used_moves.size())
        {
            if (used_moves[i].source == 11)
                FOR(j, 0, used_moves[i].advances) next_moves.push_back("ADVANCE");
            next_moves.push_back(used_moves[i].to_string());
        }
        return 1.5;
    }
    return res;//gs.count_prob();
}



void Vis(vector <string> Vs, vector <string> Vd, vector <int> Vc, int Vp) {
     //system("CLS");
     
     printf("DECK:\n");
     for (int i=0; i<Vd.size(); i++) cout << Vd[i] << " ";
     printf("\n");
     for (int i=0; i<Vp*3; i++) printf(" ");
     printf("/\\\n");
     printf("Completed: ");
     for (int i=0; i<4; i++) printf("%d ",Vc[i]);
     printf("\nStack:\n");
     vector<int> cnt; int ma=0;
     for (int i=0; i<7; i++) {
         cnt.push_back(Vs[i][0]-'0'+(Vs[i].length()-1)/3);
         if (cnt[i]>ma) ma=cnt[i];
         }
     
     for (int i=0; i<ma; i++) {
         for (int j=0; j<7; j++) {
             if (i==0 && cnt[j]==0) { printf(" -- "); continue; }
             if (cnt[j]<=i) { printf("    "); continue; }
             if (i<Vs[j][0]-'0') { printf(" ## "); continue; }
             printf(" %c%c ",Vs[j][(i-Vs[j][0]+'0')*3+2],Vs[j][(i-Vs[j][0]+'0')*3+3]);
             }
         printf("\n");
         }
     
     //system("PAUSE");
}





void illegal () {
   //system("CLS");
   printf("ILLEGAL");
   system("PAUSE");
}

int main() {
    Klondike kl;
    freopen("mfile.txt","w",stdout);
    int game=0,win=0;
    vector<int> vc;
    for (int i=0; i<52; i++) vc.push_back(i);
    while (game<1000) {
    clock_t st; st=clock();
    game++;
    random_shuffle(vc.begin(),vc.end());
    while (game<501) {
       game++;
       random_shuffle(vc.begin(),vc.end());
       }
    //for (int i=0; i<52; i++) printf("%d ",vc[i]);
    //printf("\n");
    //system("PAUSE");
    vector<string> Rs,Rd;
    vector<int> Rc;
    int Rp=2;
    int tc=0,tm=0;
    while (tm<52) {
       string ts=""; ts+=tc+'0';
       ts+=" "+n2c(vc[tm]);
       Rs.push_back(ts);
       tc++;
       tm+=tc;
       }
    for (int i=28; i<52; i++) Rd.push_back(n2c(vc[i]));
    for (int i=0; i<4; i++) Rc.push_back(0);
    while (true) {
       //Vis(Rs,Rd,Rc,Rp);
       string cmd;
       cmd=kl.move(Rs,Rd,Rc,Rp);
       //cout << cmd << endl;
       //system("PAUSE");
       //getline(cin,cmd);
       if (cmd=="DONE") break;
       if (cmd=="ADVANCE") {
          if (Rp==Rd.size()-1) {
             if (Rd.size()>=3) Rp=2;
                else Rp=Rd.size()-1;
             }
             else {
                  Rp+=3;
                  if (Rp>Rd.size()-1) Rp=Rd.size()-1;
                  }
          continue;
          }
       int ns=cmd[1]-'1',nd=cmd[4]-'1';
       if (cmd.length()==7) {
          nd=cmd[6]-'1';
          if (cmd[5]=='S') {
             int l=Rs[nd].length(),sci=c2n(Rd[Rp]);
             if (l==1) {
                if (sci%13==12) {
                   Rd.erase(Rd.begin()+Rp);
                   Rs[nd].append(" ");
                   Rs[nd].append(n2c(sci));
                   if (Rp!=0) Rp--;
                   continue;
                   }
                   else { illegal(); break; }
                }
             string dcs=""; dcs+=Rs[nd][l-2]; dcs+=Rs[nd][l-1];
             int dci=c2n(dcs);
             if (suitcheck(dci,sci) && sci%13==dci%13-1) {
                Rd.erase(Rd.begin()+Rp);
                Rs[nd].append(" ");
                Rs[nd].append(n2c(sci));
                if (Rp!=0) Rp--;
                   else Rp=min((int)(Rd.size()-1),2);
                }
                else { illegal(); break; }
             }
             else {
             int sci=c2n(Rd[Rp]);
             if (sci/13==nd && sci%13==Rc[nd]) {
                Rc[nd]++;
                Rd.erase(Rd.begin()+Rp);
                if (Rc[0]+Rc[1]+Rc[2]+Rc[3]==52) break;
                if (Rp!=0) Rp--;
                   else Rp=min((int)(Rd.size()-1),2);
                }
                else { illegal(); break; }
             }
          continue;
          }
       
       if (cmd[0]=='S' && cmd[3]=='C') {
          int l=Rs[ns].length();
          if (l==1) { illegal(); break; }
          string scs=""; scs+=Rs[ns][l-2]; scs+=Rs[ns][l-1];
          int sci=c2n(scs);
          if (sci/13==nd && sci%13==Rc[nd]) {
             Rc[nd]++;
             Rs[ns].erase(Rs[ns].end()-3,Rs[ns].end());
             if (Rc[0]+Rc[1]+Rc[2]+Rc[3]==52) break;
             if (Rs[ns].length()==1 && Rs[ns][0]!='0') {
                Rs[ns].append(" ");
                int nci=vc[(ns*(ns+1))/2+ns-Rs[ns][0]+'0'+1];
                Rs[ns].append(n2c(nci));
                Rs[ns][0]=Rs[ns][0]-1;
                }
             }
             else { illegal(); break; }
          continue;
          }
       if (cmd[0]=='C' && cmd[3]=='S') {
          int l=Rs[nd].length();
          string dcs=""; dcs+=Rs[nd][l-2]; dcs+=Rs[nd][l-1];
          int dci=c2n(dcs),sci=ns*13+Rc[ns]-1;
          if (l==1) {
             if (sci%13==12) {
                Rc[ns]--;
                Rs[nd].append(" ");
                Rs[nd].append(n2c(sci));
                continue;
                }
                else { illegal(); break; }
             }
          if (suitcheck(dci,sci) && sci%13==dci%13-1) {
             Rc[ns]--;
             Rs[nd].append(" ");
             Rs[nd].append(n2c(sci));
             }
             else { illegal(); break; }
          continue;
          }
       if (cmd[0]=='S' && cmd[3]=='S') {
          int l=Rs[nd].length();
          //cout << Rs[ns] << endl;
          //cout << Rs[nd] << endl;
          if (l==1) {
             string scs=""; scs+=Rs[ns][2]; scs+=Rs[ns][3];
             int sci=c2n(scs);
             if (sci%13==12) {
                string ts=Rs[ns].substr(1,Rs[ns].length()-1);
                Rs[ns].erase(Rs[ns].begin()+1,Rs[ns].end());
                Rs[nd].append(ts);
                if (Rs[ns].length()==1 && Rs[ns][0]!='0') {
                    Rs[ns].append(" ");
                    int nci=vc[(ns*(ns+1))/2+ns-Rs[ns][0]+'0'+1];
                    Rs[ns].append(n2c(nci));
                    Rs[ns][0]=Rs[ns][0]-1;
                    }
                continue;
                }
                else { illegal(); break; }
             }
          string dcs=""; dcs+=Rs[nd][l-2]; dcs+=Rs[nd][l-1];
          int dci=c2n(dcs),ip;
          for (ip=Rs[ns].length()-2; ip>0; ip-=3) {
              string scs=""; scs+=Rs[ns][ip]; scs+=Rs[ns][ip+1];
              int sci=c2n(scs);
              //cout << ip << endl;
              //cout << dci << " " << sci << endl;
              if (suitcheck(dci,sci) && sci%13==dci%13-1) {
                 string ts=Rs[ns].substr(ip-1,Rs[ns].length()-ip+1);
                 //cout << ts << endl;
                 Rs[ns].erase(Rs[ns].begin()+ip-1,Rs[ns].end());
                 Rs[nd].append(ts);
                 if (Rs[ns].length()==1 && Rs[ns][0]!='0') {
                    Rs[ns].append(" ");
                    int nci=vc[(ns*(ns+1))/2+ns-Rs[ns][0]+'0'+1];
                    Rs[ns].append(n2c(nci));
                    Rs[ns][0]=Rs[ns][0]-1;
                    }
                 //system("PAUSE");
                 break;
                 }
              }
          if (ip<=0) { illegal(); break; }
          continue;
          }
       illegal(); break;
       }
    Vis(Rs,Rd,Rc,Rp);
    clock_t end; end=clock();
    double dif=(end-st)/1000.0;
    //printf("NEW GAME\n");
    printf("Game %d finished in %0.3lfs\n",game,dif);
    if (Rc[0]+Rc[1]+Rc[2]+Rc[3]==52) {
       printf("Game %d is WON!\n",game);
       win++;
       }
    printf("\n");
    //system("PAUSE");
    fflush(stdout);
    }
    printf("TOTAL WIN: %d\n",win);
}
