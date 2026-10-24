// Runs the base handler, then, when the owner's AI is a CCharClassCrazyTrack, either
// notifies it (flag set in the message) or updates and resets a pending timer.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "types.h"

using base::reflection::CClass;

bool fn_710117b348(void* obj, const CClass* cls);

struct UnkStruct_710036974c_Msg {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

struct UnkStruct_710036974c_Owner {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkStruct_710036974c {
    u8 pad_0x0[0x30];
    UnkStruct_710036974c_Owner* field_0x30;
    u8 pad_0x38[0x14674];
    f32 field_0x146ac;
};

u32 fn_7101111d88(UnkStruct_710036974c* self, UnkStruct_710036974c_Msg* msg);
void fn_710110cc04(UnkStruct_710036974c* self, bool flag);
void fn_71003697f0(UnkStruct_710036974c* self);

u32 fn_710036974c(UnkStruct_710036974c* self, UnkStruct_710036974c_Msg* msg) {
    u32 r = fn_7101111d88(self, msg);
    void* ai = self->field_0x30->field_0x220;
    bool is_track = ai && fn_710117b348(ai, fn_7100cf3508());
    if (msg->field_0x8) {
        if (is_track)
            fn_710110cc04(self, true);
    } else if (is_track) {
        fn_71003697f0(self);
        if (self->field_0x146ac > 0.0f)
            self->field_0x146ac = -1.0f;
    }
    return r;
}
