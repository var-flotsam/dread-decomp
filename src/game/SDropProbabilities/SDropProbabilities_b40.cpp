// SDropProbabilities (reflection: size 0x90): its copy assignment (fn_71007c81d0), guarded against
// self-assignment. The leading words are copied as a block, the string id at +0x60 by CStrId's copy
// assignment, then the six words one by one and the tail bytes. The layout is the one of
// src/game/SDropProbabilities/fn_71007c8028.cpp. fn_71007c7ddc and fn_71007c7fb8 are two identical
// copies of its default constructor: the defaults, an id built from "", the rest cleared.
#include <new>

#include "base/global/CStrId.h"
#include "game/SDropProbabilities.h"
#include "types.h"

struct UnkPair_dp {
    u64 first;
    u64 second;
};

struct UnkDropHead_dp {
    UnkPair_dp field_0x0[4];
    u64 field_0x40;
    UnkPair_dp field_0x48;
    u32 field_0x58;
};

struct UnkDropTail_dp {
    u64 field_0x80;
    u16 field_0x88;
    u8 field_0x8a;
};

struct UnkDropProbabilities_dp {
    UnkDropHead_dp head;
    base::global::CStrId sNodeToDrop;
    u32 field_0x68[6];
    UnkDropTail_dp tail;
};

UnkDropProbabilities_dp* fn_71007c81d0(UnkDropProbabilities_dp* self, const UnkDropProbabilities_dp* other) {
    if (self != other) {
        __builtin_memcpy(&self->head, &other->head, 0x5c);  // the fields before the id, as one block
        self->sNodeToDrop = other->sNodeToDrop;
        for (int i = 0; i < 6; i++)
            self->field_0x68[i] = other->field_0x68[i];
        __builtin_memcpy(&self->tail, &other->tail, 0xb);  // the fields after the words, as one block
    }
    return self;
}

struct UnkDropDefaults_dp {
    u32 iMinNumOfDroppedItems;
    u32 iMaxNumOfDroppedItems;
    s32 iMaxItemTypeMin;
    s32 iMaxItemTypeMax;
    u32 field_0x10[4];
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 iXCellsToDrop;
    u32 field_0x30[8];
    f32 fMinDistance;
    f32 fMaxDistance;
    f32 fMaxZDisp;
    base::global::CStrId sNodeToDrop;
    u32 field_0x68[6];
    UnkDropTail_dp tail;
};

static inline __attribute__((always_inline)) void Init_dp(UnkDropDefaults_dp* self) {
    self->iMinNumOfDroppedItems = 1;
    self->iMaxNumOfDroppedItems = 1;
    self->iMaxItemTypeMin = -1;
    self->iMaxItemTypeMax = -1;
    __builtin_memset(self->field_0x10, 0, sizeof(self->field_0x10));
    self->field_0x20 = 0;
    self->field_0x24 = 0;
    self->field_0x28 = 0;
    self->iXCellsToDrop = 1;
    __builtin_memset(self->field_0x30, 0, sizeof(self->field_0x30));
    self->fMinDistance = -5.0f;
    self->fMaxDistance = 20.0f;
    self->fMaxZDisp = 10.0f;
    new (&self->sNodeToDrop) base::global::CStrId("", true);
    __builtin_memset(self->field_0x68, 0, sizeof(self->field_0x68));
    self->tail.field_0x80 = 0;
    self->tail.field_0x88 = 0;
    self->tail.field_0x8a = 0;
}

void SDropProbabilities_Construct(UnkDropDefaults_dp* self) { Init_dp(self); }

void fn_71007c7fb8(UnkDropDefaults_dp* self) { Init_dp(self); }
