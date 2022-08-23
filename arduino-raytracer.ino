
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#include "vec3.h"
#include "ray.h"

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

double hit_sphere(const point3& center, double radius, const ray& r) {
  vec3 oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius*radius;
  auto discriminant = half_b*half_b - a*c;
  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-half_b - sqrt(discriminant) ) / a;
  }
}

color ray_color(const ray& r) {
  auto t = hit_sphere(point3(0,0,-1), 0.5, r);
  if (t > 0.0) {
    vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
    return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
  }
  t = hit_sphere(point3(0,-100.5,-1), 100, r);
  if (t > 0.0) {
    vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
    return color(0.0, 1.0, 0.0);
  }
  
  vec3 unit_direction = unit_vector(r.direction());
  t = 0.5*(unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void setup(void) {
  randomSeed(analogRead(3));
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.setRotation(1);
  main_f();
}

void main_f() {
  // image
  const auto aspect_ratio = 4.0 / 3.0;
  const int image_width = tft.width();  // 320
  const int image_height = tft.height(); // 240
  Serial.println("width height");
  Serial.println(image_width);
  Serial.println(image_height);

  // camera
  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;
  auto focal_length = 1.0;

  auto origin = point3(0, 0, 0);
  auto horizontal = vec3(viewport_width, 0, 0);
  auto vertical = vec3(0, viewport_height, 0);
  auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

  // render
  for (int j = image_height - 1; j >= 0; --j) {
    for (int i = 0; i < image_width; ++i) {

      auto u = double(i) / (image_width - 1);
      auto v = double(j) / (image_height - 1);

      ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
      unsigned int color = color_to_565(ray_color(r));
      tft.drawPixel(i, j,  color);
    }
  }
}


unsigned int color_to_565(color input) {
  return rgb_to_565(input.e[0], input.e[1], input.e[2]);
}
unsigned int rgb_to_565(double r, double g, double b) {
  // |r|r|r|r|r|g|g|g|g|g|g|b|b|b|b|b|
  //  f e d c b a 9 8 7 6 5 4 3 2 1 0
  //  Serial.println("----------");
  //  Serial.print(r);
  //  Serial.print(' ');
  //  Serial.print(g);
  //  Serial.print(' ');
  //  Serial.print(b);
  //  Serial.println();
  unsigned int ir = (unsigned int)(r * 31) << 11;
  unsigned int ig = (unsigned int)(g * 63) << 5;
  unsigned int ib = (unsigned int)(b * 31) & 31;
  unsigned int result = ir | ig | ib;
  return result;
}



void loop(void) {
}
