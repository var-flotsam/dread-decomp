// Unit 301 (tools/tu_map.py): the view box (a box with virtual accessors, src/base/spatial/UnkFrustumCull.cpp,
// followed by its corners quantised to tenths): its default constructor (a box of the largest range,
// fn_710021234c), its copy of another box (fn_7100212438) and growing a box to take in a range of
// boxes (fn_710021257c). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkSphere.h"
#include "types.h"

// A box with virtual accessors (0x20 bytes): its minimum and maximum corners.
struct UnkPoolBox {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void SetMin(const UnkVec3f* v);  // vf_0x28
    virtual void SetMax(const UnkVec3f* v);  // vf_0x30
    virtual UnkVec3f GetMin();               // vf_0x38
    virtual UnkVec3f GetMax();               // vf_0x40
    UnkVec3f min;
    UnkVec3f max;
};

void fn_7100215dcc(UnkPoolBox* self, const UnkVec3f* min, const UnkVec3f* max);
extern const char g_unk_7101a20090[];

// A box and its corners quantised to tenths (0x40 bytes).
struct UnkViewBox_b0247 {
    UnkPoolBox box;
    s32 cellMin[3];  // 0x20
    s32 cellMax[3];  // 0x2c
    u32 field_0x38;
    u32 pad_0x3c;
};

__attribute__((always_inline)) static inline void Quantise(UnkViewBox_b0247* self) {
    UnkPoolBox* box = &self->box;
    *reinterpret_cast<const char**>(box) = g_unk_7101a20090 + 0x10;
    UnkVec3f min = box->GetMin();
    UnkVec3f max = box->GetMax();
    self->cellMin[0] = (s32)(min.x * 10.0f);
    self->cellMin[1] = (s32)(min.y * 10.0f);
    self->cellMin[2] = (s32)(min.z * 10.0f);
    self->cellMax[0] = (s32)(max.x * 10.0f);
    self->cellMax[1] = (s32)(max.y * 10.0f);
    self->cellMax[2] = (s32)(max.z * 10.0f);
    self->field_0x38 = 0;
}

void fn_710021234c(UnkViewBox_b0247* self) {
    UnkVec3f min(0.0f, 0.0f, 0.0f);
    UnkVec3f max(0.0f, 0.0f, 0.0f);
    reinterpret_cast<u32&>(min.x) = 0x7f7fffff;
    reinterpret_cast<u32&>(min.y) = 0x7f7fffff;
    reinterpret_cast<u32&>(min.z) = 0x7f7fffff;
    reinterpret_cast<u32&>(max.x) = 0xff7fffff;
    reinterpret_cast<u32&>(max.y) = 0xff7fffff;
    reinterpret_cast<u32&>(max.z) = 0xff7fffff;
    fn_7100215dcc(&self->box, &min, &max);
    Quantise(self);
}

void fn_7100212438(UnkViewBox_b0247* self, UnkPoolBox* other) {
    {
        UnkVec3f min = other->GetMin();
        UnkVec3f max = other->GetMax();
        fn_7100215dcc(&self->box, &min, &max);
    }
    Quantise(self);
}

struct UnkBoxSlot_b0247 {
    u8 bytes[0x40];
};

void fn_710021257c(UnkPoolBox* self, UnkBoxSlot_b0247* const* first, UnkBoxSlot_b0247* const* last) {
    UnkBoxSlot_b0247* it = *first;
    while (it != *last) {
        UnkPoolBox* element = reinterpret_cast<UnkPoolBox*>(it);
        {
            UnkVec3f m;
            UnkVec3f a = self->GetMin();
            UnkVec3f b = element->GetMin();
            m.x = a.x < b.x ? a.x : b.x;
            m.y = a.y < b.y ? a.y : b.y;
            m.z = a.z < b.z ? a.z : b.z;
            self->SetMin(&m);
        }
        ++it;
        {
            UnkVec3f m;
            UnkVec3f a = self->GetMax();
            UnkVec3f b = element->GetMax();
            m.x = a.x > b.x ? a.x : b.x;
            m.y = a.y > b.y ? a.y : b.y;
            m.z = a.z > b.z ? a.z : b.z;
            self->SetMax(&m);
        }
    }
}
