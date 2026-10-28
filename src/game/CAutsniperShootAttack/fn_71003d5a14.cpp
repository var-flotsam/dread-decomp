// Applies the pending mode at 0x54 (unless the state at 0x50 is 4 to 6 and `force` is clear):
// mode 2 sends a message through the object at 0x220 of the owner, any other mode hands the
// vector at 0x7c to fn_71003d5450.
#include "types.h"

struct UnkVec3 {
    u32 x, y, z;
};
struct UnkOwner {
    u8 unk_0x0[0x220];
    void* field_0x220;
};
struct UnkSelf {
    u8 unk_0x0[8];
    UnkOwner* owner;
    u8 unk_0x10[0x40];
    u32 state;
    u32 mode;
    u8 unk_0x58[0x24];
    UnkVec3 field_0x7c;
};

extern char g_unk_71726bf850[] __attribute__((visibility("hidden")));
void fn_710117abfc(void* target, u32 a1, void* a2, u64 id);
void fn_71003d5450(UnkSelf* self, const UnkVec3* v, u32 a2);

// NON_MATCHING(minor): the state range test is `cmp #3; b.hs` in the original, `cmp #2; b.hi` here.
void fn_71003d5a14(UnkSelf* self, bool force) {
    UnkVec3 v;
    switch (self->state) {
    case 4:
    case 5:
    case 6:
        if (!force)
            return;
        break;
    }
    u32 mode = self->mode;
    if (!mode)
        return;
    self->mode = 0;
    if (mode == 2) {
        fn_710117abfc(self->owner->field_0x220, 0, g_unk_71726bf850, 0xf4061ff25a2163d6);
        return;
    }
    v.x = self->field_0x7c.x;
    v.y = self->field_0x7c.y;
    v.z = self->field_0x7c.z;
    fn_71003d5450(self, &v, 0);
}
