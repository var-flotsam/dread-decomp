// The unit tools/tu_map.py calls 2702 (hint CAIComponent): functions of an AI component. The component keeps its
// owner (an actor) at +0x30, a blackboard at +0x1260 and a data block at +0x1610. They set a blackboard flag under a
// property of the id table, switch a state in a list of states, rewind a fade effect on the owner's effect object and
// resolve links. Names and layouts are placeholders.
#include <new>

#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CCharClassAIComponent.h"
#include "game/CCharacterMovement.h"
#include "game/CFXComponent.h"
#include "game/CFrozenComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCallbackSetVector.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU2702.h"
#include "game/UnkTUProperty.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

// ---- Blackboard flags under properties of the id table

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

extern base::global::CStrId g_unk_71726b7cb8;

struct UnkGameManagerIdTable {
    u8 pad_0x0[0x8b8];
    UnkPropertyName field_0x8b8;
    u8 pad_0x8c0[0x28];
    UnkPropertyName field_0x8e8;
    UnkPropertyName field_0x8f0;
    u8 pad_0x8f8[0x2578 - 0x8f8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

struct UnkAIBlackboard_b0144 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;
};

static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name) {
    bool value = true;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &value;
            fn_7100332c80(s, name, &v);
        }
    }
}

void fn_7101105c94(UnkAIBlackboard_b0144* self) {
    SetSectionBool(&self->blackboard, g_unk_71726b7cb8, &GameManagerIdTable().field_0x8e8);
}

struct UnkAIOwnerA_b0144 {
    u8 pad_0x0[0x1f0];
    void* field_0x1f0;
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* field_0x220;
};

struct UnkAIData_b0144 {
    u8 pad_0x0[0x141];
    bool field_0x141;
};

struct UnkAIBB_b0144 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAIOwnerA_b0144* owner;  // 0x30
    u8 pad_0x38[0x10f0];
    u32 field_0x1128;
    u8 pad_0x112c[0x1260 - 0x112c];
    CBlackboard blackboard;  // 0x1260
    u8 pad_after[0x13d8 - 0x1260 - sizeof(CBlackboard)];
    u32 field_0x13d8;
    u8 pad_0x13dc[0x1610 - 0x13dc];
    UnkAIData_b0144* data;  // 0x1610

    void vf_0x258() { reinterpret_cast<void (*)(UnkAIBB_b0144*)>(vtable[0x258 / 8])(this); }
    bool vf_0x780() { return reinterpret_cast<bool (*)(UnkAIBB_b0144*)>(vtable[0x780 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkAIBB_b0144, blackboard) == 0x1260, "layout");
static_assert(__builtin_offsetof(UnkAIBB_b0144, data) == 0x1610, "layout");

void fn_7101105b90(UnkAIBB_b0144* self) {
    self->field_0x1128 = 0;
    SetSectionBool(&self->blackboard, g_unk_71726b7cb8, &GameManagerIdTable().field_0x8b8);
    self->vf_0x258();
}

void fn_7101108124(UnkAIBB_b0144* self, void* arg) {
    if (!self->vf_0x780())
        return;
    self->field_0x13d8 = 0;
    if (self->data->field_0x141) {
        UnkAnimationComponent* cls = self->owner->field_0x220;
        if (cls && fn_710117b348(cls, fn_7100cf96a8()) && self->owner->field_0x1f0)
            fn_7101108004(self, arg);
    }
    SetSectionBool(&self->blackboard, g_unk_71726b7cb8, &GameManagerIdTable().field_0x8f0);
}

// ---- A fade effect on the owner's effect object

struct UnkObject;
struct UnkComponent;
struct UnkRinkaEntry_ruc;
struct UnkRinkaControl_ovb0090;
struct UnkRinkaControl_ruc;
struct UnkFxObject_ovb0041;

extern u8* g_unk_7172a062e0 __attribute__((visibility("hidden")));
extern char g_unk_7172a062e8[] __attribute__((visibility("hidden")));

UnkComponent* fn_7101152b90(UnkObject*, u64);
bool fn_710114e668(UnkObject*, u8);
struct UnkFrostbiteComponent;
void fn_710115217c(void* object, void* effect, u32 a, u32 b, const base::global::CStrId* name,
                   const base::math::CVector3D& offset, const base::math::CVector3D& rotation, void* extra);

struct UnkAIOwnerC_b0144 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x58];
    UnkObject* field_0x280;
    u8 pad_0x288[0x40];
    UnkSwitchObject* field_0x2c8;
};

