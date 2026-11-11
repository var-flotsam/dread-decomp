// behaviortree::CInFuryPatrolAction's unit (tools/tu_map.py unit 486): the action's end step, its update and
// its destructor. A blackboard flag (a section named by g_unk_71726b7cb8, the property named by the string id
// at +0x8f0 of the id table built once by fn_7100031894) is cleared when it is found set; the
// update then stops the patrol (fn_71003893dc) once the actor is within the radius at +0x64 of the point at
// +0x58. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CEnemyMovement.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBaseAction.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern base::global::CStrId g_unk_71726b7cb8;
extern void* g_unk_7101a265d0[];  // the action's vtable
extern void* g_unk_7101a22d48[];  // the listener's vtable

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// The id table (fn_7100031894 builds it, as in src/base/reflection/UnkEnumValueNames.cpp); the flag's
// property name is the string id at +0x8f0.
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x17e0];
    u8 field_0x17e0[8];
    u8 pad_0x17e8[0x2578 - 0x17e8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};

inline CStrId* FlagName_ovb0124() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return reinterpret_cast<CStrId*>(reinterpret_cast<u8*>(&s_table) + 0x8f0);
}

struct UnkPatrolOwner_ovb0124 {
    u8 pad_0x0[0x1138];
    f32 field_0x1138;
    u8 pad_0x113c[0x1260 - 0x113c];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1350[0x13d2 - 0x1350];
    bool field_0x13d2;
};

struct UnkPatrolActor_ovb0124 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x218 - 0x68];
    void* field_0x218;
};

// A listener holding a weak handle to what it listens to; it unregisters on destruction.
struct UnkPatrolListener_ovb0124 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakHandle* link;  // 0x18

    ~UnkPatrolListener_ovb0124() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkWeakHandle* l = link) {
            if (UnkWeakTarget* target = l->target) {
                fn_710117b3f0(target, this);
                l = link;
            }
            if (l && --l->ref_count == 0)
                l->vf_0x10();
        }
    }
};

struct UnkPatrolAction_ovb0124 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkPatrolActor_ovb0124* actor;  // 0x18
    u8 pad_0x20[0x10];
    UnkPatrolOwner_ovb0124* owner;  // 0x30
    void* field_0x38;
    u8 pad_0x40[0x10];
    bool field_0x50;
    u8 pad_0x51[3];
    s32 field_0x54;
    f32 x, y, z;  // 0x58
    f32 radius;   // 0x64
    bool field_0x68;
    u8 pad_0x69[7];
    UnkPatrolListener_ovb0124 listener;  // 0x70
    base::UnkStrIdMember field_0x90;
    base::UnkStrIdMember field_0x98;
    u8 pad_0xa0[9];
    bool field_0xa9;
};
static_assert(__builtin_offsetof(UnkPatrolAction_ovb0124, field_0xa9) == 0xa9, "layout");

void fn_71003893dc(UnkPatrolAction_ovb0124* self, s32* state);

void fn_7100389864(UnkPatrolAction_ovb0124* self) {
    fn_7100394bd4(self);
    if (self->field_0x68) {
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.field_0x34 = 5;
        params.field_0x1c = 1;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x18 = 0;
        params.field_0x30 = 1;
        fn_71006f4b0c(self->field_0x38, &params, 1);
    }
    // A plain byte store: the owner is reloaded after it.
    reinterpret_cast<u8*>(self->owner)[0x13d2] = 0;
    bool value = false;
    if (UnkPatrolOwner_ovb0124* owner = self->owner) {
        CBlackboard* blackboard = &owner->blackboard;
        CStrId* name = FlagName_ovb0124();
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, g_unk_71726b7cb8)) {
            bool found = fn_710006d11c(section, name, &value);
            if (value && found) {
                CStrId* name2 = FlagName_ovb0124();
                bool cleared = false;
                if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
                    if (!blackboard->field_0xe8) {
                        UnkTypedValue v;
                        v.type = fn_710000d228();
                        v.value = &cleared;
                        fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(name2), &v);
                    }
                }
            }
        }
    }
    if (UnkWeakHandle* l = self->listener.link) {
        if (UnkWeakTarget* target = l->target) {
            fn_710117b3f0(target, &self->listener);
            l = self->listener.link;
            if (!l)
                return;
        }
        if (--l->ref_count == 0)
            l->vf_0x10();
        self->listener.link = nullptr;
    }
}

// Clears the owner's blackboard flag when it is set, and with it the flag at +0x50.
static inline void TakeFlag_ovb0124(UnkPatrolAction_ovb0124* self, UnkPatrolOwner_ovb0124* owner) {
    bool value = false;
    if (!owner)
        return;
    CBlackboard* blackboard = &owner->blackboard;
    CStrId* name = FlagName_ovb0124();
    CBlackboard::CSection* section = fn_7101047270(blackboard, g_unk_71726b7cb8);
    if (!section)
        return;
    bool found = fn_710006d11c(section, name, &value);
    if (!value || !found)
        return;
    CStrId* name2 = FlagName_ovb0124();
    bool cleared = false;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = &cleared;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(name2), &v);
        }
    }
    self->field_0x50 = false;
}

s32 fn_7100389a84(UnkPatrolAction_ovb0124* self) {
    UnkPatrolOwner_ovb0124* owner = self->owner;
    if (!owner)
        return 6;
    if (self->field_0xa9) {
        if (void* target = self->actor->field_0x218) {
            if (fn_71010cc3a4(target) == 4)
                return 2;
            owner = self->owner;
        }
        self->field_0xa9 = false;
    }
    if (owner->field_0x1138 < 0.001f)
        self->field_0x50 = false;
    TakeFlag_ovb0124(self, owner);
    if (self->field_0x50) {
        UnkPatrolActor_ovb0124* actor = self->actor;
        f32 ax = actor->x;
        f32 ay = actor->y;
        f32 az = actor->z;
        f32 dx = self->x - ax;
        f32 dy = self->y - ay;
        f32 dz = self->z - az;
        if (!(dx * dx + dy * dy + dz * dz < self->radius * self->radius))
            return 2;
        self->field_0x50 = false;
        self->field_0x54 = 1;
        fn_71003893dc(self, &self->field_0x54);
        if (self->field_0x50)
            return 2;
    }
    fn_71003893dc(self, &self->field_0x54);
    return 2;
}

// The destructor: the string ids at +0x98 and +0x90, the listener, then the base's destructor.
void fn_710038a088(UnkPatrolAction_ovb0124* self) {
    self->vtable = &g_unk_7101a265d0[2];
    self->field_0x98.~UnkStrIdMemberOf();
    self->field_0x90.~UnkStrIdMemberOf();
    self->listener.~UnkPatrolListener_ovb0124();
    fn_7100393ea0(self);
}
