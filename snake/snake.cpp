// snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <array>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

static const char EMPTY_CHAR = ' ';
static const char SNAKE_CHAR = '*';
static const char FOOD_CHAR = '+';

// Adapted from:
// https://stackoverflow.com/questions/20303821/how-to-check-if-string-is-in-array-of-strings
bool in_array(const pair<int, int> value, const vector<pair<int, int>>& array)
{
    return find(array.begin(), array.end(), value) != array.end();
}

bool isKeyDown(int key) {
    return (GetKeyState(key) & 0x8000);
}

void draw(array<array<char, 15>, 15>& grid, int score) {
    system("cls");
    cout << "Your score: " << score << endl;
    cout << "-----------------" << endl;
    for (int i = 0; i < 15; i++) {
        cout << "|";
        for (int j = 0; j < 15; j++) {
            cout << grid[i][j];
        }
        cout << "|" << endl;
    }
    cout << "-----------------" << endl;
}

int main()
{   
    srand(time(NULL)); // Random seed for random numbers

    array<array<char, 15>, 15> grid;

    // Snake variables
    vector<pair<int, int>> tail;
    pair<int, int> currentPos (7, 7);
    int snakeXspeed = 1;
    int snakeYspeed = 0;
    int snakeSize = 1;

    // Food variables
    pair<int, int> foodPos ((rand() % 15), (rand() % 15)); // Create first food at random pos

    bool alive = true;

    do
    {
        // Clean grid
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                grid[i][j] = ' ';
            }
        }

        // Update speeds (movement)
        if (isKeyDown(VK_LEFT)) {
            snakeXspeed = (snakeXspeed != 1) ? -1 : 1; // Prevent snake from walking back into itself
            snakeYspeed = 0;
        } else if (isKeyDown(VK_RIGHT)) {
            snakeXspeed = (snakeXspeed != -1) ? 1 : -1;
            snakeYspeed = 0;
        } else if (isKeyDown(VK_UP)) {
            snakeXspeed = 0;
            snakeYspeed = (snakeYspeed != 1) ? -1 : 1;
        } else if (isKeyDown(VK_DOWN)) {
            snakeXspeed = 0;
            snakeYspeed = (snakeYspeed != -1) ? 1 : -1;
        }

        // Update tail
        tail.insert(tail.begin(), currentPos);
        if (tail.size() >= snakeSize) {
            tail.erase(tail.end() - 1);
        }

        // Move head
        currentPos.first = currentPos.first + snakeXspeed;
        currentPos.second = currentPos.second + snakeYspeed;

        if (currentPos.first > 14) {
            currentPos.first = 0;
        }
        if (currentPos.second > 14) {
            currentPos.second = 0;
        }
        if (currentPos.first < 0) {
            currentPos.first = 14;
        }
        if (currentPos.second < 0) {
            currentPos.second = 14;
        }

        // If there is food at new position eat it
        if (foodPos == currentPos) {
            snakeSize++;

            // Spawn new food but don't do it in a square ocupied by the snake

            // Create a vector to represent the whole snake
            vector<pair<int, int>> snakeVector = tail;
            snakeVector.insert(snakeVector.begin(), currentPos);
            do
            {
                foodPos = make_pair((rand() % 15), (rand() % 15)); // Spawn more food
            } while (in_array(foodPos, snakeVector));
        }

        // Update grid
        grid[currentPos.second][currentPos.first] = SNAKE_CHAR;
        grid[foodPos.second][foodPos.first] = FOOD_CHAR;
        for (int i = 0; i < tail.size(); i++) {
            pair<int, int> coords = tail[i];
            grid[coords.second][coords.first] = SNAKE_CHAR;
        }
        
        // Check for game over
        if (in_array(currentPos, tail)) {
            alive = false;
        }

        draw(grid, snakeSize-1);
        Sleep(200); // This prevents the snake from zooooming across the screen
    } while (alive);

    cout << "You died :(" << endl;
    cout << "Final score: " << snakeSize - 1 << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
