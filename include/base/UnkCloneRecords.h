#pragma once

// Records of string ids and other members, matched one family at a time (tools/gen_clones.py
// scan and reference kinds): their copy constructors, assignments, destructors and clears.
// Layouts and names are placeholders inferred from the code.

#include <functional>
#include <new>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

void fn_7100067ebc(char* self, const char* other);
namespace base { struct UnkStrIdVec; }
void fn_710000542c(base::UnkStrIdVec* self, u32 capacity);
void fn_71011cf394(void* a0, void* a1, void* holder, void* a3, u32 a4);

namespace base {

// A record of an 8-byte field, a u32 and two string ids, copy-assigned past its first field
// (fn_7100bc59fc and its identical copies).
template <class Tag>
struct UnkStrIdPairRecord {
    u64 field_0x0;
    u32 field_0x8;
    global::CStrId a;
    global::CStrId b;

    UnkStrIdPairRecord& operator=(const UnkStrIdPairRecord& other) {
        if (this != &other) {
            field_0x8 = other.field_0x8;
            a = other.a;
            b = other.b;
        }
        return *this;
    }
};

// A copy constructor of a class derived from a polymorphic base copied out of line
// (fn_7100067ebc): the middle class's vtable, string ids at 0x10, 0x30 and 0x60, a file path
// at 0x20 and plain fields between them, then the class's own field at 0x68 and vtable
// (fn_7100ba10d4 and its copies, one per class). UnkCopyCtorTraits names mid_vtable() and
// vtable().
template <class Tag>
void refl_PathRecordCopyCtor(char* self, const char* other) {
    using Traits = UnkCopyCtorTraits<Tag>;
    fn_7100067ebc(self, other);
    *reinterpret_cast<char**>(self) = Traits::mid_vtable() + 0x10;
    new (self + 0x10) global::CStrId(*reinterpret_cast<const global::CStrId*>(other + 0x10));
    *reinterpret_cast<u32*>(self + 0x18) = *reinterpret_cast<const u32*>(other + 0x18);
    new (self + 0x20) global::CFilePathStrId(*reinterpret_cast<const global::CFilePathStrId*>(other + 0x20));
    *reinterpret_cast<u32*>(self + 0x28) = *reinterpret_cast<const u32*>(other + 0x28);
    new (self + 0x30) global::CStrId(*reinterpret_cast<const global::CStrId*>(other + 0x30));
    *reinterpret_cast<u64*>(self + 0x38) = *reinterpret_cast<const u64*>(other + 0x38);
    __builtin_memcpy(self + 0x40, other + 0x40, 0x1c);
    new (self + 0x60) global::CStrId(*reinterpret_cast<const global::CStrId*>(other + 0x60));
    *reinterpret_cast<u64*>(self + 0x68) = *reinterpret_cast<const u64*>(other + 0x68);
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
}

// A vector of string ids (UnkVector's layout) whose Reserve is out of line (fn_710000542c).
// PushBack grows by the step as UnkVector::PushBack does and stores the id bitwise, so the
// count is reloaded after the retain.
struct UnkStrIdVec {
    global::CStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    UnkAllocator* allocator;

