#pragma once

// Destructors of classes whose members are string ids or refcounted pointers
// (tools/gen_clones.py, kind "dtor"): store the class's vtable, release each member, then call
// the base destructor (and, for the deleting form, operator delete). The class layouts are
// unknown; each destructor is an instantiation over a traits type that names its vtable and
// base, and a list of member descriptors.

#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

namespace base {

// A string id released in two steps (the original reloads the id and the count after the
// decrement, and the pool after a release), unlike CStrId. Which engine type it is is unknown.
// Pool is the global pool pointer: most use g_unk_7101d47108; some keyed collections use
// CFilePathStrId's (g_unk_7101d47200).
template <global::UnkStringPool*& Pool>
struct UnkStrIdMemberOf {
    global::UnkStringData* data;

    // Moves take the id and leave the pool's empty string; copy-assignment retains the new id
    // first. Inferred from fn_71000de484/fn_71000de574, which these don't match yet (scheduling).
    UnkStrIdMemberOf(UnkStrIdMemberOf&& other) : data(other.data) { other.data = &Pool->empty; }
    UnkStrIdMemberOf& operator=(const UnkStrIdMemberOf& other) {
        if (data != other.data) {
            global::UnkStringData* d = other.data;
            global::UnkStringPool* pool = Pool;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }

    bool operator==(const UnkStrIdMemberOf& other) const { return data == other.data; }
    bool operator!=(const UnkStrIdMemberOf& other) const { return data != other.data; }

    ~UnkStrIdMemberOf() {
        if (global::UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
};
using UnkStrIdMember = UnkStrIdMemberOf<g_unk_7101d47108>;

// An intrusively refcounted object (count at +8), deleted through its vtable.
struct UnkRefCounted {
    virtual void vf_0x0();
    virtual ~UnkRefCounted();
    s32 ref_count;
};

struct UnkRefPtrMember {
    UnkRefCounted* ptr;

    ~UnkRefPtrMember() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};

// Member descriptors: the member's type and offset.
template <class T, u32 Offset>
struct UnkMemberAt {
    static void Release(char* self) { reinterpret_cast<T*>(self + Offset)->~T(); }
};
template <u32 Offset>
using UnkStrIdAt = UnkMemberAt<UnkStrIdMember, Offset>;
template <u32 Offset>
using UnkRefPtrAt = UnkMemberAt<UnkRefPtrMember, Offset>;

template <class Tag>
struct UnkDtorTraits;  // vtable(), and base(char*) unless the class has no base destructor

// Members are released in the order given (the reverse of their declaration order).
// The deleting forms free through __builtin_operator_delete, as a compiler-generated deleting
// destructor does, so dead stores to the object before the free are dropped.
template <class Tag, class... Members>
void refl_Dtor(char* self) {
    using Traits = UnkDtorTraits<Tag>;
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
    (Members::Release(self), ...);
    Traits::base(self);
}

// A class without a base destructor call (fn_7100256184 and its copies).
template <class Tag, class... Members>
void refl_DtorNoBase(char* self) {
    *reinterpret_cast<char**>(self) = UnkDtorTraits<Tag>::vtable() + 0x10;
    (Members::Release(self), ...);
}

template <class Tag, class... Members>
void refl_DeletingDtor(char* self) {
    using Traits = UnkDtorTraits<Tag>;
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
    (Members::Release(self), ...);
    Traits::base(self);
    __builtin_operator_delete(self);
}

// The deleting destructor of a class without a base destructor call.
template <class Tag, class... Members>
void refl_DeletingDtorNoBase(char* self) {
    using Traits = UnkDtorTraits<Tag>;
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
    (Members::Release(self), ...);
    __builtin_operator_delete(self);
}

// An element of unknown type whose destructor stores its vtable and releases members (no base
// destructor call), as held by some collection iterator values.
template <class Tag, u64 Size, class... Members>
struct UnkDtorElem {
    alignas(8) char data[Size];

    ~UnkDtorElem() {
        *reinterpret_cast<char**>(data) = UnkDtorTraits<Tag>::vtable() + 0x10;
        (Members::Release(data), ...);
    }
};

// The destructor of a singly linked list (count at +8, head and tail at +0x10/+0x18) that
// frees each node through the allocator at `Alloc`; `Next` is the node's link (fn_7100084838
// and its copies).
template <class Tag, u32 Alloc, u32 Next>
void refl_ListDtor(char* self) {
    *reinterpret_cast<char**>(self) = UnkDtorTraits<Tag>::vtable() + 0x10;
    char* n = *reinterpret_cast<char**>(self + 0x10);
    while (n) {
        char* next = *reinterpret_cast<char**>(n + Next);
        (*reinterpret_cast<UnkAllocator**>(self + Alloc))->vf_0x48(n);
        n = next;
    }
    *reinterpret_cast<u32*>(self + 8) = 0;
    *reinterpret_cast<char**>(self + 0x10) = nullptr;
    *reinterpret_cast<char**>(self + 0x18) = nullptr;
}

// The deleting form (fn_7100085188 and its copies): the stores before the free are dropped.
template <class Tag, u32 Alloc, u32 Next>
void refl_ListDeletingDtor(char* self) {
    refl_ListDtor<Tag, Alloc, Next>(self);
    __builtin_operator_delete(self);
}

// A secondary base at `Sub` that is linked into a list owned by a refcounted object (held at
// Sub+0x18; the list at +0x10 of the owner). Its destructor, inlined into the class's, unlinks it
// and releases the owner. link_vtable(): the secondary base's vtable; unlink(list, link).
template <class Tag>
struct UnkLinkTraits;

template <class Tag, u32 Sub>
inline void refl_ReleaseLink(char* self) {
    using Traits = UnkLinkTraits<Tag>;
    char* link = self + Sub;
    *reinterpret_cast<char**>(link) = Traits::link_vtable() + 0x10;
    UnkRefPtrMember* owner = reinterpret_cast<UnkRefPtrMember*>(link + 0x18);
    if (owner->ptr) {
        if (char* list = *reinterpret_cast<char**>(reinterpret_cast<char*>(owner->ptr) + 0x10))
            Traits::unlink(list, link);
    }
    owner->~UnkRefPtrMember();
}

// fn_7100390618 and its copies.
template <class Tag, u32 Sub>
void refl_LinkedDtor(char* self) {
    *reinterpret_cast<char**>(self) = UnkDtorTraits<Tag>::vtable() + 0x10;
    refl_ReleaseLink<Tag, Sub>(self);
    UnkDtorTraits<Tag>::base(self);
}

// fn_71003906a8 and its copies.
template <class Tag, u32 Sub>
void refl_LinkedDeletingDtor(char* self) {
    *reinterpret_cast<char**>(self) = UnkDtorTraits<Tag>::vtable() + 0x10;
    refl_ReleaseLink<Tag, Sub>(self);
    UnkDtorTraits<Tag>::base(self);
    __builtin_operator_delete(self);
}

}  // namespace base
