#include "TextureRect.h"
TextureRect::TextureRect(float width,float height)
{
	vCount = 6;
	dataByteCount = sizeof(float) * 6 * 8;
	vdata = new float[6 * 8]{
			-width,0,-height,0,0,0,0,1,
			-width,0,height,0,1,0,0,1,
			width,0,height,1,1,0,0,1,
			width,0,height,1,1,0,0,1,
			width,0,-height,1,0,0,0,1,
			-width,0,-height,0,0,0,0,1
	};
}
TextureRect::~TextureRect() {
	delete[] vdata;
}
