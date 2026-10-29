#pragma once

// An unidentified class holding a file-path string id at +0x500, and the lazily created 0x70-byte
// registry its objects register that id with (fn_7100db37ec constructs the registry, fn_7100db3adc
// registers, fn_7100db3f24 unregisters). Constructor fn_7100416db8, destructor fn_7100416e80,
// registration fn_7100416fe0 (game_s2). Both globals are reached through the GOT.

#include "types.h"

struct UnkPathRegistry;  // 0x70 bytes, opaque
extern UnkPathRegistry* g_unk_71726b3240;  // the registry instance
extern char g_unk_7101a34218[];            // the class's vtable

void fn_7100db37ec(UnkPathRegistry* self);  // the registry's constructor
