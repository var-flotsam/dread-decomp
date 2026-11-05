// CKraidAIComponent's unit (tools/tu_map.py): an update of the block at +0x149a0. When the owner's
// character class (+0x220 of the owner at +0x30) holds, under the key 0x9317bfcf28edee2b, a value
// whose id (+0x38) contains the text of one of the ids g_unk_71726f3778, g_unk_71726f3780 or
// g_unk_71726f3788, and the class has a CCharClassKraidIgnoreMouthAdditiveActionsTrack, the block's
// first two words are cleared and the hash 0xdec1d0efc9e2415f is sent under that key with mode 1
// (as src/game_s2/UnkSpitSend.h). Then the timer at +0x4 counts down by `dt` and, when it runs out,
// fn_71005fe760 sends again. The ids' counts at +0x44 are raised and never lowered, as in the
// original. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

void* fn_7101177d2c(UnkAnimationComponent* self, u64 key);
struct UnkKraidAI_b56a3;
void fn_71005fe760(UnkKraidAI_b56a3* self);

struct UnkKraidMouthIdData_b63a4 {
    u8 pad_0x0[0x10];
    const char* text;  // a CRntString starts here
    u8 pad_0x18[0x2c];
    union {
        s32 count;  // a union member: the original reloads around its stores
    };
};

struct UnkKraidMouthId_b63a4 {
    UnkKraidMouthIdData_b63a4* data;
};

using base::global::CStrId;
extern CStrId g_unk_71726f3778 __attribute__((visibility("hidden")));  // as CKraidAIComponent_Send_b56.cpp
extern UnkKraidMouthId_b63a4 g_unk_71726f3780 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f3788 __attribute__((visibility("hidden")));

struct UnkKraidMouthValue_b63a4 {
    u8 pad_0x0[0x38];
    UnkKraidMouthId_b63a4 id;
};

struct UnkKraidMouthBlock_b63a4 {
    u64 field_0x0;  // cleared as one word with the timer
    u8 pad_0x8[0];
};

struct UnkKraidMouthAI_b63a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x149a0 - 0x38];
    u32 field_0x149a0;
    f32 timer;
};

__attribute__((always_inline)) static inline bool Contains_b63a4(UnkKraidMouthValue_b63a4* value,
                                                             const UnkKraidMouthId_b63a4& other) {
    ++value->id.data->count;
    UnkKraidMouthIdData_b63a4* d = value->id.data;
    if (d == reinterpret_cast<UnkKraidMouthIdData_b63a4*>(&g_unk_7101d47108->empty))
        return false;
    UnkKraidMouthIdData_b63a4* o = other.data;
    ++o->count;
    return fn_710007fb9c(reinterpret_cast<const base::global::CRntString*>(&d->text), o->text ? o->text : "");
}

void fn_71005fd5cc(UnkKraidMouthAI_b63a4* self, f32 dt) {
    UnkAnimationComponent* cls = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
    UnkKraidMouthValue_b63a4* value =
        static_cast<UnkKraidMouthValue_b63a4*>(fn_7101177d2c(cls, 0x9317bfcf28edee2b));
    if (Contains_b63a4(value, reinterpret_cast<const UnkKraidMouthId_b63a4&>(g_unk_71726f3778)) || Contains_b63a4(value, g_unk_71726f3780) ||
        Contains_b63a4(value, reinterpret_cast<const UnkKraidMouthId_b63a4&>(g_unk_71726f3788))) {
        if (fn_710117b348(cls, fn_7100c3a088())) {
            *reinterpret_cast<u64*>(&self->field_0x149a0) = 0;
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 1;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(cls, 0xdec1d0efc9e2415f, 0x9317bfcf28edee2b, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
        }
    }
    if (self->timer > 0.0f) {
        self->timer -= dt;
        if (self->timer <= 0.0f)
            fn_71005fe760(reinterpret_cast<UnkKraidAI_b56a3*>(self));
    }
}
