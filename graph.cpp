#include "graph.h"
MemTileGraph g;

int main(void) {
  g.init();
  g.run(1);
  g.end();
  return 0;
}