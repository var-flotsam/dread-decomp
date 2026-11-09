// SLogicPathNode unit (tools/tu_map.py, unit 2657): the path node's point lists and their helpers.
// A point list (UnkPathPoints, 0x50 bytes) holds 0x20-byte polymorphic points whose slot 3 gives
// their position, a dirty flag and cached bounds; a node holds an array of such lists. Here are the
// list's length, nearest-point and bounds queries, the reflected function's registration and
// parameter list, the point array's resize and insert, and the reflected-collection helpers
// (iterator begin/end, iterator value move/assign, destructor) for the arrays the unit reflects.
// Names and layouts are placeholders.
#include <math.h>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflFunction;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

extern char g_unk_71729ffb90[] __attribute__((visibility("hidden")));  // SLogicPathNode's class
extern char g_unk_7101cac8f0[];  // the path point's vtable
extern void* g_unk_7101cac898[];

// ---- Positions and bounds ----

struct UnkPathVec3 {
    f32 x, y, z;
};

struct UnkPathBox {
    UnkPathVec3 min;
    UnkPathVec3 max;

    void Reset() {
        min.x = 3.4028235e38f;
        min.y = 3.4028235e38f;
        min.z = 3.4028235e38f;
        max.x = -3.4028235e38f;
        max.y = -3.4028235e38f;
        max.z = -3.4028235e38f;
    }
    static UnkPathVec3 Min(const UnkPathVec3& a, const UnkPathVec3& b) {
        return {a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z};
    }
    static UnkPathVec3 Max(const UnkPathVec3& a, const UnkPathVec3& b) {
        return {a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z};
    }
    void Add(const UnkPathVec3& lo, const UnkPathVec3& hi) {
        min = Min(min, lo);
        max = Max(max, hi);
    }
};

// A point of a list: polymorphic, 0x20 bytes.
struct UnkPathNodePoint {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkPathVec3* GetPos();
    u8 pad_0x8[0x18];
};

// A list of points with cached bounds (slot 7 returns them).
struct UnkPathPoints {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual const UnkPathBox* GetBounds();
    base::UnkVector<UnkPathNodePoint> points;
    bool dirty;  // 0x30
    UnkPathBox bounds;  // 0x34
};
static_assert(sizeof(UnkPathPoints) == 0x50, "UnkPathPoints layout");

struct UnkPathShapes {
    u64 field_0x0;
    u64 field_0x8;
    base::UnkVector<UnkPathPoints> shapes;
    bool dirty;  // 0x38
    UnkPathBox bounds;  // 0x3c
};
static_assert(__builtin_offsetof(UnkPathShapes, bounds) == 0x3c, "UnkPathShapes layout");

// The bounds of all the lists, recomputed when dirty.
const UnkPathBox* fn_710105a460(UnkPathShapes* self) {
    if (self->dirty) {
        self->bounds.Reset();
        for (UnkPathPoints* it = self->shapes.data; it != self->shapes.data + self->shapes.count; ++it) {
            const UnkPathBox* b = it->GetBounds();
            self->bounds.Add(b->min, b->max);
        }
        self->dirty = false;
    }
    return &self->bounds;
}

// The length of the path between points a and b, in either order.
f32 fn_71010598e0(UnkPathPoints* self, u32 a, u32 b) {
    u32 hi = b < a ? a : b;
    u32 lo = b < a ? b : a;
    f32 length = 0.0f;
    for (u32 i = lo; i < hi; ++i) {
        const UnkPathVec3* p = self->points.data[i].GetPos();
        const UnkPathVec3* q = self->points.data[i + 1].GetPos();
        f32 dx = p->x - q->x;
        f32 dy = p->y - q->y;
        f32 dz = p->z - q->z;
        length += sqrtf(dx * dx + dy * dy + dz * dz);
    }
    return length;
}

// The nearest point to p on one list: its segment, the parameter along it and the distance
// (optional), from fn_7101059344.
void fn_7101059344(UnkPathPoints* self, const UnkPathVec3* p, s32* segment, f32* t, f32* distance);

// The nearest point to p over all the lists: the list, its segment, the parameter and the distance
// (optional).
void fn_7101059fec(UnkPathShapes* self, const UnkPathVec3* p, s32* shape, s32* segment, f32* t, f32* distance) {
    *shape = -1;
    *segment = -1;
    *t = 0.0f;
    f32 best = 3.4028235e38f;
    for (u32 i = 0; i < self->shapes.count; ++i) {
        s32 seg = -1;
        f32 tt = 0.0f;
        f32 d = 3.4028235e38f;
        fn_7101059344(&self->shapes.data[i], p, &seg, &tt, &d);
        if (d < best) {
            *shape = i;
            *segment = seg;
            *t = tt;
            best = d;
        }
    }
    if (distance)
        *distance = best;
}

