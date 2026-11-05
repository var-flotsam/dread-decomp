// CMissileBaseGun's unit (tools/tu_map.py unit 1309): when the game manager's current object
// (fn_7100df5460) is in state 6 (kind 7 takes it from its +0x20 object), the owner's (+0x18) part at
// +0x2a8 is flagged (+0x194) and the object's value at +0x3d8 is below 1, the owner's timeline gets a
// key with the all-flags mask; then, when the gun's amount (virtual slot 0x1e0) is used up, the menu's
// HUD gets "OnMissileEmptyPulse" (virtual slot 0x1e8) and the owner another key (fn_7100d7c828).
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

void* fn_7100df5460(UnkGameManager* manager);
extern u64 g_unk_710164ad18;
extern u64 g_unk_710164ad20;

struct UnkEmptySub_A5_b62 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkEmptyState_A5_b62 {
    u8 pad_0x0[0x20];
    UnkEmptySub_A5_b62* sub;
    u8 pad_0x28[0xb4 - 0x28];
    s32 kind;
    u8 pad_0xb8[0x3d8 - 0xb8];
    f32 field_0x3d8;
};

struct UnkEmptyPart_A5_b62 {
    u8 pad_0x0[0x194];
    bool field_0x194;
};

struct UnkEmptyOwner_A5_b62 {
    u8 pad_0x0[0x2a8];
    UnkEmptyPart_A5_b62* part;
    u8 pad_0x2b0[0x2e8 - 0x2b0];
    UnkTimelineComponent* timeline;
};

struct UnkEmptyHud_A5_b62 {
    void** vtable;
    void vf_0x1e8(const char* name, u32 arg) {
        reinterpret_cast<void (*)(UnkEmptyHud_A5_b62*, const char*, u32)>(vtable[0x1e8 / 8])(this, name, arg);
    }
};

struct UnkMissileBaseGun_A5_b62 {
    void** vtable;
    u8 pad_0x8[0x18 - 0x8];
    UnkEmptyOwner_A5_b62* owner;
    f32 vf_0x1e0() { return reinterpret_cast<f32 (*)(UnkMissileBaseGun_A5_b62*)>(vtable[0x1e0 / 8])(this); }
};

void fn_710084f904(UnkMissileBaseGun_A5_b62* self) {
    if (auto* state = static_cast<UnkEmptyState_A5_b62*>(fn_7100df5460(g_unk_7101d47310))) {
        s32 kind = state->kind;
        if (kind == 7) {
            if (!state->sub)
                goto empty;
            kind = state->sub->state;
        }
        if (kind == 6) {
            UnkEmptyPart_A5_b62* part = self->owner->part;
            if (part && part->field_0x194 && !(state->field_0x3d8 >= 1.0f)) {
                if (UnkTimelineComponent* timeline = self->owner->timeline) {
                    base::reflection::UnkReflTypeIdArg mask;
                    mask.id = UnkAll_SpitSend().value;
                    fn_7100a2cb7c(timeline, &g_unk_710164ad18, &mask.id, false, -1.0f);
                }
            }
        }
    }
empty:
    if (self->vf_0x1e0() <= 0.0f) {
        UnkIngameMenu* menu = fn_7100deac84(g_unk_7101d47310);
        if (auto* hud = reinterpret_cast<UnkEmptyHud_A5_b62*>(menu->field_0x2d0))
            hud->vf_0x1e8("OnMissileEmptyPulse", 0);
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164ad20, false);
    }
}
