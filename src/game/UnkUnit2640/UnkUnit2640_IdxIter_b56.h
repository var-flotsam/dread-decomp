#pragma once

// Unit 2640 (tools/tu_map.py; no reflected class): (index, element) iterators over arrays whose
// current element is kept in a two-state variant (the shape of
// src/game/CLightManager/UnkListIterVariantNext_b43.cpp). State 1 holds an object with an array
// (base::UnkVector) of 0x18-byte records led by a file path id at +0x10 of the object, destroyed
// inline (the ids released, the array freed); src/game/UnkUnit2640/UnkUnit2640_VariantReset_b55.cpp
// resets them. Sync makes the variant the pair {iterator, element} when there is an element, else
// resets it. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkIdRecord18_b56 {
    base::UnkStrIdMemberOf<g_unk_7101d47200> id;  // CFilePathStrId's pool
    u8 unk_0x8[0x10];
};

template <int Tag>
struct UnkRecordsIdxIter_b56 {
    u32 index;
    u8* current;
    union {
        struct {
            u8 unk_0x0[0x10];
            base::UnkVector<UnkIdRecord18_b56> items;
            u8 unk_tail[0x20];
        } object;
        struct {
            void* first;
            void* second;
        } pair;
    };
    s32 state;

    UnkRecordsIdxIter_b56() {}
    ~UnkRecordsIdxIter_b56() {}

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            object.items.Free();
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
            object.items.Free();
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
        u8* c = current;
        if (c)
            SetPair(this, c);
        else
            Reset();
    }
};

void fn_7100ff1534(UnkRecordsIdxIter_b56<0>* self);
