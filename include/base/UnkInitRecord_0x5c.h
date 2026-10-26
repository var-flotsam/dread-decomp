#pragma once

// fn_7100025624 initialises a 0x5c-byte record in place (zeroes and unit scales; the owners embed
// it, e.g. at +0x18, or start with it): the callers pass its storage, so it takes any object.

#include "types.h"
void fn_7100025624(void* self);
