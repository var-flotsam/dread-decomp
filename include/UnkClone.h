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

// An element of the given size with an out-of-line destructor, called through
// UnkCloneDtorTraits<Tag>::destroy (one specialisation per clone, naming the destructor).
template <class Tag>
struct UnkCloneDtorTraits;

template <class Tag, u64 Size>
struct UnkCloneDtorElem {
    alignas(8) char data[Size];

    ~UnkCloneDtorElem() { UnkCloneDtorTraits<Tag>::destroy(data); }
};

// A block of unknown, trivially copyable contents and the given size (a multiple of 8). Held as
// words: a byte array changes how the element address is formed.
template <class Tag, u64 Size>
struct UnkCloneBlock {
    u64 words[Size / 8];
};

// A polymorphic object deleted through its vtable (deleting destructor in slot 0x10).
struct UnkCloneObject {
    virtual void vf_0x0();
    virtual ~UnkCloneObject();
};

// Polymorphic elements of the given size destroyed through their vtable: the destructor in slot
// 0 (UnkClonePolyA) or slot 1 (UnkClonePolyB).
struct UnkClonePolyA {
    virtual ~UnkClonePolyA();
};

struct UnkClonePolyB {
    virtual void vf_0x0();
    virtual ~UnkClonePolyB();
};

// An owning pointer with unique_ptr's moves and reset-style destructor. The pointer is a union
// member: the original reloads the array's data pointer after each element store, which a
// typed field store wouldn't force. Object is what it deletes (UnkClonePolyA for an object whose
// deleting destructor is in slot 0x8).
template <class Tag, class Object = UnkCloneObject>
struct UnkCloneOwner {
    union {
        Object* ptr;
    };

    UnkCloneOwner() : ptr(nullptr) {}
    UnkCloneOwner(UnkCloneOwner&& other) {
        Object* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    UnkCloneOwner& operator=(UnkCloneOwner&& other) {
        Object* p = other.ptr;
        other.ptr = nullptr;
        Object* old = ptr;
        ptr = p;
        if (old)
            delete old;
        return *this;
    }
    bool operator==(const UnkCloneOwner& other) const { return ptr == other.ptr; }
    ~UnkCloneOwner() {
        Object* p = ptr;
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

// A 4-byte and a 1-byte value in an 8-byte slot, copied member by member (the copy loads and
// stores the value's width, not the slot's).
template <class Tag>
struct alignas(8) UnkCloneSlot32 {
    u32 value;

    UnkCloneSlot32& operator=(const UnkCloneSlot32& other) {
        value = other.value;
        return *this;
    }
    bool operator==(const UnkCloneSlot32& other) const { return value == other.value; }
};

template <class Tag>
struct alignas(8) UnkCloneSlot8 {
    u8 value;

    UnkCloneSlot8& operator=(const UnkCloneSlot8& other) {
        value = other.value;
        return *this;
    }
    bool operator==(const UnkCloneSlot8& other) const { return value == other.value; }
};

// A 16-byte, 8-aligned trivially copyable element.
template <class Tag>
struct UnkCloneWord2 {
    u64 first;
    u64 second;

    bool operator==(const UnkCloneWord2& other) const { return first == other.first && second == other.second; }
};

// A string id released in two steps (base::UnkStrIdMember).
template <class Tag>
struct UnkCloneStrId : base::UnkStrIdMember {};

// The same, from CFilePathStrId's pool.
template <class Tag>
struct UnkCloneStrIdB : base::UnkStrIdMemberOf<g_unk_7101d47200> {};

// An element of the given size whose only non-trivial member is a string id (Id, one of the
// above) at Off.
template <class Tag, u64 Off, u64 Size, class Id>
struct UnkCloneStrIdElem {
    u64 words[Size / 8];

    ~UnkCloneStrIdElem() { reinterpret_cast<Id*>(reinterpret_cast<char*>(words) + Off)->~Id(); }
};


template <class Tag, class Base, u64 Size>
struct UnkClonePolyElem : Base {
    u64 words[(Size - 8) / 8];
};

// The same with two string ids, at Lo and Hi (destroyed in reverse order).
template <class Tag, u64 Lo, u64 Hi, u64 Size, class Id>
struct UnkCloneStrIdElem2 {
    u64 words[Size / 8];

    ~UnkCloneStrIdElem2() {
        reinterpret_cast<Id*>(reinterpret_cast<char*>(words) + Hi)->~Id();
        reinterpret_cast<Id*>(reinterpret_cast<char*>(words) + Lo)->~Id();
    }
};

// An element of the given size whose move constructor is out of line, called through
// UnkCloneMoveTraits<Tag>::move (one specialisation per clone).
template <class Tag>
struct UnkCloneMoveTraits;

template <class Tag, u64 Size>
struct UnkCloneMoveElem {
    u64 words[Size / 8];

    UnkCloneMoveElem(UnkCloneMoveElem&& other) { UnkCloneMoveTraits<Tag>::move(this, &other); }
};

// An element of the given size whose copy-assignment is out of line, called through
// UnkCloneAssignTraits<Tag>::assign (one specialisation per clone).
template <class Tag>
struct UnkCloneAssignTraits;

template <class Tag, u64 Size>
struct UnkCloneAssignElem {
    u64 words[Size / 8];

    UnkCloneAssignElem& operator=(const UnkCloneAssignElem& other) {
        UnkCloneAssignTraits<Tag>::assign(this, &other);
        return *this;
    }
};
