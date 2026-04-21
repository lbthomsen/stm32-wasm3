//
// Wasm source
//

#include "api.h"

void demo() {
    uint32_t led = 0;
    uint8_t color = 0;
    static const uint32_t led_color[][3] = {
        {10, 0, 0}, 
        {0, 10, 0}, 
        {0, 0, 10}, 
        {10, 10, 0}, 
        {10, 0, 10}, 
        {0, 10, 10}
    };
	
	for (;;) {
		host_zero_led_values();
		host_set_led(led, led_color[color][0], led_color[color][1], led_color[color][2]);
		++led;
		if (led >= 64) {
			led = 0;
			++color;
			if (color >= sizeof(led_color) / sizeof(led_color[0]) ) {
				color = 0;
			}
		}
		host_delay(20);
	}
}

// vim: ts=4 et nowrap
