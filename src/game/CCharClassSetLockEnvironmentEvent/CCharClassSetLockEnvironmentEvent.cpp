// CCharClassSetLockEnvironmentEvent's unit: passes the event's three flags to the environment
// manager (created on first use, as the lua_bind getters do in src/script/lua_bind.cpp) through
// fn_7100d8efd0. Names and the layout are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101d47368;  // the environment manager (0x4c8 bytes)
void fn_7100d8ced8(void* self);  // its constructor
void fn_7100d8efd0(void* self, bool a, bool b, bool c);

struct UnkSetLockEnvironmentEvent {
    u8 unk_0x0[0xc];
    bool field_0xc;
    bool field_0xd;
    bool field_0xe;
};

void fn_7100cd3c10(UnkSetLockEnvironmentEvent* self) {
    void* m = g_unk_7101d47368;
    if (!m) {
        m = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(m);
        g_unk_7101d47368 = m;
    }
    fn_7100d8efd0(m, self->field_0xd, self->field_0xe, self->field_0xc);
}
