// CUsableComponent unit (tools/tu_map.py, the name from reflection): sets the flag at +0xfb and gives the
// owner, and the actor at +0xe0's +0x10 when there is one, one of two timeline keys for it, then runs
// the update. Names and layouts are placeholders.
#include "game/UnkTimelineComponent.h"
#include "types.h"

void fn_7100a9f7f0(char* self);
extern const u64 g_unk_710164b638;
extern const u64 g_unk_710164b640;

struct UnkUsableLink {
    u8 pad_0x0[0x10];
    UnkActor* field_0x10;
};

struct UnkUsableComponent {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
    u8 pad_0x38[0xe0 - 0x38];
    UnkUsableLink* field_0xe0;
    u8 pad_0xe8[0xfb - 0xe8];
    bool field_0xfb;
};

void fn_7100a9f764(UnkUsableComponent* self, bool on) {
    self->field_0xfb = on;
    if (on) {
        fn_7100d7c828(self->field_0x30, &g_unk_710164b638, false);
        if (self->field_0xe0 && self->field_0xe0->field_0x10)
            fn_7100d7c828(self->field_0xe0->field_0x10, &g_unk_710164b638, false);
    } else {
        fn_7100d7c828(self->field_0x30, &g_unk_710164b640, false);
        if (self->field_0xe0 && self->field_0xe0->field_0x10)
            fn_7100d7c828(self->field_0xe0->field_0x10, &g_unk_710164b640, false);
    }
    fn_7100a9f7f0(reinterpret_cast<char*>(self));
}
