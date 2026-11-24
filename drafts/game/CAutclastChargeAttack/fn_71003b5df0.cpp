// CAutclastChargeAttack (tools/tu_map.py): the attack's start. After the base start (fn_71003608ac), the
// list of charge entries (+0x90) is sized to twice the Autclast tunable's count (+0x24), new entries
// unset (x -1), then the owner's movement (+0x218) and animation (+0x298) get their parameter blocks and
// the state (+0x30) is cleared. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game/behaviortree/CSetAnimDependsOnDirAction.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTunable;
extern UnkTunable g_unk_71726bd2c8;  // CTunableCharClassAutclastAIComponent

struct UnkAutclastTunable_l3b5df0 {
    u8 pad_0x0[0x24];
    u32 charge_count;  // 0x24
};

// A charge entry: a point (x -1 while unset), six words and a counted reference. The constructor leaves
// two words unset; a value-initialised entry has them zero.
struct UnkChargeEntry_l3b5df0 {
    base::math::CVector2D pos = {-1.0f, 0.0f};
    u32 field_0x8;
    u32 field_0xc = 0;
    u32 field_0x10 = 0;
    u32 field_0x14 = 0;
    u32 field_0x18 = 0;
    u32 field_0x1c;
    UnkRefObject* ref = nullptr;

    ~UnkChargeEntry_l3b5df0() {
        if (UnkRefObject* p = ref) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkChargeEntry_ovb0016;
void CAutclastChargeAttack_ReserveChargeEntries(base::global::CRntVector<UnkChargeEntry_ovb0016>* entries, u32 n);

// The block handed to the owner's +0x218 (fn_71010c7e6c); the vtable tells the kinds apart.
struct UnkEffectParams_b0117 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
    u8 field_0x3e;
    u8 field_0x3f;
    u8 pad_0x40;
    u8 field_0x41;
    u8 pad_0x42[2];
    u8 field_0x44[13];
    u8 field_0x51;
    u8 field_0x52;
};

struct UnkAutclastChargeAttack_l3b5df0 {
    void** vtable;
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    u32 state;  // 0x30
    u8 pad_0x34[0x5c];
    base::global::CRntVector<UnkChargeEntry_l3b5df0> entries;  // 0x90
};
static_assert(__builtin_offsetof(UnkAutclastChargeAttack_l3b5df0, entries) == 0x90, "layout");

void fn_71003b5df0(UnkAutclastChargeAttack_l3b5df0* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));

    u32 n = reinterpret_cast<UnkAutclastTunable_l3b5df0*>(&g_unk_71726bd2c8)->charge_count * 2;
    base::global::CRntVector<UnkChargeEntry_l3b5df0>& entries = self->entries;
    if (entries.count < n) {
        CAutclastChargeAttack_ReserveChargeEntries(reinterpret_cast<base::global::CRntVector<UnkChargeEntry_ovb0016>*>(&entries), n);
        for (u32 i = entries.count; i < n; ++i)
            new (&entries.data[i]) UnkChargeEntry_l3b5df0();
        entries.count = n;
    } else if (entries.count > n) {
        entries.Truncate(n);
    }

    UnkEffectSink* sink = static_cast<UnkEffectSink*>(self->owner->field_0x218);
    UnkEffectParams_b0117 effect;
    effect.field_0xc = 0;
    effect.field_0x14 = 0;
    __builtin_memset(effect.field_0x18, 0, 13);
    __builtin_memset(effect.field_0x28, 0, 13);
    effect.vtable = g_unk_7101a24db8 + 2;
    effect.field_0x3d = 0;
    effect.field_0x3f = 0;
    effect.field_0x41 = 0;
    effect.field_0x52 = 0;
    __builtin_memset(effect.field_0x44, 0, 13);
    effect.field_0x38 = 1;
    fn_71010c7e6c(sink, &effect, 1);

    void* animation = self->owner->field_0x298;
    UnkParams params;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.field_0x30 = 0;
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    fn_71006f4b0c(animation, &params, 1);

    self->state = 0;
}
