#include <pebble.h>
  
#define CLOUDYGFX_NUM_CLOUDS (3)
#define CLOUDYGFX_CLOUD_CIRCLES (10)
  
static uint8_t cloudygfx_first = 1;
static GPoint cloudygfx_clouds[CLOUDYGFX_NUM_CLOUDS * CLOUDYGFX_CLOUD_CIRCLES];
static uint8_t cloudygfx_speeds[CLOUDYGFX_NUM_CLOUDS];

static void cloudygfx_gencloud(){
  uint8_t i;
  for (i = 0; i < CLOUDYGFX_NUM_CLOUDS; i++){
    uint8_t x = rand() % 144;
    uint8_t y = rand() % 168;
    cloudygfx_speeds[i] = 3 + (rand()%7);
    //cloudygfx_speeds[i] = 5;
    uint8_t j;
    for (j = 0; j < CLOUDYGFX_CLOUD_CIRCLES; j++){
      cloudygfx_clouds[i * CLOUDYGFX_CLOUD_CIRCLES + j].x = x + (rand() % 40);
      cloudygfx_clouds[i * CLOUDYGFX_CLOUD_CIRCLES + j].y = y + (rand() % 15);
    }
  }
}

static void cloudygfx_update(Layer *layer, GContext *ctx, uint8_t mode) {
  GColor8 cloud_color;
  
  if (mode == 0){
    graphics_context_set_fill_color(ctx, GColorVividCerulean);
    cloud_color = GColorWhite;
  }else if (mode < 3){
    graphics_context_set_fill_color(ctx, GColorImperialPurple);
    cloud_color = GColorLightGray;
  }else{
    graphics_context_set_fill_color(ctx, GColorMidnightGreen);
    cloud_color = GColorWhite;
  }
  
  if (mode == 2){
    if ((rand()%5) == 0){
      graphics_context_set_fill_color(ctx, GColorWhite);
      graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
      uint8_t x = rand() % 144;
      graphics_context_set_fill_color(ctx, GColorChromeYellow);
      graphics_fill_rect(ctx, GRect(x,0,5,168), 0, GCornerNone);
      return;
    }
  }
  
  graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
  
  if (cloudygfx_first){
    cloudygfx_gencloud();
    cloudygfx_first = 0;
  }
  
  graphics_context_set_fill_color(ctx, cloud_color);
  uint8_t i;
  for (i = 0; i < CLOUDYGFX_NUM_CLOUDS; i++){
    uint8_t j;
    uint8_t move = (cloudygfx_clouds[i * CLOUDYGFX_CLOUD_CIRCLES].x > 170);
    for (j = 0; j < CLOUDYGFX_CLOUD_CIRCLES; j++){
      uint8_t current_index = i * CLOUDYGFX_CLOUD_CIRCLES + j;
      cloudygfx_clouds[current_index].x += cloudygfx_speeds[i];
      if (move){
        cloudygfx_clouds[current_index].x += -200;
      }
      graphics_fill_circle(ctx, cloudygfx_clouds[current_index], 15);
    }
  }
}