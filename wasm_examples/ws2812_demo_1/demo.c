//
// Wasm source
//

#include "api.h"

void demo() {

    uint32_t led = 0;
	
	for (;;) {
		host_zero_led_values();
		host_set_led(led, 0, 0, 20);
		++led;
		if (led >= 64) {
			led = 0;
		}
		host_delay(20);
	}
}

// vim: ts=4 et nowrap
