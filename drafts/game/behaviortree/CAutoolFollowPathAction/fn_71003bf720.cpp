// behaviortree::CAutoolFollowPathAction's unit (tools/tu_map.py unit 568): the start. After the base start it
// finds, for an owner whose AI is the autool component and that has a navigation (+0x298), the path point
// nearest to the owner (or the one it already follows), picks the direction to head in and hands the
// navigation a follow-path parameter block carrying the path and two callbacks. Names and layouts are
// placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkPart48.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkItem50.h"
#include "game_s2/UnkParams.h"
#include "types.h"
#include <math.h>

using base::global::CRntVector;
using base::reflection::UnkReflObject;

extern char g_unk_7101a264d0[];
extern f32* g_unk_71726beb18;
u32 CAINavigationComponent_FindIndexOfPointNearestOwner(void* navigation, void* path_holder);
void fn_7101059fec(void* path, const UnkVec3f* position, u32* a, u32* index, f32* t, f32* distance);
struct UnkFollowPointOut {
    f32 x, y, z;
};
UnkFollowPointOut fn_71010596ac(void* path, u32 index, f32 t);
void fn_71003c03c8();
void fn_71003c0404();

struct UnkAutoolOptDir_l3bf720 {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    bool set = false;
};

struct UnkAutoolTwoWords_l3bf720 {
    u64 first = 0;
    u64 second = 0;
};

// The follow-path parameter block of kind 2.
struct UnkAutoolParams_l3bf720 {
    void** vtable = reinterpret_cast<void**>(g_unk_7101a264d0 + 0x10);
    UnkAutoolOptDir_l3bf720 field_0x8;
    u32 field_0x18;
    bool has_0x18 = false;
    UnkAutoolTwoWords_l3bf720 field_0x20;
    bool has_0x20 = false;
    u32 mode = 2;
    u64 field_0x38;
    bool has_0x38 = false;
    u32 field_0x44;
    bool has_0x44 = false;
    u32 field_0x4c;
    bool has_0x4c = false;
    u32 field_0x54;
    bool has_0x54 = false;
    u32 field_0x5c;
    bool has_0x5c = false;
    u32 field_0x64;
    bool has_0x64 = false;
    u32 field_0x6c;
    bool has_0x6c = false;
    u32 field_0x74;
    bool has_0x74 = false;
    UnkAutoolOptDir_l3bf720 field_0x7c;
    u32 field_0x8c;
    bool has_0x8c = false;
    u8 field_0x91;
    bool has_0x91 = false;
    u8 field_0x93;
    bool has_0x93 = false;
    u32 field_0x98;
    bool has_0x98 = false;
    u8 field_0x9d;
    bool has_0x9d = false;
    u8 field_0x9f;
    bool has_0x9f = false;
    UnkAutoolTwoWords_l3bf720 field_0xa8;
    bool has_0xa8 = false;
    UnkAutoolTwoWords_l3bf720 field_0xc0;
    bool has_0xc0 = false;
    CRntVector<UnkVec3f> path{0xf};
    bool has_path = false;
    u8 field_0x101;
    bool has_0x101 = false;
    u8 field_0x103;
    bool has_0x103 = false;
    u8 field_0x105;
    bool has_0x105 = false;
};
static_assert(__builtin_offsetof(UnkAutoolParams_l3bf720, path) == 0xd8, "layout");

// A point of the path: its position answers vf_0x18.
struct UnkAutoolPoint_l3bf720 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkVec3f* vf_0x18();
    u8 pad_0x8[0x18];
};

// The holder of the path: its first point's data is copied out (+0x40).
struct UnkAutoolHolder_l3bf720 {
    u8 pad_0x0[0x30];
    u8 field_0x30[8];
    u8 pad_0x38[8];
    UnkItem* field_0x40;
};

struct UnkAutoolPathSlot_l3bf720 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 count;  // 0x8
    u8 pad_0xc[4];
    UnkAutoolHolder_l3bf720* holder;  // 0x10
};

struct UnkAutoolState_l3bf720 {
    s32 index;  // 0x0
    UnkVec3f direction;  // 0x4
    u8 pad_0x10[0x78];
    UnkAutoolPathSlot_l3bf720* slot;  // 0x88
    u8 pad_0x90[0x78];
    u32 field_0x108;
};

struct UnkAutoolAI_l3bf720 : UnkReflObject {
    u8 pad_0x8[0x14670];
    UnkAutoolState_l3bf720 state;  // 0x14678
};

struct UnkAutoolFollow_l3bf720 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // 0x18
};

static bool UnkIsNearZero(f32 v) {
    return !(v + -0.001f > 0.0f) && !(v + 0.001f < 0.0f);
}

