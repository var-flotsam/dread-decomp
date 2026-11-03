// CMapAcquisitionComponent unit (tools/tu_map.py, name from reflection): fn_7100a86c40 keeps a weak
// handle (+0xe8) to the given target, then either runs fn_7100a86d00 (flag at +0x539) or one of two
// virtual steps chosen by vf_0x188 (vf_0x298 when it holds, vf_0x2a8 otherwise). Always returns
// false. Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

struct UnkMapAcquisition_b50a5;
void fn_7100a86d00(UnkMapAcquisition_b50a5* self);

struct UnkMapAcquisition_b50a5 {
    void** vtable;
    u8 unk_0x8[0xe0];
    UnkWeakHandle* handle;  // 0xe8
    u8 unk_0xf0[0x449];
    bool flag;  // 0x539

    template <class R>
    R CallSlot(u32 offset) {
        return reinterpret_cast<R (*)(UnkMapAcquisition_b50a5*)>(vtable[offset / 8])(this);
    }
};

bool fn_7100a86c40(UnkMapAcquisition_b50a5* self, UnkWeakTarget* target) {
    if (!target)
        return false;
    AssignRef(&self->handle, fn_7100d78378(target));
    if (self->flag) {
        fn_7100a86d00(self);
        return false;
    }
    if (self->CallSlot<bool>(0x188))
        self->CallSlot<void>(0x298);
    else
        self->CallSlot<void>(0x2a8);
    return false;
}
