#ifndef RENDER_H_
#define RENDER_H_

#include "olcPixelGameEngine.h"

#include "point.h"

class pgeDescartes : public olc::PixelGameEngine {
 public:
  pgeDescartes();

 private:
  olc::vf2d offset;
  olc::vf2d start_pan;
  float scale;
  float grid_edge;

  olc::vf2d cursor;
  olc::vf2d last_cursor;

  PointList pointlist;

  void WorldToScreen(const olc::vf2d& v, int& nScreenX, int& nScreenY);
  void ScreenToWorld(int nScreenX, int nScreenY, olc::vf2d& v);
  void MyDrawString(int x, int y, const std::string& s, olc::Pixel color,
                    int nScale);

 public:
  olc::vf2d GetOffset();
  float GetScale();

  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;
};

#endif  // RENDER_H_