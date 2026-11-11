// CCharClassLockOnMissileMovement's unit (tools/tu_map.py): fn_7100ab6bb8 hands the lock-on target at +0x858
// back to the player's MultiLockon ability (looked up by a function-local static id, checked against the
// CAbilityMultiLockon class) and clears it; fn_7100ab6828 runs the base initialiser, keeps the character class
// when it is a CCharClassLockOnMissileMovement and copies its speed values, then creates the trail effect at
// +0x860. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkLockOnPlayer_ovb0121 {
    u8 pad_0x0[0x2e0];
    void* abilities;  // 0x2e0
};

struct UnkLockOnMovement_ovb0121 {
    u8 pad_0x0[0x858];
    void* lock;  // 0x858
};

struct UnkAbility;
struct UnkAbilitySet;
// The ability named by the id `id` points at (src/game/CProjectileMovement/CProjectileMovement_HitAbility_b46.cpp).
void fn_710031c7b8(UnkReflObject* ability, void* lock);

void fn_7100ab6bb8(UnkLockOnMovement_ovb0121* self) {
    if (!self->lock)
        return;
    auto* player = reinterpret_cast<UnkLockOnPlayer_ovb0121*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    void* abilities = player->abilities;
    if (!abilities)
        return;
    static CStrId s_name("MultiLockon", true);
    auto* ability = reinterpret_cast<UnkReflObject*>(
        fn_71003012f8(static_cast<UnkAbilitySet*>(abilities), reinterpret_cast<u64>(&s_name)));
    if (!ability)
        return;
    if (!fn_710009d50c(ability->vf_0x0(), fn_7100316638()))
        return;
    fn_710031c7b8(ability, self->lock);
    self->lock = nullptr;
}

using base::reflection::CType;

extern CType g_unk_7172754a80 __attribute__((visibility("hidden")));  // the class's type
extern const u64 g_unk_7172754a68[] __attribute__((visibility("hidden")));

void* fn_7100149930(UnkRenderPool* pool, void* a, UnkSceneObject** out, base::global::CStrId* name);
void* fn_71001be020(UnkSceneObjectSub* target, const u64* key);

struct UnkLockOnCharClass_ovb0121 {
    u8 pad_0x0[0xd4];
    u32 field_0xd4;
    u8 pad_0xd8[0x134 - 0xd8];
    u32 field_0x134;
    u32 field_0x138;
    u32 field_0x13c;
};

struct UnkLockOnInit_ovb0121 {
    u8 pad_0x0[0x10];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x571 - 0x18];
    bool field_0x571;
    u8 pad_0x572[0x620 - 0x572];
    UnkLockOnCharClass_ovb0121* field_0x620;
    u8 pad_0x628[0x664 - 0x628];
    u32 field_0x664;
    u32 field_0x668;
    u32 field_0x66c;
    u32 field_0x670;
    u32 field_0x674;
    u8 pad_0x678[0x860 - 0x678];
    UnkSceneObject* effect;  // 0x860
};
static_assert(__builtin_offsetof(UnkLockOnInit_ovb0121, effect) == 0x860, "layout");

bool fn_7100aba508(UnkLockOnInit_ovb0121* self);

static inline UnkLockOnCharClass_ovb0121* AsLockOn(UnkReflObject* obj) {
    if (!obj)
        return nullptr;
    const CType* type = obj->vf_0x0();
    fn_7100ab4bc0();
    return fn_710009d50c(type, &g_unk_7172754a80) ? reinterpret_cast<UnkLockOnCharClass_ovb0121*>(obj) : nullptr;
}

bool fn_7100ab6828(UnkLockOnInit_ovb0121* self) {
    bool ok = fn_7100aba508(self);
    if (UnkLockOnCharClass_ovb0121* cls = AsLockOn(self->char_class)) {
        self->field_0x620 = cls;
        self->field_0x664 = cls->field_0x138;
        self->field_0x668 = cls->field_0x13c;
        self->field_0x66c = cls->field_0x134;
        self->field_0x674 = cls->field_0x134;
        self->field_0x670 = cls->field_0xd4;
    } else {
        self->field_0x620 = nullptr;
    }
    self->field_0x571 = true;
    if (!self->effect) {
        UnkRenderManager* manager = g_unk_7101d47300;
        UnkSceneObject* effect = nullptr;
        fn_7100149930(manager->field_0x218, nullptr, &effect, &g_unk_7101d470e0);
        fn_7100219ae0(manager, effect ? reinterpret_cast<UnkSceneNode*>(reinterpret_cast<char*>(effect) + 8) : nullptr, 0);
        self->effect = effect;
        if (effect) {
            fn_71001be020(effect->field_0x130, g_unk_7172754a68);
            fn_7100149490(self->effect, false);
        }
    }
    return ok;
}
