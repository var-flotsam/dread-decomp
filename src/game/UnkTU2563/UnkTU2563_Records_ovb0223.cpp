// The CMinimapData unit (unit 2563, tools/tu_map.py): the template instantiations of its keyed record arrays and
// the room components that rebuild a point record. Arrays of 0x138-byte records (copy construction, copy
// assignment and inserts, the record's special members out of line), arrays of 0x60-byte records with a pointer
// array or a string-id array alongside (copy construction and assignment, shrinking, swap-erase, inserts), maps
// of string ids to vectors of string ids, keyed arrays of 0x28- and 0x60-byte items with string-id keys (grow,
// swap-erase), the object file loader and the components that move, flatten and bound every point record of a
// class of reflected objects. Names and layouts are placeholders inferred from the code.
#include <new>
#include "base/UnkCloneRecords.h"
#include "base/UnkFreedVectorPair.h"
#include "base/UnkHashStream.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflObjectRead.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CChangeStageNavMeshItemComponent.h"
#include "game/CCharClassWaterPoolComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU25.h"
#include "game/UnkTU2563.h"
#include "game/UnkTU365.h"
#include "game/UnkTU83.h"
#include "types.h"
#include "base/UnkStrPair.h"
#include "base/UnkFileReader.h"
// The CMinimapData unit (unit 2563): arrays of 0x138-byte records (copy construction, copy assignment and
// inserts, with the record's special members out of line) and the room components that rebuild a point record
// (a moved, flattened and bounded point array) for every reflected object of one class. Names and layouts are
// placeholders inferred from the code.


struct UnkRec138_b0223;
void fn_7100e50e20(void* dst, void* src);

struct UnkRec138_b0223 {
    u8 unk_0x0[0x138];

    UnkRec138_b0223(UnkRec138_b0223&& other) { fn_7100e51078(this, &other); }
    UnkRec138_b0223(const UnkRec138_b0223& other) { fn_7100e50e20(this, const_cast<UnkRec138_b0223*>(&other)); }
    ~UnkRec138_b0223() { fn_7100e4e310(reinterpret_cast<char*>(this)); }
};

