// CAbilityGhostAura unit: whether the aura can be used now. False without an owner actor, while its track
// ignores the aura, while a grab is held, outside the world's allowed state, or while the world's mode
// forbids it; otherwise decided by the state at +0x497 and its floats. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CGrabComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

extern base::reflection::UnkTunable g_unk_7172a03d80;

struct UnkAuraIgnoreRecord_l30bf8c {
    u8 pad_0x0[0x18];
    bool ignore;
};

struct UnkAuraMode_l30bf8c {
    u8 pad_0x0[0x10];
    s32 mode;
    u8 pad_0x14[0x1b4 - 0x14];
    f32 field_0x1b4;
};

struct UnkAuraWorld_l30bf8c {
    u8 pad_0x0[0x7e4];
    u8 field_0x7e4;
    u8 pad_0x7e5[0x2d08 - 0x7e5];
    UnkAuraMode_l30bf8c* mode;
};

struct UnkAuraActor_l30bf8c {
    u8 pad_0x0[0x1f8];
    u8* field_0x1f8;
    u8 pad_0x200[8];
    UnkAuraWorld_l30bf8c* world;
    u8 pad_0x210[0x220 - 0x210];
    UnkAnimationComponent* character_class;
    u8 pad_0x228[0x260 - 0x228];
    void* grab;
};

struct UnkAuraOwner_l30bf8c {
    u8 pad_0x0[0x30];
    UnkAuraActor_l30bf8c* actor;
    u8 pad_0x38[0xe8 - 0x38];
    void* field_0xe8;
};

struct UnkAuraRange_l30bf8c {
    u8 pad_0x0[0x40];
    f32 low;
    f32 high;
};

struct UnkAuraTarget_l30bf8c {
    u8 pad_0x0[0x10];
    UnkAuraRange_l30bf8c* range;
};

struct UnkAbility {
    u8 pad_0x0[0xc0];
    UnkAuraOwner_l30bf8c* owner;
    u8 pad_0xc8[0x47c - 0xc8];
    f32 field_0x47c;
    u8 pad_0x480[0x497 - 0x480];
    u8 field_0x497;
};

bool fn_710030bf8c(UnkAbility* self) {
    if (self->owner == nullptr || self->owner->actor == nullptr || self->owner->actor->world == nullptr)
        return false;
    UnkAuraActor_l30bf8c* actor = self->owner->actor;
    UnkAuraWorld_l30bf8c* world = actor->world;
    UnkTrackQuery query = {fn_7100d0b788(), nullptr, -1.0f};
    fn_710117b08c(actor->character_class, &query);
    UnkAuraIgnoreRecord_l30bf8c* record = reinterpret_cast<UnkAuraIgnoreRecord_l30bf8c*>(query.result);
    bool ignored = false;
    if (record != nullptr)
        ignored = record->ignore;
    if (ignored)
        return false;
    if (actor->character_class == nullptr)
        return false;
    if (actor->grab != nullptr && fn_710113d83c(actor->grab))
        return false;
    if (!reinterpret_cast<const u8*>(&g_unk_7172a03d80)[0x3d]) {
        if (world->field_0x7e4 != 0 && !fn_7100df5740(g_unk_7101d47310))
            return false;
    }
    UnkAuraMode_l30bf8c* mode_state = world->mode;
    s32 mode = mode_state->mode;
    if (mode_state != nullptr && mode == 3 && mode_state->field_0x1b4 >= 0.0f)
        return false;
    if (self->owner->field_0xe8 == nullptr)
        return false;
    UnkAuraTarget_l30bf8c* target = static_cast<UnkAuraTarget_l30bf8c*>(self->owner->field_0xe8);
    if (target->range == nullptr)
        return false;
    if (!self->field_0x497)
        return true;
    if (!(target->range->high < target->range->low))
        return true;
    if (mode == 13)
        return true;
    if (actor->field_0x1f8[0x3a8] != 0)
        return true;
    return self->field_0x47c > 0.0f;
}
