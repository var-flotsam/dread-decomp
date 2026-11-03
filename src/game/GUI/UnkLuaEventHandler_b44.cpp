// Unit 2672 of tools/tu_map.py (no reflected class): a script event handler, as fn_71011d3568
// (CDisplayObject_AddHandler_b44.cpp) builds one: its constructor (its vtable, the Lua state and a
// copy of the handler's name) and its call (the script function of that name, when there is one, with
// the arguments `fmt` describes; whether the call succeeded). Names and layouts are placeholders.
#include <cstdarg>
#include <new>

#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

struct lua_State;
struct UnkPropBag;
s32 fn_71010c0bb0(lua_State* L, const char* str);
s32 fn_71010bf400(lua_State* L, const char* fmt, va_list args);
s32 fn_71010c0cd0(lua_State* L, s32 nargs, char* error, u32 size, UnkPropBag* results);
extern char g_unk_7101caf6e8[];  // the handler's vtable

struct UnkLuaEventHandler {
    char* vtable;
    lua_State* state;
    CRntString name;
};

void fn_710109e8dc(UnkLuaEventHandler* self, lua_State* state, const CRntString* name) {
    self->vtable = g_unk_7101caf6e8 + 0x10;
    self->state = state;
    new (&self->name) CRntString(*name);
}

bool fn_710109e978(UnkLuaEventHandler* self, UnkPropBag* results, const char* fmt, ...) {
    if (!fn_71010c0bb0(self->state, self->name.data))
        return false;
    va_list args;
    va_start(args, fmt);
    s32 n = fn_71010bf400(self->state, fmt, args);
    char error[0x100];
    bool ok = fn_71010c0cd0(self->state, n, error, 0x100, results) != 0;
    va_end(args);
    return ok;
}
