#include <iostream>
#include <conio.h>
#include <thread>
#include <Windows.h>
#include <cstdlib>
#include<fstream>
#include<ostream>
#include<string>

using namespace std;

//Cố định màn hình console
void FixConsoleWindow()
{
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void ShowCur(bool CursorVisibility)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ConCurInf;

    ConCurInf.dwSize = 10;
    ConCurInf.bVisible = CursorVisibility;

    SetConsoleCursorInfo(handle, &ConCurInf);
}
//Di chuyển
void GotoXY(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void SetColor(int textcolor) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorcode = textcolor;
    SetConsoleTextAttribute(hStdout, colorcode);
}
//Khai báo các biến toàn cục
#define MAX_CAR 19
#define MAX_CAR_LENGTH 8
#define MAX_CAR_HEIGHT 3
#define MAX_SPEED 6

POINT** X; //Mảng MAX_CAR xe
POINT Y; // Đại diện người qua đường
bool** C; // Đánh dấu vị trí xe đi qua
int cnt = 0;//Biên hỗ trợ tăng tôc
int MOVING;//Hướng di chuyển của người
int SPEED;// Tốc độ/Level
int HEIGH_CONSOLE = 40, WIDTH_CONSOLE = 70;// Kích thước màn hình game
bool STATE; // Trạng thái sống chết của người qua đường
int soLanXe = 8;
int q = 6;
bool* S;


void MenuGame()
{
    GotoXY(20, 5); cout << "  ___     ___     ___     ___     ___             _____   _  _     ___              ___     ___     ___     ___" << endl;
    GotoXY(20, 6); cout << " / __|   | _ " << char(92) << "   / _ " << char(92) << "   / __|   / __|     o O O |_   _| | || |   | __|     o O O  | _ " << char(92) << "   / _ " << char(92) << "   /   " << char(92) << "   |   " << char(92) << " " << endl;
    GotoXY(20, 7); cout << " | (__   |   /  | (_) |  " << char(92) << "__ " << char(92) << "   " << char(92) << "__ " << char(92) << "    o        | |   | __ |   | _|     o       |   /  | (_) |  | - |   | |) |" << endl;
    GotoXY(20, 8); cout << " " << char(92) << "___|   |_|_" << char(92) << "   " << char(92) << "___/   |___/   |___/   TS__[O]  _|_|_  |_||_|   |___|   TS__[O]  |_|_" << char(92) << "   " << char(92) << "___/   |_|_|   |___/" << endl;
    GotoXY(20, 9); cout << "_|''''|_ |''''|_|''''|_  |''''|_|''''|   {======|_|''''|_|''''|_  |''''|  {======|_|''''|_|''''|_  |''''|_ |''''|" << endl;
    GotoXY(20, 10); cout << "'`-0-0-''`-0-0-''`-0-0-''`-0-0-''`-0-0-'./o--000''`-0-0-''`-0-0-''`-0-0-'./o--000''`-0-0-''`-0-0-''`-0-0-''`-0-0-''';";

    GotoXY(68, 15); cout << "1. Start game";
    GotoXY(68, 16); cout << "2. Load game";
    GotoXY(68, 17); cout << "3. Save game";
    GotoXY(68, 18); cout << "4. Exit";
    GotoXY(50, 19); cout << "Type the number in proportion with the action you want.";
}


void DrawTrafficLights(int x, int y, int curPosX = 0, int curPosY = 0)
{
    for (int i = 0; i < soLanXe * 4; i += 4) {
        if (i == 6 * 4 || i == 3 * 4)
            continue;
        GotoXY(x, y + i);
        SetColor(1);
        cout << char(254);
    }
    SetColor(15);
    GotoXY(curPosX, curPosY = 0);
}

