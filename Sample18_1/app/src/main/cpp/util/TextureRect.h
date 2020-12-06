#pragma once
#ifndef TextureRect_h
#define TextureRect_h
class TextureRect {
public :
	TextureRect(float width, float height);
	float* vdata;
	int dataByteCount;
	int vCount;
	~TextureRect();
};
#endif 
