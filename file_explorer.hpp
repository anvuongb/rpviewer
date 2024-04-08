#ifndef FILE_EXPLORER
#define FILE_EXPLORER_HPP

#include <raylib.h>

#include <filesystem>
#include <iostream>
#include <iterator>
#include <string>
using namespace std;

namespace fs = std::filesystem;

typedef struct {
  Font font;
  float spacing;
  float fontSize;
  int scrollSpeed;
  Color folderColor;
  Color fileColor;
} FileExplorerConfig;

typedef struct {
  fs::path current_path;
  fs::path parent_path;
  string current_path_s;
  string parent_path_s;
  string current_file;
  string current_selecting_file;
  string error;
} Path;

typedef struct {
  string imgPath;
  Texture texture;
  int targetWidth;
  Vector2 posImg;
  int screenWidth;
  int screenHeight;
  int captionSize;
} ImageConfig;

fs::directory_iterator get_directory(Path& p);

const char* check_directory_access(string p);

void init_explorer(FileExplorerConfig& config, Path& path);

void handle_font_size(FileExplorerConfig& config);

void handle_load_image(ImageConfig& conf);

void handle_open_pdf(string p);

void handle_scroll(const FileExplorerConfig& config, int& scrollOffset,
                   int& filesCount, int& mouseFileIdx);

void handle_click(const FileExplorerConfig& config, ImageConfig& imgConfig,
                  Path& path, Vector2& posText, Vector2& posMouse,
                  int& filesCount, int& mouseFileIdx, int& scrollOffset);

void draw_loaded_image(const ImageConfig& conf,
                       const FileExplorerConfig fileConf);

void draw_file_explorer(const FileExplorerConfig& config, Path& path,
                        Vector2& posText, Vector2& posMouse,
                        const int& mouseFileIdx, const int& scrollOffset);

#endif