//Khởi tạo dữ liệu ban đầu
void ResetData()
{
    MOVING = 'D';
    SPEED = 1;
    Y = { 35,37 }; // Vị trí lúc đầu của người

    time_t current_time;
    current_time = time(NULL);
    if (X == NULL)
    {
        X = new POINT * [MAX_CAR];
        C = new bool* [WIDTH_CONSOLE];
        for (int i = 0; i < WIDTH_CONSOLE; i++) {
            C[i] = new bool[HEIGH_CONSOLE];
        }
        S = new bool[WIDTH_CONSOLE];
        for (int i = 0; i < WIDTH_CONSOLE; i++) {
            S[i] = true;
        }
        memset(C, false, sizeof(C));
        for (int i = 0; i < MAX_CAR; i++)
            X[i] = new POINT[MAX_CAR_LENGTH];
        int temp = (rand() % (WIDTH_CONSOLE - MAX_CAR_LENGTH)) + 1;
        for (int j = 0; j < MAX_CAR_LENGTH; j++)
        {
            X[0][j].x = 1;
            X[0][j].y = 5;
        }
        int cnt = 0;
        for (int i = 1; i < MAX_CAR - 1; i++)
        {
            if (i % 3 == 0) cnt++;
            if (cnt == 3) cnt += 4;
            if (cnt == 9) cnt += 4;
            temp = (rand() % (WIDTH_CONSOLE - MAX_CAR_LENGTH)) + 1;
            for (int j = 0; j < MAX_CAR_LENGTH; j++)
            {
                X[i][j].x = temp + j;
                if ((i % 3) == 0) {
                    X[i][j].y = 5 + i + cnt;
                }
                else {
                    X[i][j].x = X[i - 1][j].x;
                    X[i][j].y = 5 + i + cnt;
                }
            }
        }
    }
}

//Vẽ màn hình game
void DrawSquare(int start_x, int start_y, int end_x, int end_y)
{
    for (int i = start_x + 1; i < end_x; i++)
    {
        GotoXY(i, start_y); cout << "_";
        GotoXY(i, end_y); cout << "_";
    }
    for (int i = start_y + 1; i <= end_y; i++)
    {
        GotoXY(start_x, i); cout << "|";
        GotoXY(end_x, i); cout << "|";
    }
    for (int i = start_x + 1; i < end_x; i++)
    {
        GotoXY(i, end_y - 1); cout << "/";
        GotoXY(i, end_y - 2); cout << "/";
    }
}
void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0)
{
    GotoXY(x, y); cout << "#";
    for (int i = 1; i < width; i++) cout << "#";
    cout << "#";
    GotoXY(x, height + y); cout << "#";
    for (int i = 1; i < width; i++) cout << "#";
    cout << "#";
    for (int i = y + 1; i < height + y; i++)
    {
        GotoXY(x, i); cout << "#";
        GotoXY(x + width, i); cout << "#";
    }
    for (int i = 1; i < HEIGH_CONSOLE; i++)
    {
        GotoXY(70, i);  cout << "#";
    }
    //Lan duong:
    {
        for (int j = 4; j < 16; j += 4)
        {
            for (int i = 1; i < 70; i++)
            {
                GotoXY(i, j); cout << ".";
            }
        }
        for (int i = 1; i < 70; i++)
        {
            GotoXY(i, 24); cout << ".";
        }
        for (int i = 1; i < 70; i++)
        {
            GotoXY(i, 36); cout << ".";
        }
    }
    DrawSquare(1, 16, 10, 20);
    DrawSquare(20, 16, 30, 20);
    DrawSquare(40, 16, 50, 20);
    DrawSquare(60, 16, 69, 20);

    DrawSquare(1, 28, 15, 32);
    DrawSquare(25, 28, 45, 32);
    DrawSquare(55, 28, 69, 32);



    GotoXY(80, 3); cout << "CROSSING ROAD!!!";
    GotoXY(80, 8); cout << "Level: " << SPEED;;
    GotoXY(73, 11); cout << "Please Pause before Load/Save game!";
    GotoXY(80, 12); cout << "P: Pause/Resume";
    GotoXY(80, 13); cout << "L: Save";
    GotoXY(80, 14); cout << "K: Load";
    GotoXY(80, 15); cout << "ESC: Return";
    GotoXY(80, 19); cout << "Control:";
    GotoXY(80, 20); cout << "[W]  :  UP";
    GotoXY(80, 21); cout << "[S]  :  DOWN";
    GotoXY(80, 22); cout << "[A]  :  LEFT";
    GotoXY(80, 23); cout << "[D]  :  RIGHT";

    GotoXY(curPosX, curPosY);
}

//Băt đầu game
void StartGame()
{

    system("cls");
    ResetData();
    DrawBoard(0, 0, WIDTH_CONSOLE + 40, HEIGH_CONSOLE);
    DrawTrafficLights(WIDTH_CONSOLE + 2, 6);
    STATE = true;
}