    void Reserve(u32 n) { fn_710000542c(this, n); }
    void PushBack(const global::CStrId& v) {
        u32 n = count;
        u32 c = capacity;
        if (c <= n && step != 0)
            c = (n / step + 1) * step;
        if ((s32)n >= (s32)c)
            for (;;) {
            }
        Reserve(c);
        global::CStrId* slot = &data[count];
        global::UnkStringData* d = v.data;
        __builtin_memcpy(slot, &d, 8);
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
        ++count;
    }
    UnkStrIdVec(const UnkStrIdVec& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        Reserve(other.capacity);
        for (const global::CStrId *p = other.data, *e = p + other.count; p != e; ++p)
            PushBack(*p);
    }
};

// A copy constructor through three vtables: the base's with 0x20 bytes from 8, the middle
// class's with a string-id vector at 0x28 and 0x10 bytes from 0x50, then the class's with a
// word at 0x60 (fn_71005b4d50 and its copies, one per class). UnkCopyCtorTraits names
// base_vtable(), mid_vtable() and vtable().
template <class Tag>
void refl_StrIdVecRecordCopyCtor(char* self, const char* other) {
    using Traits = UnkCopyCtorTraits<Tag>;
    *reinterpret_cast<char**>(self) = Traits::base_vtable() + 0x10;
    __builtin_memcpy(self + 8, other + 8, 0x20);
    *reinterpret_cast<char**>(self) = Traits::mid_vtable() + 0x10;
    new (self + 0x28) UnkStrIdVec(*reinterpret_cast<const UnkStrIdVec*>(other + 0x28));
    __builtin_memcpy(self + 0x50, other + 0x50, 0x10);
    *reinterpret_cast<char**>(self) = Traits::vtable() + 0x10;
    *reinterpret_cast<u64*>(self + 0x60) = *reinterpret_cast<const u64*>(other + 0x60);
}

// A heap object of a vtable (UnkFunctionHolderTraits<Tag>::vtable(), stored before the
// members) and a copied std::function; refl_AddFunctionHolder creates one from a function
// passed by reference (copied once into the by-value parameter, then into the object) and
// hands it to fn_71011cf394 (fn_7100af30a0 and its copies, one per vtable). The function's
// signature is unknown; copying doesn't depend on it.
template <class Tag>
struct UnkFunctionHolderTraits;

template <class Tag>
struct UnkFunctionHolder {
    char* vtable;
    u64 field_0x8;
    std::function<void()> fn;

    UnkFunctionHolder(std::function<void()> f) : vtable(UnkFunctionHolderTraits<Tag>::vtable() + 0x10), fn(f) {}
};

template <class Tag>
void refl_AddFunctionHolder(void* a0, void* a1, const std::function<void()>& f, void* a3, u32 a4) {
    fn_71011cf394(a0, a1, new UnkFunctionHolder<Tag>(f), a3, a4);
}

// Parallel arrays of byte vectors and string-id keys; Clear releases the keys (leaving the
// empty id), frees the vectors and empties the array (fn_710027e494 and its identical copies).
// The vector loop counts in u32, as the original does.
template <class Tag>
struct UnkKeyedVecArray {
    UnkVector<u8>* values;
    UnkStrIdMember* keys;
    u32 count;

    void Clear() {
        if (keys && count)
            for (u64 i = 0; i < count; ++i)
                keys[i].~UnkStrIdMember();
        if (values && count)
            for (u32 i = 0; i < count; ++i)
                values[i].Free();
        count = 0;
    }
};

// A keyed array (values of `Size` bytes with an out-of-line destructor, string-id keys) moved
// into: the keys are released and the values destroyed (the old arrays are not freed, as in
// UnkVector's move assignment), then other's fields are taken (fn_71000d8428 and its copies).
// Clear alone is out of line too (fn_71000d78ec and its copies).
template <class Tag, u64 Size>
struct UnkKeyedMoveArray {
    UnkCloneDtorElem<Tag, Size>* values;
    UnkStrIdMember* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    UnkAllocator* allocator;

    // Out of line in fn_71000d78ec and its copies.
    __attribute__((always_inline)) void Clear() {
        if (keys && count)
            for (u64 i = 0; i < count; ++i)
                keys[i].~UnkStrIdMember();
        if (values && count)
            for (u64 i = 0; i < count; ++i)
                values[i].~UnkCloneDtorElem<Tag, Size>();
        count = 0;
    }
    UnkKeyedMoveArray& operator=(UnkKeyedMoveArray&& other) {
        Clear();
        allocator = other.allocator;
        keys = other.keys;
        values = other.values;
        capacity = other.capacity;
        count = other.count;
        flags = other.flags;
        field_0x18 = other.field_0x18;
        other.count = 0;
        other.values = nullptr;
        other.keys = nullptr;
        return *this;
    }
};

// A callback list and a string id, with or without a key in front; only the destructor is
// out of line (fn_71008933d4 and its copies). UnkCallbackTraits names the list's vtables.
template <class Tag>
struct UnkCallbackEntry {
    UnkCallbackListShort<Tag> list;
    UnkStrIdMember id;
    ~UnkCallbackEntry() {}
};

template <class Tag>
struct UnkKeyedCallbackEntry {
    u64 key;
    UnkCallbackListShort<Tag> list;
    UnkStrIdMember id;
    ~UnkKeyedCallbackEntry() {}
};

}  // namespace base
