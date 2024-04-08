#include <raylib.h>

#include <file_explorer.hpp>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <string>

using namespace std;

namespace fs = std::filesystem;

void init_explorer(FileExplorerConfig& config, Path& path) {
  path.current_path = path.current_path_s;
  path.parent_path = path.parent_path_s;
}

fs::directory_iterator get_directory(Path& p) {
  fs::directory_iterator dir_iter;
  try {
    dir_iter = fs::directory_iterator(p.current_path_s);
  } catch (fs::filesystem_error& e) {
    // cout << "An exception occurred. Exception Nr. " << e.what() << '\n';
    p.error = e.what();
    return fs::directory_iterator(p.current_path_s);
  }
  return dir_iter;
}

const char* check_directory_access(string p) {
  try {
    (void)fs::directory_iterator(p);
  } catch (fs::filesystem_error& e) {
    return e.what();
  }
  return NULL;
}

void handle_font_size(FileExplorerConfig& config, ImageConfig& imgConf) {
  bool isChange = false;
  // increase fontSize
  if (IsKeyPressed(KEY_EQUAL)) {
    ++config.fontSize;
    isChange = true;
  }

  // decrease fontSize
  if (IsKeyPressed(KEY_MINUS)) {
    --config.fontSize;
    isChange = true;
  }

  // handle resize loaded image
  if (isChange && !imgConf.imgPath.empty()) {
    // remeasure scale
    string tmpText = "0123456789012345678901";
    Vector2 tmpTextSize = MeasureTextEx(config.font, tmpText.c_str(),
                                        config.fontSize, config.spacing);

    // update image draw region
    imgConf.imageRegionStart = Vector2{.x = tmpTextSize.x, .y = tmpTextSize.y};
    UnloadTexture(imgConf.texture);
    handle_load_image(imgConf);
    draw_loaded_image(imgConf, config);
  }
}

void handle_load_image(ImageConfig& conf) {
  cout << "conf.imageRegionStart.x = " << conf.imageRegionStart.x << "\n";
  Image image = LoadImage(conf.imgPath.c_str());
  int targetWidth = conf.screenWidth - conf.imageRegionStart.x - 10;
  ImageResize(&image, targetWidth,
              image.height / float(image.width) * targetWidth);

  conf.texture = LoadTextureFromImage(image);  // pass to GPU
  conf.posImg.x = (conf.screenWidth / 2 + conf.imageRegionStart.x / 2 -
                   conf.texture.width / 2);
  conf.posImg.y = (conf.screenHeight / 2 - conf.texture.height / 2 - 40);
  UnloadImage(image);  // remove from RAM
}

void handle_open_pdf(string p) {
  stringstream exec;
  exec << "evince "
       << "\"" << p << "\" &";
  // cout << "cmd = " << exec.str() << "\n";
  system(exec.str().c_str());
}

void handle_scroll(const FileExplorerConfig& config, int& scrollOffset,
                   int& filesCount, int& mouseFileIdx) {
  scrollOffset -= (GetMouseWheelMove() * config.scrollSpeed);
  if (scrollOffset < 0) scrollOffset = 0;
  if (scrollOffset > filesCount) scrollOffset = filesCount;
}