//Dọn dẹp tài nguyên
void GabageCollect()
{
    for (int i = 0; i < MAX_CAR; i++)
    {
        delete[] X[i];
    }
    delete[] X;
    for (int i = 0; i < WIDTH_CONSOLE; i++) {
        delete[] C[i];
    }
    delete[] C;
    delete[] S;
}
//Thoát game
void ExitGame(HANDLE t)
{
    system("cls");
    TerminateThread(t, 0);
    GabageCollect();
}
//Dừng xe
void PauseGame(HANDLE t)
{
    SuspendThread(t);
}

//Xử lý người đụng xe
void ProcessDead()
{
    STATE = 0;
}

void ProcessFinish(POINT& p)
{
    S[max(p.x - 2, 1)] = S[p.x - 1] = S[p.x] = S[p.x + 1] = S[p.x + 2] = false;
    SPEED == MAX_SPEED ? SPEED = 1 : SPEED++;
    p = { 35,37 };
    MOVING = 'D';
    DrawBoard(0, 0, WIDTH_CONSOLE + 40, HEIGH_CONSOLE);

}

//Vẽ xe
void DrawCars(const char* s)
{
    for (int k = 0; k < MAX_CAR - 1; k++) {
        if (k >= 3 && k <= 5 || k >= 9 && k <= 11) {
            //Row1
            if (k % 3 == 0) {
                for (int i = 0; i < 8; i++) {
                    C[X[k][i].x][X[k][i].y] = true;
                    GotoXY(X[k][i].x, X[k][i].y);
                    if (i == 0 || i == 1 || i == 7)
                        cout << " ";
                    else
                        cout << "_";
                }
            }
            //Row2
            if (k % 3 == 1) {
                for (int i = 0; i < 8; i++) {
                    GotoXY(X[k][i].x, X[k][i].y);
                    if (i == 1) cout << "/";
                    if (i == 2)	cout << char(220);
                    if (i == 7)	cout << char(92);
                    else cout << " ";
                    C[X[k][i].x][X[k][i].y] = true;
                }
            }
            //Row3
            if (k % 3 == 2) {
                for (int i = 0; i < 8; i++) {
                    GotoXY(X[k][i].x, X[k][i].y);
                    if (i == 7) cout << char(240);
                    else
                        if (i == 6 || i == 2) cout << "0";
                        else
                            if (i == 0) cout << char(192);
                            else cout << "-";
                    C[X[k][i].x][X[k][i].y] = true;
                }
            }
        }
        else {
            //Row1
            if (k % 3 == 0) {
                for (int i = 0; i < 8; i++) {
                    C[X[k][i].x][X[k][i].y] = true;
                    GotoXY(X[k][i].x, X[k][i].y);
                    if (i == 0 || i == 6 || i == 7 || i == 8)
                        cout << " ";
                    else
                        cout << "_";
                }
            }
            //Row2
            if (k % 3 == 1) {
                for (int i = 0; i < 8; i++) {
                    GotoXY(X[k][i].x, X[k][i].y);
                    if (i == 0) cout << "/";
                    if (i == 5)	cout << char(220);
                    if (i == 6)	cout << char(92);
                    else cout << " ";
                    C[X[k][i].x][X[k][i].y] = true;
                }
            }
            //Row3
            if (k % 3 == 2) {
                for (int i = 0; i < 8; i++) {
                    GotoXY(X[k][i].x, X[k][i].y);
                    if (i == 0) cout << char(240);
                    else
                        if (i == 1 || i == 5) cout << "0";
                        else
                            if (i == 7) cout << char(217);
                            else cout << "-";
                    C[X[k][i].x][X[k][i].y] = true;
                }
            }
        }
    }
}
//Vẽ người qua đường
void ClrSticker(const POINT& p) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            GotoXY(p.x + i, p.y + j);
            cout << " ";
        }
    }
}

void DrawSticker(const POINT& p) {
    //Row1
    for (int i = 0; i < 3; i++) {
        GotoXY(p.x + i, p.y);
        if (i == 1) cout << char(64);
        else cout << " ";
    }
    //Row2
    for (int i = 0; i < 3; i++) {
        GotoXY(p.x + i, p.y + 1);
        if (i == 0) cout << "/";
        if (i == 1) cout << "|";
        if (i == 2) cout << char(92);
    }
    //Row3
    for (int i = 0; i < 3; i++) {
        GotoXY(p.x + i, p.y + 2);
        if (i == 0) cout << "/";
        if (i == 1) cout << " ";
        if (i == 2) cout << char(92);
    }
}