struct UnkRec138Vec_b0223 {
    UnkRec138_b0223* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reserve(u32 n) {
        if (capacity < n) {
            UnkRec138_b0223* p = static_cast<UnkRec138_b0223*>(allocator->vf_0x40((u64)n * sizeof(UnkRec138_b0223), 8, flags));
            if (data) {
                UnkRec138_b0223* src = data;
                UnkRec138_b0223* dst = p;
                for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                    new (dst) UnkRec138_b0223(static_cast<UnkRec138_b0223&&>(*src));
                    src->~UnkRec138_b0223();
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }
    __attribute__((always_inline)) void PushBackCopy(const UnkRec138_b0223& value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        Reserve(new_capacity);
        n = count;
        new (&data[n]) UnkRec138_b0223(value);
        ++count;
    }
};

void fn_7100e6257c(UnkRec138Vec_b0223* self, const UnkRec138Vec_b0223* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    if (other->count)
        for (const UnkRec138_b0223* p = other->data, *end = p + other->count; p != end; ++p)
            self->PushBackCopy(*p);
}

UnkRec138Vec_b0223* fn_7100e62780(UnkRec138Vec_b0223* self, const UnkRec138Vec_b0223* other) {
    if (self != other) {
        for (u32 i = 0; i < self->count; ++i)
            self->data[i].~UnkRec138_b0223();
        self->count = 0;
        self->Reserve(other->capacity);
        if (other->count)
            for (const UnkRec138_b0223* p = other->data, *end = p + other->count; p != end; ++p)
                self->PushBackCopy(*p);
    }
    return self;
}

struct UnkRec138Insert_b0223 : UnkRec138Vec_b0223 {
    __attribute__((always_inline)) UnkRec138_b0223* MoveTo(u32 new_capacity) {
        UnkRec138_b0223* p = static_cast<UnkRec138_b0223*>(allocator->vf_0x40((u64)new_capacity * sizeof(UnkRec138_b0223), 8, flags));
        if (data) {
            UnkRec138_b0223* src = data;
            UnkRec138_b0223* dst = p;
            for (u32 i = 0; i < count; ++i, ++dst, ++src) {
                new (dst) UnkRec138_b0223(static_cast<UnkRec138_b0223&&>(*src));
                src->~UnkRec138_b0223();
            }
            allocator->vf_0x48(data);
        }
        return p;
    }
    template <class V>
    __attribute__((always_inline)) void Insert(V&& value, u32 index) {
        u32 cap = capacity;
        u32 n = count;
        UnkRec138_b0223* p;
        if (n == index) {
            u32 new_capacity = cap;
            if (cap <= n && step != 0)
                new_capacity = (index / step + 1) * step;
            if ((s32)new_capacity <= (s32)index)
                for (;;) {
                }
            if (cap < new_capacity) {
                p = MoveTo(new_capacity);
                index = count;
                data = p;
                capacity = new_capacity;
            } else {
                p = data;
            }
        } else {
            if (cap <= n && step != 0) {
                u32 grown = (n / step + 1) * step;
                if (cap < grown) {
                    UnkRec138_b0223* q = MoveTo(grown);
                    data = q;
                    capacity = grown;
                    n = count;
                }
            }
            for (u32 i = n - 1; i >= index; --i) {
                UnkRec138_b0223* q = &data[i];
                new (q + 1) UnkRec138_b0223(static_cast<UnkRec138_b0223&&>(*q));
                data[i].~UnkRec138_b0223();
                if (i == 0)
                    break;
            }
            new (reinterpret_cast<char*>(data) + (u64)index * sizeof(UnkRec138_b0223)) UnkRec138_b0223(static_cast<V&&>(value));
            ++count;
            return;
        }
        new (&p[index]) UnkRec138_b0223(static_cast<V&&>(value));
        ++count;
    }
};

void fn_7100e629b4(UnkRec138Insert_b0223* self, const UnkRec138_b0223* value, u32 index) {
    self->Insert(*value, index);
}

void fn_7100e62bec(UnkRec138Insert_b0223* self, UnkRec138_b0223* value, u32 index) {
    self->Insert(static_cast<UnkRec138_b0223&&>(*value), index);
}

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkMinimapRec_b0223;
UnkMinimapRec_b0223* fn_7100e44f58(void* self, const u64* key);

struct UnkPlaceOwner_b0223 {
    u8 unk_0x0[0x30];
    u64 id;  // 0x30
    u8 unk_0x38[0xc];
    f32 x;  // 0x44
    u8 unk_0x48[0xc];
    f32 y;  // 0x54
    u8 unk_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkBoundsRec_b0223 {
    CVector3D* points;  // 0x0
    u32 count;          // 0x8
    u8 unk_0xc[0x1c];
    u8 field_0x28[0x28];
    CVector2D min;  // 0x50
    CVector2D max;  // 0x58
};

struct UnkShapeResult_b0223 {
    u64 a, b, c;
    ~UnkShapeResult_b0223() {}
};

struct UnkShape_b0223 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
    virtual void vf_0x20();
    virtual UnkShapeResult_b0223 vf_0x28(void* arg);
    u64 field_0x8;
    u8 unk_0x10[0x98];
    u8 field_0xa8[1];
};

struct UnkHandle_b0223 {
    u8 unk_0x0[0x10];
    UnkReflObject* target;
};
struct UnkHandleList_b0223 {
    UnkHandle_b0223** data;
    u32 count;
};
struct UnkRoom_b0223;
struct UnkTracker_b0223 {
    u8 unk_0x0[0x30];
    UnkShape_b0223* shape;
};
struct UnkTracker;

struct UnkFreezeRoom_ovb0106;
struct UnkFreezeList_ovb0106;
UnkHandleList_b0223* fn_7100914230(UnkRoom_b0223* self);
UnkHandleList_b0223* fn_7100a26740(UnkRoom_b0223* self);
void fn_7100131020(void* shape, UnkBoundsRec_b0223* rec, void* v, bool flag);
void fn_710012f880(UnkShapeResult_b0223* shape, UnkBoundsRec_b0223* rec, void* v, bool flag);

struct UnkPoolComp_b0223 : UnkReflObject {
    u8 unk_0x8[0x10];
    UnkPlaceOwner_b0223* owner;  // 0x18
};

// per-class fields are in the subclasses below
template <u32 Off>
struct UnkWithShape : UnkPoolComp_b0223 {
    u8 unk_0x20[Off - 0x20];
    UnkShape_b0223* shape;
};

// Shifts and bounds of one record: the points of the record moved by the owner's position, flattened to a height,
// then the 2D bounds of the points.
template <u32 ShapeOff, u32 RoomOff, u32 ZBits, bool ViaTracker, class Cls>
static inline __attribute__((always_inline)) void ForEachPlace(UnkRoom_b0223* self, UnkHandleList_b0223* list, Cls cls) {
    for (UnkHandle_b0223 **it = list->data, **end = list->data + list->count; it != end; ++it) {
        UnkHandle_b0223* handle = *it;
        if (!handle)
            continue;
        UnkReflObject* target = handle->target;
        if (!target || !fn_710009d50c(target->vf_0x0(), cls()))
            continue;
        UnkWithShape<ShapeOff>* comp = static_cast<UnkWithShape<ShapeOff>*>(target);
        UnkBoundsRec_b0223* rec = reinterpret_cast<UnkBoundsRec_b0223*>(
            fn_7100e44f58(reinterpret_cast<char*>(self) + RoomOff, &comp->owner->id));
        UnkShape_b0223* shape;
        if constexpr (ViaTracker)
            shape = reinterpret_cast<UnkTracker_b0223*>(fn_710028aca8(comp->owner))->shape;
        else
            shape = comp->shape;
        if constexpr (!ViaTracker) {
            if (!shape)
                continue;
        }
        if (ViaTracker && !shape) {
        } else if (shape->vf_0x18() == 0x10) {
            fn_7100131020(shape->field_0xa8, rec, rec->field_0x28, true);
        } else if constexpr (!ViaTracker) {
            UnkShapeResult_b0223 r = shape->vf_0x28(&shape->field_0x8);
            fn_710012f880(&r, rec, rec->field_0x28, true);
        }
        {
            UnkPlaceOwner_b0223* o = comp->owner;
            f32 ox = o->x, oy = o->y, oz = o->z;
            for (CVector3D *v = rec->points, *e = v + rec->count; v != e; ++v) {
                if (fn_7100deabe4(g_unk_7101d47310)) {
                    v->x += ox;
                    v->y += oy;
                    v->z += oz;
                }
                { u32 zbits = ZBits; __builtin_memcpy(&v->z, &zbits, 4); }
            }
        }
        u32 kMaxBits = 0x7f7fffff;
        __builtin_memcpy(&rec->min.x, &kMaxBits, 4);
        __builtin_memcpy(&rec->min.y, &kMaxBits, 4);
        rec->max = {-__FLT_MAX__, -__FLT_MAX__};
        for (CVector3D *q = rec->points, *e = q + rec->count; q != e; ++q) {
            CVector2D v;
            fn_710007b430(&v, q);
            f32 x = v.x;
            f32 y = v.y;
            CVector2D lo = rec->min;
            f32 nx = lo.x < x ? lo.x : x;
            f32 ny = lo.y < y ? lo.y : y;
            rec->min.x = nx;
            // a volatile store is never paired with the one before it
            *reinterpret_cast<volatile f32*>(&rec->min.y) = ny;
            CVector2D hi = rec->max;
            f32 mx = hi.x > x ? hi.x : x;
            f32 my = hi.y > y ? hi.y : y;
            rec->max.x = mx;
            *reinterpret_cast<volatile f32*>(&rec->max.y) = my;
        }
    }
}

void fn_7100e41028(UnkRoom_b0223* self) {
    ForEachPlace<0xc00, 0x60f0, 0x437a0000, false>(self, reinterpret_cast<UnkHandleList_b0223*>(fn_71009088a0(reinterpret_cast<UnkFreezeRoom_ovb0106*>(self))), fn_71008fe62c);
}

void fn_7100e41268(UnkRoom_b0223* self) {
    ForEachPlace<0x30, 0x6050, 0x43fa0000, false>(self, fn_7100914230(self), fn_710076238c);
}

void fn_7100e41d40(UnkRoom_b0223* self) {
    ForEachPlace<0x30, 0x6118, 0x43c80000, false>(self, fn_7100914230(self), fn_7100751c60);
}

void fn_7100e40a34(UnkRoom_b0223* self) {
    ForEachPlace<0x30, 0x6078, 0, true>(self, fn_7100a26740(self), fn_7100a259f4);
}



// Removes the record `it` names by moving the last record into its slot (and the pointer alongside).
UnkFreedVectorPair* fn_7100e551fc(UnkSecondPtrArray<UnkFreedVectorPair>* self, UnkFreedVectorPair* it) {
    u32 index = (u32)(it - self->data);
    self->data[index].~UnkFreedVectorPair();
    u32 last = self->count - 1;
    if (last != index) {
        UnkFreedVectorPair* dst = &self->data[index];
        UnkFreedVectorPair* src = &self->data[last];
        new (dst) UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(*src));
        self->data[self->count - 1].~UnkFreedVectorPair();
        void** ptrs = static_cast<void**>(self->field_0x8);
        // Copied bytewise (the original reloads the count after it).
        __builtin_memcpy(&ptrs[index], &ptrs[self->count - 1], sizeof(void*));
    }
    self->count = self->count - 1;
    return &self->data[index];
}

// Shrinks the array to its count: moves the records (and the pointers alongside) into storage of exactly that size.
void fn_7100e54ef0(UnkSecondPtrArray<UnkFreedVectorPair>* self) {
    if (self->count == self->capacity)
        return;
    UnkFreedVectorPair* data = static_cast<UnkFreedVectorPair*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkFreedVectorPair), 8, self->field_0x1c));
    void** ptrs = static_cast<void**>(self->allocator->vf_0x40((u64)self->count * sizeof(void*), 8, self->field_0x1c));
    if (UnkFreedVectorPair* old = self->data) {
        UnkFreedVectorPair* dst = data;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(*old));
            old->~UnkFreedVectorPair();
        }
        self->allocator->vf_0x48(self->data);
    }
    if (self->field_0x8) {
        __builtin_memcpy(ptrs, self->field_0x8, (u64)self->count * sizeof(void*));
        self->allocator->vf_0x48(self->field_0x8);
    }
    self->data = data;
    self->field_0x8 = ptrs;
    self->capacity = self->count;
}

