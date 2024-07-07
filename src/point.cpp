#include "point.h"

Point::Point(float _x, float _y) : x(_x), y(_y) {}

// Convert coordinates from World Space --> Screen Space
void WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY,
                   olc::vf2d offset, float scale) {
  nScreenX = (int)((v.x - offset.x) * scale);
  nScreenY = (int)((v.y - offset.y) * scale);
}

void Point::DrawMyPoint(olc::PixelGameEngine* pge, const olc::vf2d& offset,
                        const float& scale) {
  int sx, sy;
  ::WorldToScreen({x, y}, sx, sy, offset, scale);
  pge->FillCircle(sx, sy, 2, olc::GREEN);
}

// TODO: Error checking...
bool PointList::ReadData(const std::string& filename) {
  std::ifstream fin;
  fin.open(filename, std::ios::in);
  if (!fin.good()) {
    return false;
  }
  float x, y;
  std::vector<Point> pending;
  while (fin >> x >> y) {
    pending.push_back(Point(x, y));
  }
  point.swap(pending);
  return true;
}