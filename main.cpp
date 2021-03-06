﻿#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include <algorithm> 
#include <iostream> 



Model *model = NULL;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const int width = 800;
const int height = 500;


void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
	int x0 = p0.x;
	int y0 = p0.y;
	int x1 = p1.x;
	int y1 = p1.y;
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

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	int total_height = t2.y - t0.y;

	for (int i = t0.y; i <= t1.y; i++) {
		int segment_height = t1.y - t0.y + 1;

		float x1 = t0.x + (((i - t0.y)*(t1.x-t0.x))/(segment_height));
		float x2 = t0.x + (((i - t0.y)*(t2.x - t0.x)) / (total_height));
		if (x1 > x2) std::swap(x1, x2);
		for (int j = x1; j <= x2; j++) {
			image.set(j, i, color);
		}

	}

	for (int i = t1.y; i <= t2.y; i++) {
		int segment_height = t2.y - t1.y + 1;

		float x1 = t1.x + (((i - t1.y)*(t2.x - t1.x)) / (segment_height));
		float x2 = t0.x + (((i - t0.y)*(t2.x - t0.x)) / (total_height));
		if (x1 > x2) std::swap(x1, x2);
		for (int j = x1; j <= x2; j++) {
			image.set(j, i, color);
		}

	}
}



int main(int argc, char** argv) {


	TGAImage scene(width, height, TGAImage::RGB);

	// scene "2d mesh"
	line(Vec2i(20, 34), Vec2i(744, 400), scene, red);
	line(Vec2i(120, 434), Vec2i(444, 400), scene, green);
	line(Vec2i(330, 463), Vec2i(594, 200), scene, blue);

	// screen line
	line(Vec2i(10, 10), Vec2i(790, 10), scene, white);

	scene.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	scene.write_tga_file("scene.tga");


	return 0;
}

