// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): for every entry of the component's list
// (+0x15658), sends the owner's animation (+0x220) the hash that goes with the entry's kind: with `start`,
// from the lava-drops attack's (+0x15150) phase values; otherwise from the component's own (+0x15438, with
// `own`) or the attack's, and then clears the entry's state (fn_7100232864); a fixed hash when the entry
// has no state. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::UnkReflObject;

// The four values the hashes are picked by, one per kind.
struct UnkPhaseValues_l4d0cf4 {
    u8 pad_0x0[0x8];
    s32 field_0x8;   // kind 0x913c5999cc2002f8
    s32 field_0xc;   // kind 0xfc0afa18dd48957d
    s32 field_0x10;  // kind 0x8b59361142c681d2
    s32 field_0x14;  // kind 0x8bb4f4733ec20d8c
};

struct UnkLavaDropsAttack_l4d0cf4;
UnkPhaseValues_l4d0cf4* fn_71004ae918(UnkLavaDropsAttack_l4d0cf4* attack, u32 id);

struct UnkEntryKind_l4d0cf4 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkEntryTarget_l4d0cf4 {
    u8 pad_0x0[0x10];
    UnkEntityState* state;  // 0x10
};

struct UnkEntry_l4d0cf4 {
    UnkEntryKind_l4d0cf4* kind;
    u8 pad_0x8[0x28];
    UnkEntryTarget_l4d0cf4* target;  // 0x30
    u8 pad_0x38[0x8];

    UnkEntityState* State() const { return target ? target->state : nullptr; }
};

struct UnkOwner_l4d0cf4 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

// The component's phase data at +0x15150.
struct UnkPhaseData_l4d0cf4 {
    UnkReflObject* attack;  // 0x0
    u8 pad_0x8[0x2e0];
    UnkPhaseValues_l4d0cf4 values;  // 0x2e8
    u8 pad_0x300[0x208];
    UnkEntry_l4d0cf4* entries;  // 0x508
    u8 pad_0x510[0x8];
    u32 count;  // 0x518
    u8 pad_0x51c[0xe4];
    u32 phase;  // 0x600
};
static_assert(__builtin_offsetof(UnkPhaseData_l4d0cf4, values) == 0x2e8, "layout");
static_assert(__builtin_offsetof(UnkPhaseData_l4d0cf4, count) == 0x518, "layout");
static_assert(__builtin_offsetof(UnkPhaseData_l4d0cf4, phase) == 0x600, "layout");

struct UnkCooldownAI_l4d0cf4 {
    u8 pad_0x0[0x30];
    UnkOwner_l4d0cf4* owner;  // 0x30
    u8 pad_0x38[0x15150 - 0x38];
    UnkPhaseData_l4d0cf4 data;  // 0x15150
};

// The lava-drops attack, when the object at +0x15150 is one.
static inline UnkLavaDropsAttack_l4d0cf4* LavaDrops(UnkPhaseData_l4d0cf4* data) {
    UnkReflObject* attack = data->attack;
    if (attack && fn_710009d50c(attack->vf_0x0(), fn_71004add20()))
        return reinterpret_cast<UnkLavaDropsAttack_l4d0cf4*>(attack);
    return nullptr;
}

// Sends a request with mode 1 under the two ids.
inline void SendMode1(void* target, u64 hash, u64 kind, base::reflection::UnkReflTypeIdArg& mode,
                      base::reflection::UnkReflTypeIdArg& mask) {
    mode.id = 1;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

static inline u64 StartHash(u64 kind, const UnkPhaseValues_l4d0cf4* v) {
    switch (kind) {
    case 0x913c5999cc2002f8:
        return v->field_0x8 == 0 ? 0x6a451c31f608596c : 0x975e0dea118fa629;
    case 0xfc0afa18dd48957d:
        return v->field_0xc == 0 ? 0x24ee350753db2065 : 0xd0c6b6ef1a578c3f;
    case 0x8b59361142c681d2:
        return v->field_0x10 == 0 ? 0x3ccbb712f3eaecdc : 0x907e08f33fe6edde;
    case 0x8bb4f4733ec20d8c:
        return v->field_0x14 == 0 ? 0x53e7a5c0872bd9e5 : 0x491bb6ef82cca89c;
    default:
        return static_cast<u64>(-1);
    }
}

static inline u64 StopHash(u64 kind, const UnkPhaseValues_l4d0cf4* v) {
    switch (kind) {
    case 0x913c5999cc2002f8:
        return v->field_0x8 == 0 ? 0xe80b7f59a52d90be : 0x15106e8242aa6ffb;
    case 0xfc0afa18dd48957d:
        return v->field_0xc == 0 ? 0x55c716e30c3d52b3 : 0xa1ef950b45b1fee9;
    case 0x8b59361142c681d2:
        return v->field_0x10 == 0 ? 0x188c12415e8e87e4 : 0xb439ada0928286e6;
    case 0x8bb4f4733ec20d8c:
        return v->field_0x14 == 0 ? 0x0c83a45759cc2f55 : 0x167fb7785c2b5e2c;
    default:
        return static_cast<u64>(-1);
    }
}

static inline u64 IdleHash(u64 kind) {
    switch (kind) {
    case 0x913c5999cc2002f8:
        return 0xf2e92c6f4debeb35;
    case 0xfc0afa18dd48957d:
        return 0x7a34fff137d4cc58;
    case 0x8b59361142c681d2:
        return 0x61f15a67eb180d8f;
    case 0x8bb4f4733ec20d8c:
        return 0xea4a9471812cc1e8;
    default:
        return static_cast<u64>(-1);
    }
}

// Declared as CCooldownXBossAIComponent_Phases_ovb0088.cpp declares it.
struct UnkCooldownXMsgAI_ovb0088;

void fn_71004d0cf4(UnkCooldownXMsgAI_ovb0088* p, bool start, bool own) {
    UnkCooldownAI_l4d0cf4* self = reinterpret_cast<UnkCooldownAI_l4d0cf4*>(p);
    void* animation = self->owner->animation;
    if (!animation)
        return;
    UnkPhaseData_l4d0cf4* data = &self->data;
    base::reflection::UnkReflTypeIdArg start_mode, start_mask;
    base::reflection::UnkReflTypeIdArg stop_mode, stop_mask;
    base::reflection::UnkReflTypeIdArg idle_mode, idle_mask;
    for (UnkEntry_l4d0cf4 *e = data->entries, *end = e + data->count; e != end; ++e) {
        UnkEntityState* state = e->State();
        if (start) {
            if (!state)
                continue;
            const UnkPhaseValues_l4d0cf4* values = fn_71004ae918(LavaDrops(data), data->phase);
            u64 kind = e->kind->id;
            SendMode1(animation, StartHash(kind, values), kind, start_mode, start_mask);
        } else if (!state) {
            u64 kind = e->kind->id;
            SendMode1(animation, IdleHash(kind), kind, idle_mode, idle_mask);
        } else {
            const UnkPhaseValues_l4d0cf4* values =
                own ? &data->values : fn_71004ae918(LavaDrops(data), data->phase);
            u64 kind = e->kind->id;
            u64 hash = StopHash(kind, values);
            SendMode1(animation, hash, kind, stop_mode, stop_mask);
            fn_7100232864(e->State(), false);
        }
    }
}
