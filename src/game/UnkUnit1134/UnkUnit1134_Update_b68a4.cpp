// Unit 1134 of tools/tu_map.py (no name hint). fn_710070d970: one update step. Nothing happens without a
// player (+0x28) or once it is done (fn_7100c95ed8); while the member at +0xb0 is running
// (fn_710002fb70) it is advanced instead; otherwise the player's current entry (+0x10) and the value at
// +0x64 go to fn_710070ddf8, and the player, held by a counted reference, is given the member at +0x38.
// Names and layouts are placeholders.
#include "game/CActionInstance.h"
#include "game/UnkTU39.h"
#include "types.h"

struct UnkPlayer_b42;

struct UnkUnit1134Player_b68a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
    void* current;  // 0x10
    u8 pad_0x18[0x4c];
    f32 field_0x64;
};

struct UnkUnit1134Upd_b68a4 {
    u8 pad_0x0[0x28];
    UnkUnit1134Player_b68a4* player;  // 0x28
    u8 pad_0x30[0x8];
    u8 field_0x38[0x78];
    u8 field_0xb0[0x8];
};

void fn_710070ddf8(UnkUnit1134Upd_b68a4* self, void* entry, f32 value, f32 dt);
void fn_7100c96870(UnkUnit1134Player_b68a4* player, void* arg);

void fn_710070d970(UnkUnit1134Upd_b68a4* self, f32 dt) {
    UnkUnit1134Player_b68a4* player = self->player;
    if (!player || reinterpret_cast<bool (*)(UnkUnit1134Player_b68a4*)>(fn_7100c95ed8)(player))
        return;
    if (fn_710002fb70(self->field_0xb0)) {
        fn_710002fb80(self->field_0xb0, dt);
        return;
    }
    fn_710070ddf8(self, self->player->current, self->player->field_0x64, dt);
    UnkUnit1134Player_b68a4* held = self->player;
    ++held->ref_count;
    fn_7100c96870(self->player, self->field_0x38);
    if (--held->ref_count == 0)
        held->vf_0x10();
}
