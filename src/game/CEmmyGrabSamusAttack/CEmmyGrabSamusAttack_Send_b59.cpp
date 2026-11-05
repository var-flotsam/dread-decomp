// CEmmyGrabSamusAttack unit (tools/tu_map.py): fn_710050a500 sends the id `id` (mode 0) to the owner's
// (+0x8) animation (+0x220); then, under kind 0x7c5476647f60cdaa, the same id or, when the owner's AI
// component (+0x240) is a CEmmyAIComponent in state 0 (+0x29ef0), the hash of the id's string followed by
// g_unk_71726de700; when the first send went through, also sends the id to the player's animation and
// returns that send's result. Descriptors as UnkSpitSend (src/game_s2/UnkSpitSend.h). Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CRntString;

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
u64* fn_7100066630(u64* out, const CRntString* str);
extern CRntString g_unk_71726de700 __attribute__((visibility("hidden")));

// The pooled data of an id: the string at +0x10 (its key at +0x30 is the hash) and a counter at +0x44
// bumped whenever the string is read (as in src/game/CEmmyGrabSamusAttack/fn_71005077f8.cpp).
struct UnkGrabIdData_b59a3 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    union {  // keeps the reload of the id's data the original has after the bump
        s32 field_0x44;
    };
};

struct UnkGrabId_b59a3 {
    UnkGrabIdData_b59a3* data;

    u64 Hash() const { return static_cast<u64>(data->str.key); }
    const CRntString& Str() const {
        ++data->field_0x44;
        return data->str;
    }
};

struct UnkGrabAI_b59a3 {
    void** vtable;
    u8 pad_0x8[0x29ef0 - 0x8];
    s32 field_0x29ef0;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkGrabAI_b59a3*)>(vtable[0])(this);
    }
};

struct UnkGrabAttack_b59a3 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

bool fn_710050a500(UnkGrabAttack_b59a3* self, const UnkGrabId_b59a3* id) {
    void* target = self->owner->field_0x220;
    void* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310))->field_0x220;
    base::reflection::UnkReflTypeIdArg mode1;
    base::reflection::UnkReflTypeIdArg mask1;
    u64 hash1 = id->Hash();
    mode1.id = 0;
    mask1.id = UnkAll_SpitSend().value;
    bool sent = fn_7101176690(target, hash1, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740,
                              &mask1);
    auto* ai = static_cast<UnkGrabAI_b59a3*>(self->owner->field_0x240);
    // The gotos skip the declarations below, so they get no lifetime markers and keep slots of their own.
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710053b934()) || ai->field_0x29ef0 != 0)
        goto plain;
    u64 hash;
    {
        CRntString s = fn_710007eee0(&id->Str(), &g_unk_71726de700);
        fn_7100066630(&hash, &s);
    }
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
    base::reflection::UnkReflTypeIdArg mode3;
    base::reflection::UnkReflTypeIdArg mask3;
    base::reflection::UnkReflTypeIdArg mode4;
    base::reflection::UnkReflTypeIdArg mask4;
    {
        u64 hash2 = hash;
        mode2.id = 0;
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(target, hash2, 0x7c5476647f60cdaa, &mode2, &g_unk_710164b958, &g_unk_7172769740, &mask2);
    }
    goto player;
plain : {
    u64 hash3 = id->Hash();
    mode3.id = 0;
    mask3.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash3, 0x7c5476647f60cdaa, &mode3, &g_unk_710164b958, &g_unk_7172769740, &mask3);
}
player:
    if (!sent)
        return false;
    {
        u64 hash4 = id->Hash();
        mode4.id = 0;
        mask4.id = UnkAll_SpitSend().value;
        return fn_7101176690(player, hash4, 0xf4061ff25a2163d6, &mode4, &g_unk_710164b958, &g_unk_7172769740,
                             &mask4);
    }
}
