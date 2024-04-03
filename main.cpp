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

    InitWindow(screenWidth, screenHeight, "Research Paper Viewer");
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
    Vector2 textSize = {0};
    std::string currentFile;
    std::string currentSelectingFile;
    fs::path currentPath;
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        pos.y = fontSize * 2; // leave 2 units space for system
        mousePos = GetMousePosition();
        // TODO: handle permission error when open directory
        auto fi = fs::directory_iterator(path);
        int filesCount = std::distance(fi, fs::directory_iterator{});
        int mouseFileIdx = (mousePos.y - pos.y + scrollOffset * fontSize) / fontSize;
        std::cout << "file count = " << filesCount << "\n";
        std::cout << "mouse pos x = " << mousePos.x << " mouse index = " << mouseFileIdx << "\n";
        int i = 0;
        std::cout << "current dir = " << path << "\n";
        std::cout << "textSize x = " << textSize.x << "\n";
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
                    // check if actually click within the filename
                    std::string tmpPath = fi->path().filename().string();
                    if (tmpPath.length() >= 20)
                    {
                        tmpPath = tmpPath.substr(0, 17) + "...";
                    }
                    Vector2 tmpTextSize = MeasureTextEx(font, currentSelectingFile.c_str(), fontSize, spacing);
                    // clicked on filename
                    if (mousePos.x < tmpTextSize.x)
                    {
                        path = fi->path().string();
                        scrollOffset = 0;
                        mouseFileIdx = (mousePos.y + scrollOffset * fontSize) / fontSize;
                    }
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
        scrollOffset -= (GetMouseWheelMove() * scrollSpeed);
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

        // Draw current path
        Vector2 posc = {.x = 10, .y = 0};
        DrawTextEx(font, path.c_str(), posc, fontSize, spacing, GRAY);

        // Draw going back path
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
                if (currentFile.length() > 20)
                {
                    currentFile = currentFile.substr(0, 17) + "...";
                }
                currentPath = entry.path();
                if (entry.is_directory())
                    DrawTextEx(font, currentFile.c_str(), pos, fontSize, spacing, folderColor);
                else
                    DrawTextEx(font, currentFile.c_str(), pos, fontSize, spacing, fileColor);
                textSize = MeasureTextEx(font, currentFile.c_str(), fontSize, spacing);
                if (mouseFileIdx == i && mousePos.x <= textSize.x)
                {
                    currentSelectingFile = currentFile;
                    DrawTextEx(font, currentFile.c_str(), pos, fontSize, spacing, RED);
                }

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