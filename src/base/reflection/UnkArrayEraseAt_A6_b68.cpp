// base::reflection collection slot 0x128 (src/reflection_types/collection_types_00.cpp) for (data, count)
// arrays of trivially copyable 4- and 8-byte elements: erase the element an index iterator names, copying the
// tail down in one block. The index goes through a 64-bit byte offset from the collection itself and a pointer
// difference (the erase-at-index helper is inlined). One template in twelve functions (CAllowCoolShinesparkLogicAction,
// GUI::CDisplayObjectDef, GUI::IDisplayObjectTrack, CCollider, CEntity, CMinimapManager::CTunableMinimapManager
// and others). Names are placeholders.
#include "types.h"

template <typename T>
struct UnkPodArray_A6_b68 {
    T* data;
    u32 count;
};

template <typename T>
static inline __attribute__((always_inline)) void EraseAtIndex_A6_b68(UnkPodArray_A6_b68<T>* self, u32 index) {
    if (index < self->count) {
        T* pos = self->data + index;
        __builtin_memcpy(pos, pos + 1, (self->count - index - 1) * sizeof(T));
        --self->count;
    }
}

template <typename T>
static inline __attribute__((always_inline)) void EraseAtIter_A6_b68(UnkPodArray_A6_b68<T>* self, const u32* it) {
    T* base = reinterpret_cast<T*>(self);
    T* p = reinterpret_cast<T*>(reinterpret_cast<char*>(base) + (u64)*it * sizeof(T));
    EraseAtIndex_A6_b68(self, (u32)(p - base));
}

#define ERASE_AT_A6_B68(addr, T) \
    void fn_##addr(UnkPodArray_A6_b68<T>* self, const u32* it) { EraseAtIter_A6_b68(self, it); }

ERASE_AT_A6_B68(7100a35214, u32)
ERASE_AT_A6_B68(7100a360f4, u32)
ERASE_AT_A6_B68(7100adb204, u32)
ERASE_AT_A6_B68(7100eb22d0, u32)
ERASE_AT_A6_B68(7100b08d2c, u64)
ERASE_AT_A6_B68(7100b0ccc8, u64)
ERASE_AT_A6_B68(7100b1652c, u64)
ERASE_AT_A6_B68(7100b78dec, u64)
ERASE_AT_A6_B68(7100b83f80, u64)
ERASE_AT_A6_B68(7100d424a4, u64)
ERASE_AT_A6_B68(7100d8525c, u64)
ERASE_AT_A6_B68(7100ffe3e0, u64)
