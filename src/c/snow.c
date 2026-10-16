#include <pebble.h>
  
#define SNOWGFX_NUM_FLAKES (20)
  
static GPoint snowgfx_flakes[SNOWGFX_NUM_FLAKES];
static uint8_t snowgfx_first = 1;

static void snowgfx_update(Layer *layer, GContext *ctx) {
  if (snowgfx_first){
    uint8_t i;
    for (i = 0; i < SNOWGFX_NUM_FLAKES; i++){
      snowgfx_flakes[i].x = rand() % 144;
      snowgfx_flakes[i].y = rand() % 168;
    }
    snowgfx_first = 0;
  }
  
  graphics_context_set_fill_color(ctx, GColorOxfordBlue);
  graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
  
  graphics_context_set_fill_color(ctx, GColorWhite);
  uint8_t i;
  for(i = 0; i < SNOWGFX_NUM_FLAKES; i++){
    graphics_fill_circle(ctx, snowgfx_flakes[i], 7);
    snowgfx_flakes[i].y += 7;
    snowgfx_flakes[i].x += (rand()%6) - 3;
    if (snowgfx_flakes[i].y > 178){
      snowgfx_flakes[i].y = -10;
      snowgfx_flakes[i].x = rand() % 144;
    }
  }
}