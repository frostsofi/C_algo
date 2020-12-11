// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <sstream>
#include "bitmap_image.hpp"
#include "LSB.h"

std::string readFile(const std::string& fileName) {
  std::ifstream f(fileName);
  std::stringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

int main()
{
    std::cout << "Start" << std::endl;

    /* Name of you file */
    std::string mes;
    mes = readFile("C:\\Users\\frost\\source\\repos\\ConsoleApplication1\\in.txt");

    /* Path to image, which we want to hide message*/
    bitmap_image image("C:\\Users\\frost\\source\\repos\\ConsoleApplication1\\tree.bmp");

    LSB lsb;
    std::vector<LSB::Coords> info = lsb.HideInfo(image, mes);

    /*Load image with hide information*/
    bitmap_image imageOut("C:\\Users\\frost\\source\\repos\\ConsoleApplication1\\Out.bmp");
    std::string answer = lsb.FindInfo(imageOut, info);

    std::cout << "Answer: " << answer << std::endl;
}