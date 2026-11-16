// CAnimationComponent (tools/tu_map.py unit 2720). fn_7101178564: sets the bits of `id` in the word at +0x74
// and clears them from +0x78; when asked, folds +0x74 into +0x70 minus the bits of +0x78 and, if that
// changed it, lets fn_71011787dc refresh. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkShakernautId.h"
#include "types.h"

struct UnkShakernautCtrl_b53 {
    u8 pad_0x0[0x70];
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
};

struct UnkAnimTags_ovb0010;
void fn_71011787dc(UnkAnimTags_ovb0010* self, bool flag);

inline void SetBits(u32& word, const u32& bits) {
    word = word | bits;
}

inline void ClearBits(u32& word, const u32& bits) {
    word &= ~bits;
}

void fn_7101178564(UnkShakernautCtrl_b53* self, const UnkShakernautId_b53* id, bool a, bool b) {
    SetBits(self->field_0x74, id->value);
    ClearBits(self->field_0x78, id->value);
    if (!a)
        return;
    u32 old = self->field_0x70;
    u32 value = old;
    value |= self->field_0x74;
    value &= ~self->field_0x78;
    self->field_0x70 = value;
    if (value != old)
        fn_71011787dc(reinterpret_cast<UnkAnimTags_ovb0010*>(self), b);
}
