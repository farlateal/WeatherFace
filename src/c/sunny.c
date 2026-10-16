#include <pebble.h>
  
#define SUNNYGFX_NUM_STARS (20)
  
static uint8_t sunnygfx_offset = 0;
static GPoint sunnygfx_stars[SUNNYGFX_NUM_STARS];
static uint8_t sunnygfx_first = 1;

static void sunnygfx_day_update(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorVividCerulean);
  graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
  
  int8_t i;
  for (i = 2; i >= 0; i--){
    switch(i){
      case 0: graphics_context_set_fill_color(ctx, GColorOrange); break;
      case 1: graphics_context_set_fill_color(ctx, GColorChromeYellow); break;
      case 2: graphics_context_set_fill_color(ctx, GColorYellow); break;
    }
    graphics_fill_circle(ctx, GPoint(72 + (rand()%5), -30 + (rand()%5)), 90 + (i*10) + (rand()%5));
  }
    
  graphics_context_set_fill_color(ctx, GColorYellow);
  graphics_context_set_stroke_color(ctx, GColorIcterine);
  graphics_fill_circle(ctx, GPoint(72, -30), 80);
}

static void sunnygfx_night_update(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
    
  if (sunnygfx_first){
    uint8_t i;
    for (i = 0; i < SUNNYGFX_NUM_STARS; i++){
      sunnygfx_stars[i] = GPoint(rand()%144, rand()%168);
    }
    sunnygfx_first = 0;
  }
  uint8_t i;
  for (i = 0; i < SUNNYGFX_NUM_STARS; i++){
    graphics_context_set_fill_color(ctx, GColorLightGray);
    if (rand()%10 == 0){
      graphics_context_set_fill_color(ctx, GColorYellow);
      graphics_fill_circle(ctx, sunnygfx_stars[i], 3);
      //graphics_fill_circle_antialiased(ctx, sunnygfx_stars[i], 3, GColorYellow);
    }else{
      graphics_fill_circle(ctx, sunnygfx_stars[i], 2);
    }
  }
    
  graphics_context_set_fill_color(ctx, GColorLightGray);
  graphics_fill_circle(ctx, GPoint(72, 10), 60);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(100, 10), 60);
}

static void sunnygfx_update(Layer *layer, GContext *ctx, uint8_t mode) {
  if (mode == 0){
    sunnygfx_day_update(layer, ctx);
  }else{
    sunnygfx_night_update(layer, ctx);
  }
}