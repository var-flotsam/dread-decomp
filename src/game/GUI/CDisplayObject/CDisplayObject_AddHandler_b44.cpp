// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): a reflected call with two
// arguments, each picked from its value (null without a type; inline, or behind a pointer for 9 or
// more bytes or kind 1), likely the object's side of lua_bind::AddLuaEventHandler: builds a script
// handler (0x40 bytes, fn_710109e8dc) from the Lua state and the second argument, wraps it in a
// handle (0x10 bytes, fn_7100b33f90), and adds the handle to the object's handler lists at +0xb8
// under the event kind named by the first argument (fn_7101202300, src/game/GUI/UnkGuiEvent.cpp;
// fn_7100b1e34c, src/game/UnkUnit1674/UnkUnit1674_Push.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU1686.h"
#include "game/UnkTU2668.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct lua_State;

struct UnkLuaEventHandler;
// The handler's constructor (src/game/GUI/UnkLuaEventHandler_b44.cpp).
void fn_710109e8dc(UnkLuaEventHandler* self, lua_State* state, const base::global::CRntString* name);
s32 fn_7101202300(const base::global::CRntString* name);  // an event's kind from its name
struct UnkUnit1674Item;
UnkUnit1674Item* fn_7100b1e34c(void* a, void* b, void* p, void* q, u32 value);

struct UnkGuiCallArgs_b44a3 {
    UnkReflValue first;
    UnkReflValue second;
};

struct UnkGuiCallOwner_b44a3 {
    u8 pad_0x0[8];
    char* target;
};

static inline __attribute__((always_inline)) void* PickArg_b44a3(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    if (v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8)
        return v->field_0x0;
    return v;
}

void fn_71011d3568(UnkGuiCallOwner_b44a3* owner, UnkGuiCallArgs_b44a3** args) {
    UnkGuiCallArgs_b44a3* a = *args;
    char* target = owner->target;
    void* first = PickArg_b44a3(&a->first);
    void* second = PickArg_b44a3(&a->second);
    void* built = fn_71000839a0(0x40, 0xc);
    fn_710109e8dc(static_cast<UnkLuaEventHandler*>(built), g_unk_7172a01d18,
                  static_cast<const base::global::CRntString*>(second));
    void* object = built;
    s32 kind = fn_7101202300(static_cast<const base::global::CRntString*>(first));
    void* handle = fn_71000839a0(0x10, 0xc);
    fn_7100b33f90(handle, &object);
    fn_7100b1e34c(target + 0xb8, &kind, handle, nullptr, 6);
}
