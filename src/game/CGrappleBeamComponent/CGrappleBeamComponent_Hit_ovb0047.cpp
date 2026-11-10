// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): the beam's hit handler (the hit's object at +0x10 of
// +0x10 of its first word, the position at +0x14, the material at +0x40 when bit 6 of +0x3c is set). Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" int rand();

using base::reflection::UnkReflObject;

struct UnkGrappleBeam_ovb0047;
void fn_710081ba20(UnkGrappleBeam_ovb0047* self, const UnkVec3f* at, const UnkVec3f* rotation);

struct UnkGrappleHitRefB_ovb0047 {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};
struct UnkGrappleHitRefA_ovb0047 {
    u8 pad_0x0[0x10];
    UnkGrappleHitRefB_ovb0047* ref;  // 0x10
};
struct UnkGrappleHitData_ovb0047 {
    UnkGrappleHitRefA_ovb0047* ref;
    u8 pad_0x8[0xc];
    UnkVec3f position;  // 0x14
    u8 pad_0x20[0x1c];
    u8 flags;  // 0x3c
    u8 pad_0x3d[3];
    u32 material;  // 0x40
};
static_assert(__builtin_offsetof(UnkGrappleHitData_ovb0047, material) == 0x40, "hit");

struct UnkGrappleHitListener_ovb0047 {
    void** vtable;
    void OnHit(u32 id, const UnkVec3f* at, u32 material, void* extra) {
        reinterpret_cast<void (*)(UnkGrappleHitListener_ovb0047*, u32, const UnkVec3f*, u32, void*)>(
            vtable[0x158 / 8])(this, id, at, material, extra);
    }
};

struct UnkGrappleSmStateB_ovb0047 {
    u8 pad_0x0[0x10];
    u32 id;
};

struct UnkGrappleBeamHit_ovb0047 {
    u8 pad_0x0[0x3fc];
    u32 id;  // 0x3fc
    u8 pad_0x400[0x490 - 0x400];
    UnkGrappleSmStateB_ovb0047* current;  // 0x490
    u8 pad_0x498[0x524 - 0x498];
    u32 pending;  // 0x524
    u8 pad_0x528[0x530 - 0x528];
    UnkGrappleHitListener_ovb0047* listener;  // 0x530
    u8 pad_0x538[0x714 - 0x538];
    bool field_0x714;
};
static_assert(__builtin_offsetof(UnkGrappleBeamHit_ovb0047, field_0x714) == 0x714, "layout");

struct UnkGrappleCollider_ovb0047 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

void fn_7100ac7db8(UnkGrappleBeamHit_ovb0047* self, UnkReflObject* tiles, void* a, void* b,
                   UnkGrappleHitData_ovb0047* hit);
s32 fn_7100ac7f08(UnkGrappleBeamHit_ovb0047* self, void* object, UnkGrappleHitData_ovb0047* hit, void* extra);
UnkReflObject* fn_710034dd54(void* object);
void fn_710081c220(UnkGrappleBeamHit_ovb0047* self, UnkGrappleHitData_ovb0047* hit, UnkReflObject* point);

// A random value in [min, max] from rand()'s 15 bits.
static inline f32 RandomRange_ovb0047(f32 min, f32 max) {
    f32 t = (f32)((f64)(rand() & 0x7fff) * (1.0 / 32767.0));
    return t * (max - min) + min;
}

template <const base::reflection::CClass* (*Getter)()>
static inline UnkReflObject* Cast_ovb0047(UnkReflObject* object) {
    return fn_710009d50c(object->vf_0x0(), Getter()) ? object : nullptr;
}

// What a grapple beam does on a hit: scenario hits are reported, breakable tile groups are hit, grapple
// points are attached to; anything else that does not refuse it gets a spark turned at random.
bool fn_710081bcd0(UnkGrappleBeamHit_ovb0047* self, void* a, void* b, UnkGrappleHitData_ovb0047* hit) {
    UnkReflObject* object = hit->ref->ref->object;
    const base::reflection::CClass* cls = object->vf_0x0();
    if (fn_710009d50c(cls, fn_710119ea84())) {
        self->listener->OnHit(self->id, &hit->position, (hit->flags & 0x40) ? hit->material : 0, nullptr);
        self->field_0x714 = true;
    }
    if (fn_710009d50c(cls, fn_7100ca0484())) {
        UnkReflObject* tiles = object ? Cast_ovb0047<fn_7100ca0484>(object) : nullptr;
        self->listener->OnHit(self->id, &hit->position, 1, nullptr);
        fn_7100ac7db8(self, tiles, a, b, hit);
        self->field_0x714 = true;
    } else if (fn_710009d50c(cls, fn_710085fc44())) {
        void* owner = reinterpret_cast<UnkGrappleCollider_ovb0047*>(Cast_ovb0047<fn_710085fc44>(object))->field_0x30;
        if (UnkReflObject* point = fn_710034dd54(owner)) {
            fn_710081c220(self, hit, point);
            u32 state = self->pending;
            if (state == 5) {
                if (!self->current)
                    goto refuse;
                state = self->current->id;
            }
            if (state == 3)
                return true;
        }
    refuse:
        s32 r = fn_7100ac7f08(self, owner, hit, nullptr);
        if (r == 1)
            return false;
        if (r == 0)
            self->field_0x714 = true;
    }
    fn_710081ba20(reinterpret_cast<UnkGrappleBeam_ovb0047*>(self), &hit->position,
                  &static_cast<const UnkVec3f&>(UnkVec3f(0.0f, 0.0f, RandomRange_ovb0047(0.0f, 6.2831855f))));
    return true;
}
