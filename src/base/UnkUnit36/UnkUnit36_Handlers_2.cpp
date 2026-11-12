// Unit 36 of tools/tu_map.py: another state handler of the NFP worker's machine (g_unk_7101a19948,
// see UnkUnit36_Handlers.cpp). Once the worker's answer event fired it reads the answer: an error
// moves the machine to state 0x17 with the code; success clears the request, the two events of the
// current slot (+0xb4: arrays of 0x30-byte events at +0xb8 and +0xc0) and the one at +0xc8, hands
// the slot's word (+0x44) to the worker (fn_710002ccfc), posts the next request (fn_710002d3e0)
// and moves to state 6. Names and layouts are placeholders.
#include <nn/os.h>

#include "UnkUnit36_Nfp.h"
#include "game/UnkTU36.h"
#include "types.h"

namespace nn::os {
void ClearSystemEvent(SystemEventType* event);
}

void fn_710002d3e0(UnkUnit36Object* self, nn::os::EventType* done);

static inline nn::os::SystemEventType* SlotEvent(char* base, s32 slot) {
    return reinterpret_cast<nn::os::SystemEventType*>(base + (s64)slot * 0x30);
}

void fn_710002bb70() {
    UnkUnit36Request& request = g_unk_7101d4f508;
    if (!nn::os::TryWaitEvent(UnkUnit36RequestEvent(request)))
        return;
    u32 result = fn_710002d320(fn_710002cc34());
    UnkUnit36Machine& machine = UnkUnit36GetMachine();
    if (result != 0) {
        machine.state = 0x17;
        machine.field_0x218 = 0;
        machine.field_0x21c = 0;
        machine.field_0x8 = result;
        return;
    }
    char* raw = reinterpret_cast<char*>(&request);
    request.field_0x28 = machine.state;
    request.field_0x2c = 0xb;
    request.field_0x30 = 0;
    request.field_0x38 = 0;
    nn::os::ClearSystemEvent(SlotEvent(*reinterpret_cast<char**>(raw + 0xb8), *reinterpret_cast<s32*>(raw + 0xb4)));
    nn::os::ClearSystemEvent(SlotEvent(*reinterpret_cast<char**>(raw + 0xc0), *reinterpret_cast<s32*>(raw + 0xb4)));
    nn::os::ClearSystemEvent(reinterpret_cast<nn::os::SystemEventType*>(request.buffer));
    UnkUnit36Static* worker = fn_710002cc34();
    fn_710002ccfc(worker, raw + 0x44 + (s64)*reinterpret_cast<s32*>(raw + 0xb4) * 8);
    fn_710002d3e0(reinterpret_cast<UnkUnit36Object*>(worker), UnkUnit36RequestEvent(request));
    machine.state = 6;
    machine.field_0x218 = 0;
    machine.field_0x21c = 0;
}