void MoveCars(int a, int b) {
    for (int i = 0; i < MAX_CAR - 1; i++)
    {
        if (i == 3) i += 3;
        if (i == 9) i += 3;
        if (i < b && i >= a) {
            continue;
        }
        cnt = 0;
        do {
            cnt++;
            for (int j = 0; j < MAX_CAR_LENGTH - 1; j++) {
                X[i][j] = X[i][j + 1];
            }
            if (X[i][MAX_CAR_LENGTH - 1].x + 2 == WIDTH_CONSOLE) {
                X[i][MAX_CAR_LENGTH - 1].x = 1;
            }

            else  X[i][MAX_CAR_LENGTH - 1].x++;
        } while (cnt < SPEED);
    }
    for (int i = 3; i < 12; i++)
    {
        if (i == 6) i += 3;
        if (i < b&& i >= a) {
            continue;
        }
        cnt = 0;
        do {
            cnt++;
            for (int j = MAX_CAR_LENGTH - 1; j > 0; j--)
            {
                X[i][j] = X[i][j - 1];
            }
            X[i][0].x - 1 == 0 ? X[i][0].x = WIDTH_CONSOLE - 2 : X[i][0].x--;
        } while (cnt < SPEED);
    }
}

//Xóa xe
void EraseCars()
{
    for (int i = 3; i < 12; i++) {
        if (i == 6) i += 3;
        cnt = 0;
        do {
            GotoXY(X[i][MAX_CAR_LENGTH - 1 - cnt].x, X[i][MAX_CAR_LENGTH - 1 - cnt].y);
            C[X[i][MAX_CAR_LENGTH - 1 - cnt].x][X[i][MAX_CAR_LENGTH - 1 - cnt].y] = false;
            printf(" ");
            cnt++;
        } while (cnt < SPEED);
    }
    for (int i = 0; i < MAX_CAR - 1; i++) {
        cnt = 0;
        do {
            GotoXY(X[i][0 + cnt].x, X[i][0 + cnt].y);
            C[X[i][0 + cnt].x][X[i][0 + cnt].y] = false;
            printf(" ");
            cnt++;
        } while (cnt < SPEED);
    }
}

