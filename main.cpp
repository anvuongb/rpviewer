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
  const int screenWidth = 800;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "Research Paper Viewer");

  // File explorer explorerConfig
  FileExplorerConfig explorerConfig = {
      .font = LoadFont("fonts/CascadiaMono.ttf"),
      .spacing = 1,
      .fontSize = 20,
      .scrollSpeed = 1,
      .folderColor = DARKBLUE,
      .fileColor = DARKPURPLE};
  // End File explorer explorerConfig

  ImageConfig imageConfig = {
      .targetWidth = 200,
  };
  imageConfig.posImg.x = 400;
  imageConfig.posImg.y = 5;
  imageConfig.screenHeight = screenHeight;
  imageConfig.screenWidth = screenWidth;

  Vector2 posText = {.x = 5, .y = 0};
  Vector2 posMouse = {0};
  int scrollOffset = 0;
  int filesCount = 0;
  int mouseFileIdx = 0;

  SetTargetFPS(60);

  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    // Handle fontsize
    handle_font_size(explorerConfig);

    // Handle click
    handle_click(explorerConfig, imageConfig, path, posText, posMouse, filesCount,
                 mouseFileIdx, scrollOffset);

    // Handle scroll
    handle_scroll(explorerConfig, scrollOffset, filesCount, mouseFileIdx);

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(RAYWHITE);

    draw_file_explorer(explorerConfig, path, posText, posMouse, mouseFileIdx,
                       scrollOffset);

    draw_load_image(imageConfig);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}