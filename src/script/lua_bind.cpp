// Lua bindings (unit 2666 of tools/tu_map.py, 0x7101076e20-0x7101097e40, the lua_bind functions;
// their names come from the bindings' registration strings). Each one here reads one setting of a
// lazily created manager and pushes it with fn_71010c0b40, the unit's printf-style push ("b" for a
// bool, "s" for a string). Layouts are placeholders.
// Holds fn_7101078d20, fn_7101085e00, fn_7101087980, fn_710108da20, fn_710108eda0, fn_710108ee00,
// fn_71010945d0 and fn_7101094e20.
#include "../../lib/lua/lua.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// Pushes the values `fmt` names (one letter each) and returns how many it pushed; the variadic
// front of fn_71010bf400, which takes them as a va_list.
int fn_71010c0b40(lua_State* L, const char* fmt, ...);

// The managers, each a global pointer set on first use (allocated with fn_71000839a0(size, 0x3f),
// then constructed in place), and their constructors. The declarations repeat the other files'.
struct UnkUserDataOwner;  // src/game/CGameManager.cpp
extern UnkUserDataOwner* g_unk_7101d47308;
void fn_710120320c(UnkUserDataOwner* self);
extern char* g_unk_71726b2398;
void fn_71011a664c(char* instance);
extern void* g_unk_7172758b20;
void fn_7101192628(void* self);
extern void* g_unk_7101d47368;
void fn_7100d8ced8(void* self);
extern void* g_unk_71726eb3e8;
void fn_7100e211cc(void* self);
extern void* g_unk_7101d47360;  // the sub-area manager (src/game/CSubAreaManager.cpp)
void fn_7100ee1f34(void* self);

namespace {

// The instance in `*global`, created on first use.
template <class T, u64 Size>
__attribute__((always_inline)) inline T* GetOrCreate(T*& global, void (*ctor)(T*)) {
    T* p = global;
    if (!p) {
        p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        ctor(p);
        global = p;
    }
    return p;
}

inline bool FlagAt(const void* p, u64 offset) {
    return *reinterpret_cast<const bool*>(static_cast<const u8*>(p) + offset);
}

// The sub-area manager's current sub-area: its first member is its name.
struct UnkSubAreaName {
    CStrId name;
};
struct UnkSubAreaManagerView {
    u8 pad_0x0[0x3f0];
    UnkSubAreaName* current;
};

}  // namespace

// lua_bind::IsSaveDataInDummyMode
int fn_7101078d20(lua_State* L) {
    UnkUserDataOwner* owner = GetOrCreate<UnkUserDataOwner, 0x2b8>(g_unk_7101d47308, fn_710120320c);
    return fn_71010c0b40(L, "b", FlagAt(owner, 0x2a8));
}

// lua_bind::GetControlledAI
int fn_7101085e00(lua_State* L) {
    UnkManager_71726af468* m = GetOrCreate<UnkManager_71726af468, 0x2610>(g_unk_71726af468, fn_7100da7e2c);
    return fn_71010c0b40(L, "b", FlagAt(m, 0x8));
}

// lua_bind::IsAttackLocked
int fn_7101087980(lua_State* L) {
    char* m = GetOrCreate<char, 0x108>(g_unk_71726b2398, fn_71011a664c);
    return fn_71010c0b40(L, "b", FlagAt(m, 0xf0));
}

// lua_bind::GetCaptionsAllowed
int fn_710108da20(lua_State* L) {
    void* m = GetOrCreate<void, 0x40>(g_unk_7172758b20, fn_7101192628);
    return fn_71010c0b40(L, "b", FlagAt(m, 0x38));
}

// lua_bind::IsEnvironmentSoundEnabled
int fn_710108eda0(lua_State* L) {
    void* m = GetOrCreate<void, 0x4c8>(g_unk_7101d47368, fn_7100d8ced8);
    return fn_71010c0b40(L, "b", FlagAt(m, 0x4a2));
}

// lua_bind::IsEnvironmentSoundEffectsEnabled
int fn_710108ee00(lua_State* L) {
    void* m = GetOrCreate<void, 0x4c8>(g_unk_7101d47368, fn_7100d8ced8);
    return fn_71010c0b40(L, "b", FlagAt(m, 0x4a3));
}

// lua_bind::IsDroppedItemsAttractionEnabled
int fn_71010945d0(lua_State* L) {
    void* m = GetOrCreate<void, 0xa0f8>(g_unk_71726eb3e8, fn_7100e211cc);
    return fn_71010c0b40(L, "b", FlagAt(m, 0x7888));
}

// lua_bind::GetCurrentSubAreaId: the current sub-area's name, or the empty one's.
int fn_7101094e20(lua_State* L) {
    auto* m = static_cast<UnkSubAreaManagerView*>(GetOrCreate<void, 0xae8>(g_unk_7101d47360, fn_7100ee1f34));
    const CStrId& id = m->current ? m->current->name : g_unk_7101d470e0;
    const CRntString& str = base::reflection::refl_RntStringOf(id);
    return fn_71010c0b40(L, "s", str.data ? str.data : "");
}
