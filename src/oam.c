#include "oam.h"

// Buffer is used because you can only write to oam during vblank (short time).
// Therefore logic to place data into oam should be done to buffer.
// Then copy buffer to oam.
OBJ_ATTR oam_buffer[128];
