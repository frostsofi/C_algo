#include "LSB.h"
#include <random>

LSB::Coords::Coords(int iX, int iY)
{
  x = iX;
  y = iY;
}

void ChangeColor(unsigned char& color, char& mask, unsigned char& symbol, int shift)
{
  color &= ~mask;
  color |= (mask & symbol);
  symbol >>= shift;
}

/* Transfer parameter without ref to make copy of image */
std::vector<LSB::Coords> LSB::HideInfo(bitmap_image iBmp, std::string& iMes)
{
  std::random_device rd;
  std::mt19937 _gen(rd());
  int size_w = iBmp.width();
  int size_h = iBmp.height();

  /*Create distributors to get uniformly distributed values on the interval [a, b)*/
  std::uniform_real_distribution<double> distributor_X;
  distributor_X = std::uniform_real_distribution<double>(0, size_w);
  
  std::uniform_real_distribution<double> distributor_Y;
  distributor_Y = std::uniform_real_distribution<double>(0, size_h);

  std::vector<LSB::Coords> secretInfo;

  for (size_t i = 0; i < iMes.size(); i++)
  {
    unsigned char symbol = iMes[i];
    int x = (int)distributor_X(_gen);
    int y = (int)distributor_Y(_gen);
    
    rgb_t pixel;
    iBmp.get_pixel(x, y, pixel);

    for (size_t color_n = 0; color_n < 3; ++color_n)
    {
      switch (color_n)
      {
        case 0: {
         ChangeColor(pixel.red, mask1, symbol, 3);
          break;
        }
        case 1: {
          ChangeColor(pixel.green, mask1, symbol, 3);
          break;
        }
        case 2 : {
          ChangeColor(pixel.blue, mask2, symbol, 2);
          break;
        }
        default:
          break;
      }
    }

    iBmp.set_pixel(x, y, pixel);
    Coords coord(x, y);
    secretInfo.push_back(coord);
  }

  /*Save image with hidden message*/
  iBmp.save_image("C:\\Users\\frost\\source\\repos\\ConsoleApplication1\\Out.bmp");
  return secretInfo;
}

std::string LSB::FindInfo(bitmap_image& iBmp, std::vector<Coords>& iSInfo)
{
  std::string mes;
  for (size_t i = 0; i < iSInfo.size(); ++i)
  {
    rgb_t pixel;
    iBmp.get_pixel(iSInfo[i].x, iSInfo[i].y, pixel);

    unsigned char resSymbol = 0b00000000;

    for (int color_n = 0; color_n < 3; color_n++)
    {
      switch (color_n)
      {
        case 0: {
          resSymbol |= (pixel.blue & mask2);
          resSymbol <<= 3;
          break;
        }
        case 1: {
          resSymbol |= (pixel.green & mask1);
          resSymbol <<= 3;
          break;
        }
        case 2: {
          resSymbol |= (pixel.red & mask1);
          break;
        }
        default:
         break;
        }
    }

    mes.push_back(resSymbol);
  }
  return mes;
}