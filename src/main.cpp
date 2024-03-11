#include <vector>
#include <cmath>
#include "renderer.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

Model* model = nullptr;
const int width = 1000;
const int height = 1000;

int main(int argc, char** argv) {
	if (2 == argc) {
		model = new Model(argv[1]);
	}
	else {
		model = new Model("obj/african_head.obj");
	}

	const char* filename = "SampleOutputs/modelRandomColorsTest.tga";
	Renderer* renderer = new Renderer(width, height, filename);

	{
		//renderer->drawModelWireframe(model, white);
	}
	
	{
		//renderer->drawModelRandomColors(model);
	}

	{
		renderer->drawModelLighting(model);
	}

	{
		//Vec2i pts[3] = { Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160) };
		//renderer->drawTriangle(pts, green);
	}

	delete renderer;
	delete model;
	return 0;
}
