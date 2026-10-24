// State-change handler of a component (old state in w1, new one in w2). Going from state 1 to 2 flips the
// flag at +0x146e8 when the value at +0x146e4 is negative; going from 2 to 1 hands a parameter block
// (vtable g_unk_7101a25fb0, kind 6, carrying that flag) to fn_71006f4b0c and stores three components of
// the owner's matrix as a vector at +0x146e0.
#include "game_s2/UnkParams.h"
#include "types.h"

extern void* g_unk_7101a25fb0[];

struct UnkMatrix {
    u8 pad_0x0[4];
    u32 field_0x4;
    u8 pad_0x8[0xc];
    u32 field_0x14;
    u8 pad_0x18[0xc];
    u32 field_0x24;
};

struct UnkOwner {
    u8 pad_0x0[0x38];
    u8 field_0x38[0x1e0];
    void* field_0x218;
    u8 pad_0x220[0x78];
    void* field_0x298;
};

struct UnkVec3 {
    u32 x;
    u32 y;
    u32 z;
};

// The kind-6 block.
struct UnkParams6 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    f32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 field_0x38;
    u8 field_0x39;
};

struct UnkState {
    f32 field_0x0;
    u8 field_0x4;
};

struct UnkComponent {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
    u8 pad_0x38[0x146a8];
    u8 field_0x146e0[4];
};

UnkMatrix* fn_710002614c(void* object);
void fn_710007b444(void* dst, const UnkVec3* src);

void fn_71003f832c(UnkComponent* self, u32 old_state, u32 new_state) {
    if (old_state == new_state)
        return;
    UnkState* state = reinterpret_cast<UnkState*>(reinterpret_cast<char*>(self) + 0x146e4);
    if (old_state == 1 && new_state == 2) {
        if (state->field_0x0 < 0.0f)
            state->field_0x4 ^= 1;
        return;
    }
    if (old_state == 2 && new_state == 1) {
        UnkOwner* owner = self->field_0x30;
        if (owner->field_0x218) {
            UnkParams6 params;
            __builtin_memset(params.field_0x8, 0, 13);
            params.field_0x30 = 0;
            __builtin_memset(params.field_0x20, 0, 16);
            params.vtable = g_unk_7101a25fb0 + 2;
            params.field_0x34 = 6;
            params.field_0x38 = state->field_0x4;
            params.field_0x39 = 1;
            params.field_0x18 = 1.0f;
            params.field_0x1c = 1;
            fn_71006f4b0c(owner->field_0x298, &params, 1);
            UnkMatrix* m = fn_710002614c(self->field_0x30->field_0x38);
            u32 x = m->field_0x4;
            u32 y = m->field_0x14;
            u32 z = m->field_0x24;
            UnkVec3 v = {x, y, z};
            fn_710007b444(self->field_0x146e0, &v);
        }
    }
}
