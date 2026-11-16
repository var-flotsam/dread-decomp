// CFlockingSwarmControllerComponent unit: after the base update, puts the swarm's free tile blockers
// along its way: one between the two slots of each active member that is moving into or out of the
// current group (once per slot), and one on each path segment that members of the current group span.
// The blockers placed are listed for the group (+0x140). Names and layouts are placeholders.
#include <algorithm>
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CRntVector;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkSwarmController_ovb6;
void fn_710074a0d4(UnkSwarmController_ovb6* self, f32 dt);
UnkOwnedList* fn_7100738c9c(CRntVector<UnkOwnedList>* self, u32 index);

// A place in the swarm's formation: a group and an index within it.
struct UnkSwarmSlot_l738564 {
    u32 group;
    u32 index;

    ~UnkSwarmSlot_l738564() {}
    bool operator==(const UnkSwarmSlot_l738564& o) const { return index == o.index && group == o.group; }
};

// Something with a position (vf_0x18).
struct UnkPositioned_l738564 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const CVector3D* Position();  // vf_0x18
};

// A path node, 0x20 bytes.
struct UnkPathNode_l738564 : UnkPositioned_l738564 {
    u8 pad_0x8[0x18];
};
static_assert(sizeof(UnkPathNode_l738564) == 0x20, "UnkPathNode_l738564 placeholder layout");

// The blocking shape of a tile blocker: a segment from (a) to (b).
struct UnkBlockerShape_l738564 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    u8 pad_0x8[0xa0];
    CVector2D a;  // +0xa8
    CVector2D b;  // +0xb0
    f32 field_0xb8;
};
static_assert(__builtin_offsetof(UnkBlockerShape_l738564, field_0xb8) == 0xb8, "layout");

struct UnkTileBlocker_l738564 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual bool InUse();  // vf_0x58
    virtual void vf_0x60(u32 flags);
    u8 pad_0x8[0x150];
    UnkBlockerShape_l738564* shape;  // +0x158
};

// The tile blockers created at start-up, in raw storage after the count.
struct UnkTileBlockers_l738564 {
    u32 count;
    u8 storage[16 * sizeof(UnkTileBlocker_l738564*)];

    UnkTileBlocker_l738564** begin() { return reinterpret_cast<UnkTileBlocker_l738564**>(storage); }
    UnkTileBlocker_l738564** end() { return begin() + count; }
};

struct UnkSwarmMember_l738564 {
    u8 pad_0x0[0x24];
    bool active;  // +0x24
    u8 pad_0x25[0x23];
    UnkSwarmSlot_l738564 slot;  // +0x48
    UnkSwarmSlot_l738564 next;  // +0x50
    u8 pad_0x58[0xf0];
};
static_assert(sizeof(UnkSwarmMember_l738564) == 0x148, "UnkSwarmMember_l738564 placeholder layout");

struct UnkSwarmMembers_l738564 {
    u8 pad_0x0[0x50];
    UnkSwarmMember_l738564* data;
    u8 pad_0x58[0xb8];
    s32 count;  // +0x110
};

struct UnkSwarmMembersOwner_l738564 {
    u8 pad_0x0[0x10];
    UnkSwarmMembers_l738564* members;
};

struct UnkSwarmGroups_l738564 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual u32 Current();  // vf_0x28
};

struct UnkFlockingSwarm_l738564 {
    u8 pad_0x0[0x80];
    UnkSwarmGroups_l738564* groups;  // +0x80
    u8 pad_0x88[0x60];
    UnkSwarmMembersOwner_l738564* field_0xe8;
    u8 pad_0xf0[0x50];
    CRntVector<UnkOwnedList> lists;  // +0x140, the blockers placed, by group
    u8 pad_0x168[0x168];
    UnkPathNode_l738564* path;  // +0x2d0
    u32 path_count;             // +0x2d8
    u8 pad_0x2dc[0xb4];
    u32 state;  // +0x390
    u8 pad_0x394[0x4c];
    UnkTileBlockers_l738564 blockers;  // +0x3e0

