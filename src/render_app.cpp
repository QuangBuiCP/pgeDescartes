#include "render_app.h"

pgeDescartes::pgeDescartes() {
  sAppName = "pgeDescares";
  offset = {0.0f, 0.0f};
  start_pan = {0.0f, 0.0f};
  scale = 40.0f;
  grid_edge = 1.0f;
  cursor = {0.0f, 0.0f};
  last_cursor = {0.0f, 0.0f};
  pointlist = PointList();
}

// Convert coordinates from World Space --> Screen Space
void pgeDescartes::WorldToScreen(const olc::vf2d& v, int& nScreenX,
                                 int& nScreenY) {
  nScreenX = (int)((v.x - offset.x) * scale);
  nScreenY = (int)((v.y - offset.y) * scale);
}

// Convert coordinates from Screen Space --> World Space
void pgeDescartes::ScreenToWorld(int nScreenX, int nScreenY, olc::vf2d& v) {
  v.x = (float)(nScreenX) / scale + offset.x;
  v.y = (float)(nScreenY) / scale + offset.y;
}

void pgeDescartes::MyDrawString(int x, int y, const std::string& s,
                                olc::Pixel color, int nScale) {
  olc::vi2d vStringSize = GetTextSize(s);
  FillRect(x - 4, y - 4, vStringSize.x * nScale + 6, vStringSize.y * nScale + 6,
           olc::BLACK);
  DrawString(x, y, s, color, nScale);
}

olc::vf2d pgeDescartes::GetOffset() {
  return offset;
}

float pgeDescartes::GetScale() {
  return scale;
}

// =========================================

bool pgeDescartes::OnUserCreate() /* override */ {
  offset = {((float)(-ScreenWidth()) / 2) / scale,
            ((float)(-ScreenHeight()) / 2) / scale};
  return true;
}

bool pgeDescartes::OnUserUpdate(float fElapsedTime) /* override */ {
  olc::vf2d cur_mouse = {(float)GetMouseX(), (float)GetMouseY()};
  bool changing_state = false;
  // Handle panning & zooming
  if (GetMouse(1).bPressed) {
    start_pan = cur_mouse;
    changing_state = true;
  }
  if (GetMouse(1).bHeld) {
    offset -= (cur_mouse - start_pan) / scale;
    start_pan = cur_mouse;
    changing_state = true;
  }
  olc::vf2d mouse_before;
  olc::vf2d mouse_after;
  ScreenToWorld((int)cur_mouse.x, (int)cur_mouse.y, mouse_before);
  if (GetKey(olc::Key::E).bHeld) {
    scale += 0.2f;
    changing_state = true;
  }
  if (GetMouseWheel() > 0) {
    scale *= 1.1f;
    changing_state = true;
  }
  if (GetKey(olc::Key::Q).bHeld) {
    scale -= 0.2f;
    changing_state = true;
  }
  if (GetMouseWheel() < 0) {
    scale *= 0.9f;
    changing_state = true;
  }
  ScreenToWorld((int)cur_mouse.x, (int)cur_mouse.y, mouse_after);
  offset += (mouse_before - mouse_after);

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
  // Draw World Axis: vertical
  WorldToScreen({0, world_topleft.y}, sx, sy);
  WorldToScreen({0, world_bottomright.y}, ex, ey);
  DrawLine(sx, sy, ex, ey, olc::GREY, 0xF0F0F0F0);
  // Draw World Axis: horizontal
  WorldToScreen({world_topleft.x, 0}, sx, sy);
  WorldToScreen({world_bottomright.x, 0}, ex, ey);
  DrawLine(sx, sy, ex, ey, olc::GREY, 0xF0F0F0F0);
  // Draw grid on axis root
  for (float x = world_topleft.x; x < world_bottomright.x; x += grid_edge) {
    WorldToScreen({x, 0}, sx, sy);
    FillCircle(sx, sy, 2, olc::YELLOW);
  }
  for (float y = world_topleft.y; y < world_bottomright.y; y += grid_edge) {
    WorldToScreen({0, y}, sx, sy);
    FillCircle(sx, sy, 2, olc::YELLOW);
  }
  // Draw bigger root point
  WorldToScreen({0, 0}, sx, sy);
  FillCircle(sx, sy, 4, olc::WHITE);
  // Check if coor is drawable
  bool drawable_coor = true;
  if (grid_edge * scale < 7) {
    drawable_coor = false;
  }
  {
    std::string s;
    olc::vi2d vStringSize;
    s = std::to_string((int)world_topleft.x);
    vStringSize = GetTextSize(s);
    if (grid_edge * scale < vStringSize.x + 10) {
      drawable_coor = false;
    }
    s = std::to_string((int)world_bottomright.x);
    vStringSize = GetTextSize(s);
    if (grid_edge * scale < vStringSize.x + 10) {
      drawable_coor = false;
    }
  }
  // Draw coor for vertical axis
  if (drawable_coor) {
    for (float y = world_topleft.y; y < world_bottomright.y; y += grid_edge) {
      if (y == 0) {
        continue;
      }
      WorldToScreen({0, y}, sx, sy);
      DrawString(sx + 8, sy - 4, std::to_string((int)(-y)), olc::YELLOW, 1);
    }
  }
  // Draw coor for horizontal axis
  if (drawable_coor) {
    for (float x = world_topleft.x; x < world_bottomright.x; x += grid_edge) {
      if (x == 0) {
        continue;
      }
      WorldToScreen({x, 0}, sx, sy);
      std::string s = std::to_string((int)x);
      DrawString(sx - GetTextSize(s).x / 2, sy - 12, s, olc::YELLOW, 1);
    }
  }

  // =========================================

  // Load file point.txt and draw the list if pressed key 'L'
  if (GetKey(olc::Key::L).bPressed) {
    pointlist.ReadData("data/point.txt");
  }
  if (pointlist.Drawable()) {
    pointlist.DrawGraph(this, offset, scale);
  }

  // =========================================

#if 0
  if (changing_state) {
    std::cout << "scale: " << scale << '\n';
    std::cout << "world: " << world_topleft.x << ' ' << world_topleft.y << "; "
              << world_bottomright.x << ' ' << world_bottomright.y << '\n';
    std::cout << "offset: " << offset.x << ' ' << offset.y << '\n';
  }
#endif  // 0
  // Draw cursor position
  olc::vf2d cur_coor;
  ScreenToWorld(cur_mouse.x, cur_mouse.y, cur_coor);
  cur_coor.y = -cur_coor.y;
  MyDrawString(
      10, 10,
      "X=" + std::to_string(cur_coor.x) + ", Y=" + std::to_string(cur_coor.y),
      olc::YELLOW, 2);
  if (!drawable_coor) {
    MyDrawString(10, 34, "Coordinate not being rendered!", olc::RED, 1);
  }
  return true;
}