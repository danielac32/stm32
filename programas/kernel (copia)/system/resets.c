#include "resets.h"




void reset_set(unsigned int hw) {
	rst->reset |= hw;
}

void reset_release(unsigned int hw) {
	rst->reset &= (~hw);
}

void reset_release_wait(unsigned int hw) {
	rst->reset &= (~hw);
	while (!(rst->reset_done & hw));
}
