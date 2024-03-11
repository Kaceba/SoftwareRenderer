#include "renderer.h"

Renderer::Renderer(int width, int height, const char* filename)
	: _image(width, height, TGAImage::RGB), _outputfilename(filename)
{

}

Renderer::~Renderer() {
	_image.flip_vertically();
	_image.write_tga_file(_outputfilename);
}

void Renderer::drawLine(int x0, int y0, int x1, int y1, TGAColor color) {
	//Using Bresenham's Line drawing algorithm https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

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

void Renderer::drawTriangle(Vec2i* triangleVA, TGAColor color) {
	if (triangleVA[0].y == triangleVA[1].y && triangleVA[0].y == triangleVA[2].y) return; // I dont care about degenerate triangles 
	// sort the vertices, triangleVA[0], triangleVA[1], triangleVA[2] lower−to−upper (bubblesort basically) 
	if (triangleVA[0].y > triangleVA[1].y) std::swap(triangleVA[0], triangleVA[1]);
	if (triangleVA[0].y > triangleVA[2].y) std::swap(triangleVA[0], triangleVA[2]);
	if (triangleVA[1].y > triangleVA[2].y) std::swap(triangleVA[1], triangleVA[2]);
	int total_height = triangleVA[2].y - triangleVA[0].y;
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > triangleVA[1].y - triangleVA[0].y || triangleVA[1].y == triangleVA[0].y;
		int segment_height = second_half ? triangleVA[2].y - triangleVA[1].y : triangleVA[1].y - triangleVA[0].y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? triangleVA[1].y - triangleVA[0].y : 0)) / segment_height; // be careful: with above conditions no division by zero here 
		Vec2i A = triangleVA[0] + (triangleVA[2] - triangleVA[0]) * alpha;
		Vec2i B = second_half ? triangleVA[1] + (triangleVA[2] - triangleVA[1]) * beta : triangleVA[0] + (triangleVA[1] - triangleVA[0]) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			_image.set(j, triangleVA[0].y + i, color); // attention, due to int casts triangleVA[0].y+i != A.y 
		}
	}
}

void Renderer::drawModelWireframe(Model* model, TGAColor color) {
	const int width = _image.get_width();
	const int height = _image.get_height();

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.) * width / 2.;
			int y0 = (v0.y + 1.) * height / 2.;
			int x1 = (v1.x + 1.) * width / 2.;
			int y1 = (v1.y + 1.) * height / 2.;
			drawLine(x0, y0, x1, y1, color);
		}
	}
}

void Renderer::drawModelRandomColors(Model* model) {
	const int width = _image.get_width();
	const int height = _image.get_height();

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f world_coords = model->vert(face[j]);
			screen_coords[j] = Vec2i((world_coords.x + 1.) * width / 2., (world_coords.y + 1.) * height / 2.);
		}
		drawTriangle(screen_coords, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	}
}

void Renderer::drawModelLighting(Model* model) {
	const int width = _image.get_width();
	const int height = _image.get_height();
	Vec3f light_dir(0, 0, -1); // define light_dir

	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];
		Vec3f world_coords[3];
		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(face[j]);
			screen_coords[j] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
			world_coords[j] = v;
		}
		Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
		n.normalize();
		float intensity = n * light_dir;
		if (intensity > 0) {
			drawTriangle(screen_coords, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
		}
	}
}

