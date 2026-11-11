// lua_lib unit (tools/tu_map.py): a reflected value visited with a small stack visitor (vtable 0x7101cab628, address point 0x7101cab638, unit-local)
// that carries the first argument and two words; its result word is returned. The typed pair's type is
// refined once through fn_710009dcd8 before fn_71000a1784 walks it. Names are placeholders.
#include "base/reflection.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "types.h"

struct UnkFormatArgs;
struct UnkTextBuffer200;

extern void* g_unk_7101cab638[] __attribute__((visibility("hidden")));

struct UnkTypedPair_A6_b72 {
    char* type;
    void* obj;
};

struct UnkLuaVisitor_A6_b72 {
    void** vtable;
    u32 kind;
    void* state;
    s32 arg0;
    s32 arg1;
    s32 result;
};

s32 fn_7100fbde70(void* state, UnkTypedPair_A6_b72* value, s32 arg0, s32 arg1) {
    UnkLuaVisitor_A6_b72 v;
    v.kind = 1;
    v.vtable = g_unk_7101cab638;
    v.state = state;
    v.arg0 = arg0;
    v.arg1 = arg1;
    v.result = 0;
    void* obj = value->obj;
    UnkTypedPair_A6_b72 args = {fn_710009dcd8(value->type, obj), obj};
    fn_71000a1784(reinterpret_cast<UnkFormatArgs*>(&args), reinterpret_cast<UnkTextBuffer200*>(&v));
    return v.result;
}
