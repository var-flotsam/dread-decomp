// Unit 259 (tools/tu_map.py): forwards to fn_71001d70b8 with the slot at +4 of the current entry of a
// table of 0x90-byte records (fn_710015abb0), clearing it on first use. Names and layout are placeholders.
#include "types.h"
struct UnkCtxEntry_b47 {
    u32 initialized;
    u32 field_0x4;
    u32 field_0x8;
};
struct UnkV2_b47 { f32 x, y; };
UnkCtxEntry_b47* fn_710015abb0();
void fn_71001d70b8(void* a, void* b, u32* slot, bool flag, UnkV2_b47 v);
void fn_71001d72b4(void* a, UnkV2_b47 v, void* b, bool flag) {
    UnkCtxEntry_b47* e = fn_710015abb0();
    if (!e->initialized) {
        __builtin_memset(&e->field_0x4, 0, 8);
        e->initialized = 1;
    }
    fn_71001d70b8(a, b, &e->field_0x4, flag, v);
}
