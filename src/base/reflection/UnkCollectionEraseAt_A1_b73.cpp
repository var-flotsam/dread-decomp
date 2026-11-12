// Reflected collection erase slots (0x128) of unit 83, b68's inlined erase (CCutsceneComponent_EraseAt_A5_b68.cpp):
// destroy the entry an index iterator names, then move each later entry down one and destroy the moved-from one.
// The index goes through a byte offset from the vector itself (build/b67/b67a6.md). Elements: UnkReflValue (copy
// constructor fn_71000a179c, destructor fn_71000a1c60) and CRntString (move fn_710007eab4, destructor fn_710007ecd4).
#include <new>

#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::UnkReflValue;

template <typename T, void (*Move)(T*, T*), void (*Destroy)(T*)>
static inline __attribute__((always_inline)) void EraseAt_A1_b73(base::UnkVector<T>* self, u32 index) {
    if (index >= self->count)
        return;
    Destroy(&self->data[index]);
    for (u32 i = index; i < self->count - 1; ++i) {
        T* src = &self->data[i] + 1;
        Move(src - 1, src);
        Destroy(&self->data[i] + 1);
    }
    self->count = self->count - 1;
}

static void MoveValue_A1_b73(UnkReflValue* dst, UnkReflValue* src) { fn_71000a179c(dst, src); }
static void DestroyValue_A1_b73(UnkReflValue* v) { fn_71000a1c60(v); }

template <typename T, void (*Move)(T*, T*), void (*Destroy)(T*)>
static inline __attribute__((always_inline)) void EraseAtIter_A1_b73(base::UnkVector<T>* self, const u32* it) {
    T* base = reinterpret_cast<T*>(self);
    u64 offset = (u64)*it * sizeof(T);
    T* p = reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
    EraseAt_A1_b73<T, Move, Destroy>(self, (u32)(p - base));
}

void fn_71000d538c(base::UnkVector<UnkReflValue>* self, const u32* it) {
    EraseAtIter_A1_b73<UnkReflValue, MoveValue_A1_b73, DestroyValue_A1_b73>(self, it);
}

void fn_71000dff68(base::UnkVector<CRntString>* self, const u32* it) {
    EraseAtIter_A1_b73<CRntString, fn_710007eab4, fn_710007ecd4>(self, it);
}

// Unit 341's copy (8-byte elements, the index used as is): owning pointers to objects released through their vf_0x90 and then vf_0x10.
struct UnkOwnedObj_A1_b73 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
};

struct UnkOwnedPtr_A1_b73 {
    UnkOwnedObj_A1_b73* p;
};

static inline __attribute__((always_inline)) void MoveOwned_A1_b73(UnkOwnedPtr_A1_b73* dst, UnkOwnedPtr_A1_b73* src) {
    UnkOwnedObj_A1_b73* p = src->p;
    src->p = nullptr;
    __builtin_memcpy(&dst->p, &p, sizeof(p));
}

static inline __attribute__((always_inline)) void DestroyOwned_A1_b73(UnkOwnedPtr_A1_b73* o) {
    UnkOwnedObj_A1_b73* p = o->p;
    UnkOwnedObj_A1_b73* null = nullptr;
    __builtin_memcpy(&o->p, &null, sizeof(null));
    if (p) {
        p->vf_0x90();
        p->vf_0x10();
    }
}

void fn_7100243850(base::UnkVector<UnkOwnedPtr_A1_b73>* self, const u32* it) {
    EraseAt_A1_b73<UnkOwnedPtr_A1_b73, MoveOwned_A1_b73, DestroyOwned_A1_b73>(self, *it);
}

// Unit 25's copy: string id and file-path id pairs (base/UnkStrIdPathPair.h), moved and destroyed inline.
static inline __attribute__((always_inline)) void MovePair_A1_b73(UnkStrIdPathPair* dst, UnkStrIdPathPair* src) {
    dst->id.data = src->id.data;
    src->id.data = &g_unk_7101d47108->empty;
    dst->path.data = src->path.data;
    base::global::UnkStringData* empty = &g_unk_7101d47200->empty;
    __builtin_memcpy(&src->path.data, &empty, sizeof(empty));
}

static inline __attribute__((always_inline)) void DestroyPair_A1_b73(UnkStrIdPathPair* p) {
    p->~UnkStrIdPathPair();
}

void fn_710001dc08(base::UnkVector<UnkStrIdPathPair>* self, const u32* it) {
    EraseAtIter_A1_b73<UnkStrIdPathPair, MovePair_A1_b73, DestroyPair_A1_b73>(self, it);
}
