// CCooldownXBossLaserBiteAttack's unit (tools/tu_map.py): fills in a request when the owner (+0x8) has an
// animation component (+0x220): flags +0x49 and +0xc8, a direction at +0x4c picked by the component's
// id-4 test (fn_7101178e6c) from two constants, and 1.0 at +0x50. Names and layouts are placeholders.

#include "game/CAnimationComponent.h"
#include "types.h"

struct UnkTakumakuComponent;

// As src/game/CBatalloonAIComponent/fn_71003de7b4.cpp declares them.
struct UnkVec3 {
    u32 a;
    u32 b;
    u32 c;
};
void fn_710007b444(void* dst, const UnkVec3* value);

extern const UnkVec3 g_unk_7101649a50 __attribute__((visibility("hidden")));
extern const UnkVec3 g_unk_7101649a5c __attribute__((visibility("hidden")));

struct UnkCooldownOwner_ovb0037 {
    u8 pad_0x0[0x220];
    UnkTakumakuComponent* anim;  // 0x220
};

struct UnkCooldownAttack_ovb0037 {
    u8 pad_0x0[0x8];
    UnkCooldownOwner_ovb0037* owner;  // 0x8
};

struct UnkCooldownRequest_ovb0037 {
    u8 pad_0x0[0x49];
    bool field_0x49;
    u8 pad_0x4a[0x2];
    u8 field_0x4c[0x4];  // a vector, 0x4c
    f32 field_0x50;
    u8 pad_0x54[0x74];
    u32 field_0xc8;
};
static_assert(__builtin_offsetof(UnkCooldownRequest_ovb0037, field_0xc8) == 0xc8, "layout");

void fn_71004c2aa8(UnkCooldownAttack_ovb0037* self, void*, UnkCooldownRequest_ovb0037* request) {
    UnkTakumakuComponent* anim = self->owner->anim;
    if (!anim)
        return;
    request->field_0x49 = true;
    request->field_0xc8 = 1;
    bool high = fn_7101178e6c(anim, 4);
    fn_710007b444(request->field_0x4c, high ? &g_unk_7101649a50 : &g_unk_7101649a5c);
    request->field_0x50 = 1.0f;
}
