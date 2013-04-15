#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x40, 0xFC, 0x8F, 0x22, 0x53, 0x5D, 0x42, 0x66, 0xAC, 0x45, 0x36, 0x83, 0xF5, 0x3F, 0x45, 0x42 }
PBL_APP_INFO(MY_UUID,
        "Squid Watch", "Maximilian Burkhardt",
        1, 0, /* App version */
        DEFAULT_MENU_ICON,
        APP_INFO_WATCH_FACE);

Window window;

BmpContainer squid_container;

TextLayer text_date_layer;
TextLayer text_time_layer;

void handle_init(AppContextRef ctx) {
    (void)ctx;

    window_init(&window, "Squid");
    window_stack_push(&window, true /* Animated */);
    window_set_background_color(&window, GColorWhite);

    resource_init_current_app(&SQUID_RESOURCES);

    bmp_init_container(RESOURCE_ID_IMAGE_SQUID, &squid_container);
    GRect frame = layer_get_frame(&squid_container.layer.layer);
    frame.origin.x = 0;
    frame.origin.y = 0;
    layer_set_frame(&squid_container.layer.layer, frame);
    layer_add_child(&window.layer, &squid_container.layer.layer);

    text_layer_init(&text_date_layer, window.layer.frame);
    text_layer_set_text_color(&text_date_layer, GColorBlack);
    text_layer_set_background_color(&text_date_layer, GColorClear);
    layer_set_frame(&text_date_layer.layer, GRect(35, 10, 80, 50));
    text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PF_AGORA_BLACK_24)));
    layer_add_child(&window.layer, &text_date_layer.layer);

    text_layer_init(&text_time_layer, window.layer.frame);
    text_layer_set_text_color(&text_time_layer, GColorBlack);
    text_layer_set_background_color(&text_time_layer, GColorClear);
    layer_set_frame(&text_time_layer.layer, GRect(35, 80, 80, 50));
    text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PF_AGORA_BLACK_24)));
    layer_add_child(&window.layer, &text_time_layer.layer);
}

void handle_deinit(AppContextRef ctx) {
    (void)ctx;
    bmp_deinit_container(&squid_container);
}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;

  static char time_text[] = "00:00";
  static char date_text[] = "00 00";

  string_format_time(date_text, sizeof(date_text), "%m %e", t->tick_time);
  text_layer_set_text(&text_date_layer, date_text);

  string_format_time(time_text, sizeof(time_text), "%R", t->tick_time);
  text_layer_set_text(&text_time_layer, time_text);
}

void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
        .init_handler = &handle_init,

        .tick_info = {
            .tick_handler = &handle_minute_tick,
            .tick_units = MINUTE_UNIT
        }

    };
    app_event_loop(params, &handlers);
}
