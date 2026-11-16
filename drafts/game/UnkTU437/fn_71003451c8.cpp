// Unit 437 (aim component): fn_71003451c8 finds the first central unit of kind 5 or 6 and gives the point to aim
// at: kind 5, its source's position (taking the source when it is due); kind 6, its target's position (or where its
// movement aims), pushed 90 units along the aim direction when the segment from the owner is blocked. Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/UnkRefHandle.h"
#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU437.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector3D;

struct UnkAimFilterSelf_ovb0198u {
    void** target;
};
bool fn_71003523bc(UnkAimFilterSelf_ovb0198u* self, const CVector3D* a, const CVector3D* b);

struct UnkUnit1444_o0309;
struct UnkUnit1444_ovb0177;
UnkVec3f fn_7100973658(UnkUnit1444_o0309* self);
UnkVec3f fn_71009737e8(UnkUnit1444_ovb0177* self);

struct UnkAimActor_l3451c8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;
    UnkVec3f Position() const { return UnkVec3f(transform.m[3], transform.m[7], transform.m[11]); }
};

struct UnkAimMoveState_l3451c8;

struct UnkAimMovement_l3451c8 {
    u8 pad_0x0[0x17a0];
    u8 state_4[1];  // the state of kind 4
    UnkAimMoveState_l3451c8* State4() { return reinterpret_cast<UnkAimMoveState_l3451c8*>(state_4); }
};

struct UnkAimTarget_l3451c8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;
    u8 pad_0x78[0x218 - 0x78];
    UnkAimMovement_l3451c8* movement;
    UnkVec3f Position() const { return UnkVec3f(transform.m[3], transform.m[7], transform.m[11]); }
};

struct UnkAimTargetRef_l3451c8 {
    u8 pad_0x0[0x10];
    UnkAimTarget_l3451c8* target;
};

struct UnkAimSourceRef_l3451c8 {
    u8 pad_0x0[0x10];
    UnkAimActor_l3451c8* actor;
};

struct UnkUnitKind_l3451c8 {
    u8 pad_0x0[0x10];
    s32 kind;
};

struct UnkCentralUnit_l3451c8 {
    u8 pad_0x0[0x40];
    UnkUnitKind_l3451c8* kind;
    u8 pad_0x48[0x500 - 0x48];
    u8 sources[0x28];
    s32 source_state;
    u8 pad_0x52c[4];
    base::UnkRefHandle source;
    u8 pad_0x538[0x6d8 - 0x538];
    UnkAimTargetRef_l3451c8* target;
};
static_assert(__builtin_offsetof(UnkCentralUnit_l3451c8, source) == 0x530, "layout");
static_assert(__builtin_offsetof(UnkCentralUnit_l3451c8, target) == 0x6d8, "layout");

struct UnkCentralUnitEntry_l3451c8 {
    u8 pad_0x0[0x10];
    UnkCentralUnit_l3451c8* unit;
};

struct UnkCentralUnitList_l3451c8 {
    UnkCentralUnitEntry_l3451c8** data;
    u32 count;
};

struct UnkAimOwner_l3451c8 {
    u8 pad_0x0[0x30];
    UnkAimActor_l3451c8* actor;
};

// Moves `point` `distance` units along `dir`.
static inline void Push(UnkVec3f& point, f32 x, f32 y, f32 z, f32 distance) {
    point.x = x * distance + point.x;
    point.y = y * distance + point.y;
    point.z = z * distance + point.z;
}

bool fn_71003451c8(UnkAimOwner_l3451c8* self, UnkVec3f* out) {
    UnkCentralUnitList_l3451c8* list = reinterpret_cast<UnkCentralUnitList_l3451c8*>(fn_7100772c98());
    for (UnkCentralUnitEntry_l3451c8** it = list->data; it != list->data + list->count; ++it) {
        if (!*it)
            continue;
        UnkCentralUnit_l3451c8* unit = (*it)->unit;
        if (!unit || !unit->kind)
            continue;
        s32 kind = unit->kind->kind;
        if (kind == 5) {
            if (unit->source_state == 1) {
                base::UnkRefPtrMember next = fn_7100353be8(unit->sources);
                fn_710031d208(&unit->source, reinterpret_cast<const base::UnkRefHandle*>(&next));
                unit->source_state = 2;
            }
            UnkAimSourceRef_l3451c8* source = reinterpret_cast<UnkAimSourceRef_l3451c8*>(unit->source.ptr);
            if (!source || !source->actor)
                return false;
            *out = source->actor->Position();
            return true;
        }
        if (kind == 6) {
            void* target = unit->target ? unit->target->target : nullptr;
            if (!target)
                return false;
            UnkAimMovement_l3451c8* movement = static_cast<UnkAimTarget_l3451c8*>(target)->movement;
            if (!movement)
                return false;
            UnkAimFilterSelf_ovb0198u filter = {&target};
            UnkVec3f aim = static_cast<UnkAimTarget_l3451c8*>(target)->Position();
            u32 mode = fn_71010cc3a4(movement);
            // The movement's state while it is in mode 4.
            UnkAimMoveState_l3451c8* state = movement->State4();
            if (mode == 4)
                aim = fn_7100973658(reinterpret_cast<UnkUnit1444_o0309*>((fn_71010cc3a4(movement) == 4 ? state : nullptr)));
            UnkVec3f from = self->actor->Position();
            if (!fn_71003523bc(&filter, reinterpret_cast<CVector3D*>(&from), reinterpret_cast<CVector3D*>(&aim))) {
                if (mode == 4) {
                    UnkVec3f dir = fn_71009737e8(reinterpret_cast<UnkUnit1444_ovb0177*>((fn_71010cc3a4(movement) == 4 ? state : nullptr)));
                    Push(aim, dir.x, dir.y, dir.z, 90.0f);
                } else {
                    const UnkMatrix44& m = fn_710002614c(&self->actor->transform);
                    Push(aim, m.m[1], m.m[5], m.m[9], 90.0f);
                }
                from = self->actor->Position();
                if (!fn_71003523bc(&filter, reinterpret_cast<CVector3D*>(&from), reinterpret_cast<CVector3D*>(&aim)))
                    return false;
            }
            *out = aim;
            return true;
        }
    }
    return false;
}
