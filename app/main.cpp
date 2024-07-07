#define OLC_PGE_APPLICATION

#include "render_app.h"

int main() {
  pgeDescartes demo;
  if (demo.Construct(1280, 720, 1, 1)) {
    demo.Start();
  }
  return 0;
}