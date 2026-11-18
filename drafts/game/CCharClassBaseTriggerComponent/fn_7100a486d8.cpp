// CCharClassBaseTriggerComponent's unit (tools/tu_map.py, unit 1562): while the trigger is active (+0x9bd),
// collects (up to 64) the active objects of the global list (g_unk_71726af0b0) inside its shape: each object's
// position is tested and, when it moved enough since the last frame, so is the path it swept (a circle at its
// last position). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CAbilityEnergyWave.h"
#include "game/UnkShapeHit_A5_b68.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

// The vtable of the contact collector below.
extern void* g_unk_7101a22c60[];

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

// What the shape tester reports back: the contact point at +0xc.
struct UnkContactCollector_la486d8 {
    void** vtable;
    s32 field_0x8;
    base::math::CVector3D point;  // 0xc
    base::math::CVector3D field_0x18;
    base::math::CVector3D field_0x24;
    f32 field_0x30;
    u32 field_0x34;

    UnkContactCollector_la486d8()
        : vtable(g_unk_7101a22c60 + 2), field_0x8(-1), point(), field_0x18(), field_0x24(), field_0x30(0.0f),
          field_0x34(0) {}
};

struct UnkTriggerable_la486d8 {
    u8 pad_0x0[0xd0];
    base::math::CVector3D pos;   // 0xd0
    base::math::CVector3D prev;  // 0xdc; the position last frame
    u8 pad_0xe8[0x62];
    bool active;  // 0x14a
};

struct UnkTriggerables_la486d8 {
    u8 pad_0x0[0x58];
    UnkTriggerable_la486d8** items;  // 0x58
    u32 count;                       // 0x60
};

struct __attribute__((packed, aligned(4))) UnkTriggerHit_la486d8 {
    UnkTriggerable_la486d8* object;
    base::math::CVector3D point;
    u8 pad_0x14[0x4];
};

struct UnkTriggerHits_la486d8 {
    u32 count;
    UnkTriggerHit_la486d8 hits[64];  // 0x4
};

struct UnkTriggerShape_la486d8 {
    u8 pad_0x0[0x158];
    void* owner;  // 0x158
};

struct UnkTriggerComponent_la486d8 {
    u8 pad_0x0[0x88];
    UnkTriggerShape_la486d8* shape;  // 0x88
    u8 pad_0x90[0x9bd - 0x90];
    bool active;  // 0x9bd
};

static inline base::math::CVector3D Sub(const base::math::CVector3D& a, const base::math::CVector3D& b) {
    base::math::CVector3D d = {a.x - b.x, a.y - b.y, a.z - b.z};
    return d;
}

static inline UnkShapeTester_A5_b68* Tester() {
    if (!g_unk_71726a47a0) {
        UnkShapeTester_A5_b68* t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return g_unk_71726a47a0;
}


// The position seen from the side, in the plane the shapes are tested in.
static inline void Flatten(base::math::CVector2D* out, const base::math::CVector3D& v) { fn_710007b430(out, &v); }

// Whether the trigger's shape holds the point `pos`.
static inline bool Holds(UnkTriggerComponent_la486d8* self, const base::math::CVector3D* pos,
                         UnkContactCollector_la486d8* collector) {
    base::math::CVector2D at;
    UnkShapeTester_A5_b68* tester = Tester();
    void* owner = self->shape->owner;
    Flatten(&at, *pos);
    return fn_7100d572f8(tester, owner, &at, &g_unk_710164779c, reinterpret_cast<u64>(collector)).flags & 1;
}

// Whether the trigger holds the object or (with `sweep`) the path it moved along `move` since last frame.
static inline bool Touches(UnkTriggerComponent_la486d8* self, UnkTriggerable_la486d8** it,
                           const base::math::CVector2D& move, UnkContactCollector_la486d8* collector, bool sweep) {
    if (Holds(self, &(*it)->pos, collector))
        return true;
    if (!sweep)
        return false;
    UnkLavaDropsShape_A2_b71 swept(&(*it)->prev, 10.0f);
    UnkShapeTester_A5_b68* tester = Tester();
    void* owner = self->shape->owner;
    UnkMatrix44 transform = fn_7100d36a2c(reinterpret_cast<UnkShape*>(self->shape));
    base::math::CVector2D dir = move;
    return fn_7100d541c0(tester, static_cast<UnkShape*>(owner), &transform, reinterpret_cast<UnkShape*>(&swept),
                         swept.body, &dir, collector, false)
               .bits &
           1;
}

void fn_7100a486d8(UnkTriggerComponent_la486d8* self, UnkTriggerHits_la486d8* out) {
    if (!self->active)
        return;
    UnkTriggerables_la486d8* list = static_cast<UnkTriggerables_la486d8*>(g_unk_71726af0b0);
    UnkTriggerable_la486d8** end = list->items + list->count;
    for (UnkTriggerable_la486d8** it = list->items; it != end; ++it) {
        if (!*it || !(*it)->active)
            continue;
        base::math::CVector2D move;
        Flatten(&move, Sub((*it)->pos, (*it)->prev));
        UnkContactCollector_la486d8 collector;
        bool hit;
        if (move.x * move.x + move.y * move.y < 0.1f)
            hit = Touches(self, it, move, &collector, false);
        else
            hit = Touches(self, it, move, &collector, true);
        if (!hit || out->count > 63)
            continue;
        UnkTriggerHit_la486d8& entry = out->hits[out->count];
        entry.object = *it;
        entry.point = collector.point;
        out->count++;
    }
}