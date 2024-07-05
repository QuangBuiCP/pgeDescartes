#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class pgeDescartes : public olc::PixelGameEngine {
 public:
  pgeDescartes() { sAppName = "pgeDescartes"; }

 private:
  olc::vf2d offset = {0.0f, 0.0f};
  olc::vf2d start_pan = {0.0f, 0.0f};
  float scale = 10.0f;
  float grid_edge = 1.0f;

  // Convert coordinates from World Space --> Screen Space
  void WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY) {
    nScreenX = (int)((v.x - offset.x) * scale);
    nScreenY = (int)((v.y - offset.y) * scale);
  }

  // Convert coordinates from Screen Space --> World Space
  void ScreenToWorld(int nScreenX, int nScreenY, olc::vf2d& v) {
    v.x = (float)(nScreenX) / scale + offset.x;
    v.y = (float)(nScreenY) / scale + offset.y;
  }

  olc::vf2d cursor = {0.0f, 0.0f};

 public:
  bool OnUserCreate() override {
    offset = {((float)(-ScreenWidth()) / 2) / scale,
              ((float)(-ScreenHeight()) / 2) / scale};
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override {
    olc::vf2d cur_mouse = {(float)GetMouseX(), (float)GetMouseY()};
    bool changing_scale = false;
    // Handle Pan & Zoom
    if (GetMouse(2).bPressed) {
      start_pan = cur_mouse;
      changing_scale = true;
    }
    if (GetMouse(2).bHeld) {
      offset -= (cur_mouse - start_pan) / scale;
      start_pan = cur_mouse;
      changing_scale = true;
    }
    olc::vf2d mouse_before_zoom;
    ScreenToWorld((int)cur_mouse.x, (int)cur_mouse.y, mouse_before_zoom);
    if (GetKey(olc::Key::E).bHeld || GetMouseWheel() > 0) {
      scale += 0.2f;
      changing_scale = true;
    }
    if (GetKey(olc::Key::Q).bHeld || GetMouseWheel() < 0) {
      scale -= 0.2f;
      changing_scale = true;
    }
    olc::vf2d mouse_after_zoom;
    ScreenToWorld((int)cur_mouse.x, (int)cur_mouse.y, mouse_after_zoom);
    offset += (mouse_before_zoom - mouse_after_zoom);

    // Clear screen
    Clear(olc::VERY_DARK_BLUE);

    int sx, sy;
    int ex, ey;

    // Get visible world
    olc::vf2d world_topleft, world_bottomright;
    ScreenToWorld(0, 0, world_topleft);
    ScreenToWorld(ScreenWidth(), ScreenHeight(), world_bottomright);
    // Get values just beyond screen boundaries
    world_topleft.x = floor(world_topleft.x);
    world_topleft.y = floor(world_topleft.y);
    world_bottomright.x = ceil(world_bottomright.x);
    world_bottomright.y = ceil(world_bottomright.y);
    // Draw grid_edge dots
    for (float x = world_topleft.x; x < world_bottomright.x; x += grid_edge) {
      for (float y = world_topleft.y; y < world_bottomright.y; y += grid_edge) {
        WorldToScreen({x, y}, sx, sy);
        Draw(sx, sy, olc::BLUE);
      }
    }
    // Draw World Axis: verticle
    WorldToScreen({0, world_topleft.y}, sx, sy);
    WorldToScreen({0, world_bottomright.y}, ex, ey);
    DrawLine(sx, sy, ex, ey, olc::GREY, 0xF0F0F0F0);
    // Draw World Axis: horizontal
    WorldToScreen({world_topleft.x, 0}, sx, sy);
    WorldToScreen({world_bottomright.x, 0}, ex, ey);
    DrawLine(sx, sy, ex, ey, olc::GREY, 0xF0F0F0F0);

    if (changing_scale) {
      std::cout << world_topleft.x << ' ' << world_topleft.y << "; " << world_bottomright.x << ' ' << world_bottomright.y << '\n';
    }
    // Draw cursor position
    olc::vf2d cur_coor;
    ScreenToWorld(cur_mouse.x, cur_mouse.y, cur_coor);
		DrawString(10, 10, "X=" + std::to_string(cur_coor.x) + ", Y=" + std::to_string(cur_coor.y), olc::YELLOW, 2);
    return true;
  }
};

int main() {
  pgeDescartes demo;
  if (demo.Construct(1280, 720, 1, 1)) {
    demo.Start();
  }
  return 0;
}