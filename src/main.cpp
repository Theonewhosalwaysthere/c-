#include "tea.h"
#include <vector>
#include <fstream>
#include <math.h>
#include "teaVector.h"
#include "lodepng.h"

#define MAXDISTANCE 200

#define TEA_PI2 1.57079632679

typedef struct {
  bool transparent = true;
  teaVector3Color color = {0, 0, 0};
} Voxel;

struct RayData {
  bool hit;
  teaVector3f origin;
  teaVector3f direction;
  float distance;
  teaVector3i position;
};

class teaImage {
  public:
    std::vector<unsigned char> data;
    int width, height;
    teaImage(int width, int height) {
      this->width = width;
      this->height = height;

      data.resize(width * height * 4);

      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          data[4 * width * y + 4 * x + 0] = 0; // R
          data[4 * width * y + 4 * x + 1] = 0; // G
          data[4 * width * y + 4 * x + 2] = 0; // B
          data[4 * width * y + 4 * x + 3] = 255;
        }
      }
    }

    void fill(int r, int g, int b, int a) {
      for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
          data[4 * this->width * y + 4 * x + 0] = r; // R
          data[4 * this->width * y + 4 * x + 1] = g; // G
          data[4 * this->width * y + 4 * x + 2] = b; // B
          data[4 * this->width * y + 4 * x + 3] = a;
        }
      }
    }

    void save(const std::string& filename) {
      lodepng::encode(filename, data, width, height);
    }
};

float safeDevide(float a, float b) {
  if (b == 0) return INFINITY;
  return a / b;
}

float intBound(float s, float ds) {
  if (ds == 0) return INFINITY;
  if (ds > 0) {
    return (floor(s + 1) - s) / ds;
  } else {
    return (s - floor(s)) / ds;
  }
}

RayData castRay(teaVector3f origin, teaVector3f direction, Voxel world[100][100][100]) {
  RayData ray;
  teaVector3i currentV;
  currentV.x = (int)(origin.x);
  currentV.y = (int)(origin.y);
  currentV.z = (int)(origin.z);

  teaVector3i step;
  step.x = (direction.x >= 0) ? 1 : -1;
  step.y = (direction.y >= 0) ? 1 : -1;
  step.z = (direction.z >= 0) ? 1 : -1;

  teaVector3f t_delta = {
    abs(safeDevide(1.0, direction.x)),
    abs(safeDevide(1.0, direction.y)),
    abs(safeDevide(1.0, direction.z))
  };

  ray.origin = origin;
  ray.direction = direction;

  teaVector3f t_max;
  t_max.x = intBound(origin.x, direction.x);
  t_max.y = intBound(origin.y, direction.y);
  t_max.z = intBound(origin.z, direction.z);

  float distance_traveled = 0;
  for (int i = 0; distance_traveled <= MAXDISTANCE || i < 200; i++) {

    if (currentV.x < 0 || currentV.x >= 100 || currentV.y < 0 || currentV.y >= 100 || currentV.z < 0 || currentV.z >= 100) {
      ray.hit = false;
      return ray;
    }

    if (!world[currentV.x][currentV.y][currentV.z].transparent) {
      ray.position = currentV;
      ray.distance = distance_traveled;
      ray.hit = true;
      return ray;
    }

    if (t_max.x < t_max.y && t_max.x < t_max.z) {
      currentV.x += step.x;
      distance_traveled = t_max.x;
      t_max.x += t_delta.x;
    } else if (t_max.y < t_max.z) {
      currentV.y += step.y;
      distance_traveled = t_max.y;
      t_max.y += t_delta.y;
    } else {
      currentV.z += step.z;
      distance_traveled = t_max.z;
      t_max.z += t_delta.z;
    }
  }

  ray.hit = false;
  return ray;
}

Voxel world[100][100][100];

teaVector3f getEulerCords(teaVector3f angle) {
  // Abbreviations
  float cy = cos(angle.x);
  float sy = sin(angle.x);
  float cp = cos(angle.y);
  float sp = sin(angle.y);
  float cr = cos(angle.z);
  float sr = sin(angle.z);

  // Forward vector
  float forward_x = cp * sy;
  float forward_y = sp;
  float forward_z = cp * cy;

  // Right vector
  float right_x = cr * cy + sr * sp * sy;
  float right_y = -sr * cp;
  float right_z = -cr * sy + sr * sp * cy;

  // Up vector
  float up_x = sr * cy - cr * sp * sy;
  float up_y = cr * cp;
  float up_z = -sr * sy - cr * sp * cy;
  return (teaVector3f){forward_x, forward_y, forward_z};
}

void pixelFunc(teaImage image, teaVector3Color (*colorfunc)(int, int)) {
  for (int y = 0; y < image.height; y++) {
    for (int x = 0; x < image.width; x++) {
        teaVector3Color color = colorfunc(x, y);
        image.data[4 * image.width * y + 4 * x + 0] = color.r; // R
        image.data[4 * image.width * y + 4 * x + 1] = color.g; // G
        image.data[4 * image.width * y + 4 * x + 2] = color.b; // B
        image.data[4 * image.width * y + 4 * x + 3] = 255; // A
    }
  }
}

int main() {
  teaImage screen = teaImage(100, 100);
  std::ofstream data_file("data.txt");
  teaVector3f angle = {(float)((image * 10.0) * (M_PI / 180)), 0.0, TEA_PI2};

  for (int x = 0; x < 100; x++) {
    for (int y = 0; y < 100; y++) {
      for (int z = 0; z < 100; z++) {
        world[x][y][z].color = {0,0,0};
      }
    }
  }

  for (int x = 0; x < 100; x++) {
    for (int y = 0; y < 100; y++) {
      world[x][y][50].transparent = false;
      world[x][y][50].color.r = x * 2;
      world[x][y][50].color.g = y * 2;
      world[x][y][50].color.b = y + x;
    }
  }

  teaVector3f euler = getEulerCords(angle);
  printf("%f, %f, %f\n", euler.x, euler.y, euler.z);

  for (int x = 0; x < 100; x++) {
    for (int y = 0; y < 100; y++) {
      RayData ray = castRay({(float)x, (float)y, 0}, euler, world);
      if (ray.hit) {
        teaVector3Color color = world[ray.position.x][ray.position.y][ray.position.z].color;
        screen.data[4 * screen.width * y + 4 * x + 0] = color.r; // R
        screen.data[4 * screen.width * y + 4 * x + 1] = color.g; // G
        screen.data[4 * screen.width * y + 4 * x + 2] = color.b; // B
        screen.data[4 * screen.width * y + 4 * x + 3] = 255; // A
      } else {
        screen.data[4 * screen.width * y + 4 * x + 0] = 0;   // R
        screen.data[4 * screen.width * y + 4 * x + 1] = 0;   // G
        screen.data[4 * screen.width * y + 4 * x + 2] = 105; // B
        screen.data[4 * screen.width * y + 4 * x + 3] = 255; // A
      }
    }
  }

  screen.save("images/test1");

  return 0;
}