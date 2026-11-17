// Unit 1444 of tools/tu_map.py: plays the owner's animation action that a string id names: tells the AI
// first (keeping its answer in the stored object), then picks the action's variant by `flag` and the unit's
// flags. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU376.h"
#include "types.h"

using base::global::CStrId;

struct UnkS16;
struct UnkS12;
bool fn_7101177258(UnkAnimationComponent* self, void* id, u64 variant, u64 key, const u32& flags, const UnkS16& a,
                   const UnkS12& b, const u32& c);

struct UnkAI_l972fd8 {
    void** vtable;

    void Ask(UnkObject_0x100* out, const CStrId* name) {
        reinterpret_cast<void (*)(UnkAI_l972fd8*, UnkObject_0x100*, const CStrId*)>(vtable[0x5a0 / 8])(this, out,
                                                                                                    name);
    }
};

struct UnkVariant_l972fd8 {
    u8 raw[0x120];
};

// An action's variants, found by key.
struct UnkAction_l972fd8 {
    u8 pad_0x0[0x80];
    UnkVariant_l972fd8* variants;  // 0x80
    u64* keys;                     // 0x88
    u32 count;                     // 0x90

    UnkVariant_l972fd8* End() { return &variants[count]; }
    UnkVariant_l972fd8* Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &variants[i];
        return End();
    }
    UnkVariant_l972fd8* Get(u64 key) {
        UnkVariant_l972fd8* it = Find(key);
        return it == End() ? nullptr : it;
    }
};

struct UnkOwner_l972fd8 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkAI_l972fd8* ai;  // 0x240
};

struct UnkUnit1444_l972fd8 {
    u8 pad_0x0[0x30];
    UnkOwner_l972fd8* owner;  // 0x30
    u8 pad_0x38[0xd0 - 0x38];
    UnkObject_0x100 stored;  // 0xd0
    bool has_stored;         // 0x1d0
    bool field_0x1d1;
    bool field_0x1d2;
    u8 pad_0x1d3[0x1ff - 0x1d3];
    bool field_0x1ff;
};
static_assert(__builtin_offsetof(UnkUnit1444_l972fd8, field_0x1ff) == 0x1ff, "layout");

static const u64 kTrack_l972fd8 = 0xf4061ff25a2163d6;

bool fn_7100972fd8(UnkUnit1444_l972fd8* self, const CStrId& name, bool flag) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return false;
    CStrId id(name);
    if (id.data == &g_unk_7101d47108->empty)
        return false;
    // The AI's answer: its storage lives as long as the function, built only when there is an AI.
    alignas(8) u8 answer_raw[0x100];
    UnkObject_0x100* answer = reinterpret_cast<UnkObject_0x100*>(answer_raw);
    if (UnkAI_l972fd8* ai = self->owner->ai) {
        fn_71002b57bc(answer);
        if (self->has_stored)
            fn_71002b5c90(answer, &self->stored);
        ai->Ask(answer, &id);
        fn_71002b5c90(&self->stored, answer);
        self->has_stored = true;
        fn_71002b6094(answer);
    }
    u32 mode;
    if (fn_71011782ec(animation, kTrack_l972fd8) || fn_710117ae38(animation, id.data->Key(), false))
        mode = 10;
    else
        mode = 8;
    bool alternate = self->field_0x1d2 ? self->field_0x1d1 : false;
    UnkAction_l972fd8* action = static_cast<UnkAction_l972fd8*>(fn_7101176d34(
        reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation), reinterpret_cast<void*>(id.data->Key())));
    base::reflection::UnkReflTypeIdArg mode1, mask1;
    base::reflection::UnkReflTypeIdArg mode2, mask2;
    base::reflection::UnkReflTypeIdArg mode3, mask3;
    base::reflection::UnkReflTypeIdArg mode4, mask4;
    UnkVariant_l972fd8* first = nullptr;
    UnkVariant_l972fd8* second = nullptr;
    UnkVariant_l972fd8* third = nullptr;
    if (action) {
        first = action->Get(0x4f403ef0516a1314);
        second = action->Get(0x812f56e120664fcf);
        third = action->Get(0xf3f8923042110a1e);
    }
    if (flag) {
        if (alternate && first) {
            mode1.id = mode;
            mask1.id = UnkAll_SpitSend().value;
            bool played = fn_7101177258(animation, reinterpret_cast<void*>(id.data->Key()), 0x4f403ef0516a1314,
                                        kTrack_l972fd8, mode1.id, reinterpret_cast<const UnkS16&>(g_unk_710164b968),
                                        reinterpret_cast<const UnkS12&>(g_unk_7172769740), mask1.id);
            self->field_0x1ff = played;
            return played;
        }
        if (second) {
            mode2.id = mode;
            mask2.id = UnkAll_SpitSend().value;
            return fn_7101177258(animation, reinterpret_cast<void*>(id.data->Key()), 0x812f56e120664fcf,
                                 kTrack_l972fd8, mode2.id, reinterpret_cast<const UnkS16&>(g_unk_710164b968),
                                 reinterpret_cast<const UnkS12&>(g_unk_7172769740), mask2.id);
        }
    }
    if (alternate && third) {
        mode3.id = mode;
        mask3.id = UnkAll_SpitSend().value;
        bool played = fn_7101177258(animation, reinterpret_cast<void*>(id.data->Key()), 0xf3f8923042110a1e,
                                    kTrack_l972fd8, mode3.id, reinterpret_cast<const UnkS16&>(g_unk_710164b958),
                                    reinterpret_cast<const UnkS12&>(g_unk_7172769740), mask3.id);
        self->field_0x1ff = played;
        return played;
    }
    mode4.id = mode;
    mask4.id = UnkAll_SpitSend().value;
    return fn_7101176690(animation, id.data->Key(), kTrack_l972fd8, &mode4, &g_unk_710164b958, &g_unk_7172769740,
                         &mask4);
}