// Appends a moved record (the PushBack of the same array).
void fn_7100e53418(UnkPairArray* self, UnkFreedVectorPair* value);

// Inserts a moved record at `index`: an append goes to the PushBack; otherwise the array grows by its step through
// the out-of-line Reserve and the later records move up one.
void fn_7100e531e8(UnkPairArray* self, UnkFreedVectorPair* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        fn_7100e53418(self, value);
        return;
    }
    u32 new_capacity = self->capacity;
    if (new_capacity <= n && self->step != 0)
        new_capacity = (n / self->step + 1) * self->step;
    fn_7100e52b58(self, new_capacity);
    for (u32 i = self->count - 1; i >= index; --i) {
        UnkFreedVectorPair* q = &self->data[i];
        new (q + 1) UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(*q));
        self->data[i].~UnkFreedVectorPair();
        if (i == 0)
            break;
    }
    new (&self->data[index]) UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(*value));
    ++self->count;
}



// base::UnkVector's layout, copied out of line.
struct UnkPart48 {
    union {
        void* data;
    };
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // base::UnkVector's defaults.
    __attribute__((always_inline)) UnkPart48()
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf),
          allocator(fn_7100083990()) {}
    UnkPart48(const UnkPart48& other);
    __attribute__((always_inline)) UnkPart48(UnkPart48&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    __attribute__((always_inline)) ~UnkPart48() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
inline UnkPart48::UnkPart48(const UnkPart48& other) { fn_71000cbae4(this, &other); }

// base::UnkVector<u16>'s layout.
struct UnkU16Array_b0223 {
    union {
        u16* data;
    };
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // base::UnkVector's defaults.
    __attribute__((always_inline)) UnkU16Array_b0223()
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf),
          allocator(fn_7100083990()) {}
    // Takes the settings, reserves the source's capacity and copies the elements.
    __attribute__((always_inline)) UnkU16Array_b0223(const UnkU16Array_b0223& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        u32 n = other.capacity;
        if (capacity < n) {
            u16* p = static_cast<u16*>(allocator->vf_0x40((u64)n * sizeof(u16), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(u16));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(u16));
        count = other.count;
    }
    __attribute__((always_inline)) UnkU16Array_b0223(UnkU16Array_b0223&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    __attribute__((always_inline)) ~UnkU16Array_b0223() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkRec60_b0223 {
    UnkPart48 field_0x0;
    UnkU16Array_b0223 field_0x28;
    // Floats (a bounds' minimum and maximum, by the defaults), copied as words.
    u32 field_0x50 = 0x7f7fffff;  // FLT_MAX
    u32 field_0x54 = 0x7f7fffff;
    u32 field_0x58 = 0xff7fffff;  // -FLT_MAX
    u32 field_0x5c = 0xff7fffff;

    UnkRec60_b0223() = default;

    __attribute__((always_inline)) UnkRec60_b0223(const UnkRec60_b0223& other)
        : field_0x0(other.field_0x0), field_0x28(other.field_0x28), field_0x50(other.field_0x50),
          field_0x54(other.field_0x54), field_0x58(other.field_0x58), field_0x5c(other.field_0x5c) {}
    __attribute__((always_inline)) UnkRec60_b0223(UnkRec60_b0223&& other)
        : field_0x0(static_cast<UnkPart48&&>(other.field_0x0)),
          field_0x28(static_cast<UnkU16Array_b0223&&>(other.field_0x28)), field_0x50(other.field_0x50),
          field_0x54(other.field_0x54), field_0x58(other.field_0x58), field_0x5c(other.field_0x5c) {}
};
static_assert(sizeof(UnkRec60_b0223) == 0x60, "UnkRec60_b0223 layout");


using base::global::CStrId;

// Records with their ids alongside in a second array.
struct UnkRec60IdArray_b0223 {
    UnkRec60_b0223* data;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100e56634(UnkRec60IdArray_b0223* self, const UnkRec60IdArray_b0223* other) {
    self->data = nullptr;
    self->ids = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->field_0x18 = other->field_0x18;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100e56814(self, other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        new (&self->data[i]) UnkRec60_b0223(other->data[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        // Stored bytewise (the original reloads the count after it), then retained.
        base::global::UnkStringData* id = other->ids[i].data;
        __builtin_memcpy(&self->ids[i], &id, sizeof(id));
        if (&g_unk_7101d47108->empty != id)
            ++id->ref_count;
    }
}

struct UnkRec60Array_b0223 {
    UnkRec60_b0223* data;
    void** field_0x8;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkRecord60Array;
void fn_7100e54ef0(UnkSecondPtrArray<UnkFreedVectorPair>* self);

UnkRec60Array_b0223* fn_7100e54cc4(UnkRec60Array_b0223* self, const UnkRec60Array_b0223* other) {
    if (self != other) {
        if (self->data)
            for (u64 i = 0; i < self->count; ++i)
                self->data[i].~UnkRec60_b0223();
        self->count = 0;
        if (other->count != self->capacity) {
            fn_7100e54ef0(reinterpret_cast<UnkSecondPtrArray<UnkFreedVectorPair>*>(self));
            fn_7100e54ad8(reinterpret_cast<UnkRecord60Array*>(self), other->count);
        }
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i)
            new (&self->data[i]) UnkRec60_b0223(other->data[i]);
        __builtin_memcpy(self->field_0x8, other->field_0x8, (u64)other->count * sizeof(void*));
        self->count = other->count;
    }
    return self;
}

void fn_7100e4efa0(UnkRec60IdArray_b0223* self);
void fn_7100e56c64(UnkRec60IdArray_b0223* self);

UnkRec60IdArray_b0223* fn_7100e56a74(UnkRec60IdArray_b0223* self, const UnkRec60IdArray_b0223* other) {
    if (self != other) {
        fn_7100e4efa0(self);
        if (other->count != self->capacity) {
            fn_7100e56c64(self);
            fn_7100e56814(self, other->count);
        }
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i)
            new (&self->data[i]) UnkRec60_b0223(other->data[i]);
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i) {
            // Stored bytewise (the original reloads the count after it), then retained.
            base::global::UnkStringData* id = other->ids[i].data;
            __builtin_memcpy(&self->ids[i], &id, sizeof(id));
            if (&g_unk_7101d47108->empty != id)
                ++id->ref_count;
        }
    }
    return self;
}



using base::UnkStrIdVec;
using base::global::CStrId;

// Vectors of string ids keyed by string ids, in two parallel arrays.
struct UnkStrIdVecMap_b0223 {
    UnkStrIdVec* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkNavMeshItemMember;
void fn_7100e5dd0c(UnkStrIdVecMap_b0223* self);

void fn_7100e5d924(UnkStrIdVecMap_b0223* self, const UnkStrIdVecMap_b0223* other) {
    self->values = nullptr;
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_71009ba838(self, other->count);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i)
        new (&self->values[i]) UnkStrIdVec(other->values[i]);
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        // Stored bytewise (the original reloads the count after it), then retained.
        base::global::UnkStringData* id = other->keys[i].data;
        __builtin_memcpy(&self->keys[i], &id, sizeof(id));
        if (&g_unk_7101d47108->empty != id)
            ++id->ref_count;
    }
}

UnkStrIdVecMap_b0223* fn_7100e5db10(UnkStrIdVecMap_b0223* self, const UnkStrIdVecMap_b0223* other) {
    if (self != other) {
        fn_71009ba68c(reinterpret_cast<UnkNavMeshItemMember*>(self));
        if (other->count != self->capacity) {
            fn_7100e5dd0c(self);
            fn_71009ba838(self, other->count);
        }
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i)
            new (&self->values[i]) UnkStrIdVec(other->values[i]);
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i) {
            base::global::UnkStringData* id = other->keys[i].data;
            __builtin_memcpy(&self->keys[i], &id, sizeof(id));
            if (&g_unk_7101d47108->empty != id)
                ++id->ref_count;
        }
    }
    return self;
}


using base::reflection::UnkReflObject;

// The id and path pair built from a path, destroyed out of line.
struct UnkPathPairTemp_b45a1 {
    void* id;
    void* path;
    ~UnkPathPairTemp_b45a1() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};

// The file read into memory and two words the opener sets (+0x38 is zero after a good read).

bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);

