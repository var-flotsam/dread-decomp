// CChozoCommanderAttack's unit (tools/tu_map.py unit 644): aims the commander's beam. From the beam's
// node on the model, casts a segment 5000 units along the node's facing (on the plane); the first hit
// (or nothing) goes to `hit`, and the beam object is placed at the node, turned towards the end point
// and stretched to it with the given width. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU133.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

// The segment query's callback vtable (reached through the GOT).
extern void* g_unk_7101a23e70[];

// A counted object deleted through its virtual destructor when the last reference goes.
struct UnkCounted_l427048 {
    virtual void vf_0x0();
    virtual ~UnkCounted_l427048();
    s32 ref_count;
};

// A counted reference; assigned out of line (fn_71002a5d1c).
struct UnkHitRef_l427048 {
    UnkCounted_l427048* ptr;

    UnkHitRef_l427048() : ptr(nullptr) {}
    ~UnkHitRef_l427048() {
        if (UnkCounted_l427048* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRef_l427048& operator=(const UnkHitRef_l427048& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    void Release() {
        if (UnkCounted_l427048* old = ptr) {
            if (--old->ref_count == 0)
                delete old;
            ptr = nullptr;
        }
    }
};

// A zero whose constructor is not constexpr, so the static is guarded.
struct UnkStaticZero_l427048 {
    u32 value;
    UnkStaticZero_l427048() : value(0) {}
};

inline const UnkStaticZero_l427048& StaticZero_l427048() {
    static UnkStaticZero_l427048 s_value asm("g_unk_71726b5354");
    return s_value;
}

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, the
// reversed normal and the normal, and flags.
struct UnkCollisionHit_l427048 {
    UnkHitRef_l427048 object;
    UnkHitRef_l427048 field_0x8;
    s32 id;
    UnkVec3f point;
    UnkVec3f field_0x20;
    UnkVec3f normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l427048()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
    UnkCollisionHit_l427048& operator=(const UnkCollisionHit_l427048& o) {
        if (this == &o)
            return *this;
        object = o.object;
        field_0x8 = o.field_0x8;
        id = o.id;
        point = o.point;
        field_0x20 = o.field_0x20;
        normal = o.normal;
        field_0x38 = o.field_0x38;
        flags = o.flags;
        field_0x40 = o.field_0x40;
        return *this;
    }
    // No hit.
    void Reset() {
        object.Release();
        field_0x8.Release();
        flags = StaticZero_l427048().value;
    }
};
static_assert(sizeof(UnkCollisionHit_l427048) == 0x48, "UnkCollisionHit_l427048 layout");

// The segment query's callback object: the closest hit so far and the segment's origin.
struct UnkBeamQuery_l427048 {
    void** vtable;
    UnkCollisionHit_l427048 hit;
    bool field_0x50;
    u32 field_0x54;
    UnkVec3f origin;  // 0x58
    u64 field_0x68;
    bool field_0x70;
    u32 field_0x74;
    UnkVec3f field_0x78;
    u64 field_0x88;

    explicit UnkBeamQuery_l427048(const UnkVec3f& o) : origin(o), field_0x78(o) {
        vtable = g_unk_7101a23e70 + 2;
        field_0x50 = false;
        field_0x68 = 0;
        field_0x70 = false;
        field_0x88 = 0;
    }
    ~UnkBeamQuery_l427048() { vtable = g_unk_7101a20ed8 + 2; }
};

static_assert(sizeof(UnkBeamQuery_l427048) == 0x90, "UnkBeamQuery_l427048 layout");

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkBeamSegment_l427048 {
    f32 start_x, start_y;
    f32 end_x, end_y;
    f32 delta_x, delta_y;
};

struct UnkBeam_l427048 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void SetScale(const UnkVec3f* scale);  // 0x20
    void SetTarget(u32 slot, void* target, bool flag) {
        reinterpret_cast<void (*)(UnkBeam_l427048*, u32, void*, bool)>((*reinterpret_cast<void***>(this))[0xe0 / 8])(
            this, slot, target, flag);
    }
};

struct UnkCommanderOwner_l427048 {
    u8 pad_0x0[0x228];
    UnkEntity* model;  // 0x228
};

struct UnkCommanderAttack_l427048 {
    u8 pad_0x0[0x8];
    UnkCommanderOwner_l427048* owner;  // 0x8
    u8 pad_0x10[0x28];
    bool active;  // 0x38
    u8 pad_0x39[0x7];
    UnkBeam_l427048* beam;  // 0x40
};

// The model node named by an id: its transform, when the model has it.
static inline bool FindNode(UnkEntity* model, const CStrId& name, UnkMatrix44* out) {
    u64 key = name.data->Key();
    return fn_71011735e8(model, &key, out, 0);
}

void fn_7100427048(UnkCommanderAttack_l427048* self, f32 unused, f32 width, void* first, void* second,
                   UnkCollisionHit_l427048* hit) {
    if (!self->active)
        return;
    UnkEntity* model = self->owner->model;
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    UnkMatrix44 node = {};
    node.m[15] = 1.0f;
    node.m[10] = 1.0f;
    node.m[5] = 1.0f;
    node.m[0] = 1.0f;
    FindNode(model, *reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0xd48 + 0x10), &node);
    UnkVec3f facing(node.m[0], node.m[4], 0.0f);
    UnkVec3f position(node.m[3], node.m[7], node.m[11]);
    UnkVec3f start = position;
    f32 inv = InvLengthEps(facing);

    UnkVec3f end(facing.x * inv * 5000.0f + start.x, facing.y * inv * 5000.0f + start.y,
                 facing.z * inv * 5000.0f + start.z);
    UnkBeamSegment_l427048 segment;
    segment.start_x = start.x;
    segment.start_y = start.y;
    segment.end_x = end.x;
    segment.end_y = end.y;
    segment.delta_x = end.x - start.x;
    segment.delta_y = end.y - start.y;
    UnkBeamQuery_l427048 query(start);
    if (fn_7100d5c518(tree, 0x80429, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
        *hit = query.hit;
        end = hit->point;
    } else {
        hit->Reset();
    }

    f32 length = Distance(end, start);
    UnkVec3f direction = Normalized(end - position);
    fn_7100145f28(reinterpret_cast<UnkSwitchObject*>(self->beam), &position);
    UnkVec3f angles(0.0f, 0.0f, atan2f(direction.y, direction.x));
    fn_7100145f3c(reinterpret_cast<UnkSwitchObject*>(self->beam), &angles);
    UnkVec3f scale(length * 0.999f, width, width);
    self->beam->SetScale(&scale);
    self->beam->SetTarget(0, first, true);
    self->beam->SetTarget(2, second, true);
}
