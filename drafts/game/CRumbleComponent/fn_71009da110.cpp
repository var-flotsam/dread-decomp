// CRumbleComponent's unit (tools/tu_map.py unit 1500): fn_71009da110 marks every entry of the record list
// (+0x48) stopped, fading out the rumble of each that was not yet stopped and has the flag at +0x60; then
// fades out the rumble every stopped entry still refers to. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU50.h"
#include "types.h"

struct UnkRumbleTarget_l9da110 {
    void* rumble;
    s64 id;
};

// A record of the list (0x68 bytes, as src/game/CRumbleComponent/CRumbleComponent_Entries_ovb0083.cpp lays it
// out); only the members read here are named.
struct UnkRumbleEntry_l9da110 {
    UnkRumbleTarget_l9da110* target;  // current while target->id == id
    s64 id;
    u8 pad_0x10[0x38];
    f32 fade;  // 0x48
    u8 pad_0x4c[0x14];
    bool field_0x60;
    u8 pad_0x61[2];
    bool stopped;  // 0x63
    u8 pad_0x64[4];

    // The rumble while the handle is current.
    void* Get() const {
        if (!target || id == -1 || target->id != id)
            return nullptr;
        return target->rumble;
    }
};
static_assert(sizeof(UnkRumbleEntry_l9da110) == 0x68, "UnkRumbleEntry_l9da110 layout");

struct UnkRumbleComponent_l9da110 {
    u8 pad_0x0[0x48];
    base::global::CRntVector<UnkRumbleEntry_l9da110> entries;  // 0x48
};

void fn_71009da110(UnkRumbleComponent_l9da110* self) {
    for (UnkRumbleEntry_l9da110& entry : self->entries) {
        if (entry.stopped)
            continue;
        if (entry.field_0x60) {
            if (void* rumble = entry.Get())
                fn_7100072328(rumble, entry.fade, 0.0f);
        }
        entry.stopped = true;
    }
    for (UnkRumbleEntry_l9da110& entry : self->entries) {
        if (!entry.stopped)
            continue;
        if (void* rumble = entry.Get())
            fn_7100072328(rumble, entry.fade, 0.0f);
    }
}