// The parser's callbacks of this unit.
extern UnkParseCallbacks g_unk_71727c7270 __attribute__((visibility("hidden")));

struct UnkMinimapFile_b0223 : UnkReflObject {
    u8 path[0x50];  // 0x8
    s32 field_0x58;
};

struct UnkMinimapLoadRequest_b0223 {
    u8 unk_0x0[0x10];
    u32 mode;
};

static inline s32 Parse(UnkFileReader* reader, UnkReadTarget* target) {
    UnkParseContext ctx(g_unk_71727c7270, reader, target);
    UnkReadEntry* end = target->entries + target->count;
    return reinterpret_cast<s32 (*)(UnkParseContext*, void*)>(&fn_710000f750)(
        &ctx, target->count ? end[-1].field_0x0->field_0x10 : nullptr);
}

// Reads the object from the file its path names; whether it parsed and the object is in its second state.
bool fn_7100e3e5c8(UnkMinimapFile_b0223* self, const UnkMinimapLoadRequest_b0223* request) {
    UnkFileReader reader;
    {
        UnkPathPairTemp_b45a1 s = fn_710001ab24(self->path);
        fn_710007be8c(&reader.file, 0x16, nullptr);
        reader.field_0x38 = 3;
        reader.field_0x3c = 2;
        bool opened;
        {
            base::global::CRntString mode("rb");
            opened = fn_710007c1ac(&reader, reinterpret_cast<UnkStrPair_0x10*>(&s), &mode);
        }
        if (opened)
            fn_710000ccd8(&reader);
    }
    UnkReadTarget target;
    {
        u32 mode = request->mode;
        const base::reflection::CClass* type = self->vf_0x0();
        base::reflection::UnkReflValue value;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        fn_71000a18d4(&value, type, self, true);
        fn_71000f0568(&target, &value, mode);
        fn_71000a1c60(&value);
    }
    bool ok = false;
    if (reader.file.position < reader.file.size && reader.field_0x38 == 0 && Parse(&reader, &target) != 0)
        ok = self->field_0x58 == 2;
    for (u64 i = 0; i < target.count; ++i) {
        UnkReadEntry& e = target.entries[i];
        if (e.ops) {
            e.ops->destroy(e.storage);
            e.ops = nullptr;
        }
    }
    fn_710007c0e4(&reader.file);
    return ok;
}