// ---- The point array (vtable g_unk_7101cac8f0) ----

struct UnkPathPointV {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    f32 field_0x18;
    u8 pad_0x1c[0x4];

    UnkPathPointV()
        : vtable(reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10)), field_0x8(0), field_0xc(0), field_0x10(0),
          field_0x14(0), field_0x18(1.0f) {}
    UnkPathPointV(UnkPathPointV&& other) {
        vtable = reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10);
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
        field_0x18 = other.field_0x18;
    }
    ~UnkPathPointV() { reinterpret_cast<void (*)(UnkPathPointV*)>(vtable[1])(this); }
};

struct UnkPathPointArray;

// Resizes the array to `count` points, the new ones at (0, 0, 0) with a weight of 1.
void fn_710106111c(UnkPathPointArray* array, u32 count) {
    auto* self = reinterpret_cast<base::UnkVector<UnkPathPointV>*>(array);
    self->Reserve(count);
    for (u32 i = self->count; i < count; ++i)
        new (&self->data[i]) UnkPathPointV();
    self->count = count;
}

struct UnkInsertTarget;
struct UnkArgObj;
void fn_710106146c(UnkInsertTarget* target, const UnkArgObj* obj, u32 value);

// A point as fn_710106146c takes it (not destroyed afterwards).
struct UnkPathPointInit {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    f32 field_0x18;
};

struct UnkPathPointsData {
    u64 field_0x0;
    base::UnkVector<UnkPathPointV> points;
    bool dirty;  // 0x30
};

// Inserts a point at the position given before `index`, marks the list dirty and returns the
// point at `index`, growing the array to it when needed.
UnkReflObject* fn_71010590dc(char* self, u32 index, void* arg) {
    UnkPathPointsData* node = reinterpret_cast<UnkPathPointsData*>(self);
    const u32* pos = static_cast<const u32*>(arg);
    UnkPathPointInit point;
    point.vtable = reinterpret_cast<void**>(g_unk_7101cac8f0 + 0x10);
    point.field_0x8 = pos[0];
    point.field_0xc = pos[1];
    point.field_0x10 = pos[2];
    point.field_0x14 = 0;
    point.field_0x18 = 1.0f;
    fn_710106146c(reinterpret_cast<UnkInsertTarget*>(&node->points), reinterpret_cast<const UnkArgObj*>(&point), index);
    node->dirty = true;
    base::UnkVector<UnkPathPointV>* v = &node->points;
    if (v->field_0x14 && v->count <= index) {
        if (v->capacity <= index && v->step != 0) {
            u32 grown = (index / v->step + 1) * v->step;
            v->Reserve(grown);
        }
        fn_710106111c(reinterpret_cast<UnkPathPointArray*>(v), index + 1);
    }
    return reinterpret_cast<UnkReflObject*>(&v->data[index]);
}

// ---- Reflection ----

UnkReflValue fn_710105d0d0(void*, char* obj, UnkReflValue* const* args);
void fn_710011ddc0(UnkReflFunction* fn);

void fn_7101055b34(UnkReflFunction* fn, const char* name) {
    fn->name = base::global::UnkStrId(name, true);
    fn_7101054330();
    fn->return_type = reinterpret_cast<const CType*>(g_unk_71729ffb90);
    fn->field_0xb2 = 0x101;
    fn->field_0x40 = nullptr;
    fn->invoker = reinterpret_cast<void*>(&fn_710105d0d0);
    fn_710011ddc0(fn);
}

// The parameter types of fn_710105d0d0: unsigned and the node's class.
struct UnkPathNodeParams;

template <>
struct base::UnkPushPairTraits<UnkPathNodeParams> {
    static void* first() { return const_cast<CType*>(fn_7100005178()); }
    static void* second() {
        fn_7101054330();
        return g_unk_71729ffb90;
    }
};

__attribute__((flatten)) void fn_710105d188(UnkReflFunction* fn) {
    base::refl_PushPair<UnkPathNodeParams>(reinterpret_cast<char*>(fn));
}

// ---- Destructor of the class with vtable g_unk_7101cac898 ----

