// Units 271 and 281 (tools/tu_map.py): handlers that pass the object held at a fixed offset, if
// any, to a function of their unit, reporting whether there was one.
#include "base/reflection.h"
#include "types.h"

void fn_71001ebb34(void* obj, void* entry, s32 tag);  // src/game/UnkSpherePool_SetTag.cpp
void fn_71001ef028(void* obj, void* arg);

struct UnkHolder10 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkHolderA0 {
    u8 pad_0x0[0xa0];
    void* field_0xa0;
};

// `entry` and `tag` (x1, w2) reach fn_71001ebb34 untouched in the original (tools/forwarded_args.py).
bool fn_71001f0364(UnkHolder10* self, void* entry, s32 tag) {
    if (!self->field_0x10)
        return false;
    fn_71001ebb34(self->field_0x10, entry, tag);
    return true;
}

// `arg` passes through untouched to fn_71001ef028 (and on to its tail call) in the original; its
// callers pass it (game_s1/fn_71001fcd4c.cpp), so it is declared or the port drops it
// (tools/forwarded_args.py).
bool fn_7100202d24(UnkHolderA0* self, void* arg) {
    if (!self->field_0xa0)
        return false;
    fn_71001ef028(self->field_0xa0, arg);
    return true;
}
