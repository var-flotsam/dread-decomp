// CBTObserverComponent's unit: hand every entry of the list at +0x90 whose flag at +0x14 is set to
// fn_7100ca3498; always returns false. Names are placeholders.
#include "types.h"

struct UnkObserverEntry_A6_b68 {
    u8 unk_0x0[0x14];
    bool pending;
};

void fn_7100ca3498(UnkObserverEntry_A6_b68* entry);

struct UnkObserverList_A6_b68 {
    u8 unk_0x0[0x90];
    UnkObserverEntry_A6_b68** data;
    u8 unk_0x98[0x8];
    u32 count;
};

bool fn_7100caad50(UnkObserverList_A6_b68* self) {
    for (UnkObserverEntry_A6_b68** it = self->data, **end = self->data + self->count; it != end; ++it) {
        if ((*it)->pending)
            fn_7100ca3498(*it);
    }
    return false;
}
