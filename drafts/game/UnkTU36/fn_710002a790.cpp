// Unit 36 of tools/tu_map.py (the NFP worker): the state handler that waits for the worker to come up.
// On an error it releases the request's event slots and moves the machine to 0x17 (or 2); otherwise it
// sets up one pair of event slots per tag, registers them with the worker and moves to state 4.
// Names and layouts are placeholders.
#include <string.h>

#include <nn/os.h>

#include "../../base/UnkUnit36/UnkUnit36_Nfp.h"
#include "game/UnkTU36.h"
#include "types.h"

namespace nn::os {
void DestroySystemEvent(SystemEventType* event);
}

void fn_710002d3e0(UnkUnit36Object* self, nn::os::EventType* done);

// One tag's word pair in the request.
struct UnkUnit36TagWord {
    u32 field_0x0;
    u32 field_0x4;
};

// The tags the request tracks: a word pair and an id per tag, and how many there are.
struct UnkUnit36TagTable {
    UnkUnit36TagWord words[9];  // 0x0
    u32 ids[9];  // 0x48
    s32 count;  // 0x6c
};

// The words of the request block g_unk_7101d4f508 this handler uses.
struct UnkUnit36ScanRequest {
    u8 event[0x28];  // nn::os::EventType
    u8 unk_0x28[0x18];
    bool cancelled;  // 0x40
    u8 unk_0x41[3];
    UnkUnit36TagTable table;  // 0x44
    s32 slot;  // 0xb4
    nn::os::SystemEventType* eventsA;  // 0xb8
    nn::os::SystemEventType* eventsB;  // 0xc0
};
static_assert(__builtin_offsetof(UnkUnit36ScanRequest, eventsB) == 0xc0, "");

extern nn::os::SystemEventType g_unk_7101d4f8d0[];
extern nn::os::SystemEventType g_unk_7101d4fa80[];

void fn_710002cccc(UnkUnit36Static* self, u32* id, nn::os::SystemEventType* eventA,
                   nn::os::SystemEventType* eventB, UnkUnit36TagWord* word);

static inline void SetState(UnkUnit36Machine& machine, s32 state) {
    machine.state = state;
    machine.field_0x218 = 0;
    machine.field_0x21c = 0;
}

static inline void SetError(UnkUnit36Machine& machine, u32 result) {
    machine.field_0x8 = result;
    SetState(machine, 0x17);
}

// Destroys the events of both slot arrays that were created and forgets the tags.
static inline void ReleaseSlots(UnkUnit36ScanRequest& request) {
    if (request.eventsA) {
        for (s32 i = 0; i < request.table.count; i++) {
            if (request.eventsA[i].state != nn::os::SystemEventType::State_NotInitialized)
                nn::os::DestroySystemEvent(&request.eventsA[i]);
        }
        request.eventsA = nullptr;
    }
    if (request.eventsB) {
        for (s32 i = 0; i < request.table.count; i++) {
            if (request.eventsB[i].state != nn::os::SystemEventType::State_NotInitialized)
                nn::os::DestroySystemEvent(&request.eventsB[i]);
        }
        request.eventsB = nullptr;
    }
    request.slot = 0;
    memset(&request.table, 0, sizeof(request.table));
}

static inline nn::os::SystemEventType* ResetSlots(nn::os::SystemEventType* events, s32 count) {
    for (nn::os::SystemEventType* event = events; event != events + count; ++event)
        event->state = nn::os::SystemEventType::State_NotInitialized;
    return events;
}

void fn_710002a790() {
    UnkUnit36Machine& machine = UnkUnit36GetMachine();
    machine.field_0x218 = 6;
    UnkUnit36ScanRequest& request = reinterpret_cast<UnkUnit36ScanRequest&>(g_unk_7101d4f508);
    if (!nn::os::TryWaitEvent(UnkUnit36RequestEvent(g_unk_7101d4f508))) {
        if (machine.field_0x21c == 2)
            request.cancelled = true;
        return;
    }

    UnkUnit36Static* worker = fn_710002cc34();
    u32 result = fn_710002d320(worker);
    if (result != 0) {
        if ((result & 0x3fe1ff) == 0x8073) {
            SetError(machine, 0x8073);
            ReleaseSlots(request);
        } else if ((result & 0x3fffff) == 0xa073) {
            SetError(machine, 0xa073);
            ReleaseSlots(request);
        } else {
            SetState(machine, 2);
            ReleaseSlots(request);
        }
        return;
    }
    if (request.cancelled) {
        SetState(machine, 0x16);
        return;
    }
    if (request.table.count < 1) {
        SetState(machine, 2);
        ReleaseSlots(request);
        return;
    }

    request.eventsA = ResetSlots(g_unk_7101d4f8d0, request.table.count);
    request.eventsB = ResetSlots(g_unk_7101d4fa80, request.table.count);
    memset(request.table.ids, 0, sizeof(request.table.ids));
    for (s32 i = 0; i < request.table.count; i++) {
        fn_710002cccc(worker, &request.table.ids[i], &request.eventsA[i], &request.eventsB[i],
                      &request.table.words[i]);
    }
    fn_710002d3e0(reinterpret_cast<UnkUnit36Object*>(worker), UnkUnit36RequestEvent(g_unk_7101d4f508));
    SetState(machine, 4);
}
