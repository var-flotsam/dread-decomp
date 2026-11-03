// Unit 36 of tools/tu_map.py: starts the NFP worker. It resets the state machine in
// g_unk_7101a19948 (see UnkUnit36_State.cpp) and the request block g_unk_7101d4f508; then, when
// the NFP library is not initialised yet and the machine asks for it, it builds the event, the
// worker object (fn_710002cbec) and a 0x30-byte buffer, hands the buffer to the object, posts the
// first request (fn_710002d3e0) and moves the machine to state 1. Names and layouts are placeholders.
#include <nn/nfp.h>
#include <nn/os.h>

#include "UnkUnit36_Nfp.h"
#include "base/UnkVector.h"
#include "types.h"

UnkUnit36Static* fn_710002cbec();
void fn_710002cc7c(UnkUnit36Static* self);
void fn_710002cf54(UnkUnit36Static* self, UnkUnit36Buffer* buffer);
void fn_710002d3e0(UnkUnit36Object* self, nn::os::EventType* done);

void fn_710002a600() {
    UnkUnit36Machine& machine = UnkUnit36GetMachine();
    UnkUnit36Request& request = g_unk_7101d4f508;
    machine.field_0x2f8 = 0xffff;
    machine.field_0x2fc = 0;
    request.field_0x28 = 0;
    request.field_0x2c = 0;
    request.field_0x30 = 0;
    request.field_0x38 = 0;
    machine.field_0x218 = 1;
    machine.field_0x8 = 0;
    if (nn::nfp::GetState() == nn::nfp::State_NonInitialized && machine.field_0x21c == 1) {
        nn::os::EventType* event = UnkUnit36RequestEvent(request);
        nn::os::InitializeEvent(event, false, nn::os::EventClearMode_AutoClear);
        UnkUnit36Static* object = fn_710002cbec();
        fn_710002cc7c(object);
        UnkUnit36Buffer* buffer = static_cast<UnkUnit36Buffer*>(fn_7100083b60(0x30));
        buffer->field_0x28 = false;
        request.buffer = buffer;
        fn_710002cf54(object, buffer);
        fn_710002d3e0(reinterpret_cast<UnkUnit36Object*>(object), event);
        machine.state = 1;
        machine.field_0x218 = 0;
        machine.field_0x21c = 0;
    }
}
