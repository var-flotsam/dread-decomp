// CSensorDoorComponent's unit (tools/tu_map.py): fn_71007b877c and fn_71007b8adc (the latter only
// while the flag at +0x1e8 is clear), when the owner (+0x30) has its sub-object at +0x220 and
// fn_710117ae38 says no for "open"/"opened" (or "close"/"closed"), send it "open" (or "close") with
// mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h), with second descriptor g_unk_7172769794 instead
// when fn_710117ae38 says yes for the other one; then give the owner's weak target (fn_71002b3b84) a
// hash (fn_71009c0f1c), clear +0x1ec and set +0x1e9. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

extern UnkDesc_SpitSend g_unk_7172769794;  // reached through the GOT

struct UnkWeakTarget;
void* fn_71002b3b84(UnkWeakTarget*);
void fn_71009c0f1c(void* object, u64 id);

struct UnkSensorDoorOwner_b57 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkSensorDoor_b57 {
    u8 pad_0x0[0x30];
    UnkSensorDoorOwner_b57* owner;
    u8 pad_0x38[0x1e8 - 0x38];
    bool field_0x1e8;
    bool field_0x1e9;
    u8 pad_0x1ea[2];
    u32 field_0x1ec;
};

template <u32 NA, u32 NB, u32 NC>
static inline __attribute__((always_inline)) void Send_b57(UnkSensorDoor_b57* self, const char (&a)[NA],
                                                           const char (&b)[NB], const char (&c)[NC],
                                                           u64 weak_hash) {
    UnkAnimationComponent* target = self->owner->field_0x220;
    if (!target)
        return;
    if (fn_710117ae38(target, base::global::CName(a).hash, false))
        return;
    if (fn_710117ae38(target, base::global::CName(b).hash, false))
        return;
    u64 hash = base::global::CName(a).hash;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    UnkDesc_SpitSend* desc =
        fn_710117ae38(target, base::global::CName(c).hash, false) ? &g_unk_7172769794 : &g_unk_7172769740;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, desc, &mask);
    if (void* weak = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(weak, weak_hash);
    self->field_0x1ec = 0;
    self->field_0x1e9 = true;
}

void fn_71007b877c(UnkSensorDoor_b57* self) {
    if (!self->owner)
        return;
    Send_b57(self, "open", "opened", "close", 0x6909d27d2c787fba);
}

void fn_71007b8adc(UnkSensorDoor_b57* self) {
    if (self->field_0x1e8 || !self->owner)
        return;
    Send_b57(self, "close", "closed", "open", 0xe43bbd07b9e307b9);
}
