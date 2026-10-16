#include <pebble.h>
  
#define MISTGFX_NUM_CLOUDS (20)
  
static uint8_t mistgfx_first = 1;
static GPoint mistgfx_clouds[MISTGFX_NUM_CLOUDS];
static uint8_t mistgfx_speeds[MISTGFX_NUM_CLOUDS];
static GColor8 mistgfx_colors[MISTGFX_NUM_CLOUDS];

static void mistgfx_gencloud(){
  uint8_t i;
  for (i = 0; i < MISTGFX_NUM_CLOUDS; i++){
    mistgfx_speeds[i] = 1 + (rand()%5);
    mistgfx_clouds[i].x = rand() % 144;
    mistgfx_clouds[i].y = rand() % 168;
    mistgfx_colors[i] = GColorWhite;
    if (rand()%2){
      mistgfx_colors[i] = GColorLightGray;
    }
  }
}

static void mistgfx_update(Layer *layer, GContext *ctx) {
  GColor8 bkgcolor = GColorDukeBlue;
  
  graphics_context_set_fill_color(ctx, bkgcolor);
  graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
  
  if (mistgfx_first){
    mistgfx_gencloud();
    mistgfx_first = 0;
  }
  
  graphics_context_set_stroke_color(ctx, GColorWhite);
  
  uint8_t i;
  for(i = 0; i < MISTGFX_NUM_CLOUDS; i++){
    uint8_t y = rand()%168;
    graphics_draw_line(ctx, GPoint(0, y), GPoint(144, y));
                       
    graphics_context_set_fill_color(ctx, mistgfx_colors[i]);
    graphics_fill_circle(ctx, mistgfx_clouds[i], 50);
    mistgfx_clouds[i].x += mistgfx_speeds[i];
    if (mistgfx_clouds[i].x > 194){
      mistgfx_clouds[i].x = -50;
    }
  }
}