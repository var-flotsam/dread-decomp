#pragma once

// Types shared by the reflected copy and move constructors of CAttack's subclasses (the class
// names come from reflection, src/reflection/). Each reflected class has a copy constructor
// (descriptor slot 0x30) and a move constructor (0x38); for these classes both are the
// implicit ones, and their members have no separate moves, so the two are identical.

#include "base/UnkVector.h"
#include "game/UnkActor.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// CAttack (reflection: size 0x28), the root class. Only its layout and vtable are needed.
// The subclass views in game/UnkAttack.h (UnkAttackBase) see the same first 0x20 bytes without
// the virtual destructor.
struct CAttack {
    virtual ~CAttack();
    UnkActor* field_0x8;  // the actor the attack belongs to
    u32 field_0x10;
    bool field_0x14;
    s64 field_0x18;
    u64 field_0x20;
};
static_assert(sizeof(CAttack) == 0x28, "CAttack layout");

// A counted pointer whose copy starts empty and assigns only when the source holds an object (the
// reference-counted types are game_s2/UnkWeakHandle.h's).
template <class T>
struct UnkAttackCountedRef {
    T* ptr = nullptr;

    UnkAttackCountedRef(const UnkAttackCountedRef& other) {
        if (other.ptr)
            *this = other;
    }

    // Unlike AssignRef there is no test for the same object.
    UnkAttackCountedRef& operator=(const UnkAttackCountedRef& other) {
        T* p = other.ptr;
        if (p)
            ++p->ref_count;
        if (T* old = ptr) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        ptr = p;
        return *this;
    }
};

using UnkAttackRefPtr = UnkAttackCountedRef<UnkRefObject>;

// A polymorphic member (vtable 0x7101a22d48 and others) with a ref-counted owner. Its copy
// constructor is user-written (the words are copied one by one). Assignment takes the source
// owner's target and shares that target's weak handle, creating it if needed.
struct UnkAttackListener {
    virtual ~UnkAttackListener();
    u64 field_0x8;
    u64 field_0x10;
    UnkAttackCountedRef<UnkWeakHandle> owner;

    UnkAttackListener(const UnkAttackListener& other)
        : field_0x8(other.field_0x8), field_0x10(other.field_0x10), owner(other.owner) {}

    __attribute__((always_inline)) UnkAttackListener& operator=(const UnkAttackListener& other) {
        __builtin_memcpy(&field_0x8, &other.field_0x8, 0x10);
        CopyWeakRef(&owner.ptr, other.owner.ptr);
        return *this;
    }
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
