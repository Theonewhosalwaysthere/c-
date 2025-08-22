#pragma once

#include "teaVector.h"

void makePPM(const char* filename, int width, int height, const teaVector3Color* pixels);
void make_svg(const char* filename, int width, int height);