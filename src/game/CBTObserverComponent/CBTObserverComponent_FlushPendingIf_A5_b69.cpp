// CBTObserverComponent's unit: when the argument's words at +0x0 and +0x10 are equal, hand every entry of
// the list at +0x90 whose flag at +0x14 is set to fn_7100ca3498 (as fn_7100caad50 does); always
// returns false. Names are placeholders.
#include "game/CBreakableTileGroup.h"
#include "types.h"

struct UnkObserverEntry_A6_b68;

struct UnkObserverEntry_A5_b69 {
    u8 unk_0x0[0x14];
    bool pending;
};

struct UnkObserverList_A5_b69 {
    u8 unk_0x0[0x90];
    UnkObserverEntry_A5_b69** data;
    u8 unk_0x98[0x8];
    u32 count;
};

struct UnkObserverArg_A5_b69 {
    void* field_0x0;
    u8 unk_0x8[0x8];
    void* field_0x10;
};

bool fn_7100caadb0(UnkObserverList_A5_b69* self, UnkObserverArg_A5_b69* arg) {
    if (arg->field_0x10 == arg->field_0x0) {
        for (UnkObserverEntry_A5_b69 **it = self->data, **end = self->data + self->count; it != end; ++it) {
            if ((*it)->pending)
                fn_7100ca3498(reinterpret_cast<UnkObserverEntry_A6_b68*>(*it));
        }
    }
    return false;
}
