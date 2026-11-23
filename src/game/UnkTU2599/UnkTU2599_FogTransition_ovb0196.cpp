// Unit 2599 (hint 'CEnvironmentData::SFogTransition'): member-wise assignments of small settings
// structs, the string-id/value arrays of its transitions (resize, set-at, push-back, assign), the
// iterator values over them and a keyed chained map's insert. Names and layouts are placeholders.
#include <new>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkCollectionIter.h"
#include "base/UnkStrIdValueVec.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "game/UnkIdValueVecSize_b43a3.h"
#include "game/UnkTU2599.h"
#include "types.h"

using base::math::CVector4D;

struct UnkPair_b0196 {
    f32 x, y;
};

struct UnkFogTransA_b0196 {
    u32 field_0x0;
    CVector4D field_0x4;
    UnkPair_b0196 field_0x14;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    CVector4D field_0x28;
    u32 field_0x38;
};
static_assert(sizeof(UnkFogTransA_b0196) == 0x3c, "UnkFogTransA_b0196 layout");

struct UnkFogTransA2_b0196 {
    u32 field_0x0;
    CVector4D field_0x4;
    UnkPair_b0196 field_0x14;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    CVector4D field_0x28;
    u32 field_0x38;
};

template <class T>
static inline __attribute__((always_inline)) void* CopyA(T* self, const T* other) {
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        Assign(self->field_0x4, other->field_0x4);
        u32 t = other->field_0x1c;
        self->field_0x14 = other->field_0x14;
        self->field_0x1c = t;
        self->field_0x20 = other->field_0x20;
        self->field_0x24 = other->field_0x24;
        Assign(self->field_0x28, other->field_0x28);
        self->field_0x38 = other->field_0x38;
    }
    return self;
}

void* fn_7100f5bb34(void* dst, const void* src) {
    return CopyA(static_cast<UnkFogTransA_b0196*>(dst), static_cast<const UnkFogTransA_b0196*>(src));
}

void* fn_7100f5bba4(void* dst, const void* src) {
    return CopyA(static_cast<UnkFogTransA2_b0196*>(dst), static_cast<const UnkFogTransA2_b0196*>(src));
}

struct UnkFogTransB_b0196 {
    u8 field_0x0;
    CVector4D field_0x4;
    UnkPair_b0196 field_0x14;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    CVector4D field_0x2c;
    u32 field_0x3c[5];
};
static_assert(sizeof(UnkFogTransB_b0196) == 0x50, "UnkFogTransB_b0196 layout");

struct UnkFogTransB2_b0196 {
    u8 field_0x0;
    CVector4D field_0x4;
    UnkPair_b0196 field_0x14;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    CVector4D field_0x2c;
    u32 field_0x3c[5];
};

template <class T>
static inline __attribute__((always_inline)) void* CopyB(T* self, const T* other) {
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        Assign(self->field_0x4, other->field_0x4);
        u32 t = other->field_0x1c;
        self->field_0x14 = other->field_0x14;
        self->field_0x1c = t;
        self->field_0x20 = other->field_0x20;
        self->field_0x24 = other->field_0x24;
        self->field_0x28 = other->field_0x28;
        Assign(self->field_0x2c, other->field_0x2c);
        u32 last = other->field_0x3c[4];
        u64 hi, lo;
        __builtin_memcpy(&hi, &other->field_0x3c[2], 8);
        __builtin_memcpy(&lo, &other->field_0x3c[0], 8);
        __builtin_memcpy(&self->field_0x3c[0], &lo, 8);
        __builtin_memcpy(&self->field_0x3c[2], &hi, 8);
        self->field_0x3c[4] = last;
    }
    return self;
}

void* fn_7100f6c0b0(void* dst, const void* src) {
    return CopyB(static_cast<UnkFogTransB_b0196*>(dst), static_cast<const UnkFogTransB_b0196*>(src));
}

void* fn_7100f6c138(void* dst, const void* src) {
    return CopyB(static_cast<UnkFogTransB2_b0196*>(dst), static_cast<const UnkFogTransB2_b0196*>(src));
}

struct UnkFogTransC_b0196 {
    u32 field_0x0;
    u8 field_0x4;
    base::global::CFilePathStrId field_0x8;
};

void* fn_7100f6f614(void* dst, const void* src) {
    UnkFogTransC_b0196* self = static_cast<UnkFogTransC_b0196*>(dst);
    UnkFogTransC_b0196* other = static_cast<UnkFogTransC_b0196*>(const_cast<void*>(src));
    if (self != other) {
        u32 a = other->field_0x0;
        u8 b = other->field_0x4;
        self->field_0x4 = b;
        self->field_0x0 = a;
        self->field_0x8 = static_cast<base::global::CFilePathStrId&&>(other->field_0x8);
    }
    return self;
}