struct UnkPathElem50 {
    virtual void vf_0x0();
    virtual ~UnkPathElem50();
    u8 pad_0x8[0x48];
};

struct UnkPathDtor;

template <>
struct base::UnkDtorTraits<UnkPathDtor> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101cac898); }
};

struct UnkPathDtorVector {
    static void Release(char* self) { reinterpret_cast<base::UnkVector<UnkPathElem50>*>(self + 0x10)->Free(); }
};

__attribute__((flatten)) void fn_7101059d40(char* self) {
    base::refl_DtorNoBase<UnkPathDtor, UnkPathDtorVector, base::UnkStrIdAt<0x8>>(self);
}

// ---- Reflected collection helpers ----

struct UnkPathBlockTag;
using UnkPathBlock20 = UnkCloneBlock<UnkPathBlockTag, 0x20>;

void fn_710105e51c(const base::UnkVector<UnkPathBlock20>* v, base::UnkCollectionIter<UnkPathBlock20>* out) {
    base::refl_End(v, out);
}

// The element a value refers to (state 2) or holds, read through the shared slot.
template <class T>
static inline T* ElementOf(base::UnkIterValue<T>* v) {
    T** slot = reinterpret_cast<T**>(&v->value.element);
    if (v->state == 2)
        return *slot;
    return reinterpret_cast<T*>(slot);
}

template <class T>
static inline const T* ElementOf(const base::UnkIterValue<T>* v) {
    T* const* slot = reinterpret_cast<T* const*>(&v->value.element);
    if (v->state == 2)
        return *slot;
    return reinterpret_cast<const T*>(slot);
}

// Points assigned by their first three words or by all five.
struct UnkPathPoint3 {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u8 pad_0x1c[0x4];

    UnkPathPoint3& operator=(const UnkPathPoint3& other) {
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        return *this;
    }
};

base::UnkIterValue<UnkPathPoint3>* fn_710105d9f8(base::UnkIterValue<UnkPathPoint3>* self,
                                                 const base::UnkIterValue<UnkPathPoint3>* other) {
    return &(*self = *other);
}

struct UnkPathPoint5 {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u8 pad_0x1c[0x4];

    UnkPathPoint5& operator=(UnkPathPoint5&& other) {
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
        field_0x18 = other.field_0x18;
        return *this;
    }
};

base::UnkIterValue<UnkPathPoint5>* fn_710105da88(base::UnkIterValue<UnkPathPoint5>* self,
                                                 base::UnkIterValue<UnkPathPoint5>* other) {
    return &(*self = static_cast<base::UnkIterValue<UnkPathPoint5>&&>(*other));
}

// The 0x50-byte item of include/game_s2/UnkItem50.h, as each helper below sees it: copy-only,
// move-only (its flag read after the source's array is cleared), assigned through fn_7100e4e898,
// or with its array inline (destroyed and move-assigned).
struct UnkPathItem {
    void** vtable;
    UnkSub field_0x8;
    u8 field_0x30;
    u8 pad_0x31[3];
    u64_a4 field_0x34;
    u64_a4 field_0x3c;
    u64_a4 field_0x44;

    UnkPathItem(const UnkPathItem& other) {
        vtable = g_unk_7101cac938 + 2;
        fn_71003c00b0(&field_0x8, &other.field_0x8);
        field_0x30 = other.field_0x30;
        field_0x34 = 0;
        field_0x3c = 0;
        field_0x44 = 0;
    }
    ~UnkPathItem() { reinterpret_cast<void (*)(UnkPathItem*)>(vtable[1])(this); }
};

void fn_710105eb00(void* dst, const void* src) {
    base::refl_IterValueMove<UnkPathItem>(dst, src);
}

struct UnkPathItemM {
    void** vtable;
    UnkSub field_0x8;
    u8 field_0x30;
    u8 pad_0x31[3];
    u64_a4 field_0x34;
    u64_a4 field_0x3c;
    u64_a4 field_0x44;

