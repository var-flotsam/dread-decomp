// CCaterzillaSpawnPointComponent unit (tools/tu_map.py unit 1527), the chain of spawn segments
// (next at +0x420, as in CCaterzillaSpawnPointComponent_b47.cpp). fn_7100a16a3c: one step of a
// segment's grow counter (+0x3cc up to +0x3c4, the next segment's counter along), the flag at +0x460
// set on the first step and the step count at +0x464. fn_7100a16554: per-frame update with `dt`: a
// segment that may still choose (mode 2 at +0x3bc, +0x3df clear) becomes head or tail once its
// owner (or the next segment's) passes fn_7100d7bea0; the timers at +0x3d8 (limit +0x3d4) and
// +0x42c (limit +0x428) reset the counters when they run out. fn_7100a16170: the component's
// destructor (the 0x40-byte polymorphic links at +0x438, the callback list at +0x3e0, the spawn-point
// base destructor fn_7101125c90). fn_7100a17628: whether the link `index` (fn_7100247340, its actor
// resolved from the reflected property on first use) blocks the direction `dir` from `pos`: by the
// actor's mode (fn_71003f7fa0) when it has one, else when the direction to the actor's position
// (fn_710007b430) is not within 0.1 of `dir` either way. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkTU2704.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"


struct UnkCaterzillaSegment_ovb0054 {
    u8 pad_0x0[0x3c4];
    union {
        u32 field_0x3c4;
    };
    u8 pad_0x3c8[0x4];
    union {
        u32 field_0x3cc;
    };
    bool field_0x3d0;
    u8 pad_0x3d1[0xb];
    bool field_0x3dc;
    u8 pad_0x3dd[0x420 - 0x3dd];
    UnkCaterzillaSegment_ovb0054* next;  // 0x420
    u8 pad_0x428[0x38];
    bool field_0x460;
    u8 pad_0x461[3];
    union {
        u32 field_0x464;
    };
};
static_assert(__builtin_offsetof(UnkCaterzillaSegment_ovb0054, field_0x464) == 0x464, "segment layout");

void fn_7100a16a3c(UnkCaterzillaSegment_ovb0054* self) {
    self->field_0x460 = self->field_0x464 == 0;
    if (self->field_0x3cc < self->field_0x3c4) {
        if (self->next)
            ++self->next->field_0x3cc;
        ++self->field_0x3cc;
    }
    self->field_0x3d0 = false;
    if (self->field_0x3cc == self->field_0x3c4)
        self->field_0x3dc = false;
    ++self->field_0x464;
}


bool fn_7100d7bea0(UnkActor* owner, bool flag);

struct UnkCaterzillaSpawn2_ovb0054 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x3bc - 0x38];
    u32 field_0x3bc;
    u32 field_0x3c0;
    u32 field_0x3c4;
    u32 field_0x3c8;
    u32 field_0x3cc;
    bool field_0x3d0;
    u8 pad_0x3d1[0x3];
    f32 field_0x3d4;
    f32 field_0x3d8;
    bool field_0x3dc;
    bool field_0x3dd;
    bool field_0x3de;
    bool field_0x3df;
    u8 pad_0x3e0[0x40];
    union {
        UnkCaterzillaSpawn2_ovb0054* next;  // 0x420
    };
    f32 field_0x428;
    f32 field_0x42c;
    bool field_0x430;
    u8 pad_0x431[0x33];
    u32 field_0x464;
};
static_assert(__builtin_offsetof(UnkCaterzillaSpawn2_ovb0054, field_0x464) == 0x464, "spawn point layout");

static inline void Link2(UnkCaterzillaSpawn2_ovb0054* self, bool head) {
    self->field_0x3d0 = head;
    self->field_0x3dc = head;
    self->next->field_0x3d0 = !head;
    self->next->field_0x3dc = !head;
    self->field_0x3de = head;
    self->next->field_0x3de = !head;
    self->field_0x3df = true;
    self->next->field_0x3df = true;
}

void fn_7100a16554(UnkCaterzillaSpawn2_ovb0054* self, f32 dt) {
    if (self->next && !self->field_0x3df && self->field_0x3bc == 2) {
        if (fn_7100d7bea0(self->owner, false) && !self->field_0x3de)
            Link2(self, true);
        else if (fn_7100d7bea0(self->next->owner, false) && !self->next->field_0x3de)
            Link2(self, false);
    }
    if (self->field_0x3dd) {
        self->field_0x3d8 = self->field_0x3d8 + dt;
        if (self->field_0x3d8 >= self->field_0x3d4) {
            self->field_0x3d0 = true;
            self->field_0x3dc = true;
            self->field_0x3dd = false;
            self->field_0x3d8 = 0.0f;
            self->field_0x464 = 0;
            self->field_0x3cc = 0;
            self->field_0x3c0 = self->field_0x3c4;
        }
    }
    if (self->field_0x430) {
        self->field_0x42c = self->field_0x42c + dt;
        if (self->field_0x42c >= self->field_0x428) {
            self->field_0x3dc = true;
            self->field_0x3d0 = true;
            self->field_0x42c = 0.0f;
            self->field_0x430 = false;
            u32 rest = self->field_0x3c8;
            self->field_0x3c4 = rest;
            self->field_0x3c0 = rest;
            self->field_0x3cc = 0;
            self->field_0x464 = 0;
            if (self->next)
                self->next->field_0x3c4 = rest;
        }
    }
}