struct UnkFogValue_b0196 {
    f32 a, b, c, d;
};
struct UnkVFogValue_b0196 {
    f32 a, b, c, d, e, f;
};

struct UnkFogTag_b0196;
struct UnkVFogTag_b0196;

template <>
struct base::UnkStrIdValueTraits<UnkFogTag_b0196> {
    static char* base_vtable() { return g_unk_7101cab1f8; }
    static char* vtable() { return g_unk_7101cab220; }
    static void init(UnkFogValue_b0196&) {}
    static void resize(base::global::CRntVector<base::UnkStrIdValueElem<UnkFogTag_b0196, UnkFogValue_b0196>>* v, u32 n) {
        fn_7100f799b0(v, n);
    }
};
template <>
struct base::UnkStrIdValueTraits<UnkVFogTag_b0196> {
    static char* base_vtable() { return g_unk_7101cab1f8; }
    static char* vtable() { return g_unk_7101cab338; }
    static void init(UnkVFogValue_b0196&) {}
    static void resize(base::global::CRntVector<base::UnkStrIdValueElem<UnkVFogTag_b0196, UnkVFogValue_b0196>>* v, u32 n) {
        fn_7100f7a7c8(v, n);
    }
};

namespace base {
// UnkStrIdValueResize with the new elements placed by byte offset.
template <class Tag, class T>
void Resize2_b0196(global::CRntVector<UnkStrIdValueElem<Tag, T>>* v, u32 n) {
    using E = UnkStrIdValueElem<Tag, T>;
    if (v->capacity < n)
        refl_StrIdValueReallocate(v, n);
    u32 c = v->count;
    u64 off = (u64)c * sizeof(E);
    for (; c < n; ++c, off += sizeof(E))
        new (reinterpret_cast<char*>(v->data) + off) E();
    v->count = n;
}
}

using UnkFogElem_b0196 = base::UnkStrIdValueElem<UnkFogTag_b0196, UnkFogValue_b0196>;
using UnkVFogElem_b0196 = base::UnkStrIdValueElem<UnkVFogTag_b0196, UnkVFogValue_b0196>;
using UnkFogVec_b0196 = base::global::CRntVector<UnkFogElem_b0196>;
using UnkVFogVec_b0196 = base::global::CRntVector<UnkVFogElem_b0196>;

__attribute__((noinline)) void fn_7100f799b0(void* v, u32 n) {
    base::Resize2_b0196(static_cast<UnkFogVec_b0196*>(v), n);
}
__attribute__((noinline)) void fn_7100f7a7c8(void* v, u32 n) {
    base::Resize2_b0196(static_cast<UnkVFogVec_b0196*>(v), n);
}

#define SET_AT(addr, Vec, Elem)                                       void fn_##addr(Vec* v, const u32* index, const Elem* value) {         base::refl_StrIdValueGrowAtInline(v, *index) = *value;        }
SET_AT(7100f5d3a4, UnkFogVec_b0196, UnkFogElem_b0196)
SET_AT(7100f5d590, UnkFogVec_b0196, UnkFogElem_b0196)
SET_AT(7100f6dfe8, UnkVFogVec_b0196, UnkVFogElem_b0196)
SET_AT(7100f6e1ec, UnkVFogVec_b0196, UnkVFogElem_b0196)

extern char g_unk_7101cab1f8[];

struct UnkIterVal16_b0196 {
    u64 a, b;
};
struct UnkIterVal8_b0196 {
    u64 a;
};
struct UnkIterVal24_b0196 {
    u64 a, b, c;
};

struct UnkIterTag16_b0196;
struct UnkIterTag24_b0196;
struct UnkIterTag8_b0196;

template <>
struct base::UnkStrIdValueTraits<UnkIterTag16_b0196> {
    static char* base_vtable() { return g_unk_7101cab1f8; }
    static char* vtable() { return g_unk_7101cab1f8; }
    static void init(UnkIterVal16_b0196&) {}
};
template <>
struct base::UnkStrIdValueTraits<UnkIterTag8_b0196> {
    static char* base_vtable() { return g_unk_7101cab1f8; }
    static char* vtable() { return g_unk_7101cab1f8; }
    static void init(UnkIterVal8_b0196&) {}
};
template <>
struct base::UnkStrIdValueTraits<UnkIterTag24_b0196> {
    static char* base_vtable() { return g_unk_7101cab1f8; }
    static char* vtable() { return g_unk_7101cab1f8; }
    static void init(UnkIterVal24_b0196&) {}
};

