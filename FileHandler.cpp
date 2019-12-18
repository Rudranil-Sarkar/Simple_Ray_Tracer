#include "FileHandler.h"
#include <fstream>
#include <iostream>

Framebuffer::Framebuffer(int w_, int h_)
    : width(w_), height(h_), data(new vec3[width * height])
{}

void Framebuffer::SetPixel(int row, int col, const vec3& p)
{
    data[col + row * height] = p;
}

vec3& Framebuffer::getPixel(int row, int col)
{
    return data[col + row * height];
}

void SaveFile::Save(const char * filePath, Framebuffer f)
{

                std::fstream file;
                file.open(filePath, std::ios::out);
                if(!file) { std::cout << "::File opening failed!\n"; exit(1); }
                
                file << "P3\n";
                file << f.getWidth() << " " << f.getHeight() << "\n";
                file << 255;
                file << "\n";
                
                for(int col = f.getHeight(); col > 0; col--)
                {
                    for(int row = 0; row < f.getWidth(); row++)
                    {
                            vec3 p = f.getPixel(row, col);
                            file << unsigned(p.x) << " " << unsigned(p.y) << " " << unsigned(p.z) << "\t";
                    }

                    file << "\n";
                
                }
                
                file.close();

}