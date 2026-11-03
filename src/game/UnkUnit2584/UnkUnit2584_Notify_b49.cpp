// Unit 2584 (tools/tu_map.py; no reflected class): fn_7100f04434 updates the object (fn_7100f04520),
// then hands every listener in its list (data +0x8, count +0x10) a pair built from +0x30 and the word at
// +0x38 (fn_7100f035dc, in the CHearingSensor unit). Names and layouts are placeholders.
#include "types.h"

struct UnkHearingListener_b49;

struct UnkHearingEvent_b49 {
    u64 field_0x0;
    u64 field_0x8;
};

struct UnkHearingSource_b49 {
    u8 pad_0x0[0x8];
    UnkHearingListener_b49** listeners;
    u32 count;
    u8 pad_0x14[0x1c];
    u64 field_0x30;
    u32 field_0x38;
};

void fn_7100f04520(UnkHearingSource_b49* self);
void fn_7100f035dc(UnkHearingListener_b49* listener, UnkHearingEvent_b49* event);

void fn_7100f04434(UnkHearingSource_b49* self) {
    fn_7100f04520(self);
    if (u32 n = self->count) {
        UnkHearingListener_b49** it = self->listeners;
        UnkHearingListener_b49** end = it + n;
        do {
            UnkHearingListener_b49* l = *it++;
            UnkHearingEvent_b49 event;
            event.field_0x0 = self->field_0x30;
            event.field_0x8 = self->field_0x38;
            fn_7100f035dc(l, &event);
        } while (it != end);
    }
}
