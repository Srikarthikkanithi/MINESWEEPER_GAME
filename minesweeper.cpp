#include <bits/stdc++.h>
using namespace std;
#define EASYMODESIZE 9
#define EASYMINESIZE 10
#define MEDMODESIZE 16
#define MEDMINESIZE 40
#define HARDMODESIZE 24
#define HARDMINESIZE 99
class Game;
class Board
{
private:
    vector<vector<char>> Cells;
    int size, minesize;
    vector<pair<int, int>> mines;

public:
    Board() = default;
    Board(int s, int ms)
    {
        Cells.resize(s, vector<char>(s, '-'));
        minesize = ms;
        size = s;
    }
    bool IsMine(int x, int y)
    {
        return Cells[x][y] == '*';
    }
    bool isValid(int x, int y)
    {
        return x >= 0 && x < size && y >= 0 && y < size;
    }
    void Replacemine(int x, int y)
    {
        Cells[x][y] = '-';
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (Cells[i][j] != '*')
                {
                    Cells[i][j] = '*';
                    break;
                }
            }
        }
    }
    int Countadjacentmines(int x, int y)
    {
        vector<int> dx = {-1, -1, -1, 0, 0, 1, 1, 1};
        vector<int> dy = {-1, 0, 1, -1, 1, -1, 0, 1};
        int count = 0;
        for (int i = 0; i < 8; i++)
        {
            if (isValid(x + dx[i], y + dy[i]) && IsMine(x + dx[i], y + dy[i]))
            {
                count++;
            }
        }
        return count;
    }
    void Createmines()
    {
        vector<bool> created(size * size, 0);
        for (int i = 0; i < minesize;)
        {
            int rand_num = rand() % (size * size);
            int x = rand_num / size, y = rand_num % size;
            if (created[rand_num] == 0)
            {
                Cells[x][y] = '*';
                mines.push_back({x, y});
                created[rand_num] == 1;
                i++;
            }
        }
    }
    void PrintBoard()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                cout << Cells[i][j] << " ";
            }
            cout << endl;
        }
    }
    pair<int, int> take_input()
    {
        cout << "Enter your move :\n";
        int x, y;
        cin >> x >> y;
        return {x, y};
    }
    friend class Game;
};
class Game
{
private:
    int difficulty_level;
    Board GameBoard, RealBoard;
    int Boardsize;
    int moveno;
    bool gameover;
    vector<pair<int, int>> moves;
    int movesleft;

public:
    Game() = default;
    Game(int d)
    {
        if (d == 0)
        {
            GameBoard = Board(EASYMODESIZE, EASYMINESIZE);
            RealBoard = Board(EASYMODESIZE, EASYMINESIZE);
            Boardsize = EASYMODESIZE;
        }
        else if (d == 1)
        {
            GameBoard = Board(MEDMODESIZE, MEDMINESIZE);
            RealBoard = Board(MEDMODESIZE, MEDMINESIZE);
            Boardsize = MEDMODESIZE;
        }
        else
        {
            GameBoard = Board(HARDMODESIZE, HARDMINESIZE);
            RealBoard = Board(HARDMODESIZE, HARDMINESIZE);
            Boardsize = HARDMODESIZE;
        }
        RealBoard.Createmines();
        moveno = 1;
        gameover = 0;
        movesleft = GameBoard.size * GameBoard.size - GameBoard.minesize;
    }
    Game Start_Stage1_Game()
    {
        int d;
        cout << "Enter the difficulty level :\n";
        cout << "Press 0 for EASY (9 * 9 Cells and 10 Mines)\n";
        cout << "Press 1 for MEDIUM (16* 16 Cells and 40 Mines)\n";
        cout << "Press 2 for HARD (24* 24 Cells and 99 Mines)\n";
        cin >> d;
        return Game(d);
    }
    bool Playauto(int x, int y, int &movesleft)
    {
        if (GameBoard.Cells[x][y] != '-')
        {
            return false;
        }
        if (RealBoard.Cells[x][y] == '*')
        {
            GameBoard.Cells[x][y] = '*';
            for (int i = 0; i < GameBoard.minesize; i++)
            {
                GameBoard.Cells[RealBoard.mines[i].first][RealBoard.mines[i].second] = '*';
            }
            GameBoard.PrintBoard();
            cout << "You Lost!\n";
            return 1;
        }
        else
        {
            int count = RealBoard.Countadjacentmines(x, y);
            movesleft--;
            GameBoard.Cells[x][y] = count + '0';
            if (!count)
            {
                int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
                int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
                for (int i = 0; i < 8; i++)
                {
                    int X = x + dx[i], Y = y + dy[i];
                    if (GameBoard.isValid(X, Y))
                    {
                        if (!RealBoard.IsMine(X, Y))
                        {
                            Playauto(X, Y, movesleft);
                        }
                    }
                }
            }
            return 0;
        }
    }
    void Start_Stage2_Game()
    {
        while (gameover == 0)
        {
            cout << "Present state of the board :\n";
            GameBoard.PrintBoard();
            pair<int, int> p1 = GameBoard.take_input();
            int x = p1.first, y = p1.second;
            moves.push_back({x, y});
            if (moveno == 1)
            {
                if (GameBoard.IsMine(x, y))
                {
                    GameBoard.Replacemine(x, y);
                }
            }
            moveno++;
            gameover = Playauto(x, y, movesleft);
            if (gameover == 0 && movesleft == 0)
            {
                cout << "You Won!!!!" << endl;
                gameover = 1;
            }
        }
    }
};
int main()
{
    Game g1;
    g1 = g1.Start_Stage1_Game();
    g1.Start_Stage2_Game();
}