extern char g_unk_7101a8c428[];

struct UnkCaterzillaCallbacks_ovb0054;
template <>
struct base::UnkCallbackTraits<UnkCaterzillaCallbacks_ovb0054> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

struct UnkCaterzillaSegmentLink_ovb0054 {
    virtual ~UnkCaterzillaSegmentLink_ovb0054();
    u8 pad_0x8[0x38];
};

struct UnkCaterzillaSpawnPoint_ovb0054 {
    char* vtable;
    u8 pad_0x8[0x3e0 - 0x8];
    base::UnkCallbackListShort<UnkCaterzillaCallbacks_ovb0054> callbacks;  // 0x3e0
    u8 pad_0x420[0x18];
    base::UnkVector<UnkCaterzillaSegmentLink_ovb0054> links;  // 0x438
};
static_assert(__builtin_offsetof(UnkCaterzillaSpawnPoint_ovb0054, links) == 0x438, "layout");

void fn_7100a16170(UnkCaterzillaSpawnPoint_ovb0054* self) {
    self->vtable = g_unk_7101a8c428 + 0x10;
    self->links.Free();
    self->callbacks.~UnkCallbackListShort();
    fn_7101125c90(reinterpret_cast<char*>(self));
}


using base::math::CVector2D;
using base::math::CVector3D;

struct UnkCaterzillaActor_ovb0054 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkCaterzillaTargetRef_ovb0054 {
    u8 pad_0x0[0x10];
    UnkCaterzillaActor_ovb0054* actor;  // 0x10
};

struct UnkCaterzillaLink_ovb0054 {
    u8 pad_0x0[0x8];
    u8 property[0x28];  // 0x8, a reflected property naming the actor
    u32 state;          // 0x30
    u8 pad_0x34[0x4];
    UnkCaterzillaTargetRef_ovb0054* target;  // 0x38, a base::UnkRefHandle

    UnkCaterzillaTargetRef_ovb0054* Target() {
        if (state == 1) {
            {
                UnkRefPtr ref = fn_710023b580(property);
                fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&target),
                              reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return target;
    }
};

struct UnkCaterzillaMode_ovb0054 {
    u8 pad_0x0[0x40];
    u32 mode;  // 0x40
};

struct UnkCaterzillaLinks_ovb0054;

struct UnkCaterzillaSpawn3_ovb0054 {
    u8 pad_0x0[0x438];
    u8 links[0x28];  // 0x438
};

UnkCaterzillaMode_ovb0054* fn_71003f7fa0(UnkCaterzillaActor_ovb0054* actor);

bool fn_7100a17628(UnkCaterzillaSpawn3_ovb0054* self, const CVector2D* pos, u32 index, const CVector2D* dir) {
    auto* links = reinterpret_cast<UnkCaterzillaLinks_ovb0054*>(self->links);
    UnkCaterzillaTargetRef_ovb0054* target = fn_7100247340(links, index)->Target();
    UnkCaterzillaMode_ovb0054* mode = fn_71003f7fa0(target ? target->actor : nullptr);
    if (mode && mode->mode != 0)
        return mode->mode != 1;
    UnkCaterzillaActor_ovb0054* actor = fn_7100247340(links, index)->Target()->actor;
    f32 x = actor->x;
    f32 y = actor->y;
    f32 z = actor->z;
    CVector3D p;
    p.x = x;
    p.y = y;
    p.z = z;
    CVector2D q;
    fn_710007b430(&q, &p);
    f32 dx = q.x - pos->x;
    f32 dy = q.y - pos->y;
    f32 len = sqrtf(dx * dx + dy * dy);
    if (len != 0.0f) {
        dx = dx / len;
        dy = dy / len;
    }
    f32 d = dx * dir->x + dy * dir->y;
    f32 lo = d - 0.1f;
    f32 hi = d + 0.1f;
    if (hi >= 1.0f && lo <= 1.0f)
        return false;
    return !(lo <= -1.0f) || !(hi >= -1.0f);
}
