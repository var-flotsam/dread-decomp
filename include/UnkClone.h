#pragma once

// Placeholder template arguments for tools/gen_clones.py: functions that are copies of one
// template get distinct instantiations through a tag type named after their address. The
// element types stand in for unknown ones with the same code generation.

#include "base/UnkDestructors.h"
#include "types.h"

// An element of unknown, trivially copyable type and the given size (accessed as bytes).
template <class Tag, u64 Size>
struct UnkCloneElem {
    u8 data[Size];
};

// A polymorphic object deleted through its vtable (deleting destructor in slot 0x10).
struct UnkCloneObject {
    virtual void vf_0x0();
    virtual ~UnkCloneObject();
};

// An owning pointer with unique_ptr's moves and reset-style destructor. The pointer is a union
// member: the original reloads the array's data pointer after each element store, which a
// typed field store wouldn't force.
template <class Tag>
struct UnkCloneOwner {
    union {
        UnkCloneObject* ptr;
    };

    UnkCloneOwner() : ptr(nullptr) {}
    UnkCloneOwner(UnkCloneOwner&& other) {
        UnkCloneObject* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    UnkCloneOwner& operator=(UnkCloneOwner&& other) {
        UnkCloneObject* p = other.ptr;
        other.ptr = nullptr;
        UnkCloneObject* old = ptr;
        ptr = p;
        if (old)
            delete old;
        return *this;
    }
    bool operator==(const UnkCloneOwner& other) const { return ptr == other.ptr; }
    ~UnkCloneOwner() {
        UnkCloneObject* p = ptr;
        ptr = nullptr;
        if (p)
            delete p;
    }
};

// A pointer-sized handle with a user-provided copy and a trivial destructor (a union member,
// as in UnkCloneOwner).
template <class Tag>
struct UnkCloneHandle {
    union {
        void* ptr;
    };

    UnkCloneHandle() : ptr(nullptr) {}
    UnkCloneHandle(const UnkCloneHandle& other) : ptr(other.ptr) {}
    bool operator==(const UnkCloneHandle& other) const { return ptr == other.ptr; }
};

// An 8-byte, 8-aligned trivially copyable element (UnkCloneElem<Tag, 8> is byte-aligned).
template <class Tag>
struct UnkCloneWord {
    u64 value;

    bool operator==(const UnkCloneWord& other) const { return value == other.value; }
};

// A 4-byte trivially copyable element.
template <class Tag>
struct UnkCloneU32 {
    u32 value;

    bool operator==(const UnkCloneU32& other) const { return value == other.value; }
};

// A string id released in two steps (base::UnkStrIdMember).
template <class Tag>
struct UnkCloneStrId : base::UnkStrIdMember {};

// The same, from CFilePathStrId's pool.
template <class Tag>
struct UnkCloneStrIdB : base::UnkStrIdMemberOf<g_unk_7101d47200> {};
