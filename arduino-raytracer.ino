
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#include "rtweekend.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);



inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

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

color ray_color(const ray& r, hittable_list world, int depth) {
  hit_record rec;
  if(depth <= 0){
    return color(0.0, 0.0, 0.0);
  }
  if(world.hit(r, 0, infinity, rec)){
    point3 target = rec.p + rec.normal + random_in_unit_sphere();
    return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
  }
  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5*(unit_direction.y() + 1.0);
  return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

void setup(void) {
  randomSeed(analogRead(5));
//  Serial.begin(9600);
//  Serial.println(F("TFT LCD test"));
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.setRotation(1);
//  for(int i=0; i< 10;i++){
//    Serial.println(random_double());
//  }
  main_f();
}

void write_color(int x, int y, color pixel_color, int samples_per_pixel){
  auto scale = 1.0/samples_per_pixel;
  color c(scale, scale, scale);

  pixel_color*=c;
  pixel_color.e[0] = clamp(sqrt(pixel_color.e[0]), 0.0, 0.999);
  pixel_color.e[1] = clamp(sqrt(pixel_color.e[1]), 0.0, 0.999);
  pixel_color.e[2] = clamp(sqrt(pixel_color.e[2]), 0.0, 0.999);
  
  unsigned int color = color_to_565(pixel_color);
  tft.drawPixel(x, y,  color);
}


void main_f() {
  // image
  const auto aspect_ratio = 4.0 / 3.0;
  const int image_width = tft.width();  // 320
  const int image_height = tft.height(); // 240
//  const int samples_per_pixel = 20;
//  const int max_depth = 10;
  const int samples_per_pixel = 2;
  const int max_depth = 2;
//  Serial.println("width height");
//  Serial.println(image_width);
//  Serial.println(image_height);

  // World
  hittable_list world;
  world.add(sphere(point3(0, 0, -1), 0.5));
  //world.add(sphere(point3(0, 1, -1), 0.5));
  world.add(sphere(point3(0, -100.5, -1), 100));
  

  // camera
  camera cam;

  // render
  for (int j = 0; j < image_height; ++j) {
    //Serial.print("line number: ");
    //Serial.println(j);
    for (int i = 0; i < image_width; ++i) {
      color pixel_color(0, 0, 0);
      for(int s=0; s < samples_per_pixel; ++s){
        auto u = double(i) / (image_width - 1);
        auto v = double(j) / (image_height - 1);
        ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world, max_depth);
      }

      write_color(i, j, pixel_color, samples_per_pixel);
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
