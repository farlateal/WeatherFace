#include <pebble.h>

static void raingfx_update(Layer *layer, GContext *ctx) {
  GColor8 bkgcolor = GColorCobaltBlue;
  GColor8 raincolor = GColorCeleste;
  
  graphics_context_set_fill_color(ctx, bkgcolor);
  graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
  
  graphics_context_set_stroke_color(ctx, raincolor);
  uint8_t i;
  for(i = 0; i < 40; i++){
    uint8_t x = rand() % 144;
    uint8_t y = rand() % 168;
    graphics_draw_line(ctx, GPoint(x, y), GPoint(x + 8, y + 10));
  }
}