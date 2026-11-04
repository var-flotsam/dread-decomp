#pragma once

// Iterators over linked lists whose current element is kept in a two-state variant (the shape of
// src/game/CLightManager/UnkListIterVariantNext_b43.cpp), in CCollider's unit: Sync sets the variant
// from the current node, as a pair {node, node + 8}, or resets it at the end. State 1 holds a
// {string id, refcounted pointer} object. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "types.h"

template <int Tag>
struct UnkIdRefObject_b56 {
    base::UnkStrIdMember id;
    base::UnkRefPtrMember ref;
};

template <int Tag>
struct UnkRefListVariant_b56 {
    union {
        UnkIdRefObject_b56<Tag> object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    UnkRefListVariant_b56() {}
    ~UnkRefListVariant_b56() {}

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            object.~UnkIdRefObject_b56();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    __attribute__((always_inline)) void SetPair(void* first, void* second) {
        s32* p = &state;
        switch (*p) {
        case 1:
            object.~UnkIdRefObject_b56();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
        state = 2;
        pair.first = first;
        pair.second = second;
    }
};

template <int Tag>
struct UnkRefListIter_b56 {
    u8* node;
    UnkRefListVariant_b56<Tag> variant;

    __attribute__((always_inline)) void Sync() {
        u8* n = node;
        if (n)
            variant.SetPair(n, n + 8);
        else
            variant.Reset();
    }
};

void fn_7100d3d538(UnkRefListIter_b56<0>* self);