using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id after the
// decrement). Its pointer is stored bytewise, so the destructor of the moved-from id reloads the pool.
struct UnkMinimapId_b0223 {
    UnkStringData* data;

    UnkMinimapId_b0223(UnkMinimapId_b0223&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        // Keeps the pool pointer loaded after the copy, as the original loads it for each id.
        asm volatile("" ::: "memory");
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkMinimapId_b0223() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// An item of the keyed arrays (0x28 bytes): six plain words, a string id and a 16-bit value.
struct UnkBoundsItem_b0223 {
    u32 field_0x0, field_0x4, field_0x8, field_0xc, field_0x10, field_0x14;
    UnkMinimapId_b0223 name;  // 0x18
    u16 field_0x20;

    UnkBoundsItem_b0223(UnkBoundsItem_b0223&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), field_0x10(other.field_0x10), field_0x14(other.field_0x14),
          name(static_cast<UnkMinimapId_b0223&&>(other.name)) {
        __builtin_memcpy(&field_0x20, &other.field_0x20, sizeof(field_0x20));
    }
};
static_assert(sizeof(UnkBoundsItem_b0223) == 0x28, "layout");

template <class V>
struct UnkKeyedItems_b0223 {
    V* items;
    UnkMinimapId_b0223* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Grows both arrays, moving the items and the ids.
void fn_7100e5ba88(UnkKeyedItems_b0223<UnkBoundsItem_b0223>* self, u32 capacity) {
    using V = UnkBoundsItem_b0223;
    if (self->capacity < capacity) {
        V* items = static_cast<V*>(self->allocator->vf_0x40((u64)capacity * sizeof(V), 8, self->flags));
        auto* keys = static_cast<UnkMinimapId_b0223*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkMinimapId_b0223), 8, self->flags));
        if (V* src = self->items) {
            V* dst = items;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) V(static_cast<V&&>(*src));
                src->~V();
            }
            self->allocator->vf_0x48(self->items);
        }
        if (UnkMinimapId_b0223* src = self->keys) {
            UnkMinimapId_b0223* dst = keys;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                new (dst) UnkMinimapId_b0223(static_cast<UnkMinimapId_b0223&&>(*src));
                src->~UnkMinimapId_b0223();
            }
            self->allocator->vf_0x48(self->keys);
        }
        self->items = items;
        self->keys = keys;
        self->capacity = capacity;
    }
}

