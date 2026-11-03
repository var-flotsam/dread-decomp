// Unit 36 of tools/tu_map.py (the NFP worker): the globals its state handlers share. The state
// machine g_unk_7101a19948 is declared as UnkUnit36_State.cpp declares it (an identical copy of its
// head); UnkUnit36Machine is a wider view of the same words. Names and layouts are placeholders.
#pragma once

#include <nn/os.h>

#include "types.h"

struct UnkStateMachine_710002c1b0 {
    s32 state;
    s32 last_state;
    u8 unk_0x8[4];
    u32 steps;
    u32 entered_step;
    u8 unk_0x14[0x208];
    u32 field_0x21c;
};

// The words of g_unk_7101a19948 the handlers touch.
struct UnkUnit36Machine {
    s32 state;
    u8 unk_0x4[4];
    u32 field_0x8;
    u8 unk_0xc[0x20c];
    u32 field_0x218;
    u32 field_0x21c;
    u8 unk_0x220[0xd8];
    u32 field_0x2f8;
    u32 field_0x2fc;
};

struct UnkUnit36Buffer {
    u8 unk_0x0[0x28];
    bool field_0x28;
};

// The request block g_unk_7101d4f508: the worker's answer event, then the request's words.
struct UnkUnit36Request {
    u8 event[0x28];  // nn::os::EventType
    s32 field_0x28;
    s32 field_0x2c;
    u64 field_0x30;
    u32 field_0x38;
    u8 unk_0x3c[0x8c];
    UnkUnit36Buffer* buffer;  // 0xc8
    u8 unk_0xd0[0x214];
    u32 field_0x2e4;
};

struct UnkUnit36Object;  // UnkUnit36.cpp's view of the worker object
struct UnkUnit36Static;  // the worker object, g_unk_7101d4fc30

extern UnkStateMachine_710002c1b0 g_unk_7101a19948;
extern UnkUnit36Request g_unk_7101d4f508;

inline UnkUnit36Machine& UnkUnit36GetMachine() {
    return reinterpret_cast<UnkUnit36Machine&>(g_unk_7101a19948);
}

inline nn::os::EventType* UnkUnit36RequestEvent(UnkUnit36Request& request) {
    return reinterpret_cast<nn::os::EventType*>(request.event);
}