void MoveRight() {
    if (Y.x < WIDTH_CONSOLE - 3)
    {
        if (Y.y == 17)
        {
            if (Y.x == 17 || Y.x == 37 || Y.x == 57)
            {
            }
            else
            {
                ClrSticker(Y);
                Y.x++;
                DrawSticker(Y);

            }
        }
        else if (Y.y == 29)
        {
            if (Y.x == 22 || Y.x == 52) {}
            else
            {
                ClrSticker(Y);
                Y.x++;
                DrawSticker(Y);

            }
        }
        else
        {
            ClrSticker(Y);
            Y.x++;
            DrawSticker(Y);

        }

    }
}
void MoveLeft() {
    if (Y.x > 1)
    {
        if (Y.y == 29)
        {
            if (Y.x == 16 || Y.x == 46)
            {
            }
            else
            {
                ClrSticker(Y);
                Y.x--;
                DrawSticker(Y);

            }
        }
        else if (Y.y == 17)
        {
            if (Y.x == 11 || Y.x == 31 || Y.x == 51)
            {
            }
            else
            {
                ClrSticker(Y);
                Y.x--;
                DrawSticker(Y);

            }
        }
        else
        {
            ClrSticker(Y);
            Y.x--;
            DrawSticker(Y);

        }
    }
}
void MoveDown() {
    if (Y.y < HEIGH_CONSOLE - 3)
    {
        if (Y.y == 13)
        {
            if ((1 <= Y.x && Y.x <= 10) || (18 <= Y.x && Y.x <= 30) || (38 <= Y.x && Y.x <= 50) || (58 <= Y.x && Y.x <= 69))
            {

            }
            else
            {
                ClrSticker(Y);
                Y.y += 4;
                DrawSticker(Y);

            }
        }

        else if (Y.y == 25)
        {
            if ((1 <= Y.x && Y.x <= 15) || (23 <= Y.x && Y.x <= 45) || (53 <= Y.x && Y.x <= 69))
            {

            }
            else
            {
                ClrSticker(Y);
                Y.y += 4;
                DrawSticker(Y);

            }
        }

        else
        {
            ClrSticker(Y);
            Y.y += 4;
            DrawSticker(Y);

        }

    }
}
void MoveUp()
{
    if (Y.y > 1)
    {
        if (Y.y - 4 == 1) {
            if (S[Y.x] == true) {
                ClrSticker(Y);
                Y.y -= 4;
                DrawSticker(Y);
            }
        }
        else {
            if (Y.y == 21)
            {
                if ((1 <= Y.x && Y.x <= 10) || (18 <= Y.x && Y.x <= 30) || (38 <= Y.x && Y.x <= 50) || (58 <= Y.x && Y.x <= 69))
                {

                }
                else
                {
                    ClrSticker(Y);
                    Y.y -= 4;
                    DrawSticker(Y);
                }
            }

            else if (Y.y == 33)
            {
                if ((1 <= Y.x && Y.x <= 15) || (23 <= Y.x && Y.x <= 45) || (53 <= Y.x && Y.x <= 69))
                {

                }
                else
                {
                    ClrSticker(Y);
                    Y.y -= 4;
                    DrawSticker(Y);
                }
            }

            else
            {
                ClrSticker(Y);
                Y.y -= 4;
                DrawSticker(Y);
            }
        }
    }
}
//Cài đặt cho tiểu trình
void endGame() {
    SetColor(14);
    GotoXY(10, 5); cout << "                ...                                       /^^^^         /^       /^^       /^^/^^^^^^^^       " << endl;
    GotoXY(10, 6); cout << "              ;::::;                                    /^    /^^      /^ ^^     /^ /^^   /^^^/^^       " << endl;
    GotoXY(10, 7); cout << "            ;::::; :;                                  /^^            /^  /^^    /^^ /^^ / /^^/^^            " << endl;
    GotoXY(10, 8); cout << "          ;:::::'   :;                                 /^^           /^^   /^^   /^^  /^^  /^^/^^^^^^           " << endl;
    GotoXY(10, 9); cout << "         ;:::::;     ;.                                /^^   /^^^^  /^^^^^^ /^^  /^^   /^  /^^/^^          " << endl;
    GotoXY(10, 10); cout << "        ,:::::'       ;           OOO" << char(92) << "                  /^^    /^  /^^       /^^ /^^       /^^/^^     " << endl;
    GotoXY(10, 11); cout << "        ::::::;       ;          OOOOO" << char(92) << "                  /^^^^^   /^^         /^^/^^       /^^/^^^^^^^^ " << endl;
    GotoXY(10, 12); cout << "        ;:::::;       ;         OOOOOOOO       " << endl;
    GotoXY(10, 13); cout << "       ,;::::::;     ;'         / OOOOOOO      " << endl;
    GotoXY(10, 14); cout << "     ;:::::::::`. ,,,;.        /  / DOOOOOO    " << endl;
    GotoXY(10, 15); cout << "   .';:::::::::::::::::;,     /  /     DOOOO   " << endl;
    GotoXY(10, 16); cout << "  ,::::::;::::::;;;;::::;,   /  /        DOOO  " << endl;
    GotoXY(10, 17); cout << " ;`::::::`'::::::;;;::::: ,#/  /          DOOO            /^^^^     /^^         /^^/^^^^^^^^/^^^^^^^ " << endl;
    GotoXY(10, 18); cout << " :`:::::::`;::::::;;::: ;::#  /            DOOO         /^^    /^^   /^^       /^^ /^^      /^^    /^^" << endl;
    GotoXY(10, 19); cout << " ::`:::::::`;:::::::: ;::::# /              DOO       /^^        /^^  /^^     /^^  /^^      /^^    /^^" << endl;
    GotoXY(10, 20); cout << " `:`:::::::`;:::::: ;::::::#/               DOO       /^^        /^^   /^^   /^^   /^^^^^^  /^ /^^" << endl;
    GotoXY(10, 21); cout << "  :::`:::::::`;; ;:::::::::##                OO       /^^        /^^    /^^ /^^    /^^      /^^  /^^" << endl;
    GotoXY(10, 22); cout << "  ::::`:::::::`;::::::::;:::#                OO         /^^     /^^      /^^^^     /^^      /^^    /^^" << endl;
    GotoXY(10, 23); cout << "  `:::::`::::::::::::;'`:;::#                O            /^^^^           /^^      /^^^^^^^^/^^      /^^" << endl;
    GotoXY(10, 24); cout << "  `:::::`::::::::; ' /  / `:#                  " << endl;
    GotoXY(10, 25); cout << "   ::::::`:::::; '  /  /   `#                  " << endl;
    SetColor(15);
    return;
}
void SubThread()
{
    time_t current_time;
    current_time = time(NULL);
    time_t current_time1;
    long z;
    int f;
    int a, b;

    q = q % 6;
    while (1) {
        if (STATE)
        {
            switch (MOVING)
            {
            case 'A':
                MoveLeft();
                break;
            case 'D':
                MoveRight();
                break;
            case 'W':
                MoveUp();
                break;
            case 'S':
                MoveDown();
                break;
            }
            MOVING = ' ';
            current_time1 = time(NULL);
            z = current_time1 - current_time;


            if (z % 8 >= 5 && z % 8 <= 7) {
                if (z % 8 >= 5 && z % 8 <= 7 && q == 0) {
                    f = 0;
                    a = 0;
                    b = 3;

                }
                if (z % 8 >= 5 && z % 8 <= 7 && q == 1) {
                    f = 1;
                    a = 3;
                    b = 6;

                }
                if (z % 8 >= 5 && z % 8 <= 7 && q == 2) {
                    f = 2;
                    a = 6;
                    b = 9;

                }
                if (z % 8 >= 5 && z % 8 <= 7 && q == 3) {
                    f = 4;
                    a = 9;
                    b = 12;

                }
                if (z % 8 >= 5 && z % 8 <= 7 && q == 4) {
                    f = 5;
                    a = 12;
                    b = 15;

                }
                if (z % 8 >= 5 && z % 8 <= 7 && q == 5) {
                    f = 7;
                    a = 15;
                    b = 18;

                }

                DrawTrafficLights(WIDTH_CONSOLE + 2, 6);
                SetColor(4);
                GotoXY(WIDTH_CONSOLE + 2, (f * 4 + 6));

                cout << char(254) << endl;
                SetColor(15);
                EraseCars();
                MoveCars(a, b);
                DrawCars(".");
            }
            else {
                DrawTrafficLights(WIDTH_CONSOLE + 2, 6);
                EraseCars();
                MoveCars(-1, -1);
                DrawCars(".");
                q = rand() % 6;
            }

            if (Y.y == 1)
                ProcessFinish(Y);
            else {
                if (C[Y.x][Y.y] == true)
                {
                    ProcessDead();
                    Sleep(50);
                    system("cls");
                    Sleep(1000);
                    endGame();
                    Sleep(1000);
                    system("cls");
                    GotoXY(50, 20);
                    cout << "1. Type Y to return Menu." << endl;
                    GotoXY(50, 21);
                    cout << "2. Type another words to exit this game.";
                }
            }
            Sleep(50);
        }
    }
}
void Clean()
{

    int i = 30;
    while (i < HEIGH_CONSOLE) {
        GotoXY(WIDTH_CONSOLE + 1, i); for (int j = 0; j < 40 - 1; j++) cout << " ";
        i += 1;
    }
}
void LoadingBar() {
    float progress = 0.0;
    while (progress < 1.0) {
        Sleep(100);
        int barWidth = 70;
        GotoXY(15, 17);
        cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << " ";
        }
        cout << "] " << 100 << " %\r";
        cout.flush();
        progress += 0.16;
    }
}
void clrscr()
{
    CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
    HANDLE	hConsoleOut;
    COORD	Home = { 0,0 };
    DWORD	dummy;

    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

    FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
    csbiInfo.dwCursorPosition.X = 0;
    csbiInfo.dwCursorPosition.Y = 0;
    SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}
