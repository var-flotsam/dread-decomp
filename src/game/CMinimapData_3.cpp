// The CMinimapData unit (tools/tu_map.py, unit 2563). Names and layouts are placeholders inferred
// from the code.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkTU2563.h"
#include "game/UnkTU83.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

// A string id built empty, as the id's default constructor does.
struct UnkMinimapEmptyStrId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkMinimapEmptyStrId() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    __attribute__((always_inline)) UnkMinimapEmptyStrId(UnkMinimapEmptyStrId&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

// Two 2D bounds (empty: min at +FLT_MAX, max at -FLT_MAX), two string ids and an array.
struct UnkMinimapBoundsEntry {
    u64 field_0x0 = 0;
    f32 min0[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max0[2] = {-__FLT_MAX__, -__FLT_MAX__};
    f32 min1[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max1[2] = {-__FLT_MAX__, -__FLT_MAX__};
    UnkMinimapEmptyStrId name0;          // +0x28
    UnkMinimapEmptyStrId name1;          // +0x30
    base::UnkVector<void*> items{0xf};  // +0x38

    __attribute__((always_inline)) UnkMinimapBoundsEntry() {}
};
static_assert(sizeof(UnkMinimapBoundsEntry) == 0x60, "layout");

// Its constructor.
void fn_7100e4fba4(UnkMinimapBoundsEntry* self) {
    new (self) UnkMinimapBoundsEntry;
}

// One 2D bound (empty), a string id and a 16-bit value, built value-initialised.
struct UnkMinimapBoundsName {
    u32 field_0x0 = 0;
    u32 field_0x4 = 0;
    f32 min[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max[2] = {-__FLT_MAX__, -__FLT_MAX__};
    UnkMinimapEmptyStrId name;  // +0x18
    u16 field_0x20 = 0;

};
static_assert(sizeof(UnkMinimapBoundsName) == 0x28, "layout");

// The same element as the keyed arrays below hold it, moved in place.
struct UnkMinimapBoundsNameItem {
    u32 field_0x0;
    u32 field_0x4;
    f32 min[2];
    f32 max[2];
    UnkMinimapEmptyStrId name;  // +0x18
    u16 field_0x20;

    __attribute__((always_inline)) UnkMinimapBoundsNameItem(UnkMinimapBoundsNameItem&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), min{other.min[0], other.min[1]},
          max{other.max[0], other.max[1]}, name(static_cast<UnkMinimapEmptyStrId&&>(other.name)) {
        // Copied bitwise, as an implicit copy would (the original reloads the array after it).
        __builtin_memcpy(&field_0x20, &other.field_0x20, sizeof(field_0x20));
    }
};
static_assert(sizeof(UnkMinimapBoundsNameItem) == sizeof(UnkMinimapBoundsName), "layout");

// Its constructor.
void fn_7100e4f6c4(UnkMinimapBoundsName* self) {
    new (self) UnkMinimapBoundsName();
}

// Two arrays and an empty 2D bound (0x60 bytes), built value-initialised.
struct UnkMinimapListPair {
    base::UnkVector<void*> list0{0xf};
    base::UnkVector<void*> list1{0xf};  // +0x28
    f32 min[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max[2] = {-__FLT_MAX__, -__FLT_MAX__};
};
static_assert(sizeof(UnkMinimapListPair) == 0x60, "layout");

// Its constructor.
void fn_7100e4f0f8(UnkMinimapListPair* self) {
    new (self) UnkMinimapListPair();
}

// Three arrays, two flags and two empty 2D bounds (0xa8 bytes), built value-initialised.
struct UnkMinimapLayer {
    base::UnkVector<void*> list0{7};
    u32 field_0x28 = 0;
    bool field_0x2c = false;
    bool field_0x2d = true;
    f32 min0[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max0[2] = {-__FLT_MAX__, -__FLT_MAX__};
    u32 field_0x40 = 0;
    base::UnkVector<void*> list1{0xf};  // +0x48
    base::UnkVector<void*> list2{0xf};  // +0x70
    f32 min1[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max1[2] = {-__FLT_MAX__, -__FLT_MAX__};
};
static_assert(sizeof(UnkMinimapLayer) == 0xa8, "layout");

// Its constructor.
void fn_7100e505e0(UnkMinimapLayer* self) {
    new (self) UnkMinimapLayer();
}

// The base::UnkVector fields, without its default values (moved into in place).
struct UnkMinimapList {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

// An element of the keyed arrays below (0x80 bytes): a string id and two arrays among plain
// fields. Moving it takes the id and both arrays and leaves them empty.
struct UnkMinimapKeyedItem {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    base::global::UnkStringData* name;  // +0x10: a string id
    UnkMinimapList list0;               // +0x18
    UnkMinimapList list1;               // +0x40
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;

    __attribute__((always_inline)) UnkMinimapKeyedItem(UnkMinimapKeyedItem&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), name(other.name) {
        other.name = &g_unk_7101d47108->empty;
        MoveList(&list0, &other.list0);
        MoveList(&list1, &other.list1);
        field_0x68 = other.field_0x68;
        field_0x6c = other.field_0x6c;
        field_0x70 = other.field_0x70;
        field_0x74 = other.field_0x74;
        field_0x78 = other.field_0x78;
    }

    static void MoveList(UnkMinimapList* dst, UnkMinimapList* src) {
        dst->data = src->data;
        dst->count = src->count;
        dst->capacity = src->capacity;
        dst->step = src->step;
        dst->field_0x14 = src->field_0x14;
        dst->flags = src->flags;
        dst->allocator = src->allocator;
        src->data = nullptr;
        src->count = 0;
        src->capacity = 0;
    }
};
static_assert(sizeof(UnkMinimapKeyedItem) == 0x80, "layout");

// The second element type (0x60 bytes): ten plain words, two string ids and an array.
struct UnkMinimapKeyedItem2 {
    u32 field_0x0, field_0x4, field_0x8, field_0xc, field_0x10;
    u32 field_0x14, field_0x18, field_0x1c, field_0x20, field_0x24;
    base::global::UnkStringData* name0;  // +0x28: string ids
    base::global::UnkStringData* name1;  // +0x30
    UnkMinimapList list;                 // +0x38

    __attribute__((always_inline)) UnkMinimapKeyedItem2(UnkMinimapKeyedItem2&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), field_0x10(other.field_0x10), field_0x14(other.field_0x14),
          field_0x18(other.field_0x18), field_0x1c(other.field_0x1c), field_0x20(other.field_0x20),
          field_0x24(other.field_0x24), name0(other.name0) {
        other.name0 = &g_unk_7101d47108->empty;
        name1 = other.name1;
        other.name1 = &g_unk_7101d47108->empty;
        UnkMinimapKeyedItem::MoveList(&list, &other.list);
    }
};
static_assert(sizeof(UnkMinimapKeyedItem2) == 0x60, "layout");

// The third element type (0x60 bytes): two arrays and four plain words.
struct UnkMinimapKeyedItem3 {
    UnkMinimapList list0;
    UnkMinimapList list1;  // +0x28
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;

    __attribute__((always_inline)) UnkMinimapKeyedItem3(UnkMinimapKeyedItem3&& other) {
        UnkMinimapKeyedItem::MoveList(&list0, &other.list0);
        UnkMinimapKeyedItem::MoveList(&list1, &other.list1);
        field_0x50 = other.field_0x50;
        field_0x54 = other.field_0x54;
        field_0x58 = other.field_0x58;
        field_0x5c = other.field_0x5c;
    }
};
static_assert(sizeof(UnkMinimapKeyedItem3) == 0x60, "layout");

// Copies a key into its slot, bitwise (the original reloads the count after the store); a string
// id also takes a reference.
static inline void CopyKey(base::global::CStrId* slot, const base::global::CStrId* key) {
    base::global::UnkStringData* data = key->data;
    __builtin_memcpy(slot, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}
static inline void CopyKey(u64* slot, const u64* key) {
    u64 value = *key;
    __builtin_memcpy(slot, &value, sizeof(value));
}

// Items with a key each, in two parallel arrays.
template <class Item, class Key = base::global::CStrId>
struct UnkMinimapKeyedItems {
    Item* items;
    Key* keys;  // +0x8
    u32 count;                    // +0x10
    u32 capacity;
    u32 step;

    // Appends `item` (moved) under `key`; returns the new item. `grow` grows both arrays.
    __attribute__((always_inline)) Item* Add(void (*grow)(void*, u32), const Key* key, Item* item) {
        if (count == capacity)
            grow(this, step + count);
        CopyKey(&keys[count], key);
        new (&items[count]) Item(static_cast<Item&&>(*item));
        u32 n = ++count;
        return n != 0 ? &items[n] - 1 : items;
    }
};

// Grow both arrays to `capacity`.
void fn_7100e60df4(void*, u32);
struct UnkIdRecordMap;
void fn_7100e59d18(UnkIdRecordMap*, u32);
struct UnkRecord60Array;

UnkMinimapKeyedItem* fn_7100e614d0(UnkMinimapKeyedItems<UnkMinimapKeyedItem>* self,
                                   const base::global::CStrId* key, UnkMinimapKeyedItem* item) {
    return self->Add(fn_7100e60df4, key, item);
}

UnkMinimapKeyedItem2* fn_7100e5a4a0(UnkMinimapKeyedItems<UnkMinimapKeyedItem2>* self,
                                    const base::global::CStrId* key, UnkMinimapKeyedItem2* item) {
    return self->Add(reinterpret_cast<void (*)(void*, u32)>(fn_7100e59d18), key, item);
}

UnkMinimapBoundsNameItem* fn_7100e5c120(UnkMinimapKeyedItems<UnkMinimapBoundsNameItem>* self,
                                        const base::global::CStrId* key, UnkMinimapBoundsNameItem* item) {
    return self->Add(fn_7100e5ba88, key, item);
}

UnkMinimapKeyedItem3* fn_7100e56ed4(UnkMinimapKeyedItems<UnkMinimapKeyedItem3>* self,
                                    const base::global::CStrId* key, UnkMinimapKeyedItem3* item) {
    return self->Add(fn_7100e56814, key, item);
}

UnkMinimapKeyedItem3* fn_7100e550dc(UnkMinimapKeyedItems<UnkMinimapKeyedItem3, u64>* self, const u64* key,
                                    UnkMinimapKeyedItem3* item) {
    return self->Add(reinterpret_cast<void (*)(void*, u32)>(fn_7100e54ad8), key, item);
}

// fn_71000cbae4 copies an array of 12-byte vectors (src/base/UnkVec3fVector.cpp).
struct UnkPart48;

// An element with a string id, an array of vectors and an array of 16-bit values (0x68 bytes).
struct UnkMinimapPathItem {
    base::global::UnkStringData* name;
    u8 points[0x28];                // +0x8: copied by fn_71000cbae4
    base::UnkVector<u16> indices;  // +0x30
    u32 field_0x58;
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
};
static_assert(sizeof(UnkMinimapPathItem) == 0x68, "layout");

// Copies an array of 16-bit values: the source's settings and allocator, its capacity and its
// elements.
static inline void CopyIndices(base::UnkVector<u16>* list, const base::UnkVector<u16>* src) {
    list->data = nullptr;
    list->count = 0;
    list->capacity = 0;
    list->step = src->step;
    list->field_0x14 = src->field_0x14;
    list->flags = src->flags;
    list->allocator = src->allocator;
    list->Reserve(src->capacity);
    __builtin_memcpy(list->data, src->data, (u64)src->count * sizeof(u16));
    list->count = src->count;
}

// Its move constructor: the id is taken, the arrays are copied.
__attribute__((flatten)) void fn_7100e55d98(UnkMinimapPathItem* self, UnkMinimapPathItem* other) {
    self->name = other->name;
    other->name = &g_unk_7101d47108->empty;
    fn_71000cbae4(reinterpret_cast<UnkPart48*>(self->points),
                  reinterpret_cast<const UnkPart48*>(other->points));
    CopyIndices(&self->indices, &other->indices);
    self->field_0x58 = other->field_0x58;
    self->field_0x5c = other->field_0x5c;
    self->field_0x60 = other->field_0x60;
    self->field_0x64 = other->field_0x64;
}

// A string id, copied in place (taking a reference).
struct UnkMinimapStrId {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkMinimapStrId(const UnkMinimapStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
};

// A path: an array of vectors, an array of 16-bit values and four plain words (0x60 bytes).
struct UnkMinimapPath {
    u8 points[0x28];               // copied by fn_71000cbae4
    alignas(8) u8 indices[0x28];  // +0x28: a base::UnkVector<u16>, built by CopyIndices
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;

    __attribute__((always_inline)) UnkMinimapPath(const UnkMinimapPath& other) {
        fn_71000cbae4(reinterpret_cast<UnkPart48*>(points), reinterpret_cast<const UnkPart48*>(other.points));
        CopyIndices(reinterpret_cast<base::UnkVector<u16>*>(indices),
                    reinterpret_cast<const base::UnkVector<u16>*>(other.indices));
        field_0x50 = other.field_0x50;
        field_0x54 = other.field_0x54;
        field_0x58 = other.field_0x58;
        field_0x5c = other.field_0x5c;
    }
};
static_assert(sizeof(UnkMinimapPath) == 0x60, "layout");

// A 0x138-byte element: a string id, an item, and two paths among plain fields.
struct UnkMinimapAreaItem {
    UnkMinimapStrId name;
    u32 field_0x8;
    UnkItem item;  // +0x10
    u8 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    UnkMinimapPath path0;  // +0x78
    UnkMinimapPath path1;  // +0xd8

    __attribute__((always_inline)) UnkMinimapAreaItem(const UnkMinimapAreaItem& other)
        : name(other.name), field_0x8(other.field_0x8), item(other.item), field_0x60(other.field_0x60),
          field_0x64(other.field_0x64), field_0x68(other.field_0x68), field_0x6c(other.field_0x6c),
          field_0x70(other.field_0x70), path0(other.path0), path1(other.path1) {}
};
static_assert(sizeof(UnkMinimapAreaItem) == 0x138, "layout");

// Its copy constructor.
__attribute__((flatten)) void fn_7100e50e20(UnkMinimapAreaItem* self, const UnkMinimapAreaItem* other) {
    new (self) UnkMinimapAreaItem(*other);
}


// A shape whose points' 2D bounds are cached at +0x50.
struct UnkMinimapShape {
    base::math::CVector3D* points;
    u32 count;  // +0x8
    u8 unk_0xc[0x44];
    base::math::CVector2D min;  // +0x50
    base::math::CVector2D max;  // +0x58
};

struct UnkMinimapShapeTask {
    u8 unk_0x0[0x8];
    UnkMinimapShape* shape;  // +0x8
};

// The step's result: two empty words and two codes.
struct UnkMinimapStepResult {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    u32 field_0x14;
};

// Recomputes the shape's 2D bounds from its points.
UnkMinimapStepResult fn_7100e4f5f4(void* self, UnkMinimapShapeTask* task) {
    UnkMinimapShape* shape = task->shape;
    shape->min = {__FLT_MAX__, __FLT_MAX__};
    shape->max = {-__FLT_MAX__, -__FLT_MAX__};
    if (u32 n = shape->count) {
        base::math::CVector3D* p = shape->points;
        for (base::math::CVector3D* end = p + n; p != end; ++p) {
            base::math::CVector2D v;
            fn_710007b430(&v, p);
            f32 x = v.x;
            f32 y = v.y;
            base::math::CVector2D lo = shape->min;
            shape->min = {lo.x < x ? lo.x : x, lo.y < y ? lo.y : y};
            base::math::CVector2D hi = shape->max;
            shape->max = {hi.x > x ? hi.x : x, hi.y > y ? hi.y : y};
        }
    }
    return {0, 0, 2, 1};
}
