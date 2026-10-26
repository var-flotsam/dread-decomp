// Switches an object on or off: records a state value, resets two sub-objects when turning
// on, and forwards the flag to an object reached through field_0x160.
#include "base/UnkCloneCalls.h"
#include "types.h"

void fn_7100208bb4(void* obj, bool flag);
void fn_7100192f08(void* obj, bool flag);

struct UnkStruct_7100146e60_Owner {
    u8 pad_0x0[0x78];
    void* field_0x78;
};

struct UnkStruct_7100146e60 {
    u8 pad_0x0[0x28];
    u8 field_0x28[0xf9];
    bool field_0x121;
    u8 pad_0x122[0x3e];
    UnkStruct_7100146e60_Owner* field_0x160;
    char field_0x168[0xd8];
    bool field_0x240;
    u8 pad_0x241[3];
    s32 field_0x244;

    void Reset(bool enable) {
        if (enable) {
            fn_7100208bb4(field_0x28, true);
            fn_7100025a88(field_0x168);
        }
        field_0x244 = 0;
    }
};

void fn_7100146e60(UnkStruct_7100146e60* self, bool enable) {
    if (self->field_0x240) {
        self->field_0x244 = !enable;
        return;
    }
    if (!self->field_0x121 && enable)
        self->Reset(enable);
    fn_7100192f08(self->field_0x160->field_0x78, enable);
    if (self->field_0x244 == 0 && !enable)
        self->field_0x244 = 2;
}
