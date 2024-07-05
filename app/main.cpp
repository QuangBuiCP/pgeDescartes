#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Testing : public olc::PixelGameEngine {
 public:
  Testing() { sAppName = "Testing"; }

 public:
  bool OnUserCreate() override {
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override {
    return true;
  }
};

int main() {
  Testing demo;
  if (demo.Construct(256, 240, 4, 4)) {
    demo.Start();
  }
  return 0;
}