struct UnkAICfgSub_b0144 {
    u8 pad_0x0[0x10];
    const char* field_0x10;
    u8 pad_0x18[0x2c];
    u32 field_0x44;
};

struct UnkAICfg_b0144 {
    u8 pad_0x0[0x178];
    UnkAICfgSub_b0144* field_0x178;
    f32 a_x, a_y, a_z;  // 0x180
    f32 b_x, b_y, b_z;  // 0x18c
};

struct UnkAIC_b0144 {
    u8 pad_0x0[0x30];
    UnkAIOwnerC_b0144* owner;  // 0x30
    u8 pad_0x38[0x1610 - 0x38];
    UnkAICfg_b0144* cfg;  // 0x1610
    u8 pad_0x1618[0x179c - 0x1618];
    bool field_0x179c;
};

static inline u64 EffectKey() { return *reinterpret_cast<u64*>(g_unk_7172a062e0 + 0x30); }

__attribute__((noinline)) void fn_710110cacc(UnkAIC_b0144* self);

void fn_71011048c0(UnkAIC_b0144* self) {
    UnkAnimationComponent* track = self->owner->field_0x220;
    if (track && fn_710117b348(track, fn_7100cfd428())) {
        if (!self->owner->field_0x2c8 || !fn_71007f9f20(self->owner->field_0x2c8) ||
            fn_71007f9fd0(reinterpret_cast<UnkFrostbiteComponent*>(self->owner->field_0x2c8))) {
            fn_710110cacc(self);
            return;
        }
    }
    if (self->field_0x179c) {
        self->field_0x179c = false;
        UnkObject* world = self->owner->field_0x280;
        if (UnkComponent* fx = fn_7101152b90(world, EffectKey())) {
            u8 kind = *(reinterpret_cast<u8*>(fx) + 0xb);
            if (fn_710114e668(world, kind)) {
                fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(fx), false);
                fn_710115596c(reinterpret_cast<UnkRinkaControl_ruc*>(world), fx);
            }
        }
    }
}

struct UnkEffectVt_b0144 {
    void** vtable;
};

void fn_710110cacc(UnkAIC_b0144* self) {
    if (self->field_0x179c)
        return;
    self->field_0x179c = true;
    UnkObject* world = self->owner->field_0x280;
    UnkComponent* fx = fn_7101152b90(world, EffectKey());
    if (!fx) {
        char* effect = fn_71011520a8(reinterpret_cast<char*>(world), 0, g_unk_7172a062e8, 1);
        if (!effect)
            return;
        fx = reinterpret_cast<UnkComponent*>(effect);
        UnkAICfg_b0144* cfg = self->cfg;
        base::math::CVector3D offset;
        offset.x = cfg->a_x;
        offset.y = cfg->a_y;
        offset.z = cfg->a_z;
        base::math::CVector3D rotation;
        rotation.x = cfg->b_x;
        rotation.y = cfg->b_y;
        rotation.z = cfg->b_z;
        // A raw increment: the original reloads the sub-object pointer after it.
        u32 uses;
        __builtin_memcpy(&uses, &cfg->field_0x178->field_0x44, 4);
        uses++;
        __builtin_memcpy(&cfg->field_0x178->field_0x44, &uses, 4);
        const char* name = cfg->field_0x178->field_0x10;
        if (!name)
            name = g_unk_710162e2ed;
        fn_710115217c(world, effect, 0, 6, &g_unk_7101d470e0, offset, rotation, const_cast<char*>(name));
        fn_7101152988(reinterpret_cast<UnkRinkaControl_ovb0090*>(world), reinterpret_cast<UnkFxObject_ovb0041*>(effect),
                      reinterpret_cast<UnkRinkaEntry_ruc**>(&g_unk_7172a062e0), nullptr, 0);
    }
    auto* vt = reinterpret_cast<UnkEffectVt_b0144*>(fx);
    reinterpret_cast<void (*)(UnkComponent*, u32)>(vt->vtable[0x40 / 8])(fx, 1);
}