// Removes the item `it` names (and its id) by moving the last item into its slot.
UnkBoundsItem_b0223* fn_7100e5c218(UnkKeyedItems_b0223<UnkBoundsItem_b0223>* self, UnkBoundsItem_b0223* it) {
    u32 index = (u32)(it - self->items);
    self->items[index].~UnkBoundsItem_b0223();
    self->keys[index].~UnkMinimapId_b0223();
    u32 last = self->count - 1;
    if (last != index) {
        UnkBoundsItem_b0223* dst = &self->items[index];
        UnkBoundsItem_b0223* src = &self->items[last];
        new (dst) UnkBoundsItem_b0223(static_cast<UnkBoundsItem_b0223&&>(*src));
        self->items[self->count - 1].~UnkBoundsItem_b0223();
        UnkMinimapId_b0223* keys = self->keys;
        new (&keys[index]) UnkMinimapId_b0223(static_cast<UnkMinimapId_b0223&&>(keys[self->count - 1]));
        self->keys[self->count - 1].~UnkMinimapId_b0223();
    }
    self->count = self->count - 1;
    return &self->items[index];
}

// The list of an item: the fields of an array, moved in place.
struct UnkItemList_b0223 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void MoveFrom(UnkItemList_b0223* src) {
        data = src->data;
        count = src->count;
        capacity = src->capacity;
        step = src->step;
        field_0x14 = src->field_0x14;
        flags = src->flags;
        allocator = src->allocator;
        src->data = nullptr;
        src->count = 0;
        src->capacity = 0;
    }
};

