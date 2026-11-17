// CGrappleBeamComponent unit (tools/tu_map.py unit 1277): adds a target to the 16 nearest kept in a fixed
// array: while there is room it is appended (3 when that filled the array, 1 otherwise); when full (2) the
// last one is compared with it, then the array is sorted nearest first (fn_710081ccd0). The entry and
// comparator types are fn_710081ccd0's. Names and layouts are placeholders.
#include <string.h>

#include "base/math/CVectorOps.h"
#include "game/CAbilityShinespark.h"
#include "types.h"

struct UnkObj_710081ccd0 {
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
    virtual u32 vf_0x58();
};

struct UnkRefCounted_710081ccd0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    union {
        s32 ref_count;
    };
    UnkObj_710081ccd0* field_0x10;
};

// A handle: assigned out of line (fn_71002a5d1c), copied by assigning to an empty one.
struct UnkHandleA_710081ccd0 {
    UnkRefCounted_710081ccd0* ptr;

    UnkHandleA_710081ccd0(const UnkHandleA_710081ccd0& o) {
        ptr = nullptr;
        UnkRefCounted_710081ccd0* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_710081ccd0* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleA_710081ccd0& operator=(const UnkHandleA_710081ccd0& o);  // fn_71002a5d1c
    ~UnkHandleA_710081ccd0() {
        UnkRefCounted_710081ccd0* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

// The same with the pointer as a union member: the copy reloads it after the increment.
struct UnkHandleB_710081ccd0 {
    union {
        UnkRefCounted_710081ccd0* ptr;
    };

    UnkHandleB_710081ccd0(const UnkHandleB_710081ccd0& o) {
        ptr = nullptr;
        UnkRefCounted_710081ccd0* p = o.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (UnkRefCounted_710081ccd0* old = ptr) {
                if (--old->ref_count == 0)
                    old->Delete();
            }
            ptr = p;
        }
    }
    UnkHandleB_710081ccd0& operator=(const UnkHandleB_710081ccd0& o);  // fn_71002a5d1c
    ~UnkHandleB_710081ccd0() {
        UnkRefCounted_710081ccd0* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

struct UnkEntry_710081ccd0 {
    UnkHandleA_710081ccd0 a;
    UnkHandleB_710081ccd0 b;
    u32 field_0x10;
    f32 x;
    f32 y;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;

#define UNK_COPY_710081ccd0(o)                                                                     \
    field_0x10 = o.field_0x10;                                                                     \
    x = o.x;                                                                     \
    y = o.y;                                                                                       \
    field_0x1c = o.field_0x1c;                                                                     \
    field_0x20 = o.field_0x20;                                                                     \
    field_0x24 = o.field_0x24;                                                                     \
    field_0x28 = o.field_0x28;                                                                     \
    field_0x2c = o.field_0x2c;                                                                     \
    field_0x30 = o.field_0x30;                                                                     \
    field_0x34 = o.field_0x34;                                                                     \
    field_0x38 = o.field_0x38;                                                                     \
    field_0x3c = o.field_0x3c;                                                                     \
    field_0x40 = o.field_0x40;

    UnkEntry_710081ccd0(const UnkEntry_710081ccd0& o) : a(o.a), b(o.b) { UNK_COPY_710081ccd0(o) }
    UnkEntry_710081ccd0& operator=(const UnkEntry_710081ccd0& o) {
        if (&o != this) {
            a = o.a;
            b = o.b;
            UNK_COPY_710081ccd0(o)
        }
        return *this;
    }
#undef UNK_COPY_710081ccd0
};

struct UnkMask_710081ccd0 {
    s32 count;
    u32 words[4];  // the caller copies count words
};

static inline bool Near_710081ccd0(const f32& x, const f32& y) {
    return x - 0.1f <= y && x + 0.1f >= y;
}

using base::math::CVector3D;

struct UnkCmp_710081ccd0 {
    CVector3D pos;
    UnkMask_710081ccd0 mask;

    bool Has(const UnkEntry_710081ccd0& e) const {
        return (e.a.ptr->field_0x10->vf_0x58() & mask.words[0]) != 0;
    }
    bool operator()(const UnkEntry_710081ccd0& a, const UnkEntry_710081ccd0& b) const {
        CVector3D pb = {b.x, b.y, 0.0f};
        f32 db = DistanceSq(pos, pb);
        CVector3D pa = {a.x, a.y, 0.0f};
        f32 da = DistanceSq(pos, pa);
        f32 dab = DistanceSq(pb, pa);
        if (Near_710081ccd0(dab, 0.0f)) {
            bool ha = Has(a);
            bool hb = Has(b);
            if (ha && !hb)
                return true;
            if (!ha && hb)
                return false;
        }
        return da < db;
    }
};

void fn_710081ccd0(UnkEntry_710081ccd0* first, UnkEntry_710081ccd0* last, UnkCmp_710081ccd0& cmp);

// What adding a target did.
struct UnkTargetAdded_l81c834 {
    u32 value;  // 1 added, 2 the array was full, 3 added and now full

    UnkTargetAdded_l81c834() {}
    UnkTargetAdded_l81c834(u32 v) : value(v) {}
    UnkTargetAdded_l81c834(const UnkTargetAdded_l81c834& o) : value(o.value) {}
};

// A fixed array of 16 targets.
struct __attribute__((packed)) UnkTargetArray_l81c834 {
    u32 count;
    UnkEntry_710081ccd0 entries[16];  // 0x4

    // Copies `entry` into the first free slot.
    void Append(const UnkEntry_710081ccd0& entry) {
        fn_7100331c7c(&entries[count], &entry);
        ++count;
    }
};

// The 16 nearest targets and the comparator that orders them.
struct __attribute__((packed)) UnkTargets_l81c834 {
    void** vtable;
    UnkTargetArray_l81c834 targets;  // 0x8
    UnkCmp_710081ccd0 cmp;           // 0x48c
};
static_assert(__builtin_offsetof(UnkTargets_l81c834, cmp) == 0x48c, "layout");

// The comparator's copy: its mask copies only the words in use.
static inline void CopyCmp(UnkCmp_710081ccd0& dst, const UnkCmp_710081ccd0& src) {
    dst.pos.x = src.pos.x;
    dst.pos.y = src.pos.y;
    dst.pos.z = src.pos.z;
    u32 n = src.mask.count;
    memcpy(dst.mask.words, src.mask.words, n * sizeof(u32));
    dst.mask.count = n;
}

UnkTargetAdded_l81c834 fn_710081c834(UnkTargets_l81c834* self, const UnkEntry_710081ccd0& entry) {
    s32 room = 16 - self->targets.count;
    UnkTargetAdded_l81c834 result;  // set on both paths below
    if (room <= 0) {
        result = 2;
        u32 last = self->targets.count - 1;
        // Faithful to the original: the last entry is assigned to itself, not replaced by `entry`.
        if (self->cmp(entry, self->targets.entries[last]))
            self->targets.entries[self->targets.count - 1] = self->targets.entries[last];
    } else {
        self->targets.Append(entry);
        result = room == 1 ? 3 : 1;
    }
    UnkEntry_710081ccd0* first = self->targets.entries;
    UnkEntry_710081ccd0* end = self->targets.entries + self->targets.count;
    UnkCmp_710081ccd0 cmp;
    CopyCmp(cmp, self->cmp);
    fn_710081ccd0(first, end, cmp);
    return result;
}
