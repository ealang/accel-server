#include <sys/time.h>
#include "src/server/sensor/util.hh"

uint32_t timeMs() {
  struct timeval t;
  gettimeofday(&t, 0);
  return t.tv_sec * 1000 + t.tv_usec / 1000;
}
