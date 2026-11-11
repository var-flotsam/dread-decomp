// CDoorCentralUnitLifeComponent's unit: outside state 1 (+0xc08), opens the door part (fn_71008ca700 with false)
// and sends the game manager the event "CENTRAL_UNIT_DOOR_IMPACT" with a zeroed argument (fn_7100e038b0, as
// src/game/CDoorGrapplePointComponent/CDoorGrapplePointComponent_Attach_A5_b66.cpp). Returns false. Names are
// placeholders.
#include "base/global/CStrId.h"
#include "game/CDoorCentralUnitLifeComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkCentralUnitDoor_b46a2;

// A 12-byte zeroed argument (as src/game_s2/fn_710035131c.cpp's).
struct UnkVec {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};
void fn_7100e038b0(UnkGameManager* dispatcher, void* key, UnkVec* vec, u64 arg);

struct UnkDoorCentralUnitLife_A5_b68 {
    u8 pad_0x0[0xc08];
    s32 field_0xc08;
};

bool fn_71008ca8ac(UnkDoorCentralUnitLife_A5_b68* self) {
    if (self->field_0xc08 != 1) {
        fn_71008ca700(reinterpret_cast<UnkCentralUnitDoor_b46a2*>(self), false);
        CStrId id("CENTRAL_UNIT_DOOR_IMPACT", true);
        UnkGameManager* gm = g_unk_7101d47310;
        UnkVec vec;
        vec.field_0x0 = 0;
        vec.field_0x4 = 0;
        vec.field_0x8 = 0;
        fn_7100e038b0(gm, &id, &vec, 0);
    }
    return false;
}
