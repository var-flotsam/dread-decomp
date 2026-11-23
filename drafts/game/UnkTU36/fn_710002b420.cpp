// Unit 36 of tools/tu_map.py (the NFP worker): the state handler that reads a tag's answer once the
// worker's answer event fired. It copies the tag's info block into the machine, sorts its type into a
// kind, then stores the outcome code and moves the machine to state 8. Names and layouts are placeholders.
#include <string.h>

#include <nn/os.h>

#include "../../../src/base/UnkUnit36/UnkUnit36_Nfp.h"
#include "game/UnkTU36.h"
#include "types.h"

struct UnkUnit36Table;
bool fn_710002d328(UnkUnit36Table* self, f32* out, u32 skip);

// The tag info block the worker fills in and the machine keeps a copy of.
struct UnkUnit36TagInfo {
    u8 unk_0x0[4];
    u16 type;  // 0x4
    u8 unk_0x6[0x3a];
};

// The words of the request block g_unk_7101d4f508 this handler uses.
struct UnkUnit36TagRequest {
    u8 event[0x28];  // nn::os::EventType
    s32 state;  // 0x28
    s32 status;  // 0x2c
    u8 unk_0x30[8];
    u32 result;  // 0x38
    u8 unk_0x3c[0xec];
    UnkUnit36TagInfo info;  // 0x128
};

// The words of the state machine g_unk_7101a19948 this handler uses.
struct UnkUnit36TagMachine {
    s32 state;
    u8 unk_0x4[4];
    u32 result;  // 0x8
    u8 unk_0xc[0x34];
    u8 field_0x40[0x58];
    UnkUnit36TagInfo info;  // 0x98
    u8 field_0xd8[0x140];
    u32 field_0x218;
    u32 field_0x21c;
    u8 field_0x220[0xd8];
    u32 kind;  // 0x2f8
};
static_assert(__builtin_offsetof(UnkUnit36TagMachine, kind) == 0x2f8, "");

static inline void Finish(UnkUnit36TagRequest& request, UnkUnit36TagMachine& machine, s32 status, u32 result) {
    request.state = machine.state;
    request.status = status;
    request.result = result;
    machine.result = result;
    machine.state = 8;
    machine.field_0x218 = 0;
    machine.field_0x21c = 0;
}

void fn_710002b420() {
    UnkUnit36TagRequest& request = reinterpret_cast<UnkUnit36TagRequest&>(g_unk_7101d4f508);
    if (!nn::os::TryWaitEvent(UnkUnit36RequestEvent(g_unk_7101d4f508)))
        return;
    UnkUnit36Static* worker = fn_710002cc34();
    u32 result = fn_710002d320(worker);
    UnkUnit36TagMachine& machine = reinterpret_cast<UnkUnit36TagMachine&>(g_unk_7101a19948);
    memset(machine.field_0x40, 0, sizeof(machine.field_0x40));

    alignas(8) u32 code;
    bool valid;
    u32 kind;
    if (fn_710002d328(reinterpret_cast<UnkUnit36Table*>(worker), reinterpret_cast<f32*>(&code), 0x12) &&
        (code == 0 || (code & 0x3ffe00) == 0x12000)) {
        machine.info = request.info;
        valid = true;
        kind = 0xffff;
        switch (machine.info.type) {
        case 6:
            kind = 0;
            break;
        case 0x1d:
            kind = 1;
            break;
        case 0x365:
            kind = 2;
            break;
        case 0x366:
            kind = 3;
            break;
        case 0x37f:
            kind = 4;
            break;
        case 0x380:
            kind = 5;
            break;
        case 0x412:
            kind = 6;
            break;
        case 0x413:
            kind = 7;
            break;
        }
    } else {
        valid = false;
        kind = 0xffff;
        machine.info = UnkUnit36TagInfo();
    }
    machine.kind = kind;
    memset(machine.field_0x220, 0, sizeof(machine.field_0x220));
    memset(machine.field_0xd8, 0, sizeof(machine.field_0xd8));

    if (valid && kind == 0xffff) {
        Finish(request, machine, 0x14, 0x17073);
    } else if (result == 0) {
        Finish(request, machine, 0xe, result);
    } else {
        switch (result & 0x3fffff) {
        case 0xf073:
            Finish(request, machine, 0x12, result);
            break;
        case 0x10073:
            Finish(request, machine, 0xf, result);
            break;
        case 0x11073:
            Finish(request, machine, 0x10, result);
            break;
        case 0x12073:
            Finish(request, machine, 0x11, result);
            break;
        case 0x13073:
            Finish(request, machine, 0x13, result);
            break;
        default:
            Finish(request, machine, 0x17, result);
            break;
        }
    }
}
