// CAIComponent unit (unit 2702 of tools/tu_map.py): on a change of the flag at 0x14510, stores it with a value
// (0x14514) and calls slot 0x2e8 (set) or 0x2f0 (cleared). Names and layouts are placeholders.
#include "types.h"

struct UnkAIModeState {
    bool flag;
    u8 pad_0x1[0x3];
    u32 value;
};

struct UnkAIModeOwner {
    virtual void vf_0x0();
    void OnSet() { ((void (**)(UnkAIModeOwner*))*(void***)this)[0x2e8 / 8](this); }
    void OnClear() { ((void (**)(UnkAIModeOwner*))*(void***)this)[0x2f0 / 8](this); }
};

void fn_71010ff50c(UnkAIModeOwner* self, bool flag, u32 value) {
    UnkAIModeState* state = (UnkAIModeState*)((char*)self + 0x14510);
    if (flag == state->flag)
        return;
    state->flag = flag;
    state->value = value;
    if (flag)
        self->OnSet();
    else
        self->OnClear();
}
