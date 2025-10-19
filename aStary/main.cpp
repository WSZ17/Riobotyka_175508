#include <iostream>
#include <cmath>

using namespace std;

//Koszt kroku = 1, kolejność sprawdzania: góra, dół, lewo, prawo
const int row = 20;
const int col = 20;

//0 - wolne, 5 - ściana
//Trzeba zreversować wokół osi y
int grid[row][col]={
    /*{0, 0, 5, 0, 0},
    {5, 0, 0, 0, 5},
    {0, 0, 5, 0, 0},
    {5, 0, 0, 0, 0},
    {0, 0, 0, 5, 0}*/
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 5},
    {0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 5, 5, 5, 5, 5, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0}
};

int grid_rev[row][col] {
    {0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,0,0},
    {0,0,0,0,0,0,0,0,0,5,0,0,5,5,5,5,5,5,0,0},
    {0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0},
    {0,5,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,5,5,5,5,5,5,5,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,5,5,5,5,5,5,5,5,5,5,0,5,5,5,5,5,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,5,5},
    {0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,5,0},
    {0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,5,0,0,0}
};

void printArray(int arr[row][col]) {
    for(int i=0; i<row; i++) {
        for(int j=0; j<col; j++) {
            cout << arr[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "----\n";
}

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
Kratka aktualna;
Kratka sasiad;

//Tablica do której dodawane będą odwiedzone już kratki
Kratka lista_closed[row*col];
//Tablica do której dodawane będą rozważane następnie kratki, będą stąd usuwane jeżeli zostaną przeniesione do zamkniętej
Kratka lista_open[row*col];

int sciezka[row*col][2];
int sciezkaLen=0;

double euklides(int x, int y, int i, int j) {
    return sqrt(pow(i - x, 2) + pow(j - y, 2));
}

bool czyDopuszczalnePole(int x, int y) {
    return (x >= 0 && x < row) && (y >= 0 && y < col) && (grid_rev[y][x] != 5);
}

void aGwiazdeczka() {
    //Liczniki które pokazują, ile obecniie elementów znajduje się na liście
    int openCount=0, closedCount=0;

    start.g = 0;
    start.h = euklides(start.x,start.y,koniec.x,koniec.y);
    start.f = start.g + start.h;
    start.rodzicX = -1; start.rodzicY = -1;
    lista_open[0] = start;
    openCount++;

    bool znaleziono=false;

    while(openCount>0) {
        //wybieranie kratki o najmniejszym f (koszcie całkowitym ruchu) z listy otwartej
        //j - indeks najlepszej kratki (najmniejsze f)
        int j=0;
        for(int i=1;i<openCount;i++)
            if(lista_open[i].f < lista_open[j].f) j=i;
        aktualna=lista_open[j];
        //tutaj z racji braku innych elementów wpierw jako element aktualny jest ustawiana kratka startowa
        //i jest równe 1, ponieważ pierwszy element listy (element 0) jest już ustawiony jako start

        //usuwanie z listy otwartej
        lista_open[j] = lista_open[--openCount];
        lista_closed[closedCount++] = aktualna;

        //warunek kończący funkcję - sprawdza, czy aktualna kratka jest kratką celową
        if(aktualna.x==koniec.x && aktualna.y==koniec.y) {
            znaleziono=true;
            break;
        }

        //sprawdzanie sąsiadów obecnej kratki
        for(int k=0;k<4;k++) {
            int sx=aktualna.x+kierunki[k][0];
            int sy=aktualna.y+kierunki[k][1];
            //nx i ny to współrzędne sąsiada
            if(!czyDopuszczalnePole(sx,sy))
                continue;
            //^ tutaj sprawdzane jest, czy sąsiad nie wychodzi poza grid oraz nie jest ścianą (5), sprawdzając po kolei górę, dół, lewo i prawo

            bool wZamknietej=false;
            //Przechodzi przez listę zamnkiętą i sprawdza, czy sąsiad już się tam znalazł, jeżeli nie, to funkcja kontynuuje działanie...
            for(int i=0;i<closedCount;i++)
                if(lista_closed[i].x==sx && lista_closed[i].y==sy) {wZamknietej=true;break;}
            if(wZamknietej)
                continue;

            //...i zapisuje jego wartości w kratce sąsiad
            sasiad.x=sx; sasiad.y=sy;
            sasiad.g=aktualna.g+1;
            sasiad.h=euklides(sx,sy,koniec.x,koniec.y);
            sasiad.f=sasiad.g+sasiad.h;
            sasiad.rodzicX=aktualna.x;
            sasiad.rodzicY=aktualna.y;

            //sprawdzanie, czy sąsiad jest w liście otwartej
            bool wOtwartej=false;
            for(int i=0;i<openCount;i++)
                if(lista_open[i].x==sx && lista_open[i].y==sy) {
                    wOtwartej=true;
                    break;
                }

            //jeżeli nie jest w otwartej to dodaje go do listy otwwartej
            if(!wOtwartej)
                lista_open[openCount++]=sasiad;
        }
    }

    if(znaleziono) {
        cout<<"Sciezka:\n";
        //tutaj mamy while'a, który sprawdza, czy aktualna kratka nie jest kratką startową (bo kratka startowa ma rodziców jako -1) i dopóki nie jest kratką startową to wykonuje dalej pętle, dodając po kolei elementy, cofając się po rodzicach
        while(!(aktualna.rodzicX==-1 && aktualna.rodzicY==-1)) {
            sciezka[sciezkaLen][0]=aktualna.x;
            sciezka[sciezkaLen][1]=aktualna.y;
            sciezkaLen++;
            for(int i=0;i<closedCount;i++)
                //tutaj sprawdza i zamienia elementy - jeżeli jakiś element listy zamkniętej jest rodzicem kratki aktualnej, to zamieniamy element aktualny na tego rodzica i ponawiamy proces
                if(lista_closed[i].x==aktualna.rodzicX && lista_closed[i].y==aktualna.rodzicY) {
                    aktualna=lista_closed[i]; break;
                }
        }
        //pętla kończy się w momencie, w którym dotrzemy do startu, więc teraz dodajemy również i element startowy
        sciezka[sciezkaLen][0]=start.x;
        sciezka[sciezkaLen][1]=start.y;
        sciezkaLen++;

        //zamiana wierszy, żeby było od kratki startowej, do kratki końcowej
        for (int i = 0; i < sciezkaLen / 2; i++) {
            for (int j = 0; j < 2; j++) {
                swap(sciezka[i][j], sciezka[sciezkaLen - 1 - i][j]);
            }
        }

        //drukowanie ścieżki
        for (int i=0; i<sciezkaLen; i++) {
            cout<<"("<<sciezka[i][0]<<","<<sciezka[i][1]<<") ";
        }
        cout<<"\n";

    } else cout<<"Brak sciezki!\n";
}

int main() {
    cout << "Podaj punkt startowy";
    cout<<"\n";
    cin >> start.x;
    cin >> start.y;
    cout << "Podaj punkt koncowy";
    cin >> koniec.x;
    cin >> koniec.y;
    cout<<"\n";

    //sprawdzanie funkcji w trakcie pisania
    /*
    cout << czyDopuszczalnePole(start.x, start.y);
    cout << euklides(start.x, start.y, koniec.x, koniec.y);
    */

    //printArray(grid);
    printArray(grid_rev);

    aGwiazdeczka();

    return 0;
}