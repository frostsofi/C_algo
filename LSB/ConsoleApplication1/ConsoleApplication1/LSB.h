#pragma once
#include "bitmap_image.hpp"
#include <vector>

class LSB
{
private:
  char mask1 = 0b00000111;
  char mask2 = 0b00000011;

public:
  LSB() = default;

  class Coords
  {
  public:
    Coords(int iX = 0, int iY = 0);
    int x;
    int y;
  };

  std::vector<Coords> HideInfo(bitmap_image bmp, std::string &iMes);
  std::string FindInfo(bitmap_image &bmp, std::vector<Coords>&);
};

