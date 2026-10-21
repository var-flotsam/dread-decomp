#pragma once

// Records of string ids and other members, matched one family at a time (tools/gen_clones.py
// scan and reference kinds): their copy constructors, assignments, destructors and clears.
// Layouts and names are placeholders inferred from the code.

#include <new>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

void fn_7100067ebc(char* self, const char* other);

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
