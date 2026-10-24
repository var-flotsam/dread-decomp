// Whether the owner is far enough above a reference point: needs a position object whose object at
// +0x208 is flagged (+0xf9) with a positive count and either a set test or the manager at
// 0x7101d47310 answering a query; then, when the mode at +0x70 differs from "nonzero and flag clear",
// the height of the owner above a point computed from the reference at +0x68 (the reference itself
// in mode 1, else the point 12 bytes in) must reach a tunable (at +0x7fc of the object at +0x149d0).
#include "game/UnkActor.h"
#include "types.h"

struct UnkFlagged {
    u8 pad_0x0[0xf9];
    u8 field_0xf9;
};

struct UnkBody {
    u8 pad_0x0[0x54];
    f32 field_0x54;
    u8 pad_0x58[0x1b0];
    UnkFlagged* field_0x208;
};

struct UnkTunables {
    u8 pad_0x0[0x7fc];
    f32 field_0x7fc;
};

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
    u8 pad_0x38[0x30];
    char* field_0x68;
    u32 field_0x70;
    u8 pad_0x74[0x20];
    u8 field_0x94;
    u8 pad_0x95[0x1];
};

struct UnkSelfFar {
    u8 pad_0x0[0x149d0];
    UnkTunables* field_0x149d0;
};

struct UnkManager;
extern UnkManager* g_unk_7101d47310;

s32 fn_71009e8590(UnkFlagged* object);
bool fn_71009e75e8(UnkFlagged* object);
bool fn_7100df5630(UnkManager* manager);
void fn_7100125b1c(f32* out, char* from, char* to);

// NON_MATCHING(minor): the two operands of the "mode matches" `and` are swapped (scheduling only)
bool fn_710049cbd8(UnkSelf* self) {
    UnkBody* body = reinterpret_cast<UnkBody*>(fn_7100d7c990(self->field_0x30));
    if (body && body->field_0x208 && body->field_0x208->field_0xf9 && fn_71009e8590(body->field_0x208) >= 1 &&
        (fn_71009e75e8(body->field_0x208) || fn_7100df5630(g_unk_7101d47310))) {
        u8 flag = self->field_0x94;
        u32 mode = self->field_0x70;
        bool clear = flag == 0;
        bool nonzero = mode != 0;
        u32 expected = nonzero & clear;
        if (mode != expected) {
            char* from = self->field_0x68;
            f32 out[6];
            fn_7100125b1c(out, from, mode == 1 ? from : from + 12);
            f32 below = out[1];
            if (body->field_0x54 - below >= reinterpret_cast<UnkSelfFar*>(self)->field_0x149d0->field_0x7fc)
                return true;
        }
    }
    return false;
}
