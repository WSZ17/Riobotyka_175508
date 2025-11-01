// pacman_astar_no_termios.cpp
// Prosta tekstowa gra Pac-Man z duchem poruszającym się A*.
// Bez użycia termios. Windows: natychmiastowy odczyt klawiszy (conio.h).
// Unix: fallback - wątek czyta stdin (wymagany Enter).

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <thread>
#include <atomic>
#include <chrono>

#ifdef _WIN32
  #include <conio.h> // _kbhit, _getch
#endif

using namespace std;

// ----- ustawienia planszy -----
const int ROWS = 10;
const int COLS = 20;
const char WALL = '#';
const char DOT = '.';
const char PAC = 'P';
const char GHT = 'G';
const char EMPTY = ' ';

// prosta mapa (możesz edytować)
vector<string> grid = {
    "####################",
    "#..............#...#",
    "#..####........#...#",
    "#..............#...#",
    "#..#########.......#",
    "#..................#",
    "#...######.........#",
    "#..............#...#",
    "#..............#...#",
    "####################"
};

// ----- A* -----
struct Node {
    int x,y;
    double g,f;
    int px,py;
};
struct Compare {
    bool operator()(Node const& a, Node const& b) const { return a.f > b.f; }
};

bool inBounds(int x,int y){ return x>=0 && x<COLS && y>=0 && y<ROWS; }
double heuristic(int x1,int y1,int x2,int y2){ return abs(x1-x2)+abs(y1-y2); }

vector<pair<int,int>> astar(pair<int,int> start, pair<int,int> goal){
    priority_queue<Node, vector<Node>, Compare> open;
    vector<vector<double>> gscore(ROWS, vector<double>(COLS, 1e9));
    vector<vector<pair<int,int>>> parent(ROWS, vector<pair<int,int>>(COLS, {-1,-1}));
    gscore[start.second][start.first] = 0.0;
    open.push({start.first, start.second, 0.0, heuristic(start.first,start.second,goal.first,goal.second), -1, -1});
    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    while(!open.empty()){
        Node cur = open.top(); open.pop();
        if(cur.x==goal.first && cur.y==goal.second){
            vector<pair<int,int>> path;
            int x=cur.x, y=cur.y;
            while(x!=-1 && y!=-1){
                path.push_back({x,y});
                auto p = parent[y][x];
                x = p.first; y = p.second;
            }
            reverse(path.begin(), path.end());
            return path;
        }
        for(auto &d : dirs){
            int nx = cur.x + d[0], ny = cur.y + d[1];
            if(!inBounds(nx,ny)) continue;
            if(grid[ny][nx]==WALL) continue;
            double ng = cur.g + 1.0;
            if(ng < gscore[ny][nx]){
                gscore[ny][nx] = ng;
                parent[ny][nx] = {cur.x, cur.y};
                double f = ng + heuristic(nx,ny,goal.first,goal.second);
                open.push({nx,ny,ng,f,cur.x,cur.y});
            }
        }
    }
    return {}; // brak ścieżki
}

// ----- rysowanie -----
void draw(pair<int,int> pac, pair<int,int> ghost, int score){
#ifdef _WIN32
    system("cls");
#else
    // "clear" może być niebezpieczne w niektórych środowiskach, ale ogólnie działa w terminalu.
    system("clear");
#endif
    for(int y=0;y<ROWS;y++){
        for(int x=0;x<COLS;x++){
            if(pac.first==x && pac.second==y) cout<<PAC;
            else if(ghost.first==x && ghost.second==y) cout<<GHT;
            else cout<<grid[y][x];
        }
        cout<<"\n";
    }
    cout<<"Score: "<<score<<"\n";
    cout<<"Sterowanie: W A S D (Windows: bez Enter; Unix: naciśnij klawisz + Enter)\n";
}

// ----- wejście (bez termios) -----
// Mechanizm: cross-platform
// - Windows: użyj _kbhit()/_getch() dla natychmiastowego działania
// - Inne: osobny wątek czyta std::cin (wymagany Enter). Zapisuje ostatni znak do atomic<char>
atomic<char> lastKey{0};
atomic<bool> runningInputThread{false};

#ifndef _WIN32
void inputThreadFunc(){
    runningInputThread = true;
    while(runningInputThread){
        int c = std::cin.get(); // wymaga Enter w terminalu
        if(c==EOF) break;
        if(c=='\n') continue; // ignoruj Enter jako osobny znak
        lastKey = static_cast<char>(c);
    }
}
#endif

// ----- program główny -----
int main(){
    pair<int,int> pac = {1,1};
    pair<int,int> ghost = {COLS-2, ROWS-2};
    int score = 0;

    // policz początkowe kropki (.)
    int totalDots=0;
    for(int y=0;y<ROWS;y++) for(int x=0;x<COLS;x++) if(grid[y][x]=='.') totalDots++;

#ifndef _WIN32
    // uruchom wątek wejściowy na Unixach (fallback - wymaga Enter)
    thread inputThread(inputThreadFunc);
#endif

    bool running=true;
    while(running){
        draw(pac, ghost, score);

        // ---- odczyt naciśniętego klawisza ----
        char key = 0;
#ifdef _WIN32
        if(_kbhit()){
            int c = _getch();
            // _getch() może zwrócić 0 lub 224 dla klawiszy specjalnych; ignorujemy
            if(c!=0 && c!=224) key = static_cast<char>(c);
        }
#else
        // Unix fallback: sprawdź ostatnią zapisaną literę przez wątek
        char c = lastKey.exchange(0);
        if(c!=0) key = c;
#endif

        // ---- ruch gracza ----
        int nx = pac.first, ny = pac.second;
        if(key=='w' || key=='W') ny--;
        else if(key=='s' || key=='S') ny++;
        else if(key=='a' || key=='A') nx--;
        else if(key=='d' || key=='D') nx++;

        if(inBounds(nx,ny) && grid[ny][nx] != WALL){
            pac.first = nx; pac.second = ny;
            // zbieranie kropki
            if(grid[ny][nx]==DOT){
                grid[ny][nx] = EMPTY;
                score += 10;
                totalDots--;
            }
        }

        // ---- ruch ducha (A*) ----
        auto path = astar(ghost, pac);
        if(path.size() > 1){
            ghost = path[1];
        }

        // ---- sprawdzenia końca gry ----
        if(pac == ghost){
            draw(pac, ghost, score);
            cout<<"\n💀 Duch cię złapał! Koniec gry.\n";
            break;
        }
        if(totalDots==0){
            draw(pac, ghost, score);
            cout<<"\n🏆 Zebrałeś wszystkie kropki! Wygrana!\n";
            break;
        }

        // małe opóźnienie pętli (reguluje prędkość gry)
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

#ifndef _WIN32
    // zatrzymaj wątek wejściowy
    runningInputThread = false;
    // wysyłamy EOF na stdin? Nie zawsze; po prostu dołączamy wątek jeśli jest joinable.
    if(std::cin.good()) {
        // nic do zrobienia - wątek zakończy się gdy użytkownik zakończy stdin (Ctrl+D) lub program zamknie terminal
    }
    // join wątku (bez blokowania wiecznie; jeśli wątek dalej czeka na input, można detach)
    if(inputThread.joinable()) inputThread.detach();
#endif

    return 0;
}
