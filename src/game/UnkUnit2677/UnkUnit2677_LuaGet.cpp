// From unit 2677 of tools/tu_map.py (0x71010bee20-0x71010c0fec, no reflected class): two Lua argument getters
// that look up a userdata box (fn_7100fbc470, fn_71010758e0) and hand out the object it holds (+0x10).
// Names and layouts are placeholders.
#include "types.h"

char* fn_7100fbc470(char* p0, u32 p1);
char* fn_71010758e0(char* p0, u32 p1);

struct UnkLuaBoxedObject {
    u8 pad_0x0[0x10];
    void* object;
};

template <char* (*Get)(char*, u32)>
static inline bool GetBoxed(char* state, void** out, u32 index) {
    UnkLuaBoxedObject** box = (UnkLuaBoxedObject**)Get(state, index);
    if (!box)
        return false;
    UnkLuaBoxedObject* p = *box;
    *out = p ? p->object : nullptr;
    return true;
}

bool fn_71010beee0(char* state, void** out, u32 index) { return GetBoxed<fn_7100fbc470>(state, out, index); }
bool fn_71010bef20(char* state, void** out, u32 index) { return GetBoxed<fn_71010758e0>(state, out, index); }
