// Unit 2670: a script call helper that formats its arguments and calls into the state (fn_710109e620). Names and layouts are placeholders.
#include <stdarg.h>

#include "types.h"

struct lua_State;
s32 fn_71010bf400(lua_State* L, const char* fmt, va_list args);
struct UnkPropBag;
s32 fn_71010c0cd0(lua_State* L, s32 nargs, char* err, u32 errSize, UnkPropBag* out);

struct UnkScriptSinkVtable_b0233 {
    u8 pad_0x0[0x10];
    void (*vf_0x10)(struct UnkScriptSink_b0233* self);
};

struct UnkScriptSink_b0233 {
    UnkScriptSinkVtable_b0233* vtable;
    lua_State* state;  // 0x8
};

bool fn_710109e620(UnkScriptSink_b0233* self, void* out, const char* format, ...) {
    self->vtable->vf_0x10(self);
    va_list args;
    va_start(args, format);
    s32 nargs = fn_71010bf400(self->state, format, args);
    char buffer[256];
    bool failed = fn_71010c0cd0(self->state, nargs, buffer, sizeof(buffer), static_cast<UnkPropBag*>(out)) != 0;
    va_end(args);
    return failed;
}
