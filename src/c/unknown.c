#include <pebble.h>
#include <math.h>
  
static bool unknownfx_first = true;

#ifdef PBL_COLOR
  static void draw_pixel(GContext* ctx, int x, int y, GColor8 color){ //resolution = 24 x 28
#else
  static void draw_pixel(GContext* ctx, int x, int y, GColor color){ //resolution = 24 x 28
#endif
  int newx = x * 6;
  int newy = y * 6;
  graphics_context_set_fill_color(ctx, color);
  graphics_fill_rect(ctx, GRect(newx,newy,6,6), 0, GCornerNone);
}
  
static void unknowngfx_update(Layer *layer, GContext *ctx, int counter) {
  counter = 28 - counter;
  if (unknownfx_first){
    srand(1);
    unknownfx_first = false;
  }
  int x;
  int y;
  for (y = 0; y <= 28; y++){
    for (x = 0; x <= 24; x++){
      if (y < counter){
        if ((counter - y) > 3){
          continue;
        }
        if (!(rand() % abs(2*(y-counter))) == 0){
          continue;
        }
      }
      int r = rand() % 4;
      GColor8 rcolor = GColorBlack;
      switch(r){
        case 0: rcolor = GColorDarkCandyAppleRed; break;
        case 1: rcolor = GColorWindsorTan; break;
        case 2: rcolor = GColorRajah; break;
        case 3: rcolor = GColorBrass; break;
      }
      draw_pixel(ctx, x, y, rcolor);
    }
  }
}