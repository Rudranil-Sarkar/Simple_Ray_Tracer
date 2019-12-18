#ifndef __FILE_HANDLER_H__
#define __FILE_HANDLER_H__
#include <cstdint>
#include "vec3.h"

/*The buffer to store all the pixels*/
class Framebuffer {
		  private:
		  		int width, height;
		 		vec3 * data;

		  public:
				Framebuffer(int w_, int h_);
				void SetPixel(int row, int col, const vec3& p);
				vec3& getPixel(int row, int col);
				inline int getWidth() { return width; } 
				inline int getHeight() { return height; }

};

class SaveFile {

		  public:
		  			 SaveFile() = delete;
					 static void Save(const char * filePath, Framebuffer f);

};

#endif
