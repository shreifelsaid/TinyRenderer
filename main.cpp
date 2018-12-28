#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

Model *model = NULL;
const int width = 800;
const int height = 800;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
	bool steep = false;
	float slope = (float)(y1 - y0) / (float)(x1 - x0);

	if (std::abs(x0 - x1) < std::abs(y0 - y1)) { // if rise is bigger than run (slope > 1 or slope < -1), we swap x and y to avoid holes. 
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	//our loop counts pixels in the x direction, therefore we need to "Force" x0 to be always smaller than x1;
	if (x0 > x1) { 
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int y = y0;
	int threshold = x1-x0;
	int increment;
	if (slope > 0) {
		increment = 1;
	}
	else {
		increment = -1;
	}
	int dy = std::abs(y1 - y0)*2;
	int test = 0;

	//loop that uses number of pixels in the x direction
	for (int x = x0; x <= x1; x++) {

		if (steep) {
			image.set(y, x, color); // if the line is steep, we reverse the x and y, here we set them straight by reversing the y and x again.
		}
		else {
			image.set(x, y, color);
		}

		test= test + dy;
		if (test > threshold) {
			y = y + increment; //could be negative if line is sloping downwards
			//incrementing the threshold variable
			threshold= threshold+ std::abs(x1 - x0) * 2;
		}
	}
}



int main(int argc, char** argv) {
	if (2 == argc) {
		model = new Model(argv[1]);
	}
	else {
		model = new Model("obj/african_head.obj");
	}

	TGAImage image(width, height, TGAImage::RGB);
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.)*width / 2.;
			int y0 = (v0.y + 1.)*height / 2.;
			int x1 = (v1.x + 1.)*width / 2.;
			int y1 = (v1.y + 1.)*height / 2.;
			line(x0, y0, x1, y1, image, white);
		}
	}

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	delete model;
	return 0;
}