void SaveGame()
{
    Clean();
    ofstream fout;
    char a = 'a';
    string name = "";
    GotoXY(WIDTH_CONSOLE + 6, 30); cout << "Pls enter your save file name: ";

    while (a != 27)
    {
        GotoXY(WIDTH_CONSOLE + 14, 32); cout << name << "                    ";
        a = _getch();
        if (a == 127 && name.size() > 0) name.pop_back();
        else if (a == 13)
        {

            fout.open("SaveFile.txt", std::ios_base::app);
            fout << name << endl << SPEED << endl;
            fout.close();

            GotoXY(WIDTH_CONSOLE + 14, 34); cout << "Save completely!!";
            Sleep(2000);
            break;
        }
        else if (a != 27) name.push_back(a);
    }

    Clean();
}
void SaveGameMenu()
{
    Clean();
    ofstream fout;
    char a = 'a';
    string name = "";
    GotoXY(50, 20); cout << "Pls enter your save file name: ";

    while (a != 27)
    {
        GotoXY(50, 22); cout << name << "                    ";
        a = _getch();
        if (a == 127 && name.size() > 0) name.pop_back();
        else if (a == 13)
        {

            fout.open("SaveFile.txt", std::ios_base::app);
            fout << name << endl << SPEED << endl;
            fout.close();

            GotoXY(50, 24); cout << "Save completely!!";
            Sleep(2000);
            break;
        }
        else if (a != 27) name.push_back(a);
    }

    Clean();
}

