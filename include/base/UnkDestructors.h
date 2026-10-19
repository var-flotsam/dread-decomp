#pragma once

// Destructors of classes whose members are string ids or refcounted pointers
// (tools/gen_clones.py, kind "dtor"): store the class's vtable, release each member, then call
// the base destructor (and, for the deleting form, operator delete). The class layouts are
// unknown; each destructor is an instantiation over a traits type that names its vtable and
// base, and a list of member descriptors.

#include <new>

#include "base/global/CStrId.h"
#include "types.h"

namespace base {

// A string id released in two steps (the original reloads the id and the count after the
// decrement, and the pool after a release), unlike CStrId. Which engine type it is is unknown.
// Pool is the global pool pointer: most use g_unk_7101d47108; some keyed collections use
// another one (g_unk_7101d47200).
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
template <class Tag, class... Members>
void refl_Dtor(char* self) {
    using Traits = UnkDtorTraits<Tag>;
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
    (Members::Release(self), ...);
    Traits::base(self);
}

template <class Tag, class... Members>
void refl_DeletingDtor(char* self) {
    using Traits = UnkDtorTraits<Tag>;
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
    (Members::Release(self), ...);
    Traits::base(self);
    ::operator delete(self);
}

// The deleting destructor of a class without a base destructor call.
template <class Tag, class... Members>
void refl_DeletingDtorNoBase(char* self) {
    using Traits = UnkDtorTraits<Tag>;
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
    (Members::Release(self), ...);
    ::operator delete(self);
}

}  // namespace base