// The second item type (0x60 bytes): ten plain words, two string ids and an array; its destructor is out of line.
struct UnkListItem_b0223 {
    u32 field_0x0, field_0x4, field_0x8, field_0xc, field_0x10;
    u32 field_0x14, field_0x18, field_0x1c, field_0x20, field_0x24;
    UnkStringData* name0;  // 0x28
    UnkStringData* name1;  // 0x30
    UnkItemList_b0223 list;  // 0x38

    __attribute__((always_inline)) UnkListItem_b0223(UnkListItem_b0223&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), field_0x10(other.field_0x10), field_0x14(other.field_0x14),
          field_0x18(other.field_0x18), field_0x1c(other.field_0x1c), field_0x20(other.field_0x20),
          field_0x24(other.field_0x24), name0(other.name0) {
        other.name0 = &g_unk_7101d47108->empty;
        name1 = other.name1;
        other.name1 = &g_unk_7101d47108->empty;
        list.MoveFrom(&other.list);
    }
    ~UnkListItem_b0223();
};
static_assert(sizeof(UnkListItem_b0223) == 0x60, "layout");

struct UnkRecord60;
inline UnkListItem_b0223::~UnkListItem_b0223() { fn_7100e4a1d0(reinterpret_cast<UnkRecord60*>(this)); }

UnkListItem_b0223* fn_7100e5a5f8(UnkKeyedItems_b0223<UnkListItem_b0223>* self, UnkListItem_b0223* it) {
    u32 index = (u32)(it - self->items);
    self->items[index].~UnkListItem_b0223();
    self->keys[index].~UnkMinimapId_b0223();
    u32 last = self->count - 1;
    if (last != index) {
        UnkListItem_b0223* dst = &self->items[index];
        UnkListItem_b0223* src = &self->items[last];
        new (dst) UnkListItem_b0223(static_cast<UnkListItem_b0223&&>(*src));
        self->items[self->count - 1].~UnkListItem_b0223();
        UnkMinimapId_b0223* keys = self->keys;
        new (&keys[index]) UnkMinimapId_b0223(static_cast<UnkMinimapId_b0223&&>(keys[self->count - 1]));
        self->keys[self->count - 1].~UnkMinimapId_b0223();
    }
    self->count = self->count - 1;
    return &self->items[index];
}
