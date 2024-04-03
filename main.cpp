#include "raylib.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <iterator>
namespace fs = std::filesystem;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

    std::string path = "/home/anvuong/Desktop";

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    Font font = LoadFont("fonts/CascadiaMono.ttf");
    float spacing = 1;
    float fontSize = 20;
    int scrollSpeed = 1;
    int scrollOffset = 0;
    Color folderColor = DARKBLUE;
    Color fileColor = DARKPURPLE;
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    Vector2 pos = {.x = 5, .y = 0};
    Vector2 mousePos = {0};
    std::string currentFile;
    fs::path currentPath;
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        pos.y = 0;
        mousePos = GetMousePosition();
        auto fi = fs::directory_iterator(path);
        int filesCount = std::distance(fi, fs::directory_iterator{});
        int mouseFileIdx = (mousePos.y + scrollOffset * fontSize) / fontSize;
        std::cout << "file count = " << filesCount << "\n";
        std::cout << "mouse pos = " << mousePos.y << " mouse index = " << mouseFileIdx << "\n";
        int i = 0;
        std::cout << "current dir = " << path << "\n";
        // onclick change directory
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (mouseFileIdx > 0 && mouseFileIdx <= filesCount)
            {
                fi = fs::directory_iterator(path);
                for (int j = 0; j < mouseFileIdx - 1; j++)
                {
                    fi++;
                }
                if (fi->is_directory())
                {
                    path = fi->path().string();
                    scrollOffset = 0;
                    mouseFileIdx = (mousePos.y + scrollOffset * fontSize) / fontSize;
                }
            }

            else
            {
                path = currentPath.parent_path().parent_path();
                scrollOffset = 0;
                mouseFileIdx = (mousePos.y + scrollOffset * fontSize) / fontSize;
            }
        }

        // onMouseScroll sift through the list
        scrollOffset += (GetMouseWheelMove() * scrollSpeed);
        std::cout << "scrollOffset = " << scrollOffset << "\n";
        if (scrollOffset < 0)
            scrollOffset = 0;
        if (scrollOffset > filesCount)
            scrollOffset = filesCount;

        fi = fs::directory_iterator(path);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextEx(font, "..", pos, fontSize, spacing, BLACK);
        if (mouseFileIdx == i)
        {
            DrawTextEx(font, "..", pos, fontSize, spacing, RED);
        }
        pos.y += fontSize;
        i++;
        for (const auto &entry : fi)
        {
            if (i > scrollOffset)
            {
                currentFile = entry.path().filename().string();
                currentPath = entry.path();
                if (entry.is_directory())
                    DrawTextEx(font, currentFile.c_str(), pos, fontSize, spacing, folderColor);
                else
                    DrawTextEx(font, currentFile.c_str(), pos, fontSize, spacing, fileColor);

                if (mouseFileIdx == i)
                    DrawTextEx(font, currentFile.c_str(), pos, fontSize, spacing, RED);

                pos.y += fontSize;
            }
            i++;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}