// UnkTU285 (tools/tu_map.py unit 285): sets a scene item's flag at 0xf9. When it changes, the item's
// target (+0x108) is moved to the item's translation when turned on, the owner is told (as
// fn_71001f3224 does), and the object at the target's +0x78 is enabled or notified. Names and layouts
// are placeholders.
#include "engine/scene/UnkToggle.h"
#include "game/UnkTU211.h"
#include "game/UnkTU248.h"
#include "game/UnkTU285.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkXformHolder_0x84;
struct UnkUnit211Object;

// The item's target as this function uses it: a transform holder whose +0x78 is another object.
struct UnkItemTarget_l208bb4 {
    u8 pad_0x0[0x78];
    UnkUnit211Object* field_0x78;
};

struct UnkItem_l208bb4 : UnkToggle {
    u32 field_0x110;
    u8 pad_0x114[0x18];
    u32 field_0x12c;
    u8 pad_0x130[0x4];
    u8 field_0x134;

    UnkItemTarget_l208bb4* Target() { return reinterpret_cast<UnkItemTarget_l208bb4*>(src_0x108); }
};
static_assert(__builtin_offsetof(UnkItem_l208bb4, field_0x110) == 0x110, "layout");
static_assert(__builtin_offsetof(UnkItem_l208bb4, field_0x134) == 0x134, "layout");

void fn_7100208bb4(void* obj, bool flag) {
    UnkItem_l208bb4* self = static_cast<UnkItem_l208bb4*>(obj);
    if (flag != self->flag_0xf9) {
        self->field_0x110 = 0;
        if (flag) {
            self->field_0x134 = 1;
            UnkXformHolder_0x84* target = reinterpret_cast<UnkXformHolder_0x84*>(self->Target());
            UnkVec3f t = reinterpret_cast<UnkXformNode*>(self)->Xform()->GetTranslation();
            fn_71001bc740(target, &t);
        }
        self->flag_0xf9 = flag;
        if (self->listener_0x8) {
            fn_71001eef58(self->listener_0x8, self, self->flag_0xf8 && flag);
            fn_71001eef74(self->listener_0x8, self, self->flag_0xf9 && self->flag_0xf6);
            if (flag)
                fn_7100192f08(self->Target()->field_0x78, true);
        }
        if (!self->flag_0xf9)
            fn_71001933cc(self->Target()->field_0x78);
    }
    self->field_0x12c = 0;
}
