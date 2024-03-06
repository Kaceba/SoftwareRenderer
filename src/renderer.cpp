#include "renderer.h"

Renderer::Renderer(int width, int height, const char* filename)
: _image(width, height, TGAImage::RGB), _outputfilename(filename)
{

}

Renderer::~Renderer() {
	_image.flip_vertically();
	_image.write_tga_file(_outputfilename);
}

void Renderer::drawLine(int x0, int y0, int x1, int y1, TGAColor color){
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			_image.set(y, x, color);
		}
		else {
			_image.set(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void Renderer::drawModel(Model* model) {
	const TGAColor white = TGAColor(255, 255, 255, 255);
	int width = _image.get_width();
	int height = _image.get_height();

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.) * width / 2.;
			int y0 = (v0.y + 1.) * height / 2.;
			int x1 = (v1.x + 1.) * width / 2.;
			int y1 = (v1.y + 1.) * height / 2.;
			drawLine(x0, y0, x1, y1, white);
		}
	}
}
