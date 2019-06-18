#include "Main/Tools.h"

#include <stdint.h>

void tool_delay(uint16_t time){
	uint32_t waitTime = time * 256;
	uint32_t counter = 0;

	while(counter < waitTime){
		counter++;
	}
}
