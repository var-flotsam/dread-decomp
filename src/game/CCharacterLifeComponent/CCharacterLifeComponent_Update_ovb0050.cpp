// CCharacterLifeComponent unit (tools/tu_map.py unit 2713): the update. After the base update
// (fn_7101160078), a character whose state (+0x70) isn't 2 and whose life (+0x54) has dropped below
// FLT_EPSILON dies: unless fn_7101160408 holds, it builds a damage request (fn_71008c0c74) and hands it
// to its own vf_0x1d0 with kind 2. Names and layouts are placeholders.
#include "game/CLifeComponent.h"
#include "game/UnkTU1370.h"
#include "types.h"

struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

struct UnkLifeRequest_b0050 {
    u8 raw[0x100];
    UnkLifeRequest_b0050() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkLifeRequest_b0050() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};

struct UnkCharacterLife_b0050 {
    void** vtable;
    u8 pad_0x8[0x4c];
    f32 life;  // 0x54
    u8 pad_0x58[0x18];
    s32 state;  // 0x70

    void vf_0x1d0(s32 kind, UnkLifeRequest_b0050* request) {
        reinterpret_cast<void (*)(UnkCharacterLife_b0050*, s32, UnkLifeRequest_b0050*)>(vtable[0x1d0 / 8])(
            this, kind, request);
    }
};

struct UnkCommanderLife_A5_b63;

void fn_7101158cb8(UnkCharacterLife_b0050* self, f32 dt) {
    fn_7101160078(reinterpret_cast<UnkCommanderLife_A5_b63*>(self), dt);
    if (self->state != 2 && self->life < 1.1920929e-7f && !fn_7101160408(self)) {
        UnkLifeRequest_b0050 request;
        self->vf_0x1d0(2, &request);
    }
}
