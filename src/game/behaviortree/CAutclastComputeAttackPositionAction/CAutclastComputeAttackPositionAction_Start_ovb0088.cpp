// behaviortree::CAutclastComputeAttackPositionAction's unit (tools/tu_map.py, the name from reflection): the
// action's start and its destructor. The start runs the base start (fn_7100394bd0), reads the two positions
// its keys (+0x40, +0x48) name (fn_710039525c) and, while the owner (+0x18) is within the Autclast tunable's
// range (+0x50 of g_unk_71726bd2c8) of its target (fn_7100d7c990), asks fn_71003b514c whether the segment
// between them (flattened, fn_710007b430) is clear: then the action is done (+0x38); otherwise it looks for
// a position (fn_71003b3288). The destructor restores the vtable, destroys the path object (+0x50,
// fn_71002ad0dc) and two string ids (+0x48, +0x40), then runs the base destructor fn_7100393ea0 (the shape
// of src/game/behaviortree/CGoToPointAction/CGoToPointAction_Dtor_b45.cpp). Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "types.h"

extern void* g_unk_7101a2ad48[];  // the node's vtable
void fn_7100393ea0(void*);         // the base's destructor

struct UnkObject_71002ad0dc;
void fn_71002ad0dc(UnkObject_71002ad0dc* self);

struct UnkPathMember_ovb0088 {
    ~UnkPathMember_ovb0088() { fn_71002ad0dc(reinterpret_cast<UnkObject_71002ad0dc*>(this)); }
};

struct CAutclastComputeAttackPositionActionDtorTag_ovb0088;
template <>
struct base::UnkDtorTraits<CAutclastComputeAttackPositionActionDtorTag_ovb0088> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a2ad48); }
    static void base(char* self) { fn_7100393ea0(self); }
};

__attribute__((flatten)) void fn_71003b4078(char* self) {
    base::refl_Dtor<CAutclastComputeAttackPositionActionDtorTag_ovb0088,
                    base::UnkMemberAt<UnkPathMember_ovb0088, 0x50>, base::UnkStrIdAt<0x48>,
                    base::UnkStrIdAt<0x40>>(self);
}

using UnkVec3f = base::math::CVector3D;
using base::math::CVector2D;

struct UnkTunable;
extern UnkTunable g_unk_71726bd2c8;  // CTunableCharClassAutclastAIComponent

struct UnkAutclastTunable_ovb0088 {
    u8 pad_0x0[0x50];
    f32 attack_range;  // 0x50
};

struct UnkSelf;
struct UnkStruct_0x68;
void fn_7100394bd0(UnkStruct_0x68*);
void fn_710039525c(UnkSelf* self, void* arg, UnkVec3f* out);

struct UnkAutclastBody_ovb0088 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkAutclastAction_ovb0088 {
    u8 pad_0x0[0x18];
    UnkAutclastBody_ovb0088* actor;  // 0x18
    u8 pad_0x20[0x18];
    bool done;          // 0x38
    u8 pad_0x39[7];
    u8 from_key[8];  // 0x40
    u8 to_key[8];    // 0x48
};

bool fn_71003b514c(UnkAutclastBody_ovb0088* actor, const CVector2D* from, const CVector2D* to);
void fn_71003b3288(UnkAutclastAction_ovb0088* self);

static inline f32 DistSq(const UnkAutclastBody_ovb0088* a, const UnkAutclastBody_ovb0088* b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 xy = dx * dx + dy * dy;
    f32 dz = a->z - b->z;
    return xy + dz * dz;
}

void fn_71003b3188(UnkAutclastAction_ovb0088* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    auto* target = reinterpret_cast<UnkAutclastBody_ovb0088*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor)));
    UnkVec3f from;
    from.x = 0;
    from.y = 0;
    from.z = 0;
    UnkVec3f to;
    to.x = 0;
    to.y = 0;
    to.z = 0;
    fn_710039525c(reinterpret_cast<UnkSelf*>(self), self->from_key, &from);
    fn_710039525c(reinterpret_cast<UnkSelf*>(self), self->to_key, &to);
    UnkAutclastBody_ovb0088* actor = self->actor;
    f32 range = reinterpret_cast<UnkAutclastTunable_ovb0088*>(&g_unk_71726bd2c8)->attack_range;
    f32 range2 = range * range;
    if (DistSq(actor, target) < range2) {
        CVector2D from2;
        fn_710007b430(&from2, &from);
        CVector2D to2;
        fn_710007b430(&to2, &to);
        if (fn_71003b514c(actor, &from2, &to2)) {
            self->done = true;
            return;
        }
    }
    fn_71003b3288(self);
}