void fn_71003bf720(UnkAutoolFollow_l3bf720* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    UnkReflObject* object = static_cast<UnkReflObject*>(self->actor->field_0x240);
    if (!object)
        return;
    UnkAutoolAI_l3bf720* ai = fn_710009d50c(object->vf_0x0(), fn_71003c2a94())
                                  ? static_cast<UnkAutoolAI_l3bf720*>(object)
                                  : nullptr;
    if (!ai)
        return;
    void* navigation = self->actor->field_0x298;
    if (!navigation)
        return;
    UnkAutoolParams_l3bf720 params;
    UnkActor* owner = self->actor;
    UnkAutoolState_l3bf720* state = &ai->state;
    UnkAutoolPathSlot_l3bf720* slot = state->slot;
    if (!slot)
        return;
    u32 count = slot->count;
    UnkAutoolHolder_l3bf720* holder = slot->holder;
    if (count == 0)
        slot->vf_0x10();
    if (!holder)
        return;
    UnkItem item(*holder->field_0x40);
    UnkAutoolPoint_l3bf720* points = reinterpret_cast<UnkAutoolPoint_l3bf720*>(item.field_0x8.field_0x0);
    u32 index = state->index;
    bool none = false;
    if (index == (u32)-1) {
        index = CAINavigationComponent_FindIndexOfPointNearestOwner(navigation, holder);
        UnkVec3f position(owner->field_0x44, owner->field_0x54, owner->field_0x64);
        u32 unused;
        u32 segment;
        f32 t;
        f32 distance;
        fn_7101059fec(holder->field_0x30, &position, &unused, &segment, &t, &distance);
        UnkFollowPointOut near = fn_71010596ac(&item, segment, t);
        f32 dx = owner->field_0x44 - near.x;
        f32 dy = owner->field_0x54 - near.y;
        f32 dz = owner->field_0x64 - near.z;
        if (dx * dx + dy * dy + dz * dz < 400.0f) {
            u32 next = segment + 1;
            const UnkVec3f* p1 = points[segment].vf_0x18();
            f32 ax = p1->x - owner->field_0x44;
            f32 ay = p1->y - owner->field_0x54;
            f32 az = p1->z - owner->field_0x64;
            const UnkVec3f* p2 = points[next].vf_0x18();
            f32 bx = p2->x - owner->field_0x44;
            f32 by = p2->y - owner->field_0x54;
            f32 bz = p2->z - owner->field_0x64;
            const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
            f32 a2 = ax * ax + ay * ay + az * az;
            f32 ia = 1.0f / (sqrtf(a2) + 0.0001f);
            f32 dot_a = m.m[2] * (ax * ia) + m.m[6] * (ay * ia) + m.m[10] * (az * ia);
            f32 b2 = bx * bx + by * by + bz * bz;
            f32 ib = 1.0f / (sqrtf(b2) + 0.0001f);
            f32 dot_b = m.m[2] * (bx * ib) + m.m[6] * (by * ib) + m.m[10] * (bz * ib);
            bool take_next;
            if (dot_b + 0.001f >= dot_a && dot_b + -0.001f <= dot_a)
                take_next = b2 < a2;
            else
                take_next = dot_b > dot_a;
            if (take_next)
                index = next;
        }
        if (index == (u32)-1)
            none = true;
    }
    params.field_0x98 = index;
    params.has_0x98 = true;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    f32 vx = state->direction.x;
    f32 vy = state->direction.y;
    f32 vz = state->direction.z;
    if (UnkIsNearZero(vx) && UnkIsNearZero(vy) && UnkIsNearZero(vz)) {
        vx = m.m[2];
        vy = m.m[6];
        vz = m.m[10];
        if (!none) {
            const UnkVec3f* p = points[index].vf_0x18();
            vx = p->x - owner->field_0x44;
            vy = p->y - owner->field_0x54;
            vz = p->z - owner->field_0x64;
        }
    }
    params.field_0x7c.x = vx;
    params.field_0x7c.y = vy;
    params.field_0x7c.z = vz;
    params.field_0x7c.set = true;
    params.field_0x38 = reinterpret_cast<u64>(holder->field_0x30);
    params.has_0x38 = true;
    params.field_0x44 = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(ai) + 0x13b8);
    params.has_0x44 = true;
    params.field_0x18 = state->field_0x108;
    params.has_0x18 = true;
    UnkReflObject* ai_class = *reinterpret_cast<UnkReflObject**>(reinterpret_cast<u8*>(ai) + 0x10);
    if (ai_class)
        fn_710009d50c(ai_class->vf_0x0(), fn_71003c855c());
    params.field_0x4c = *reinterpret_cast<u32*>(g_unk_71726beb18 + 0xf);
    params.has_0x4c = true;
    UnkPart48 part(*reinterpret_cast<UnkPart48*>(reinterpret_cast<u8*>(ai) + 0x14730));
    fn_71000cbcf0(reinterpret_cast<UnkCopiedVector<UnkThreeWords>*>(&params.path),
                  reinterpret_cast<const UnkCopiedVector<UnkThreeWords>*>(&part));
    params.has_path = true;
    params.field_0x91 = 0;
    params.has_0x91 = true;
    params.field_0x93 = 1;
    params.has_0x93 = true;
    params.field_0x9d = 1;
    params.has_0x9d = true;
    params.field_0xa8.first = reinterpret_cast<u64>(ai);
    params.field_0xa8.second = reinterpret_cast<u64>(fn_71003c03c8);
    params.has_0xa8 = true;
    params.field_0xc0.first = reinterpret_cast<u64>(ai);
    params.field_0xc0.second = reinterpret_cast<u64>(fn_71003c0404);
    params.has_0xc0 = true;
    fn_71006f4b0c(navigation, &params, 1);
}
