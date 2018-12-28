#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


void line1(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
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
	float threshold = 0.5;
	int increment;
	if (slope > 0) {
		increment = 1;
	}
	else {
		increment = -1;
	}
	float dy = std::abs(slope);
	float test = 0;

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
			threshold++;
		}
	}
}


void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
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
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
		error2 += derror2;
		if (error2 > dx) {
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}
int main(int argc, char** argv) {
	TGAImage image(100, 100, TGAImage::RGB);
	line(13, 20, 80, 40, image, white);
	line(20, 13, 40, 80, image, red);
	line(80, 40, 13, 20, image, red);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}

