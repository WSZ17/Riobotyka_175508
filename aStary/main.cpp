#include <iostream>
#include <cmath>
//Później na usuwanie -> #include <bits/stdc++.h>

using namespace std;

//Koszt kroku = 1, kolejność sprawdzania: góra, dół, lewo, prawo
const int row = 3;
const int col = 3;

//0 - wolne, 5 - ściana
int grid[row][col]={
    {0, 0, 5},
    {5, 0, 0},
    {5, 0, 0}
};

//lista kierunków: góra, dół, lewo, prawo
int kierunki[4][2] = {
    {0,-1},
    {0,1},
    {-1,0},
    {1,0}
};

struct Kratka {
    int x;
    int y;
    int rodzicX;
    int rodzicY;
    double g, h, f;
};

Kratka start;
Kratka koniec;

//Tablica do której dodawane będą odwiedzone kratki
Kratka lista_closed[row*col];
//Tablics do której dodawane będą kratki rozważane jako kolejne pola
Kratka lista_open[row*col];

double euklides(int x, int y, int i, int j) {
    return sqrt(pow(i - x, 2) + pow(j - y, 2));
}

bool czyDopuszczalnePole(int x, int y) {
    return (x >= 0 && x < row) && (y >= 0 && y < col) && (grid[y][x] != 5);
}

/*void (nie wiem jak napisać typ returnowania więc na razie jest void) aGwiazdeczka() {

    //pozycja startowa - będzie się zmieniać z każdym kolejnym polem
    int dx = start.x;
    int dy = start.y;

    Kratka aktualna;

    lista_closed[licznik_closed][0] = dx;
    lista_closed[licznik_closed][1] = dy;
    licznik_closed++;

    //int obecna[2] = {dx, dy};
    while (dx!= koniec.x && dy!=koniec.y) {
        dx = start.x+1;
        dy = start.y+1;
    }

}*/

void aGwiazdeczka() {
    int openCount=0, closedCount=0;

    start.g = 0;
    start.h = euklides(start.x,start.y,koniec.x,koniec.y);
    start.f = start.g + start.h;
    start.rodzicX = -1; start.rodzicY = -1;
    lista_open[openCount++] = start;

    bool znaleziono=false;
    Kratka aktualna;

    while(openCount>0) {
        // wybierz kratkę o najmniejszym f
        int idx=0;
        for(int i=1;i<openCount;i++)
            if(lista_open[i].f < lista_open[idx].f) idx=i;
        aktualna=lista_open[idx];

        // usuń z otwartej
        lista_open[idx] = lista_open[--openCount];
        lista_closed[closedCount++] = aktualna;

        if(aktualna.x==koniec.x && aktualna.y==koniec.y) {
            znaleziono=true;
            break;
        }

        // sprawdź sąsiadów
        for(int k=0;k<4;k++) {
            int nx=aktualna.x+kierunki[k][0];
            int ny=aktualna.y+kierunki[k][1];
            if(!czyDopuszczalnePole(nx,ny)) continue;

            bool wZamknietej=false;
            for(int i=0;i<closedCount;i++)
                if(lista_closed[i].x==nx && lista_closed[i].y==ny) {wZamknietej=true;break;}
            if(wZamknietej) continue;

            Kratka s;
            s.x=nx; s.y=ny;
            s.g=aktualna.g+1;
            s.h=euklides(nx,ny,koniec.x,koniec.y);
            s.f=s.g+s.h;
            s.rodzicX=aktualna.x;
            s.rodzicY=aktualna.y;

            bool wOtwartej=false;
            for(int i=0;i<openCount;i++)
                if(lista_open[i].x==nx && lista_open[i].y==ny) {wOtwartej=true;break;}
            if(!wOtwartej) lista_open[openCount++]=s;
        }
    }

    if(znaleziono) {
        cout<<"Sciezka:\n";
        int path[row*col][2], pathLen=0;
        while(!(aktualna.rodzicX==-1 && aktualna.rodzicY==-1)) {
            path[pathLen][0]=aktualna.x;
            path[pathLen][1]=aktualna.y;
            pathLen++;
            for(int i=0;i<closedCount;i++)
                if(lista_closed[i].x==aktualna.rodzicX && lista_closed[i].y==aktualna.rodzicY) {
                    aktualna=lista_closed[i]; break;
                }
        }
        path[pathLen][0]=start.x;
        path[pathLen][1]=start.y;
        pathLen++;

        for(int i=pathLen-1;i>=0;i--) {
            cout<<"("<<path[i][0]<<","<<path[i][1]<<") ";
        }
        cout<<"\n";
    } else cout<<"Brak sciezki!\n";
}

int main() {
    cout << "Podaj punkt startowy";
    cin >> start.x;
    cin >> start.y;
    cout << "Podaj punkt koncowy";
    cin >> koniec.x;
    cin >> koniec.y;

    //sprawdzanie funkcji w trakcie pisania
    /*
    cout << czyDopuszczalnePole(start.x, start.y);
    cout << euklides(start.x, start.y, koniec.x, koniec.y);
    */

    aGwiazdeczka();

    return 0;
}