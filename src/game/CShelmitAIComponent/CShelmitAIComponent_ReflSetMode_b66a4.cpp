// CShelmitAIComponent's unit (tools/tu_map.py): a reflected function's invoker. Stores its u32
// argument at +0x14780 of the component (the object at +8), then on the owner's (+0x30) component at
// +0x228 sets one of two flags and clears the other (fn_7101172764, fn_7101172780), by whether the
// value is non-zero; returns an empty value. Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

// As src/game/CGooplotAIComponent/CGooplotAIComponent_Reset_b49.cpp declares them.
struct UnkFlagTarget;
void fn_7101172780(UnkFlagTarget* target, const char* name);
void fn_7101172764(UnkFlagTarget* target, const char* name);
extern char g_unk_710164a738[];
extern char g_unk_710164a730[];

struct UnkShelmitOwner_b66a4 {
    u8 pad_0x0[0x228];
    UnkFlagTarget* flags;  // 0x228
};

struct UnkShelmitAI_b66a4 {
    u8 pad_0x0[0x30];
    UnkShelmitOwner_b66a4* owner;  // 0x30
    u8 pad_0x38[0x14780 - 0x38];
    u32 mode;  // 0x14780
};

static inline __attribute__((always_inline)) void* ArgPtr_b66a4(UnkReflValue* arg) {
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

UnkReflValue fn_71006bcbcc(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    UnkShelmitAI_b66a4* self = *reinterpret_cast<UnkShelmitAI_b66a4**>(obj + 8);
    u32 mode = *static_cast<const u32*>(ArgPtr_b66a4(a));
    self->mode = mode;
    if (self->owner && self->owner->flags) {
        if (mode) {
            fn_7101172764(self->owner->flags, g_unk_710164a738);
            fn_7101172780(self->owner->flags, g_unk_710164a730);
        } else {
            fn_7101172764(self->owner->flags, g_unk_710164a730);
            fn_7101172780(self->owner->flags, g_unk_710164a738);
        }
    }
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}