void LoadGameingame()
{
    Clean();
    fstream ifs("SaveFile.txt");
    GotoXY(74, 30);
    cout << "There are those save file:" << endl;
    int i = 31;
    while (!ifs.eof())
    {

        char temp[255];
        ifs.getline(temp, 255);
        string line = temp;
        GotoXY(80, i);
        if (i % 2 == 0)
        {
            GotoXY(94, i - 1);

        }

        cout << line;
        i++;

    };

    ifs.close();
    char choice;
    choice = _getch();
    if (choice == 'p' || choice == 'P') Clean();
}
void LoadGameMenu()
{
    fstream ifs("SaveFile.txt");
    GotoXY(50, 20);
    cout << "There are those save file:" << endl;
    int i = 21;
    while (!ifs.eof())
    {

        char temp[255];
        ifs.getline(temp, 255);
        string line = temp;
        GotoXY(50, i);
        if (i % 2 == 0)
        {
            GotoXY(64, i - 1);

        }

        cout << line;
        i++;
    };

    ifs.close();
}


void Choose(int choice)
{
    switch (choice)
    {
    case '1':
        // system("cls");
        StartGame();
        // rest of code here
        break;
    case '2':
        LoadGameMenu();
        // rest of code here
        break;
    case '3':
        cout << "Ahahah, you really think I will help you?\n";
        system("cls");
        Sleep(1000);
        ProcessDead();
        // rest of code here
        break;
    case '4':
        system("cls");
        //Instruction();
        //ExitGame(t1.native_handle());
        break;
    default:
        cout << "Not a Valid Choice. \n";
        break;
    }
}
int main()
{
    ShowCur(0);
    FixConsoleWindow();
    system("mode 650");
    srand(time(NULL));
    int temp;
    char choice = '1';
    LoadingBar();

    while (choice != '4')
    {
        clrscr();
        MenuGame();
        choice = _getch();
        if (choice == '1')
        {
            StartGame();
            thread t1(SubThread);
            while (1)
            {
                temp = toupper(_getch());
                if (STATE == 1)
                {
                    if (temp == 27)
                    {
                        ExitGame(t1.native_handle());
                        return 1;
                    }
                    else if (temp == 'P')
                    {
                        PauseGame(t1.native_handle());
                    }
                    else if (temp == 'L') {
                        SaveGame();
                    }
                    else if (temp == 'K') {
                        LoadGameingame();
                    }
                    else
                    {
                        ResumeThread((HANDLE)t1.native_handle());
                        if (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S')
                        {
                            MOVING = temp;
                        }
                    }
                }
                else
                {
                    if (temp == 'Y') {
                        system("cls");
                        MenuGame();
                        choice = _getch();
                        if (choice == '4') {
                            ExitGame(t1.native_handle());
                            return 1;
                        }
                        else {
                            Choose(choice);
                        }
                    }
                    else
                    {

                        ExitGame(t1.native_handle());
                        return 1;
                    }
                    //continue;
                }
            }
        }
        else if (choice == '2')
        {
            LoadGameMenu();
            Sleep(5000);
        }
        else if (choice == '3')
        {
            SaveGameMenu();
        }

    }
    return 0;
}