// ---- Switching the current state in a list of states

struct UnkCancelOwner_ovb0128;
u64 fn_7100413fe8(char* p0, u32 p1);
extern char* g_unk_71726b2398;

struct UnkAIState_b0144 {
    void** vtable;
    void vf_0x30(UnkCancelOwner_ovb0128* owner, bool flag) {
        reinterpret_cast<void (*)(UnkAIState_b0144*, UnkCancelOwner_ovb0128*, bool)>(vtable[0x30 / 8])(this, owner, flag);
    }
    void vf_0x38(UnkCancelOwner_ovb0128* owner) {
        reinterpret_cast<void (*)(UnkAIState_b0144*, UnkCancelOwner_ovb0128*)>(vtable[0x38 / 8])(this, owner);
    }
    bool vf_0x40(UnkCancelOwner_ovb0128* owner) {
        return reinterpret_cast<bool (*)(UnkAIState_b0144*, UnkCancelOwner_ovb0128*)>(vtable[0x40 / 8])(this, owner);
    }
};

struct UnkAIStates_b0144 {
    u8 pad_0x0[0x30];
    UnkCancelOwner_ovb0128* owner;  // 0x30
    u8 pad_0x38[0x1140 - 0x38];
    s32 current;  // 0x1140
    u8 pad_0x1144[0x1610 - 0x1144];
    char* list;  // 0x1610
};

static inline void NotifyOwner(UnkAIStates_b0144* self) {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    fn_71011a75f0(instance, self->owner);
}

void fn_71010ffb30(UnkAIStates_b0144* self, s32 index, bool flag) {
    auto* cur = reinterpret_cast<UnkAIState_b0144*>(fn_7100413fe8(self->list, self->current));
    auto* next = reinterpret_cast<UnkAIState_b0144*>(fn_7100413fe8(self->list, index));
    if (!next)
        return;
    NotifyOwner(self);
    if (cur)
        cur->vf_0x38(self->owner);
    self->current = index;
    next->vf_0x30(self->owner, flag);
}

bool fn_71010ffa3c(UnkAIStates_b0144* self, s32 index, bool flag) {
    auto* cur = reinterpret_cast<UnkAIState_b0144*>(fn_7100413fe8(self->list, self->current));
    if (fn_7100413fe0(self->list) <= index)
        return false;
    auto* next = reinterpret_cast<UnkAIState_b0144*>(fn_7100413fe8(self->list, index));
    if (!next || !next->vf_0x40(self->owner))
        return false;
    NotifyOwner(self);
    cur->vf_0x38(self->owner);
    self->current = index;
    next->vf_0x30(self->owner, flag);
    return true;
}

// ---- The offset from the owner's position to a navigation target, scaled by the lead's height

struct UnkNavTarget_b65a4;
struct UnkNavVec_b65a4 {
    f32 x, y, z;
    UnkNavVec_b65a4() : x(0.0f), y(0.0f), z(0.0f) {}
};
struct UnkFlagged;
bool fn_71006f4f28(UnkNavTarget_b65a4* self, UnkNavVec_b65a4* out);

