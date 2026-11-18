// CCharClassLiquidPoolComponent's unit (tools/tu_map.py unit 1394): the player touches the pool (after the base
// trigger's handler, fn_7100a45f04). The player info's two flag lists are told whether the player is in morph
// ball or the pool answers to two ids, and the surface ripple is placed at the player's x by how deep the
// player stands (under 100: mode 2, under 300: mode 3). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CLiquidSimulationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CHeatRoomComponent/CHeatRoomComponent_Parts_ovb0083.cpp declares it.
struct UnkHeatRoom_ovb0083;
struct UnkHeatActor_ovb0083;
void fn_7100a45f04(UnkHeatRoom_ovb0083* self, UnkHeatActor_ovb0083* actor);

// As src/game/CPlayerInfo/CPlayerInfo_SetFlag_A6_b72.cpp defines it.
struct UnkFlagList_A6_b72;
void fn_7100f0de24(UnkFlagList_A6_b72* self, void* key, bool flag);
void fn_7100f0dea4(UnkFlagList_A6_b72* self, void* key, bool flag, u32 value);

// The id table's string id at +0xae8 (UnkGameManagerIdTable has no member for it yet).
static inline const CStrId& LiquidId_l901a98() {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const u8*>(&GameManagerIdTable()) + 0xae8);
}

// The movement if it is a CMorphBallMovement, else null.
static inline base::reflection::UnkReflObject* AsMorphBallMovement(base::reflection::UnkReflObject* movement) {
    if (!movement)
        return nullptr;
    return fn_710009d50c(movement->vf_0x0(), fn_710093b204()) ? movement : nullptr;
}

struct UnkLiquidPoolToucher_l901a98 {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;  // 0x1b5
    u8 pad_0x1b6[0x3a];
    base::reflection::UnkReflObject* movement;  // 0x1f0
};
static_assert(__builtin_offsetof(UnkLiquidPoolToucher_l901a98, movement) == 0x1f0, "layout");

struct UnkLiquidPoolFx_l901a98 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkLiquidPoolTouch_l901a98 {
    void** vtable;
    u8 pad_0x8[0xc04];
    u32 field_0xc0c;
    u8 pad_0xc10[0x78];
    UnkLiquidPoolFx_l901a98* fx;  // 0xc88

    UnkVec3f vf_0x178() { return reinterpret_cast<UnkVec3f (*)(UnkLiquidPoolTouch_l901a98*)>(vtable[0x178 / 8])(this); }
    UnkVec3f vf_0x180() { return reinterpret_cast<UnkVec3f (*)(UnkLiquidPoolTouch_l901a98*)>(vtable[0x180 / 8])(this); }
    bool vf_0x188(const CStrId* id, const void* extra) {
        return reinterpret_cast<bool (*)(UnkLiquidPoolTouch_l901a98*, const CStrId*, const void*)>(
            vtable[0x188 / 8])(this, id, extra);
    }

    // The height of the pool's surface: its centre plus half its extent.
    f32 SurfaceTop() {
        UnkVec3f pos = vf_0x180();
        f32 centre = pos.y;
        UnkVec3f ext = vf_0x178();
        return centre + ext.y * 0.5f;
    }

    // Places the surface ripple at `height` across the pool (fn_7100901900, inlined).
    void SetRatio(const f32* height, u32 a, bool b) {
        f32 h = *height;
        UnkVec3f pos = vf_0x180();
        f32 base = pos.x;
        UnkVec3f ext = vf_0x178();
        f32 extent = ext.x;
        UnkVec3f size = vf_0x178();
        if (fx != nullptr && fx->field_0x10 != nullptr) {
            f32 width = size.x;
            f32 ratio = (h - (base + extent * -0.5f)) / width;
            fn_7100905d8c(fx->field_0x10, ratio, a, b);
        }
    }
};
static_assert(__builtin_offsetof(UnkLiquidPoolTouch_l901a98, fx) == 0xc88, "layout");

void fn_7100901a98(UnkLiquidPoolTouch_l901a98* self, UnkLiquidPoolToucher_l901a98* actor) {
    fn_7100a45f04(reinterpret_cast<UnkHeatRoom_ovb0083*>(self), reinterpret_cast<UnkHeatActor_ovb0083*>(actor));
    if (actor->field_0x1b5)
        return;
    if (!fn_7100df5c38(g_unk_7101d47310, actor))
        return;

    base::reflection::UnkReflObject* morph_ball = AsMorphBallMovement(actor->movement);
    UnkActor* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310));
    UnkFlagList_A6_b72* flags =
        reinterpret_cast<UnkFlagList_A6_b72*>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x38);

    bool first = self->vf_0x188(&LiquidId_l901a98(), nullptr);
    fn_7100f0de24(flags, self, morph_ball != nullptr || first);
    bool second = first ? self->vf_0x188(&GameManagerIdTable().field_0x1650, nullptr) : false;
    fn_7100f0dea4(flags, self, morph_ball != nullptr || second, self->field_0xc0c);

    f32 y = player->field_0x54;
    f32 x = player->field_0x44;
    f32 depth = self->SurfaceTop() - y;
    if (depth < 100.0f)
        self->SetRatio(&x, 2, false);
    else if (depth < 300.0f)
        self->SetRatio(&x, 3, false);
}
