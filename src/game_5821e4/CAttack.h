#pragma once

// Types shared by the reflected copy and move constructors of CAttack's subclasses (the class
// names come from reflection, src/reflection/). Each reflected class has a copy constructor
// (descriptor slot 0x30) and a move constructor (0x38); for these classes both are the
// implicit ones, and their members have no separate moves, so the two are identical.

#include "types.h"

// CAttack (reflection: size 0x28), the root class. Only its layout and vtable are needed.
struct CAttack {
    virtual ~CAttack();
    u64 field_0x8;
    u32 field_0x10;
    bool field_0x14;
    s64 field_0x18;
    u64 field_0x20;
};

struct UnkAttackRefCounted {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    // A union member (no type-based alias info): after the increment in the assignment the
    // pointer being replaced is reloaded.
    union {
        s32 ref_count;
    };
};

// Copying starts empty and assigns only when the source holds an object.
struct UnkAttackRefPtr {
    UnkAttackRefCounted* ptr = nullptr;

    UnkAttackRefPtr(const UnkAttackRefPtr& other) {
        if (other.ptr)
            *this = other;
    }

    UnkAttackRefPtr& operator=(const UnkAttackRefPtr& other) {
        UnkAttackRefCounted* p = other.ptr;
        if (p)
            ++p->ref_count;
        if (UnkAttackRefCounted* old = ptr) {
            if (--old->ref_count == 0)
                old->Delete();
        }
        ptr = p;
        return *this;
    }
};

// A polymorphic member (vtable 0x7101a22d48 and others) with a ref-counted owner. Its copy
// constructor is user-written (the words are copied one by one).
struct UnkAttackListener {
    virtual ~UnkAttackListener();
    u64 field_0x8;
    u64 field_0x10;
    UnkAttackRefPtr owner;

    UnkAttackListener(const UnkAttackListener& other)
        : field_0x8(other.field_0x8), field_0x10(other.field_0x10), owner(other.owner) {}
};

// Words that are copied one by one (a member with a user-written copy).
template <int N>
struct UnkAttackWords {
    u32 w[N];

    UnkAttackWords(const UnkAttackWords& other) {
        for (int i = 0; i < N; i++)
            w[i] = other.w[i];
    }
};
