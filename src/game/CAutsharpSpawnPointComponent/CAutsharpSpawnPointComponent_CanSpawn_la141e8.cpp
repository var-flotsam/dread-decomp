// CAutsharpSpawnPointComponent unit (tools/tu_map.py unit 1525): whether the spawn point fires now. The base
// check must pass, the player must stand inside the linked area and on the configured side of the owner (by
// twice the player's radius plus 50), with no cutscene or effect running; then the owner's sub-object at +0x220
// is sent the spawn request and +0x400 is set. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkRefHandle.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU365.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The area the spawn point is linked to: vtable slot 0x50 tests a point.
struct UnkAutsharpArea_la141e8 {
    void** vtable;

    bool Contains(const CVector2D* point) {
        return reinterpret_cast<bool (*)(UnkAutsharpArea_la141e8*, const CVector2D*)>(vtable[0x50 / 8])(this, point);
    }
};

struct UnkAutsharpTracker_la141e8 {
    u8 pad_0x0[0x30];
    UnkAutsharpArea_la141e8* area;  // 0x30
};

// What the link handle refers to: a counted reference to the linked object.
struct UnkAutsharpLinkRef_la141e8 : base::UnkRefCounted {
    void* object;  // 0x10
};

struct UnkAutsharpSpawn_la141e8 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x3b8 - 0x38];
    s32 side;  // 0x3b8: 0 the player left of the owner, 1 right of it, 2 not beside it
    u8 pad_0x3bc[0x3c8 - 0x3bc];
    u8 link_source[0x28];  // 0x3c8
    s32 link_state;        // 0x3f0
    u8 pad_0x3f4[0x4];
    base::UnkRefHandle link;  // 0x3f8
    bool sent;                // 0x400
};
static_assert(__builtin_offsetof(UnkAutsharpSpawn_la141e8, side) == 0x3b8, "layout");
static_assert(__builtin_offsetof(UnkAutsharpSpawn_la141e8, link) == 0x3f8, "layout");
static_assert(__builtin_offsetof(UnkAutsharpSpawn_la141e8, sent) == 0x400, "layout");

bool fn_710112a558(UnkAutsharpSpawn_la141e8* self);

// The tracker of the linked object, the handle to it fetched on first use; null without a link.
static inline UnkTracker* LinkedTracker(UnkAutsharpSpawn_la141e8* self) {
    if (self->link_state == 0)
        return nullptr;
    if (self->link_state == 1) {
        {
            UnkRefPtr fetched = fn_710023b580(self->link_source);
            fn_7100234f78(&self->link, reinterpret_cast<const base::UnkRefHandle*>(&fetched));
        }
        self->link_state = 2;
    }
    base::UnkRefCounted* ref = self->link.ptr;
    return fn_710028aca8(ref ? static_cast<UnkAutsharpLinkRef_la141e8*>(ref)->object : nullptr);
}

// How far beside the owner the player must be: twice its radius and 50.
static inline f32 Margin(UnkActor* player) {
    return player->field_0x37c + player->field_0x37c + 50.0f;
}

bool fn_7100a141e8(UnkAutsharpSpawn_la141e8* self) {
    bool ready = fn_710112a558(self);
    UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    UnkTracker* tracker = LinkedTracker(self);
    UnkAutsharpArea_la141e8* area =
        tracker ? reinterpret_cast<UnkAutsharpTracker_la141e8*>(tracker)->area : nullptr;
    if (!ready)
        return false;

    f32 x = player->field_0x44;
    f32 y = player->field_0x54;
    f32 z = player->field_0x64;
    CVector3D position = {x, y, z};
    CVector2D point;
    fn_710007b430(&point, &position);
    if (!area->Contains(&point))
        return false;

    switch (self->side) {
    case 0:
        if (self->owner->field_0x44 - Margin(player) < player->field_0x44)
            return false;
        break;
    case 1:
        if (self->owner->field_0x44 + Margin(player) > player->field_0x44)
            return false;
        break;
    case 2:
        if (player->field_0x44 < self->owner->field_0x44 + Margin(player) &&
            player->field_0x44 > self->owner->field_0x44 - Margin(player))
            return false;
        break;
    }

    if (IsOpticCamouflageAbilityInState6(g_unk_7101d47310))
        return false;
    if (fn_7101053a7c(g_unk_7101d47310->field_0x1040))
        return false;
    if (!fn_7100d7bea0(self->owner, 0))
        return false;
    void* target = self->owner->field_0x220;
    if (!target)
        return true;
    // UnkSpitSend written out, so that the point keeps the top stack slot.
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(target, 0xa0e33be014f44bd7, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                  &mask);
    self->sent = true;
    return true;
}
