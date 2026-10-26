#pragma once

// The music manager object (0xf18 bytes), built by fn_7100fd3804 into a block the lazily created
// singleton g_unk_71726bc560 points to. The unit (tools/tu_map.py: unit 2637, 0x7100fd1c7c-
// 0x7100fdeaa4) reflects sound::CMusicManager::CTunableMusicManager. Only the parts the
// constructor touches are spelled out; names and the layout of the parts are placeholders
// inferred from the code.

#include "base/UnkChainedMap.h"
#include "base/UnkVector.h"
#include "types.h"

// An entry of the request array at +0xb8 (0x60 bytes: a file-path id list and a string id; the
// copy that grows the array is fn_7100fddcb4).
struct UnkMusicEntry_0x60;

extern char g_unk_7101cac250[];  // the slot list's vtable
extern char g_unk_7101cac270[];  // its base vtable

// A list of active nodes: the element of the slot array at +0xe0 (grown by fn_7100fde040).
// field_0x30 is set to a 0x10-byte object (vtable g_unk_7101cac2d0 + 0x10, fn_7100fd37f0) by the
// manager's constructor.
struct UnkMusicSlot_0x38 {
    char* vtable;
    u32 count;
    void* head;
    void* tail;
    u32 flags;
    base::UnkAllocator* allocator;
    void* field_0x30;

    // The node list's constructor (the vtable is set twice, base first, as the list's base class does).
    __attribute__((always_inline)) UnkMusicSlot_0x38() {
        vtable = g_unk_7101cac270 + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        flags = 0x10;
        base::UnkAllocator* a = fn_7100083990();
        vtable = g_unk_7101cac250 + 0x10;
        allocator = a;
        field_0x30 = nullptr;
    }
};
static_assert(sizeof(UnkMusicSlot_0x38) == 0x38, "UnkMusicSlot_0x38 layout");

// The object stored at UnkMusicSlot_0x38::field_0x30.
struct UnkMusicSlotData_0x10 {
    char* vtable;
    void (*field_0x8)();
};

// Sub-objects the constructor builds through out-of-line constructors (each takes its index).
struct UnkMusicChannel_0x30 {
    u8 unk_0x0[0x30];
};
struct UnkMusicBank_0xb78 {
    u8 unk_0x0[0xb78];
};
struct UnkMusicTrack_0x38 {
    u8 unk_0x0[0x38];
};
struct UnkMusicTrack_0x32 {
    u8 unk_0x0[0x32];
};

struct UnkMusicManager {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    u8 unk_0x24[0x80];
    s32 field_0xa4;
    u16 field_0xa8;
    u8 unk_0xaa[0xe];
    base::UnkVector<UnkMusicEntry_0x60> entries;  // 0xb8
    base::UnkVector<UnkMusicSlot_0x38> slots;     // 0xe0
    base::UnkVector<u8> slot_active;              // 0x108
    UnkChainedMap map_0;  // 0x130 (each starts with 11 empty buckets)
    UnkChainedMap map_1;  // 0x178
    UnkChainedMap map_2;  // 0x1c0
    UnkChainedMap map_3;  // 0x208
    u32 slot_count;       // 0x250: the number of slots, from the sound manager (fn_7100108b7c)
    u8 unk_0x254[4];
    UnkMusicChannel_0x30 channel_1;   // 0x258
    UnkMusicChannel_0x30 channel_2;   // 0x288
    UnkMusicBank_0xb78 bank_3;        // 0x2b8
    UnkMusicTrack_0x38 track_4;       // 0xe30
    UnkMusicTrack_0x38 track_5;       // 0xe68
    UnkMusicTrack_0x38 track_6;       // 0xea0
    UnkMusicTrack_0x32 track_7;       // 0xed8
    u8 field_0xf0a;
    u8 unk_0xf0b[5];
    u64 field_0xf10;
};
static_assert(sizeof(UnkMusicManager) == 0xf18, "UnkMusicManager layout");

// The singleton is g_unk_71726bc560 (reached through the GOT); its users still declare it as a
// void* or a local view, so it is not declared here yet.

// The constructor, run on a fresh 0xf18-byte block (callers pass the raw allocation).
void fn_7100fd3804(void* block);

// Grow the request and slot arrays to hold n elements (out-of-line reserves).
void fn_7100fddcb4(base::UnkVector<UnkMusicEntry_0x60>* v, u32 n);
void fn_7100fde040(base::UnkVector<UnkMusicSlot_0x38>* v, u32 n);

// The channel constructor (also the base of the bank at +0x2b8): the kind is its index.
void fn_7100fe1028(UnkMusicChannel_0x30* p, u32 kind);
// The bank's constructor (src/game_7100ece7fc/fn_7100fcfcdc.cpp), the kind passed on to its channel base.
void fn_7100fcfcdc(UnkMusicBank_0xb78* bank, u32 kind);
