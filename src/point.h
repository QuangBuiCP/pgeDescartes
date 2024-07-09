#ifndef POINT_H_
#define POINT_H_

#include "olcPixelGameEngine.h"

class Point {
 private:
  olc::vf2d pos;

  void WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY,
                     olc::vf2d offset, float scale);

 public:
  Point(olc::vf2d);
  olc::vf2d GetPos();
  void DrawMyPoint(olc::PixelGameEngine* pge, const olc::vf2d& offset,
                   const float& scale);
  

  bool operator < (const Point& other) const {
    if (pos.x == other.pos.x) {
      return pos.y < other.pos.y;
    }
    return pos.x < other.pos.x;
  }
};

class PointList {
 private:
  std::vector<Point> point;  // this thing surely needs a replacement

  void WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY,
                     olc::vf2d offset, float scale);

 public:
  PointList();
  bool ReadData(const std::string& filename);
  bool Drawable();
  void DrawGraph(olc::PixelGameEngine* pge, olc::vf2d offset, float scale);
};

#endif  // POINT_H_