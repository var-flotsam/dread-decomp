// CCharClassRodomithonXAIComponent's unit (tools/tu_map.py unit 987): state codes that depend on whether
// the object at +0x60 of the fourth argument has the kind held in g_unk_71726fe718. Names are placeholders.
#include "types.h"

u32 fn_710066fbc0(void* self, void* arg, struct UnkActor991* actor);

extern u64 g_unk_71726fe718 __attribute__((visibility("hidden")));

struct UnkKindObj_b49a3 {
    u8 pad_0x0[0x20];
    u64 kind;  // 0x20
};

struct UnkKindHolder_b49a3 {
    u8 pad_0x0[0x60];
    UnkKindObj_b49a3* obj;  // 0x60
};

u32 fn_7100666a90(void* self, void* arg, UnkActor991* actor, UnkKindHolder_b49a3* holder) {
    u32 state = fn_710066fbc0(self, arg, actor);
    UnkKindObj_b49a3* obj = holder->obj;
    if (obj != nullptr && obj->kind == g_unk_71726fe718)
        state = 7;
    return state;
}

u32 fn_7100666ad0(void* self, void* arg, void* actor, UnkKindHolder_b49a3* holder) {
    UnkKindObj_b49a3* obj = holder->obj;
    if (obj == nullptr)
        return 1;
    return obj->kind == g_unk_71726fe718 ? 4 : 1;
}
