#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
  
class hittable_list : public hittable  {
    public:
        hittable_list() {}
        hittable_list(hittable* object) { add(object); }

        void clear() {
            index = 0;
          }
        void add(hittable* object) {
          objects[index] = object;
          index ++;
          }

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        int index = 0;
        int const listSize = 100;
        hittable *objects[100] = {NULL};
};


bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    auto hit_anything = false;
    auto closest_so_far = t_max;

    //for (const auto& object : objects) {
    for(int i=0; i<= index; i++){
      const auto& object = objects[i];
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}


#endif