    UnkPathItemM(UnkPathItemM&& other) {
        vtable = g_unk_7101cac938 + 2;
        field_0x8.field_0x0 = other.field_0x8.field_0x0;
        field_0x8.field_0x8 = other.field_0x8.field_0x8;
        field_0x8.field_0xc = other.field_0x8.field_0xc;
        field_0x8.field_0x10 = other.field_0x8.field_0x10;
        field_0x8.field_0x14 = other.field_0x8.field_0x14;
        field_0x8.field_0x18 = other.field_0x8.field_0x18;
        field_0x8.field_0x20 = other.field_0x8.field_0x20;
        __builtin_memset(&other.field_0x8, 0, 16);
        field_0x30 = other.field_0x30;
        field_0x34 = 0;
        field_0x3c = 0;
        field_0x44 = 0;
    }
    ~UnkPathItemM() { reinterpret_cast<void (*)(UnkPathItemM*)>(vtable[1])(this); }
};

void fn_710105e5f8(void* dst, const void* src) {
    base::refl_IterValueMove<UnkPathItemM>(dst, src);
}

struct UnkPolyVector;
UnkPolyVector* fn_7100e4e898(UnkPolyVector* self, const UnkPolyVector* other);

struct UnkPathItemA {
    void** vtable;
    UnkSub field_0x8;
    u8 field_0x30;
    u8 pad_0x31[3];
    u64_a4 field_0x34;
    u64_a4 field_0x3c;
    u64_a4 field_0x44;

    UnkPathItemA& operator=(const UnkPathItemA& other) {
        const UnkPolyVector* src = reinterpret_cast<const UnkPolyVector*>(&other.field_0x8);
        fn_7100e4e898(reinterpret_cast<UnkPolyVector*>(&field_0x8), src);
        field_0x30 = other.field_0x30;
        return *this;
    }
};

base::UnkIterValue<UnkPathItemA>* fn_710105e7b0(base::UnkIterValue<UnkPathItemA>* self,
                                                const base::UnkIterValue<UnkPathItemA>* other) {
    if (self != other) {
        self->Index() = other->Index();
        *ElementOf(self) = *ElementOf(other);
    }
    return self;
}

struct UnkPathSubElem {
    virtual void vf_0x0();
    virtual ~UnkPathSubElem();
    u8 pad_0x8[0x18];
};

struct UnkPathItemD {
    void** vtable;
    base::UnkVector<UnkPathSubElem> field_0x8;
    u8 field_0x30;
    u8 pad_0x31[3];
    u64_a4 field_0x34;
    u64_a4 field_0x3c;
    u64_a4 field_0x44;

    UnkPathItemD& operator=(UnkPathItemD&& other) {
        field_0x8 = static_cast<base::UnkVector<UnkPathSubElem>&&>(other.field_0x8);
        field_0x30 = other.field_0x30;
        return *this;
    }
    ~UnkPathItemD() {
        vtable = g_unk_7101cac938 + 2;
        field_0x8.Free();
    }
};

// The iterator re-sync is out of line in this unit.
template <>
void base::refl_IterSync(base::UnkCollectionIter<UnkPathItemD, u32>* it) asm("fn_710105b904");

__attribute__((flatten)) void fn_710105fb10(const base::UnkVector<UnkPathItemD>* v,
                                            base::UnkCollectionIter<UnkPathItemD>* out) {
    base::refl_Begin(v, out);
}

__attribute__((flatten)) void fn_710105fc24(const base::UnkVector<UnkPathItemD>* v,
                                            base::UnkCollectionIter<UnkPathItemD>* out) {
    base::refl_End(v, out);
}

// Move-assigns the pair another value holds or refers to (no self check).
__attribute__((flatten)) base::UnkIterValue<UnkPathItemD>* fn_710105e890(base::UnkIterValue<UnkPathItemD>* self,
                                                                         base::UnkIterValue<UnkPathItemD>* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        *ElementOf(self) = static_cast<UnkPathItemD&&>(other->value.element);
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        *ElementOf(self) = static_cast<UnkPathItemD&&>(*other->ref.element);
    }
    return self;
}

// ---- An array of 8-byte links ----

struct UnkPathLink {
    u32 field_0x0;
    u32 field_0x4;

    // Zeroed and copied word by word, untyped: the original reloads the array after each store.
    UnkPathLink() {
        __builtin_memset(&field_0x0, 0, sizeof(u32));
        __builtin_memset(&field_0x4, 0, sizeof(u32));
    }
    UnkPathLink(UnkPathLink&& other) {
        __builtin_memcpy(&field_0x0, &other.field_0x0, sizeof(u32));
        __builtin_memcpy(&field_0x4, &other.field_0x4, sizeof(u32));
    }
};

__attribute__((flatten)) UnkPathLink* fn_7101060dcc(base::UnkVector<UnkPathLink>* self, u32 index) {
    return &self->GrowAt(index);
}