using UnkIter16_b0196 = base::UnkIterValue<base::UnkStrIdValueElem<UnkIterTag16_b0196, UnkIterVal16_b0196>, u32>;
using UnkIter8_b0196 = base::UnkIterValue<base::UnkStrIdValueElem<UnkIterTag8_b0196, UnkIterVal8_b0196>, u32>;
using UnkIter24_b0196 = base::UnkIterValue<base::UnkStrIdValueElem<UnkIterTag24_b0196, UnkIterVal24_b0196>, u32>;

// UnkIterValue's move-assignment without the self check.
template <class It>
static inline __attribute__((always_inline)) void IterMoveAssign(It* self, It* other) {
    if (other->state == 1) {
        self->Index() = other->value.index;
        self->Element() = other->value.element;
    } else if (other->state == 2) {
        self->Index() = *other->ref.index;
        self->Element() = *other->ref.element;
    }
}

#define ITER_ASSIGN(addr, T)              T* fn_##addr(T* self, T* other) {         IterMoveAssign(self, other);          return self;                      }

ITER_ASSIGN(7100f5cd4c, UnkIter16_b0196)
ITER_ASSIGN(7100f63b9c, UnkIter8_b0196)
ITER_ASSIGN(7100f662d8, UnkIter8_b0196)
ITER_ASSIGN(7100f68758, UnkIter8_b0196)
ITER_ASSIGN(7100f6d954, UnkIter24_b0196)

// The vertical fog array's {vtable, string id, 24-byte value} elements, named as
// src/game/CEnvironmentData/UnkIdValueVectorCopy_b43.cpp (whose copy and push-back of the other
// classes share these signatures).
template <char* Vtable>
struct UnkIdValueVecTag_b43a3;

template <>
struct UnkIdValueVecSize_b43a3<g_unk_7101cab338> {
    static constexpr u64 value = 24;
};
template <char* Vtable>
using UnkIdValueVecElem_b43a3 = base::UnkIdValueElem<UnkIdValueVecTag_b43a3<Vtable>,
                                                     UnkCloneElem<UnkIdValueVecTag_b43a3<Vtable>, UnkIdValueVecSize_b43a3<Vtable>::value>>;
template <char* Vtable>
using UnkIdValueVec_b43a3 = base::global::CRntVector<UnkIdValueVecElem_b43a3<Vtable>>;

__attribute__((noinline)) void fn_7100f6e784(UnkIdValueVec_b43a3<g_unk_7101cab338>* v,
                                             const UnkIdValueVecElem_b43a3<g_unk_7101cab338>& x);

template <>
struct base::UnkIdValueTraits<UnkIdValueVecTag_b43a3<g_unk_7101cab338>> {
    static char* vtable() { return g_unk_7101cab338; }
    static void push_back(UnkIdValueVec_b43a3<g_unk_7101cab338>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab338>& x) {
        fn_7100f6e784(v, x);
    }
};

void fn_7100f6e784(UnkIdValueVec_b43a3<g_unk_7101cab338>* v, const UnkIdValueVecElem_b43a3<g_unk_7101cab338>& x) {
    base::refl_IdValuePushBack(v, x);
}

UnkIdValueVec_b43a3<g_unk_7101cab338>* fn_7100f6e964(UnkIdValueVec_b43a3<g_unk_7101cab338>* self,
                                                     const UnkIdValueVec_b43a3<g_unk_7101cab338>* other) {
    return base::refl_IdValueAssign(self, other);
}

namespace env_move {
struct CEnvironmentData;
}

void fn_7100f42c44(void*, void*);
void fn_7100f44fd8(env_move::CEnvironmentData* self, env_move::CEnvironmentData* other);

// The environment data a map node holds (0x3a8 bytes), copied and moved out of line.
struct UnkEnvValue_b0196 {
    u8 bytes[0x3a8];

    UnkEnvValue_b0196(const UnkEnvValue_b0196& other) { fn_7100f42c44(bytes, const_cast<u8*>(other.bytes)); }
    UnkEnvValue_b0196(UnkEnvValue_b0196&& other) {
        fn_7100f44fd8(reinterpret_cast<env_move::CEnvironmentData*>(bytes),
                      reinterpret_cast<env_move::CEnvironmentData*>(other.bytes));
    }
};

// The environment data by name.
using UnkEnvMap_b0196 = base::global::CRntDictionary<base::global::CStrId, UnkEnvValue_b0196>;
static_assert(sizeof(UnkEnvMap_b0196::TNode) == 0x3c8, "UnkEnvMap_b0196 node layout");

// The collection's add by copy and add by move.
void fn_7100f79314(UnkEnvMap_b0196* self, const base::global::CStrId* key, const UnkEnvValue_b0196* value) {
    self->Insert(*key, *value);
}

void fn_7100f79420(UnkEnvMap_b0196* self, const base::global::CStrId* key, UnkEnvValue_b0196* value) {
    self->Insert(*key, static_cast<UnkEnvValue_b0196&&>(*value));
}
