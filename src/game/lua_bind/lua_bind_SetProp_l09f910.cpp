// lua_bind's unit (unit 2674), fn_710109f910: a script binding that writes the fourth argument to the
// property named by the second argument in the section named by the first, in the game manager's
// blackboard at +0x820. The third argument is the type ("b", "f", "i" or "s"); it must agree with the
// property's existing type, if any, and nothing is written while the blackboard is locked.
// Names and layouts are placeholders.
#include <string.h>

#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "base/UnkTunableLine.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2676.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
    UnkStrIdHandle(const CStrId& id);  // fn_7100ae986c
};

struct UnkEnumType;
UnkEnumType* fn_710009ffd4();  // base::global::TRntString256's type

s32 fn_71010475d0(CBlackboard* self, const CStrId& key, const CStrId& name);

// Writes the typed value under `name` in `section`.
static inline void Write(CBlackboard::CSection* section, UnkStrIdHandle* name, const base::reflection::CType* type,
                         void* value) {
    UnkTypedValue typed = {type, value};
    fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(name), &typed);
}

// Writes `value` under `name` in the section `section_id` names (created if needed), unless the
// blackboard is locked.
template <class T>
static inline void SetProperty(CBlackboard* blackboard, const CStrId& section_id, UnkStrIdHandle* name, T value) {
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        if (!blackboard->field_0xe8)
            Write(section, name, base::reflection::UnkReflTypeOf<T>::Get(), &value);
    }
}

// The same for a string, kept in a string with inline storage while it is written.
static inline void SetProperty(CBlackboard* blackboard, const CStrId& section_id, UnkStrIdHandle* name,
                               const char* text) {
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        if (!blackboard->field_0xe8) {
            UnkTunableLine value(text);
            Write(section, name, reinterpret_cast<const base::reflection::CType*>(fn_710009ffd4()), &value);
        }
    }
}

int fn_710109f910(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* section = luaL_checkstring(L, 1);
    const char* name = luaL_checkstring(L, 2);
    const char* type = luaL_checkstring(L, 3);
    CStrId section_id(section, true);
    CStrId name_id(name, true);
    UnkStrIdHandle handle(name_id);
    if (strlen(type) != 1)
        return 0;
    CBlackboard* blackboard = &manager->field_0x820;
    if (fn_71010474f4(blackboard, section_id, handle.id)) {
        s32 existing = fn_71010475d0(blackboard, section_id, handle.id);
        if (existing != 'n' && type[0] != static_cast<char>(existing))
            return 0;
    }
    switch (type[0]) {
    case 'b':
        SetProperty(blackboard, section_id, &handle, fn_71010a4900(L, 4) != 0);
        break;
    case 'f':
        SetProperty(blackboard, section_id, &handle, static_cast<f32>(luaL_checknumber(L, 4)));
        break;
    case 'i':
        SetProperty(blackboard, section_id, &handle, static_cast<s32>(luaL_checkinteger(L, 4)));
        break;
    case 's':
        SetProperty(blackboard, section_id, &handle, luaL_checkstring(L, 4));
        break;
    }
    return 0;
}
