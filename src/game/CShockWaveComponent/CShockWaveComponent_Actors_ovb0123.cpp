// CShockWaveComponent's unit (tools/tu_map.py): the actors it tracks, in the node list at 0x250
// (allocated from its allocator with the list's flags). fn_71009f4bf8 adds an actor, unless the actor
// is the player (fn_7100defcd4) or the game manager's second player (+0x29a0, its +0x10) and one of
// the two is already in the list; the same as CShockWavePoolComponent's (fn_71009f7da4).
// fn_71009f3f1c starts the wave from a definition: it copies the definition into the member at +0x40
// (fn_71009f3bf8), takes its speed (+0x124 over the duration +0x130, 0 without one) and its position
// (+0x114), dropped onto the ground below it (1500 units down) when +0x160 is set, and moves the owner
// there (its slot 0xd0). fn_71009f4d18 answers whether an actor is hit: a tracked actor only while
// active (+0x244) with the timer (+0x298) at 0 or when forced; outside a full circle (+0x168) only
// within the half angle (+0x28c) around the given direction; then when its box (fn_7100d7bc70) touches
// the sphere of radius +0x284 around the owner (fn_7100126af0). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game/CShockWaveComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkOwner;

struct UnkShockWaveNode_ovb0123 {
    UnkOwner* value;
    UnkShockWaveNode_ovb0123* next;
};

struct UnkShockWavePlayerRef_ovb0123 {
    u8 pad_0x0[0x10];
    UnkOwner* actor;  // 0x10
};

struct UnkShockWaveList_ovb0123 {
    void** vtable;
    u32 count;
    UnkShockWaveNode_ovb0123* head;
    UnkShockWaveNode_ovb0123* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    s32 IndexOf(UnkOwner* value) const {
        u32 i = 0;
        for (UnkShockWaveNode_ovb0123* n = head; n; n = n->next, ++i)
            if (n->value == value)
                return static_cast<s32>(i);
        return -1;
    }

    void PushBack(UnkOwner* value) {
        auto* node = static_cast<UnkShockWaveNode_ovb0123*>(allocator->vf_0x40(0x10, 8, flags));
        if (!node)
            return;
        node->value = value;
        node->next = nullptr;
        if (count == 0) {
            tail = node;
            head = node;
            count = 1;
            return;
        }
        tail->next = node;
        // An untyped store: the original reloads the count after it.
        __builtin_memset(&node->next, 0, sizeof(node->next));
        u32 n = count + 1;
        tail = node;
        count = n;
    }
};

struct UnkShockWaveComponent_ovb0123 {
    u8 pad_0x0[0x250];
    UnkShockWaveList_ovb0123 list;  // 0x250
};

static inline UnkOwner* SecondPlayer(UnkGameManager* gm) {
    auto* ref = *reinterpret_cast<UnkShockWavePlayerRef_ovb0123**>(reinterpret_cast<char*>(gm) + 0x29a0);
    return ref ? ref->actor : nullptr;
}

static inline bool IsPlayer(UnkOwner* actor) {
    return fn_7100defcd4(g_unk_7101d47310) == actor || SecondPlayer(g_unk_7101d47310) == actor;
}

void fn_71009f4bf8(UnkShockWaveComponent_ovb0123* self, UnkOwner* actor) {
    if (IsPlayer(actor)) {
        if (self->list.IndexOf(fn_7100defcd4(g_unk_7101d47310)) >= 0)
            return;
        if (self->list.IndexOf(SecondPlayer(g_unk_7101d47310)) >= 0)
            return;
    }
    self->list.PushBack(actor);
}

struct UnkFloorRef_ovb0048;
bool fn_7100d5b714(void* tree, u32 mask, const UnkVec3f* from, const UnkVec3f* to, UnkFloorRef_ovb0048* hit,
                   bool closest, bool flag, f32 radius);

struct UnkShockWaveMember;

// A reference to a counted object.
struct UnkShockWaveHitSlot_ovb0123 {
    base::UnkRefCounted* ptr;

    UnkShockWaveHitSlot_ovb0123() : ptr(nullptr) {}
    ~UnkShockWaveHitSlot_ovb0123() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

// A collision hit (as CMovementComponent's): two references, an id, then the point.
struct UnkShockWaveHit_ovb0123 {
    UnkShockWaveHitSlot_ovb0123 slot_a;
    UnkShockWaveHitSlot_ovb0123 slot_b;
    u32 field_0x10[13];

    UnkShockWaveHit_ovb0123() {
        field_0x10[0] = 0xffffffff;
        for (u32 i = 1; i < 13; i++)
            field_0x10[i] = 0;
    }
    const UnkVec3f& Point() const { return *reinterpret_cast<const UnkVec3f*>(&field_0x10[1]); }
};

struct UnkShockWaveDef_ovb0123 {
    u8 pad_0x0[0x114];
    UnkVec3f position;  // 0x114
    u8 pad_0x120[4];
    f32 distance;  // 0x124
    u8 pad_0x128[8];
    f32 duration;  // 0x130
};

struct UnkShockWaveOwner_ovb0123 {
    void** vtable;
    u8 pad_0x8[0x3c];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64

