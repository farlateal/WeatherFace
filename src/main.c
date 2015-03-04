#include <pebble.h>
#include <rain.c>
#include <sunny.c>
#include <snow.c>
#include <cloudy.c>
#include <mist.c>
  
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define DEMO_FRAMES (6)
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_weather_layer;
static TextLayer *s_bt_layer;
static Layer *s_fg_layer;
static Layer *s_bg_layer;

static uint8_t percent = 0;
bool charging = false;
bool bt_connected = true;

static uint16_t demo_ctr = 1; //demo mode, use 1 to activate

static char conditions_buffer[32];

char* months[] = {"jan ", "feb ", "mar ", "apr ", "may ", "jun ", "jul ", "aug ", "sep ", "oct ", "nov ", "dec "};
char* days[] = {"sun ", "mon ", "tue ", "wed ", "thu ", "fri ", "sat "};
static GFont s_weather_font;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  
  //show date
  static char datebuffer[] = "                          ";
  int offset = 0;
  strcpy(datebuffer, days[tick_time->tm_wday]);
  offset = strlen(days[tick_time->tm_wday]);
  strcpy(datebuffer+offset, months[tick_time->tm_mon]);
  offset+=strlen(months[tick_time->tm_mon]);
  snprintf(datebuffer+offset, sizeof(char)*4, "%d", tick_time->tm_mday);
  text_layer_set_text(s_date_layer, datebuffer);
}

static void update_layer_fg(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack); //circle border
  graphics_fill_circle(ctx, GPoint(72, 84), 62);
  
  graphics_context_set_fill_color(ctx, GColorWhite); //circle
  graphics_fill_circle(ctx, GPoint(72, 84), 60);
  
  //vertical bars
  graphics_context_set_stroke_color(ctx, GColorBlack); 
  graphics_draw_line(ctx, GPoint(42,50), GPoint(42, 53));
  graphics_draw_line(ctx, GPoint(62,50), GPoint(62, 53));
  graphics_draw_line(ctx, GPoint(82,50), GPoint(82, 53));
  graphics_draw_line(ctx, GPoint(102,50), GPoint(102, 53));
  
  //battery bar
  if (charging){
    graphics_context_set_stroke_color(ctx, GColorGreen); 
  }
  uint8_t bar_length = (percent / 5) * 3;
  graphics_draw_line(ctx, GPoint(42,55), GPoint(42 + bar_length, 55));
  
  text_layer_set_text(s_bt_layer, "");
  if (!bt_connected){
    text_layer_set_text(s_bt_layer, "no bt");
  }
}

