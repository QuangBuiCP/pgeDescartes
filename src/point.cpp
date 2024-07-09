#include "point.h"

#include <algorithm>

Point::Point(olc::vf2d _pos) : pos(_pos) {}

// Convert coordinates from World Space --> Screen Space
void Point::WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY,
                          olc::vf2d offset, float scale) {
  nScreenX = (int)((v.x - offset.x) * scale);
  nScreenY = (int)((v.y - offset.y) * scale);
}

olc::vf2d Point::GetPos() {
  return pos;
}

void Point::DrawMyPoint(olc::PixelGameEngine* pge, const olc::vf2d& offset,
                        const float& scale) {
  int sx, sy;
  WorldToScreen(pos, sx, sy, offset, scale);
  pge->FillCircle(sx, sy, 2, olc::GREEN);
}

void PointList::WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY,
                              olc::vf2d offset, float scale) {
  nScreenX = (int)((v.x - offset.x) * scale);
  nScreenY = (int)((v.y - offset.y) * scale);
}

PointList::PointList() {}

// TODO: Error checking...
bool PointList::ReadData(const std::string& filename) {
  std::ifstream fin;
  fin.open(filename, std::ios::in);
  if (!fin.good()) {
    std::cout << "Failed to read file!\n";
    return false;
  }
  float x, y;
  std::vector<Point> pending;
  while (fin >> x >> y) {
    pending.push_back(Point({x, -y})); // kek
  }
  std::sort(pending.begin(), pending.end());
  point.swap(pending);
  std::cout << "File read successfully!\n";
  return true;
}

bool PointList::Drawable() {
  return (int)point.size() >= 2;
}

void PointList::DrawGraph(olc::PixelGameEngine* pge, olc::vf2d offset,
                          float scale) {
  size_t size = point.size();
  for (size_t i = 1; i < size; ++i) {
    int sx, sy, ex, ey;
    WorldToScreen(point[i - 1].GetPos(), sx, sy, offset, scale);
    WorldToScreen(point[i].GetPos(), ex, ey, offset, scale);
    pge->DrawLine(sx, sy, ex, ey, olc::WHITE);
  }
  for (size_t i = 0; i < size; ++i) {
    point[i].DrawMyPoint(pge, offset, scale);
  }
}