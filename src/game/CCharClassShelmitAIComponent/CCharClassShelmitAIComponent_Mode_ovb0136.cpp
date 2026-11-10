// CCharClassShelmitAIComponent's unit (tools/tu_map.py unit 1053): fn_71006b92b8, after the base character
// class's check (which wins when it answers), 2 when the actor's track allows crazy mode, 1 when it allows
// pushing, 0 when the info's name (its +0x60, +0x20) has the hash 0xae488c85e788b697, else 3 or 4 by the
// Shelmit AI's fn_71006bc178. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

u32 fn_7100414160();
struct UnkShelmitAI_sis;
s32 fn_71006bc178(UnkShelmitAI_sis*);

struct UnkShelmitNamed_ovb0136 {
    u8 pad_0x0[0x20];
    CStrId name;  // 0x20
};

struct UnkShelmitInfo_ovb0136 {
    u8 pad_0x0[0x60];
    UnkShelmitNamed_ovb0136* field_0x60;
};

static inline const CStrId& Name(const UnkShelmitInfo_ovb0136* info) {
    if (info) {
        if (UnkShelmitNamed_ovb0136* named = info->field_0x60)
            return named->name;
    }
    return g_unk_7101d470e0;
}

u32 fn_71006b92b8(void* self, UnkActor* actor, UnkActor* other, const UnkShelmitInfo_ovb0136* info) {
    if (u32 result = fn_7100414160())
        return result;
    UnkAnimationComponent* track = static_cast<UnkAnimationComponent*>(actor->field_0x220);
    CStrId name(Name(info));
    auto* ai = static_cast<base::reflection::UnkReflObject*>(actor->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_71006b98c0()))
        ai = nullptr;
    if (track && fn_710117b348(track, fn_7100ce9338()))
        return 2;
    if (track && fn_710117b348(track, fn_7100bfca98()))
        return 1;
    if (name.data != &g_unk_7101d47108->empty &&
        *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(name.data) + 0x30) == 0xae488c85e788b697)
        return 0;
    if (ai && !fn_71006bc178(reinterpret_cast<UnkShelmitAI_sis*>(ai)))
        return 4;
    return 3;
}
