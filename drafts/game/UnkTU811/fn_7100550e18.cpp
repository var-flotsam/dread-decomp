#include "types.h"

struct UnkEmmyOwner_b0193 {
    u8 unk_0x0[0x218];
    void* field_0x218;
};

struct UnkEmmyAI_b0193 {
    u8 unk_0x0[0x30];
    UnkEmmyOwner_b0193* field_0x30;
    u8 unk_0x38[0x2a258 - 0x38];
    bool field_0x2a258;
};

u32 fn_71010cc3a4(void*);
bool fn_710054ead0(UnkEmmyAI_b0193*);

u32 fn_7100550e18(UnkEmmyAI_b0193* self) {
    u32 result = 0;
    if (self->field_0x2a258) {
        u8* ai = (u8*)self->field_0x30->field_0x218;
        if (ai != nullptr && fn_71010cc3a4(ai) == 1) {
            bool r = fn_710054ead0(self);
            self->field_0x2a258 = !r;
            result = r;
        }
    }
    return result;
}
