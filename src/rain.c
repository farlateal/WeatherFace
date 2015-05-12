#include <pebble.h>
  
typedef enum {LIGHT_DRIZZLE, MOD_DRIZZLE, HEAVY_DRIZZLE, LIGHT_RAIN, MOD_RAIN, HEAVY_RAIN} raingfx_condition;
static raingfx_condition raingfx_currentcondition = LIGHT_RAIN;

static int raingfx_num = 40;
static int raingfx_lengthx = 8;
static int raingfx_lengthy = 8;

static void raingfx_update(Layer *layer, GContext *ctx) {
  GColor8 bkgcolor = GColorCobaltBlue;
  GColor8 raincolor = GColorCeleste;
  
  graphics_context_set_fill_color(ctx, bkgcolor);
  graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
  
  switch(raingfx_currentcondition){
    case LIGHT_DRIZZLE:
      raingfx_num = 15; raingfx_lengthx = 4; raingfx_lengthy = 5; break;
    case MOD_DRIZZLE:
      raingfx_num = 35; raingfx_lengthx = 4; raingfx_lengthy = 5; break;
    case HEAVY_DRIZZLE:
      raingfx_num = 60; raingfx_lengthx = 4; raingfx_lengthy = 5; break;
    case LIGHT_RAIN:
      raingfx_num = 15; raingfx_lengthx = 8; raingfx_lengthy = 10; break;
    case MOD_RAIN:
      raingfx_num = 30; raingfx_lengthx = 8; raingfx_lengthy = 10; break;
    case HEAVY_RAIN:
      raingfx_num = 60; raingfx_lengthx = 10; raingfx_lengthy = 12; break;
  }
  
  graphics_context_set_stroke_color(ctx, raincolor);
  uint8_t i;
  for(i = 0; i < raingfx_num; i++){
    uint8_t x = rand() % 144;
    uint8_t y = rand() % 168;
    graphics_draw_line(ctx, GPoint(x, y), GPoint(x + raingfx_lengthx, y + raingfx_lengthy));
  }
}

static void raingfx_classify(int id){
    switch(id){
      case 300:
      case 310:
        raingfx_currentcondition = LIGHT_DRIZZLE;
        break;
      case 301:
      case 311:
        raingfx_currentcondition = MOD_DRIZZLE;
        break;
      case 302:
      case 312:
        raingfx_currentcondition = HEAVY_DRIZZLE;
        break;
      case 313:
      case 321:
        raingfx_currentcondition = MOD_RAIN;
        break;
      case 314:
        raingfx_currentcondition = HEAVY_RAIN;
        break;
      case 500:
      case 520:
      case 531:
        raingfx_currentcondition = LIGHT_RAIN;
        break;
      case 501:
      case 521:
      case 511:
        raingfx_currentcondition = MOD_RAIN;
        break;
      default:
        raingfx_currentcondition = HEAVY_RAIN;
        break;
  }
}