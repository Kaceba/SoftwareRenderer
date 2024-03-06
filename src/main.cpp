#include <vector>
#include <cmath>
#include "renderer.h"

Model* model = NULL;
const int width = 1000;
const int height = 1000;

int main(int argc, char** argv) {
	if (2 == argc) {
		model = new Model(argv[1]);
	}
	else {
		model = new Model("obj/african_head.obj");
	}
	
	const char* filename = "renderOutput.tga";
	Renderer* renderer = new Renderer(width, height, filename);

	renderer->drawModel(model);

	delete renderer;
	delete model;
	return 0;
}
