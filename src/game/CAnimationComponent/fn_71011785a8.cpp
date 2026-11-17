// CAnimationComponent (tools/tu_map.py unit 2720). fn_71011785a8: clears the bits of `id` from the word at
// +0x74 and sets them in +0x78; when asked, folds +0x74 into +0x70 minus the bits of +0x78 and, if that
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

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71011785a8(UnkShakernautCtrl_b53* self, const UnkShakernautId_b53* id, bool a, bool b) {
    ClearBits(self->field_0x74, id->value);
    SetBits(self->field_0x78, id->value);
    if (!a)
        return;
    u32 old = self->field_0x70;
    u32 value = (self->field_0x74 | old) & ~self->field_0x78;
    self->field_0x70 = value;
    if (value != old)
        fn_71011787dc(reinterpret_cast<UnkAnimTags_ovb0010*>(self), b);
}