    void vf_0xd0(const UnkVec3f* position, bool flag) {
        reinterpret_cast<void (*)(UnkShockWaveOwner_ovb0123*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, position,
                                                                                                       flag);
    }
};

struct UnkShockWaveClass_ovb0123 {
    u8 pad_0x0[0x3c8];
    u32 collision_mask;  // 0x3c8
};

struct UnkShockWaveSource2_ovb0123 {
    u8 pad_0x0[0x218];
    UnkShockWaveClass_ovb0123* char_class;  // 0x218
};

struct UnkShockWaveSource_ovb0123 {
    u8 pad_0x0[0x10];
    UnkShockWaveSource2_ovb0123* owner;  // 0x10
};

struct UnkShockWave_ovb0123 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkShockWaveOwner_ovb0123* owner;  // 0x30
    u8 pad_0x38[8];
    u8 member[0x110];  // 0x40
    bool probe;        // 0x150
    u8 pad_0x151[0xf];
    bool drop;  // 0x160
    u8 pad_0x161[0x67];
    UnkShockWaveSource_ovb0123* source;  // 0x1c8
    u8 pad_0x1d0[0xb4];
    f32 length;  // 0x284
    f32 speed;   // 0x288
    u8 pad_0x28c[6];
    bool hit;  // 0x292
    u8 pad_0x293;
    u32 field_0x294;
    u8 pad_0x298[4];
    bool field_0x29c;

    UnkVec3f vf_0x170() {
        return reinterpret_cast<UnkVec3f (*)(UnkShockWave_ovb0123*)>(vtable[0x170 / 8])(this);
    }
};
static_assert(__builtin_offsetof(UnkShockWave_ovb0123, field_0x29c) == 0x29c, "layout");

void fn_71009f3f1c(UnkShockWave_ovb0123* self, UnkShockWaveDef_ovb0123* def) {
    fn_71009f3bf8(reinterpret_cast<UnkShockWaveMember*>(self->member),
                  reinterpret_cast<const UnkShockWaveMember*>(def));
    self->speed = 0.0f;
    if (def->duration != 0.0f)
        self->speed = def->distance / def->duration;
    UnkVec3f position = def->position;
    if (self->drop) {
        void* tree = g_unk_7101d47310->field_0x608->field_0x820;
        UnkShockWaveHit_ovb0123 hit;
        UnkVec3f below(position.x, position.y + -1500.0f, position.z);
        if (fn_7100d5b714(tree, 1, &position, &below, reinterpret_cast<UnkFloorRef_ovb0048*>(&hit), true, false,
                          0.1f))
            position = hit.Point();
    }
    self->owner->vf_0xd0(&position, true);
    self->field_0x294 = 0;
    self->field_0x29c = false;
}

struct UnkEntityBox {
    base::math::CVector3D min;
    base::math::CVector3D max;
};
struct UnkEntityBounds;
bool fn_7100126af0(const UnkVec3f* center, const UnkEntityBox* box, f32 radius);

struct UnkShockWaveActor_ovb0123 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkShockWaveHitTest_ovb0123 {
    u8 pad_0x0[0x30];
    UnkShockWaveActor_ovb0123* owner;  // 0x30
    u8 pad_0x38[0x130];
    f32 arc;  // 0x168
    u8 pad_0x16c[0xd8];
    bool active;  // 0x244
    u8 pad_0x245[0xb];
    UnkShockWaveList_ovb0123 list;  // 0x250
    u8 pad_0x280[4];
    f32 radius;  // 0x284
    u8 pad_0x288[4];
    f32 half_angle;  // 0x28c
    u8 pad_0x290[8];
    f32 timer;  // 0x298
};
static_assert(__builtin_offsetof(UnkShockWaveHitTest_ovb0123, timer) == 0x298, "layout");

bool fn_71009f4d18(UnkShockWaveHitTest_ovb0123* self, UnkOwner* actor, const UnkVec3f* direction, bool force) {
    bool listed;
    if (IsPlayer(actor))
        listed = self->list.IndexOf(fn_7100defcd4(g_unk_7101d47310)) >= 0 ||
                 self->list.IndexOf(SecondPlayer(g_unk_7101d47310)) >= 0;
    else
        listed = self->list.IndexOf(actor) >= 0;
    if (listed && !(self->active && (self->timer == 0.0f || force)))
        return false;
    const UnkShockWaveActor_ovb0123* a = reinterpret_cast<const UnkShockWaveActor_ovb0123*>(actor);
    if (self->arc < 6.2831855f) {
        UnkVec3f p(a->x, a->y, a->z);
        const UnkShockWaveActor_ovb0123* o = self->owner;
        UnkVec3f q(o->x, o->y, o->z);
        f32 dx = p.x - q.x;
        f32 dy = p.y - q.y;
        f32 dz = p.z - q.z;
        f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
        f32 nx = dx * inv;
        f32 ny = dy * inv;
        f32 nz = dz * inv;
        f32 d = direction->x * nx + direction->y * ny + direction->z * nz;
        if (!(d >= cosf(self->half_angle)))
            return false;
    }
    UnkEntityBox box = fn_7100d7bc70(reinterpret_cast<const UnkEntityBounds*>(actor));
    const UnkShockWaveActor_ovb0123* o = self->owner;
    UnkVec3f center(o->x, o->y, o->z);
    return fn_7100126af0(&center, &box, self->radius);
}