void handle_click(const FileExplorerConfig& config, ImageConfig& imgConfig,
                  Path& path, Vector2& posText, Vector2& posMouse,
                  int& filesCount, int& mouseFileIdx, int& scrollOffset) {
  posText.y = config.fontSize * 2;  // leave 2 units space for system
  posMouse = GetMousePosition();

  auto fi = get_directory(path);
  filesCount = std::distance(fi, fs::directory_iterator{});
  mouseFileIdx = (posMouse.y - posText.y + scrollOffset * config.fontSize) /
                 config.fontSize;

  // onclick change directory
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    if (mouseFileIdx > 0 && mouseFileIdx <= filesCount) {
      fi = get_directory(path);
      for (int j = 0; j < mouseFileIdx - 1; j++) {
        fi++;
      }
      std::string fn = fi->path().string();
      if (check_directory_access(fn) == NULL && fi->is_directory()) {
        // // unload texture if currently loaded
        // if (!imgConfig.imgPath.empty()) {
        //   imgConfig.imgPath = "";
        //   UnloadTexture(imgConfig.texture);
        // }
        // check if actually click within the filename
        std::string tmpPath = fi->path().filename().string();
        if (tmpPath.length() >= 20) {
          tmpPath = tmpPath.substr(0, 17) + "...";
        }
        Vector2 tmpTextSize =
            MeasureTextEx(config.font, path.current_selecting_file.c_str(),
                          config.fontSize, config.spacing);
        // clicked on filename
        if (posMouse.x < tmpTextSize.x) {
          path.current_path = fi->path();
          if (fi->path().has_parent_path()) {
            path.parent_path = fi->path().parent_path();
          }

          path.parent_path_s = path.current_path_s;
          path.current_path_s = fi->path().string();
          scrollOffset = 0;
          mouseFileIdx =
              (posMouse.y + scrollOffset * config.fontSize) / config.fontSize;
        }
      } else {
        std::string fn_ext = fn.substr(fn.find_last_of(".") + 1);
        if (fn_ext == "jpg" || fn_ext == "jpeg" || fn_ext == "png" ||
            fn_ext == "bmp") {
          // unload first
          if (!imgConfig.imgPath.empty()) {
            imgConfig.imgPath = "";
            UnloadTexture(imgConfig.texture);
          }
          imgConfig.imgPath = fn;
          handle_load_image(imgConfig);
        }

        if (fn_ext == "pdf") {
          handle_open_pdf(fn);
        }
      }
    }

    if (mouseFileIdx == 0 && posMouse.x < imgConfig.imageRegionStart.x) {
      // // unload texture if currently loaded
      // if (!imgConfig.imgPath.empty()) {
      //   imgConfig.imgPath = "";
      //   UnloadTexture(imgConfig.texture);
      // }

      path.current_path_s = path.current_path.parent_path().string();
      path.parent_path_s =
          path.current_path.parent_path().parent_path().string();
      path.current_path = path.current_path_s;
      path.parent_path = path.parent_path_s;
      scrollOffset = 0;
      mouseFileIdx =
          (posMouse.y + scrollOffset * config.fontSize) / config.fontSize;
    }
    std::cout << "current_path = " << path.current_path_s << "\n";
    std::cout << "current_parent = " << path.parent_path_s << "\n";
  }
}

void draw_loaded_image(const ImageConfig& conf,
                       const FileExplorerConfig fileConf) {
  if (conf.texture.id > 0 && !conf.imgPath.empty()) {
    DrawTexture(conf.texture, conf.posImg.x, conf.posImg.y, WHITE);

    Vector2 pos = conf.posImg;
    pos.y += conf.texture.height;
    DrawTextEx(fileConf.font, conf.imgPath.c_str(), pos, conf.captionSize,
               fileConf.spacing, GRAY);
  }
}

void draw_file_explorer(const FileExplorerConfig& config, ImageConfig& imgConf,
                        Path& path, Vector2& posText, Vector2& posMouse,
                        const int& mouseFileIdx, const int& scrollOffset) {
  // Draw vertical line
  string tmpText = "0123456789012345678901";
  Vector2 tmpTextSize = MeasureTextEx(config.font, tmpText.c_str(),
                                      config.fontSize, config.spacing);
  DrawLineV(Vector2{.x = tmpTextSize.x, .y = tmpTextSize.y},
            Vector2{.x = tmpTextSize.x, .y = float(imgConf.screenHeight)},
            GRAY);
  auto fi = get_directory(path);

  // update image draw region
  imgConf.imageRegionStart = Vector2{.x = tmpTextSize.x, .y = tmpTextSize.y};

  // Draw current path
  Vector2 posc = {.x = 10, .y = 0};
  DrawTextEx(config.font, path.current_path.string().c_str(), posc,
             config.fontSize, config.spacing, GRAY);
  int i = 0;
  // Draw going back path
  DrawTextEx(config.font, "..", posText, config.fontSize, config.spacing,
             BLACK);
  if (mouseFileIdx == i) {
    DrawTextEx(config.font, "..", posText, config.fontSize, config.spacing,
               RED);
  }
  posText.y += config.fontSize;
  i++;
  for (const auto& entry : fi) {
    if (i > scrollOffset) {
      path.current_file = entry.path().filename().string();
      string tmp_current_file = path.current_file;
      if (path.current_file.length() > 20) {
        tmp_current_file = tmp_current_file.substr(0, 17) + "...";
      }
      if (entry.is_directory())
        DrawTextEx(config.font, tmp_current_file.c_str(), posText,
                   config.fontSize, config.spacing, config.folderColor);
      else
        DrawTextEx(config.font, tmp_current_file.c_str(), posText,
                   config.fontSize, config.spacing, config.fileColor);
      Vector2 textSize = MeasureTextEx(config.font, tmp_current_file.c_str(),
                                       config.fontSize, config.spacing);
      if (mouseFileIdx == i && posMouse.x <= textSize.x) {
        path.current_selecting_file = path.current_file;
        Vector2 tmpTextSize =
            MeasureTextEx(config.font, path.current_selecting_file.c_str(),
                          config.fontSize, config.spacing);
        DrawRectangle(posText.x, posText.y, tmpTextSize.x, config.fontSize,
                      LIGHTGRAY);
        DrawTextEx(config.font, path.current_file.c_str(), posText,
                   config.fontSize, config.spacing, RED);
      }

      posText.y += config.fontSize;
    }
    i++;
  }
}