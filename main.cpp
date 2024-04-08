#include <file_explorer.hpp>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <string>

#include "raylib.h"
namespace fs = std::filesystem;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  Path path = {.current_path_s = "/home/anvuong/Desktop",
               .parent_path_s = "/home/anvuong"};

  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 1000;
  const int screenHeight = 1000;

  InitWindow(screenWidth, screenHeight, "Simple File Explorer");

  // File explorer explorerConfig
  FileExplorerConfig explorerConfig = {
      .font = LoadFont("fonts/CascadiaMono.ttf"),
      .spacing = 1,
      .fontSize = 20,
      .scrollSpeed = 1,
      .folderColor = DARKBLUE,
      .fileColor = DARKPURPLE};
  // End File explorer explorerConfig
  init_explorer(explorerConfig, path);

  ImageConfig imageConfig;
  imageConfig.screenHeight = screenHeight;
  imageConfig.screenWidth = screenWidth;
  imageConfig.captionSize = 10;

  Vector2 posText = {.x = 5, .y = 0};
  Vector2 posMouse = {0};
  int scrollOffset = 0;
  int filesCount = 0;
  int mouseFileIdx = 0;

  SetTargetFPS(60);

  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    // Handle fontsize
    handle_font_size(explorerConfig, imageConfig);

    // Handle click
    handle_click(explorerConfig, imageConfig, path, posText, posMouse,
                 filesCount, mouseFileIdx, scrollOffset);

    // Handle scroll
    handle_scroll(explorerConfig, scrollOffset, filesCount, mouseFileIdx);

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);

    draw_loaded_image(imageConfig, explorerConfig);

    draw_file_explorer(explorerConfig, imageConfig, path, posText, posMouse,
                       mouseFileIdx, scrollOffset);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}