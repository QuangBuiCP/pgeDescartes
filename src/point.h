#ifndef POINT_H_
#define POINT_H_

#include "olcPixelGameEngine.h"

class Point {
 private:
  float x, y;

  void WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY);

 public:
  Point(float _x, float _y);
  void DrawMyPoint(olc::PixelGameEngine* pge, const olc::vf2d& offset,
                   const float& scale);
};

class PointList {
 private:
  std::vector<Point> point; // this thing surely needs a replacement

 public:
  bool ReadData(const std::string& filename);
};

#endif  // POINT_H_