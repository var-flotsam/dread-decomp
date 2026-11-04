#pragma once

// Unit 2604 (tools/tu_map.py; no reflected class): iterators over linked lists whose current element is
// kept in a two-state variant (the shape of src/game/CLightManager/UnkListIterVariantNext_b43.cpp).
// State 1 holds an object with two string ids at +0 and +8, released in reverse. Sync makes the variant
// the pair {node, node + 8} when there is a node, else resets it. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "types.h"

template <int Tag>
struct UnkIdPairListIter_b56 {
    u8* node;
    union {
        struct {
            base::UnkStrIdMember first;
            base::UnkStrIdMember second;
            u8 unk_0x10[0x18];
        } object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    UnkIdPairListIter_b56() {}
    ~UnkIdPairListIter_b56() {}

    __attribute__((always_inline)) void DestroyObject() {
        object.second.~UnkStrIdMemberOf();
        object.first.~UnkStrIdMemberOf();
    }

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            DestroyObject();
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
            DestroyObject();
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

    __attribute__((always_inline)) void Sync() {
        u8* n = node;
        if (n)
            SetPair(n, n + 8);
        else
            Reset();
    }
};

void fn_7100f8fea0(UnkIdPairListIter_b56<0>* self);
