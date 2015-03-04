#include <pebble.h>
  
#define SUNNYGFX_NUM_STARS (20)
  
static uint8_t sunnygfx_offset = 0;
static GPoint sunnygfx_stars[SUNNYGFX_NUM_STARS];
static uint8_t sunnygfx_first = 1;

static void sunnygfx_update(Layer *layer, GContext *ctx, uint8_t mode) {
  if (mode == 0){
    graphics_context_set_fill_color(ctx, GColorVividCerulean);
  }else{
    graphics_context_set_fill_color(ctx, GColorBlack);
  }
  graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
  
  if (mode == 1){
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
      }else{
        graphics_fill_circle(ctx, sunnygfx_stars[i], 2);
      }
    }
  }
  
  if (mode == 0){
    graphics_context_set_fill_color(ctx, GColorYellow);
    graphics_context_set_stroke_color(ctx, GColorIcterine);
    graphics_fill_circle(ctx, GPoint(72, -30), 80);
  }else{
    graphics_context_set_fill_color(ctx, GColorLightGray);
    graphics_fill_circle(ctx, GPoint(72, 10), 60);
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_circle(ctx, GPoint(100, 10), 60);
  }
  
  if (mode == 1){
    return;
  }
  
  uint8_t i;
  for (i = 0; i < 10; i++){
    if (i == 9){
      if (sunnygfx_offset > 0) {
        graphics_context_set_stroke_color(ctx, GColorInchworm);
      }
      if (sunnygfx_offset > 1) {
        graphics_context_set_stroke_color(ctx, GColorMintGreen);
      }
      if (sunnygfx_offset > 2) {
        graphics_context_set_stroke_color(ctx, GColorMediumAquamarine);
      }
      if (sunnygfx_offset > 3) {
        graphics_context_set_stroke_color(ctx, GColorCyan);
      }
    }
    graphics_draw_circle(ctx, GPoint(72, -30), 80 + (i * 10)+sunnygfx_offset*2);
  }
  sunnygfx_offset++;
  if (sunnygfx_offset > 4) {
    sunnygfx_offset = 0;
  }
}