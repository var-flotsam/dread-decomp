// lua_bind's unit: the script binding that stores a value (fourth argument) under a property (second
// argument) of a section (first argument) of the game blackboard, as the type the third argument names
// ("b", "f", "i" or "s"), unless the property already has another type. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "../../../lib/lua/lauxlib.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkStrIdHandle.h"
#include "game/UnkTU2676.h"
#include "game/UnkTypedValue.h"
#include "types.h"

#include "../../game_aca57c/UnkBufferString.h"

using base::global::CStrId;

struct UnkEnumType;
UnkEnumType* fn_710009ffd4();  // base::global::TRntString256's type

// The length of an id's text (the id's string is a CRntString from its text on).
static inline u32 TextLength(const CStrId& id) {
    return reinterpret_cast<const base::global::CRntString*>(&id.data->text)->length;
}

// Stores `value` under `name` in the section `section` names, unless the blackboard is locked.
template <typename T>
static inline void SetSectionValue(CBlackboard* blackboard, const CStrId& section, UnkStrIdHandle& name, T value) {
    if (CBlackboard::CSection* found = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = base::reflection::UnkReflTypeOf<T>::Get();
            v.value = &value;
            fn_7100332c80(found, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}

// The same for a string, copied into a string with inline storage only when it is stored.
static inline void SetSectionString(CBlackboard* blackboard, const CStrId& section, UnkStrIdHandle& name,
                                    const char* text) {
    if (CBlackboard::CSection* found = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            UnkBufferString<0x100, g_unk_7101a194b8> value(text);
            v.type = reinterpret_cast<const base::reflection::CType*>(fn_710009ffd4());
            v.value = &value;
            fn_7100332c80(found, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}

namespace lua_bind {
int SetProp(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* section = luaL_checkstring(L, 1);
    const char* name = luaL_checkstring(L, 2);
    const char* type = luaL_checkstring(L, 3);
    CStrId sectionId(section, true);
    CStrId nameId(name, true);
    CStrId typeId(type, true);
    UnkStrIdHandle handle(nameId);
    if (TextLength(typeId) == 1) {
        CBlackboard* blackboard = &manager->field_0x968;
        bool compatible = true;
        if (fn_71010474f4(blackboard, sectionId, handle.id)) {
            s32 current = fn_71010475d0(blackboard, sectionId, handle.id);
            compatible = current == 'n' || type[0] == static_cast<char>(current);
        }
        if (compatible) {
            switch (type[0]) {
            case 'b':
                SetSectionValue(blackboard, sectionId, handle, fn_71010a4900(L, 4) != 0);
                break;
            case 'f':
                SetSectionValue(blackboard, sectionId, handle, static_cast<f32>(luaL_checknumber(L, 4)));
                break;
            case 'i':
                SetSectionValue(blackboard, sectionId, handle, static_cast<s32>(luaL_checkinteger(L, 4)));
                break;
            case 's':
                SetSectionString(blackboard, sectionId, handle, luaL_checkstring(L, 4));
                break;
            }
        }
    }
    return 0;
}
}  // namespace lua_bind