    UnkSwarmMembers_l738564* Members() { return field_0xe8 ? field_0xe8->members : nullptr; }
};
static_assert(__builtin_offsetof(UnkFlockingSwarm_l738564, path) == 0x2d0, "layout");
static_assert(__builtin_offsetof(UnkFlockingSwarm_l738564, state) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkFlockingSwarm_l738564, blockers) == 0x3e0, "layout");

UnkPositioned_l738564* fn_7100749e44(UnkFlockingSwarm_l738564* self, const UnkSwarmSlot_l738564* slot);

using UnkBlockerList_l738564 = CRntVector<UnkTileBlocker_l738564*>;

// Sets the first free blocker across from -> to and returns it, or returns null when none is free or
// its shape is not a segment.
inline UnkTileBlocker_l738564* PlaceBlocker(UnkFlockingSwarm_l738564* self, const CVector2D& from,
                                            const CVector2D& to) {
    UnkTileBlocker_l738564* blocker = nullptr;
    for (UnkTileBlocker_l738564* b : self->blockers) {
        if (!b->InUse()) {
            blocker = b;
            break;
        }
    }
    if (!blocker)
        return nullptr;
    UnkBlockerShape_l738564* shape = blocker->shape;
    if (!shape || shape->vf_0x18() != 1)
        return nullptr;
    blocker->vf_0x60(0x2000);
    shape->a.x = from.x;
    shape->a.y = from.y;
    shape->b.x = to.x;
    shape->b.y = to.y;
    shape->field_0xb8 = -1.0f;
    shape->vf_0x80();
    return blocker;
}

void fn_7100738564(UnkFlockingSwarm_l738564* self, f32 dt) {
    fn_710074a0d4(reinterpret_cast<UnkSwarmController_ovb6*>(self), dt);
    if (self->state == 0 || self->state == 3)
        return;
    u32 current = self->groups->Current();
    auto* placed = reinterpret_cast<UnkBlockerList_l738564*>(fn_7100738c9c(&self->lists, current));
    for (UnkTileBlocker_l738564** it = placed->data, **end = placed->data + placed->count; it != end; ++it)
        (*it)->vf_0x60(0);
    placed->Clear();

    u32 path_count = self->path_count;
    if (path_count == 0)
        return;
    CRntVector<UnkSwarmSlot_l738564> done(0xf);
    UnkSwarmMembers_l738564* members = self->Members();
    u32 first = 0xffffffff;
    u32 last = 0;
    for (s32 i = 0; i < members->count; ++i) {
        UnkSwarmMember_l738564& member = members->data[i];
        if (!member.active)
            continue;
        UnkSwarmSlot_l738564 slot = member.slot;
        UnkSwarmSlot_l738564 next = member.next;
        if (slot.group == current && next.group == slot.group) {
            // Moving within the current group: widen the span of path segments to block.
            if (first > slot.index)
                first = slot.index;
            if (next.index < path_count && last < next.index)
                last = next.index;
            continue;
        }
        UnkSwarmSlot_l738564* end = done.data + done.count;
        bool seen = std::find_if(done.data, end, [slot](const UnkSwarmSlot_l738564& s) { return s == slot; }) != end;
        // Only members moving into or out of the current group, once per slot.
        if (slot.group != current && next.group != current)
            continue;
        if (seen)
            continue;
        CVector2D from;
        CVector2D to;
        fn_710007b430(&from, fn_7100749e44(self, &slot)->Position());
        fn_710007b430(&to, fn_7100749e44(self, &next)->Position());
        if (UnkTileBlocker_l738564* blocker = PlaceBlocker(self, from, to)) {
            done.EmplaceBack(static_cast<UnkSwarmSlot_l738564&&>(slot));
            placed->PushBack(blocker);
        }
    }
    for (u32 j = first; j < last; ++j) {
        CVector2D from;
        CVector2D to;
        fn_710007b430(&from, self->path[j].Position());
        fn_710007b430(&to, self->path[j + 1].Position());
        if (UnkTileBlocker_l738564* blocker = PlaceBlocker(self, from, to))
            placed->PushBack(blocker);
    }
    done.Free();
}
