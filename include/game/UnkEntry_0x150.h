#pragma once

// A 0x150-byte entry of unit 2636 (tools/tu_map.py): eight of them in the object fn_7100fcfcdc constructs
// (src/game_7100ece7fc/fn_7100fcfcdc.cpp), each built by fn_7100fd10cc and destroyed by fn_7100fd11e8
// (src/game/UnkTU2636/). It holds a reflected value, two arrays and two blocks of settings, each with an
// array of path records and a string id. Names and layout are placeholders inferred from the code.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

extern void* g_unk_7101cac148[];  // the entry base's vtable

// A file path and two more words (0x18 bytes).
struct UnkEntryPathRecord_0x18 {
    base::global::CFilePathStrId path;
    u8 unk_0x8[0x10];
};

// An array freed with its owner.
template <class T>
struct UnkEntryVector : base::global::CRntVector<T> {
    explicit UnkEntryVector(u32 flags) : base::global::CRntVector<T>(flags) {}
    ~UnkEntryVector() { this->Free(); }
};

// A block of settings (0x60 bytes); fn_7100fd10cc builds two.
struct UnkEntryBlock_0x60 {
    u32 field_0x0;
    u32 field_0x4;
    f32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    f32 field_0x14;
    bool field_0x18[14];
    bool field_0x26;
    UnkEntryVector<UnkEntryPathRecord_0x18> records;  // 0x28
    void* field_0x50;
    base::global::CStrId id;  // 0x58
};
static_assert(sizeof(UnkEntryBlock_0x60) == 0x60, "UnkEntryBlock_0x60 layout");

// The entry's base: its vtable, its kind and a reflected value, released when it is destroyed.
struct UnkEntryBase_0x30 {
    void** vtable;
    void* field_0x8;
    s32 kind;
    base::reflection::UnkReflValue value;  // 0x18

    ~UnkEntryBase_0x30() {
        vtable = g_unk_7101cac148 + 2;
        fn_71000a1c60(&value);
    }
};

struct UnkEntry_0x150 : UnkEntryBase_0x30 {
    bool field_0x30;
    UnkEntryVector<void*> field_0x38;
    UnkEntryVector<void*> field_0x60;
    UnkEntryBlock_0x60 blocks[2];  // 0x88
    u8 unk_0x148[8];               // neither the constructor nor the destructor touches it

    ~UnkEntry_0x150() asm("fn_7100fd11e8");  // src/game/UnkTU2636/fn_7100fd11e8.cpp
};
static_assert(sizeof(UnkEntry_0x150) == 0x150, "UnkEntry_0x150 layout");
