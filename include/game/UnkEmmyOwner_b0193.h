#pragma once

// The object at +0x30 of UnkEmmyAI_b0193 (include/game/UnkEmmyAI_b0193.h): its AI object at +0x218 and its
// collision container at +0x2a0. Names and layouts are placeholders.
#include "types.h"

struct UnkCollisionContainer;
struct UnkEmmyAIObj_b0193;

struct UnkEmmyAIObjVtbl_b0193 {
    void* unk_0x0[85];
    void* (*vf_0x2a8)(UnkEmmyAIObj_b0193* self);
    void (*vf_0x2b0)(UnkEmmyAIObj_b0193* self, void* arg);
};

struct UnkEmmyAIObj_b0193 {
    UnkEmmyAIObjVtbl_b0193* vtbl;
};

struct UnkEmmyOwner_b0193 {
    u8 unk_0x0[0x218];
    UnkEmmyAIObj_b0193* field_0x218;
    u8 unk_0x220[0x2a0 - 0x220];
    UnkCollisionContainer* field_0x2a0;
};
