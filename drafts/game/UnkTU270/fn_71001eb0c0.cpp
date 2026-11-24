// Unit 270 (beside UnkTU270_Pool_ovb0254.cpp): places an object in the pool. Unless the pool is disabled or full it
// takes the next free entry, gives it the object, its kind and transform, fits its sphere and box round the object's
// bounds in world space, and adds it to (or, with `add` false, takes it out of) its kind's table. Names and layouts
// are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkSphere.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
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

struct UnkBoundsList;
void fn_71001eb6b4(void*, UnkBoundsList* list, UnkSphere* sphere);
void fn_71001eb7f0(void*, UnkBoundsList* list, UnkPoolBox* box);
UnkVec3f fn_7100215ec4(UnkPoolBox* box);  // the box's minimum corner
UnkVec3f fn_7100215edc(UnkPoolBox* box);  // its maximum corner

struct UnkPlacedObject_l1eb0c0 {
    u8 pad_0x0[0x78];
    UnkBoundsList* bounds;  // 0x78
};

struct UnkPlacedEntry_l1eb0c0 {
    UnkPlacedObject_l1eb0c0* object;
    UnkMatrix44 matrix;  // 0x8
    UnkSphere sphere;    // 0x48
    UnkPoolBox box;      // 0x98
    s32 cellMin[3];      // 0xb8
    s32 cellMax[3];      // 0xc4
    s32 slot;            // 0xd0: index in the slot table, or -1
    s32 kindSlot;        // 0xd4: index in its kind's table, or -1
    s32 kind;            // 0xd8
    u32 field_0xdc;

    void Clear() {
        field_0xdc = 0;
        object = nullptr;
        slot = -1;
        kindSlot = -1;
    }
};
static_assert(sizeof(UnkPlacedEntry_l1eb0c0) == 0xe0, "entry layout");

constexpr u64 kPlacedPoolSize = 25000;

// The pool's counts: the slots in use, then the size of each kind's table.
struct UnkPlacedCounts_l1eb0c0 {
    u32 slots;   // 0x0
    u32 seen;    // 0x4
    u32 kind0;   // 0x8
    u32 kind1;   // 0xc
    bool disabled;  // 0x10
    bool field_0x11;
    bool field_0x12;
    u8 changed;  // 0x13
};

struct UnkPlacedPool_l1eb0c0 {
    void* vtable;
    UnkPlacedEntry_l1eb0c0* slots[kPlacedPoolSize];    // 0x8
    UnkPlacedEntry_l1eb0c0 entries[kPlacedPoolSize];   // 0x30d48
    UnkPlacedEntry_l1eb0c0* table0[kPlacedPoolSize];   // 0x588048: the entries of kind 0
    UnkPlacedEntry_l1eb0c0* table1[kPlacedPoolSize];   // 0x5b8d88
    u8 pad_0x5e9ac8[0xac];
    UnkPlacedCounts_l1eb0c0 counts;  // 0x5e9b74
};
static_assert(__builtin_offsetof(UnkPlacedPool_l1eb0c0, counts) == 0x5e9b74, "pool layout");

// The eight corners of the box from `lo` to `hi` (x in bit 0 of the index, y in bit 1, z in bit 2).
struct UnkBoxCorners_l1eb0c0 {
    UnkVec3f corner[8];
    UnkBoxCorners_l1eb0c0(const UnkVec3f& lo, const UnkVec3f& hi)
        : corner{UnkVec3f(lo.x, lo.y, lo.z), UnkVec3f(hi.x, lo.y, lo.z), UnkVec3f(lo.x, hi.y, lo.z),
                 UnkVec3f(hi.x, hi.y, lo.z), UnkVec3f(lo.x, lo.y, hi.z), UnkVec3f(hi.x, lo.y, hi.z),
                 UnkVec3f(lo.x, hi.y, hi.z), UnkVec3f(hi.x, hi.y, hi.z)} {}
};

bool fn_71001eb0c0(UnkPlacedPool_l1eb0c0* self, UnkPlacedObject_l1eb0c0* object, UnkXform* xform, bool add, s32 kind,
                   UnkPlacedEntry_l1eb0c0** out) {
    if (!self->counts.disabled && self->counts.slots < kPlacedPoolSize) {
        u32& used = self->counts.slots;
        UnkPlacedEntry_l1eb0c0* entry = self->slots[used];
        entry->Clear();
        entry->slot = used++;
        entry->kindSlot = -1;
        entry->kind = kind;
        entry->object = object;
        UnkCopyMatrix(&entry->matrix, *reinterpret_cast<const UnkMatrix44*>(xform));
        fn_71001eb6b4(self, object->bounds, &entry->sphere);
        fn_71001eb7f0(self, object->bounds, &entry->box);

        // The sphere: its centre into world space, its radius by the largest scale.
        UnkVec3f scale = fn_7100076cec(xform);
        UnkVec3f center = entry->sphere.pos;
        center = TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(&entry->matrix), &center);
        f32 largest = scale.x < scale.y ? scale.y : scale.x;
        largest = largest < scale.z ? scale.z : largest;
        f32 radius = entry->sphere.radius * largest;
        fn_7100216978(&entry->sphere, &center);
        fn_710021693c(&entry->sphere, radius);

        // The box: the bounds of its corners in world space.
        UnkPoolBox* box = &entry->box;
        UnkVec3f lo = fn_7100215ec4(box);
        UnkVec3f hi = fn_7100215edc(box);
        UnkBoxCorners_l1eb0c0 corners(lo, hi);
        UnkVec3f min(3.40282347e+38f, 3.40282347e+38f, 3.40282347e+38f);
        UnkVec3f max(-3.40282347e+38f, -3.40282347e+38f, -3.40282347e+38f);
        for (s32 i = 0; i < 8; ++i) {
            UnkVec3f p = TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(xform), &corners.corner[i]);
            min.x = p.x < min.x ? p.x : min.x;
            min.y = p.y < min.y ? p.y : min.y;
            min.z = p.z < min.z ? p.z : min.z;
            max.x = p.x > max.x ? p.x : max.x;
            max.y = p.y > max.y ? p.y : max.y;
            max.z = p.z > max.z ? p.z : max.z;
        }
        box->SetMin(&min);
        box->SetMax(&max);

        *out = entry;
        if (entry && entry >= &self->entries[0] && entry < &self->entries[kPlacedPoolSize]) {
            s32 index = entry->kindSlot;
            s32 entry_kind = entry->kind;
            UnkPlacedEntry_l1eb0c0** table = entry_kind == 0 ? self->table0 : self->table1;
            u32* count = entry_kind == 0 ? &self->counts.kind0 : &self->counts.kind1;
            if (add) {
                if (index < 0) {
                    self->counts.changed = (entry_kind == 0) | (self->counts.changed != 0);
                    entry->kindSlot = *count;
                    table[*count] = entry;
                    ++*count;
                }
            } else if (index >= 0 && index < static_cast<s32>(*count)) {
                self->counts.changed = (entry_kind == 0) | (self->counts.changed != 0);
                u32 last = --*count;
                UnkPlacedEntry_l1eb0c0* moved = table[last];
                table[entry->kindSlot] = moved;
                moved->kindSlot = entry->kindSlot;
                table[*count] = entry;
                entry->kindSlot = -1;
            }
        }
        return true;
    }
    return false;
}