struct UnkAILead_b0144 {
    void** vtable;
    UnkVec3f vf_0x280() { return reinterpret_cast<UnkVec3f (*)(UnkAILead_b0144*)>(vtable[0x280 / 8])(this); }
};

struct UnkAIOwnerI_b0144 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1b0];
    UnkAILead_b0144* field_0x218;
    u8 pad_0x220[0x78];
    UnkNavTarget_b65a4* field_0x298;
};

struct UnkAII_b0144 {
    u8 pad_0x0[0x30];
    UnkAIOwnerI_b0144* owner;  // 0x30
};

bool fn_7101112070(UnkAII_b0144* self, UnkVec3f* out) {
    if (!self->owner->field_0x298)
        return false;
    UnkNavVec_b65a4 target;
    UnkAILead_b0144* lead = self->owner->field_0x218;
    if (!lead)
        return false;
    if (!fn_71006f4f28(self->owner->field_0x298, &target))
        return false;
    f32 px = self->owner->x, py = self->owner->y, pz = self->owner->z;
    f32 h = fn_7100977bec(reinterpret_cast<UnkFlagged*>(lead));
    UnkVec3f dir = lead->vf_0x280();
    f32 dx = dir.x, dy = dir.y, dz = dir.z;
    f32 tx = target.x, ty = target.y, tz = target.z;
    out->x = tx - (px - h * dx);
    out->y = ty - (py - h * dy);
    out->z = tz - (pz - h * dz);
    return true;
}

// ---- Picking the next mode from a random roll

extern "C" int rand();
struct UnkAIJ_b0144;
void fn_71010ff6f4(UnkAIJ_b0144* self, u32 mode);

struct UnkAIJ_b0144 {
    u8 pad_0x0[0x14570];
    u32 mode;      // 0x14570
    u32 previous;  // 0x14574
};
static_assert(__builtin_offsetof(UnkAIJ_b0144, mode) == 0x14570, "layout");

void fn_71010ff5e4(UnkAIJ_b0144* self) {
    f32 roll = static_cast<f32>(static_cast<f64>(rand() & 0x7fff) * 0x1.0002000400080p-15) + 0.0f;
    u32 previous = self->previous;
    u32 mode = self->mode;
    if (previous != mode) {
        self->previous = mode;
        if (roll < 0.33f) {
            fn_71010ff6f4(self, 0);
            return;
        }
        fn_71010ff6f4(self, !(roll < 0.66f) ? 2 : 1);
        return;
    }
    switch (previous) {
    case 0:
        fn_71010ff6f4(self, !(roll < 0.5f) ? 2 : 1);
        return;
    case 1:
        fn_71010ff6f4(self, !(roll < 0.5f) ? 2 : 0);
        return;
    case 2:
        fn_71010ff6f4(self, !(roll < 0.5f) ? 1 : 0);
        return;
    default:
        return;
    }
}

// ---- Link equality and a copy appended to the callback-set array

struct UnkLinkHandle_b0144 {
    u8 pad_0x0[0x10];
    void* target;
};

struct UnkLink_b0144 {
    u8 pad_0x0[0x8];
    u8 key[0x28];
    u32 state;
    u32 pad_0x34;
    UnkLinkHandle_b0144* handle;
    u32 field_0x40;

    void* Get() {
        if (state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(key);
                fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&handle),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return handle ? handle->target : nullptr;
    }
};

bool fn_71011136b8(UnkLink_b0144* a, UnkLink_b0144* b) {
    void* target_b = b->Get();
    if (a->Get() != target_b)
        return false;
    return a->field_0x40 == b->field_0x40;
}

void fn_7101114588(UnkCallbackSetVector* v, const UnkCallbackSetElem* value) {
    u32 n = v->count;
    u32 new_capacity = v->capacity;
    if (new_capacity <= n && v->step != 0)
        new_capacity = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_71011143d8(v, new_capacity);
    new (&v->data[v->count]) UnkCallbackSetElem(*value);
    ++v->count;
}
