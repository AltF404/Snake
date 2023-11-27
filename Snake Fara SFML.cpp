#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

using namespace std;

const int width = 20;  // Lățimea tablei de joc
const int height = 20; // Înălțimea tablei de joc

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

struct Snake {
    int x, y;
};

int fruitX, fruitY;
vector<Snake> snake;
Direction dir;
bool gameOver;
int score;

void Setup() {
    initscr(); // Inițializează ncurses
    clear();
    noecho(); // Nu afișa tastele apăsate
    curs_set(0); // Ascunde cursorul
    keypad(stdscr, TRUE); // Activează intrarea de la tastatură

    gameOver = false;
    dir = STOP;
    snake = {{width / 2, height / 2}}; // Inițializează șarpele în centrul tablei de joc
    score = 0;

    srand(time(0));
    fruitX = rand() % width; // Poziția aleatoare a fructului
    fruitY = rand() % height;
}

void Draw() {
    clear();

    // Desenează bordura
    for (int i = 0; i < width + 2; i++)
        printw("#");
    printw("\n");

    // Desenează zona de joc
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                printw("#");
            if (i == fruitY && j == fruitX)
                printw("F"); // Afiseaza fructul
            else {
                bool print = false;
                for (auto &k : snake) {
                    if (k.x == j && k.y == i) {
                        printw("O"); // Afiseaza sarpele
                        print = true;
                    }
                }
                if (!print)
                    printw(" "); // Spațiu gol
            }
            if (j == width - 1)
                printw("#");
        }
        printw("\n");
    }

    // Desenează bordura de jos
    for (int i = 0; i < width + 2; i++)
        printw("#");
    printw("\n");

    // Afișează scorul
    printw("Scor: %d\n", score);
    refresh();
}

void Input() {
    int c = getch(); // Obține input de la utilizator
    switch (c) {
        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        case KEY_UP:
            dir = UP;
            break;
        case KEY_DOWN:
            dir = DOWN;
            break;
        case 'x':
            gameOver = true; // Încheie jocul dacă se apasă 'x'
            break;
    }
}


    // Actualizează mișcarea șarpelui și logica jocului
   void Logic()
    {
    int prevX = snake[0].x;
    int prevY = snake[0].y;
    int prev2X, prev2Y;

    for (size_t i = 1; i < snake.size(); i++) {
        prev2X = snake[i].x;
        prev2Y = snake[i].y;
        snake[i].x = prevX;
        snake[i].y = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT:
            snake[0].x--;
            break;
        case RIGHT:
            snake[0].x++;
            break;
        case UP:
            snake[0].y--;
            break;
        case DOWN:
            snake[0].y++;
            break;
        default:
            break;
    }

    if (snake[0].x >= width)
        snake[0].x = 0;
    else if (snake[0].x < 0)
        snake[0].x = width - 1;

    if (snake[0].y >= height)
        snake[0].y = 0;
    else if (snake[0].y < 0)
        snake[0].y = height - 1;

    for (size_t i = 1; i < snake.size(); i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            gameOver = true;
    }

    if (snake[0].x == fruitX && snake[0].y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        Snake newBlock;
        newBlock.x = snake[snake.size() - 1].x;
        newBlock.y = snake[snake.size() - 1].y;
        snake.push_back(newBlock);
    }
}
    // Logica detaliată pentru mișcarea șarpelui, detectarea coliziunilor și mâncarea fructului


int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        napms(100); // Așteaptă 100 de milisecunde
    }
    endwin(); // Termină ncurses
    cout << "Joc terminat! Scor final: " << score << endl;
    return 0;
}