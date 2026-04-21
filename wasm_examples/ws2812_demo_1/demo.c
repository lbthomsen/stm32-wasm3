//
// Wasm source
//

#include "api.h"


void ws2812_demo() {
	int led = 0;
	
	for (;;) {
		set_led(led, 10, 0, 10);
		++led;
		if (led >= 64) led = 0;
		delay(10);
	}
}

