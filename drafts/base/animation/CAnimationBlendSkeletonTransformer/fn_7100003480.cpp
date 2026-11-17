// base::animation::CAnimationBlendSkeletonTransformer's unit (tools/tu_map.py unit 10): the destructor
// of a blend state that clears itself first (three growable arrays emptied, the rest zeroed), then
// releases its first two arrays' storage. Names and layouts are placeholders.
#include <string.h>

#include "game/UnkTU9.h"
#include "types.h"

struct UnkBlendArray_l003480 {
    s32 count;
    s32 capacity;
    void* data;

    void Free() {
        if (data) {
            count = 0;
            capacity = 0;
            fn_7100002cd0(data);
            data = nullptr;
        }
    }

    // What the owner's destructor does with the storage: frees it, leaving the members as they are.
    void Release() {
        if (data)
            fn_7100002cd0(data);
    }
};

struct UnkBlendState_l003480 {
    u32 field_0x0;
    u32 pad_0x4;
    f32 field_0x8;
    f32 field_0xc;
    UnkBlendArray_l003480 array_0x10;
    UnkBlendArray_l003480 array_0x20;
    UnkBlendArray_l003480 array_0x30;
    u64 field_0x40;
    u32 field_0x48;
    u16 field_0x4c;
    u8 field_0x4e[0x1e];

    void Clear() {
        field_0x0 = 0;
        field_0x8 = 0.0f;
        field_0xc = 1.0f;
        array_0x10.Free();
        array_0x20.Free();
        array_0x30.Free();
        field_0x40 = 0;
        field_0x48 = 0;
        memset(field_0x4e, 0, sizeof(field_0x4e));
    }
};

// The destructor: clears the state, then frees the storage of the arrays at 0x20 and 0x10 (the one at
// 0x30 is left to its owner).
void fn_7100003480(UnkBlendState_l003480* self) {
    self->Clear();
    self->array_0x20.Release();
    self->array_0x10.Release();
}
