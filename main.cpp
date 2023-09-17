#include <Windows.h>
#include <iostream>
#include <string>
#include <chrono>

const int SCREEN_WIDTH = 120;
const int SCREEN_HEIGHT = 40;

const int INITIAL_X = 0;
const int INITIAL_Y = 10;

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

const int VELOCITY = 1;

int main() {
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    wchar_t* screen = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT];
    SetConsoleActiveScreenBuffer(h_console);
    DWORD dw_bytes_written = 0;

    uint32_t target = 0;
    for (int i = 0; i < SCREEN_WIDTH * (SCREEN_HEIGHT/2); i = i + 1) {
        if (rand() % 100 < 1 && (i / SCREEN_WIDTH) % 2 == 0 ) {
            screen[i] = L'*';
            target++;
        }
        else 
            screen[i] = L' ';
    }

    // screen[INITIAL_X + INITIAL_Y * SCREEN_WIDTH] = L'@';

    screen[SCREEN_WIDTH * (SCREEN_HEIGHT) - 1] = '\0';

    WriteConsoleOutputCharacter(h_console, (LPCSTR) screen, SCREEN_WIDTH * (SCREEN_HEIGHT+1), { 0,0 }, &dw_bytes_written);
    direction direction = RIGHT;
    int current_x = INITIAL_X;
    int current_y = INITIAL_Y;

    uint32_t no_of_eaten = 0;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    while (target > no_of_eaten) {
        if (GetAsyncKeyState(VK_UP)) {
            direction = UP;
        }
        else if (GetAsyncKeyState(VK_DOWN)) {
            direction = DOWN;
        }
        else if (GetAsyncKeyState(VK_LEFT)) {
            direction = LEFT;
        }
        else if (GetAsyncKeyState(VK_RIGHT)) {
            direction = RIGHT;
        }

        if (direction == LEFT) {
            current_x -= VELOCITY;
        }
        else if (direction == RIGHT) {
            current_x += VELOCITY;
        }

        else if (direction == UP) {
            current_y -= VELOCITY;
        }
        else if (direction == DOWN) {
            current_y += VELOCITY;
        }

        if (current_x < 0) {
            direction = RIGHT;
            current_x = 0;
        } else if (current_x >= SCREEN_WIDTH) {
            direction = LEFT;
            current_x = SCREEN_WIDTH - 1;
        }

        if (current_y < 0) {
            direction = DOWN;
            current_y = 0;
        } else if (current_y >= (SCREEN_HEIGHT)/2) {
            direction = UP;
            current_y = (SCREEN_HEIGHT)/2;
        }
        
        if (screen[current_x + current_y * SCREEN_WIDTH] == L'*') {
            no_of_eaten++;
        }

        screen[current_x + current_y * SCREEN_WIDTH] = L'@';

        // display eaten number as the last line of the screen
        std::wstring eaten_string = L"Eaten: " + std::to_wstring(no_of_eaten);
        for (int i = 0; i < eaten_string.size(); i++) {
            screen[SCREEN_WIDTH * (SCREEN_HEIGHT/2) - eaten_string.size() + i] = eaten_string[i];
        }

        // display time elapsed as the last line of the screen
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::wstring time_elapsed_string = L"Time elapsed: " + std::to_wstring(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count()) + L" seconds";
        for (int i = 0; i < time_elapsed_string.size(); i++) {
            screen[SCREEN_WIDTH * (SCREEN_HEIGHT/2 - 1) + i] = time_elapsed_string[i];
        }

        WriteConsoleOutputCharacter(h_console, (LPCSTR) screen, SCREEN_WIDTH * (SCREEN_HEIGHT+1), { 0,0 }, &dw_bytes_written);
    
        screen[current_x + current_y * SCREEN_WIDTH] = L' ';

    

        Sleep(100);        
    }

    std::cout << "Game over!" << std::endl;
    std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin).count() << " seconds" <<  std::endl;
    std::cout << "Eaten: " << no_of_eaten << std::endl;


    return 0;
}