#pragma once
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

class Renderer
{
private:
	TGAImage _image;
	const char* _outputfilename;

public:
	Renderer(int width, int height, const char* filename);
	~Renderer();
	void drawLine(int x0, int y0, int x1, int y1, TGAColor color);
	void drawModel(Model* model);

};