static void update_layer_bg(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(72, 84), 55);
  graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
  
  if (demo_ctr > 0){
    if (demo_ctr < DEMO_FRAMES){
      raingfx_update(layer, ctx);
    }else if (demo_ctr < DEMO_FRAMES*2){
      sunnygfx_update(layer, ctx, 0);
    }else if (demo_ctr < DEMO_FRAMES*3){
      cloudygfx_update(layer, ctx, 0);
    }else if (demo_ctr < DEMO_FRAMES*4){
      snowgfx_update(layer, ctx);
    }else if (demo_ctr < DEMO_FRAMES*5){
      cloudygfx_update(layer, ctx, 2);
    }else if (demo_ctr < DEMO_FRAMES*6){
      mistgfx_update(layer, ctx);
    }else if (demo_ctr < DEMO_FRAMES*7){
      sunnygfx_update(layer, ctx, 1); 
    }else if (demo_ctr < DEMO_FRAMES*7){
      cloudygfx_update(layer, ctx, 1); 
    }else{
      cloudygfx_update(layer, ctx, 3);
    }
    demo_ctr++;
    if (demo_ctr > DEMO_FRAMES*8) {
      demo_ctr = 1;
    }
    return;
  }
  
  if (strcmp("01d",conditions_buffer) == 0){
    sunnygfx_update(layer, ctx, 0);
  }else if (strcmp("01n",conditions_buffer) == 0){
    sunnygfx_update(layer, ctx, 1);
  }else if (strcmp("02d",conditions_buffer) == 0){
    cloudygfx_update(layer, ctx, 0);
  }else if (strcmp("02n",conditions_buffer) == 0){
    cloudygfx_update(layer, ctx, 3);
  }else if (strcmp("03d",conditions_buffer) == 0 || strcmp("03n",conditions_buffer) == 0){
    cloudygfx_update(layer, ctx, 0);
  }else if (strcmp("04d",conditions_buffer) == 0 || strcmp("04n",conditions_buffer) == 0){
    cloudygfx_update(layer, ctx, 1);
  }else if (strcmp("09d",conditions_buffer) == 0 || strcmp("09n",conditions_buffer) == 0){
    raingfx_update(layer, ctx);
  }else if (strcmp("10d",conditions_buffer) == 0 || strcmp("10n",conditions_buffer) == 0){
    raingfx_update(layer, ctx);
  }else if (strcmp("11d",conditions_buffer) == 0 || strcmp("11n",conditions_buffer) == 0){
    cloudygfx_update(layer, ctx, 2);
  }else if (strcmp("13d",conditions_buffer) == 0 || strcmp("13n",conditions_buffer) == 0){
    snowgfx_update(layer, ctx);
  }else{
    mistgfx_update(layer, ctx);
  }
  
  //mistgfx_update(layer, ctx); //mist
  //raingfx_update(layer, ctx); //rain
  //sunnygfx_update(layer, ctx, 0); //clear day
  //sunnygfx_update(layer, ctx, 1); //clear night
  //snowgfx_update(layer, ctx); //snow
  //cloudygfx_update(layer, ctx, 0); //light clouds
  //cloudygfx_update(layer, ctx, 1); //heavy clouds
  //cloudygfx_update(layer, ctx, 2); //thunder
  //cloudygfx_update(layer, ctx, 3); //light clouds night
}

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 52, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  
  // Create date TextLayer
  s_date_layer = text_layer_create(GRect(0, 95, 144, 75));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text(s_date_layer, "mon jan 1");
  
  //Graphics layer
  s_fg_layer = layer_create(GRect(0, 0, 144, 168));
  s_bg_layer = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(s_fg_layer, update_layer_fg);
  layer_set_update_proc(s_bg_layer, update_layer_bg);
  
  //BT warning layer
  s_bt_layer = text_layer_create(GRect(0, 32, 144, 50));
  text_layer_set_background_color(s_bt_layer, GColorClear);
  text_layer_set_text_color(s_bt_layer, GColorBlack);
  text_layer_set_text(s_bt_layer, "no bt");
  text_layer_set_text_alignment(s_bt_layer, GTextAlignmentCenter);

  //Apply to TextLayer
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), s_bg_layer);
  layer_add_child(s_bg_layer, s_fg_layer);
  layer_add_child(s_fg_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(s_fg_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(s_fg_layer, text_layer_get_layer(s_bt_layer));
  
  // Create temperature Layer
  s_weather_layer = text_layer_create(GRect(0, 115, 144, 25));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorBlack);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_text(s_weather_layer, "loading...");
  
  text_layer_set_font(s_weather_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  
  // Destroy weather elements
  text_layer_destroy(s_weather_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  layer_mark_dirty(s_fg_layer);
  
  // Get weather update every 30 minutes
  if(tick_time->tm_min % 30 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send();
  }
}

static void battery_handler(BatteryChargeState charge){
  percent = charge.charge_percent;
  charging = charge.is_charging;
  layer_mark_dirty(s_fg_layer);
}

void bt_handler(bool connected) {
  bt_connected = connected;
  layer_mark_dirty(s_fg_layer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char temperature_buffer[8];
  static char weather_layer_buffer[32];
  
  // Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
    case KEY_TEMPERATURE:
      snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)t->value->int32);
      break;
    case KEY_CONDITIONS:
      snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
      break;
    }

    // Look for next item
    t = dict_read_next(iterator);
  }
  
  // Assemble full string and display
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s", temperature_buffer);
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void fg_invalidate(struct tm *tick_time, TimeUnits units_changed) {
  if (tick_time->tm_sec % 60 == 0){
    update_time();
  }
  //layer_mark_dirty(s_fg_layer);
  layer_mark_dirty(s_bg_layer);
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  //tick_timer_service_subscribe(MINUTE_UNIT, tick_handler); //not working
  tick_timer_service_subscribe(SECOND_UNIT, fg_invalidate);
  
  battery_state_service_subscribe(battery_handler);
  battery_handler(battery_state_service_peek());
  
  bluetooth_connection_service_subscribe(bt_handler);
  bt_handler(bluetooth_connection_service_peek());
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
