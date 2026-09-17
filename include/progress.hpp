#pragma once

#include <iostream>

inline void progressBar(int frame, int frameNum) {
  constexpr int barW = 40;
  float progress = static_cast<float>(frame + 1) / frameNum;
  int completed = static_cast<int>(progress * barW);

  std::clog << "\r[";
  for (int i = 0; i < 40; i++) {
    std::clog << (i < completed ? '#' : '-');
  }
  std::clog << "] " << (frame + 1) << "/" << frameNum << ' ' << std::flush;
}

