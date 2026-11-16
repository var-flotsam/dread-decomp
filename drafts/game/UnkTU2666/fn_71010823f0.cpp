// Unit 2666 Lua binding: reads the blackboard property named by (section, key) and pushes it by its
// type letter (integer, bool or float); pushes nothing when it is missing. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU500.h"
#include "types.h"

using base::global::CStrId;

struct lua_State;

struct UnkStrIdHandle {
    union {
        CStrId id;
    };
    void* field_0x8;
    UnkStrIdHandle() {}
    ~UnkStrIdHandle() { id.~CStrId(); }
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);
s32 fn_71010475d0(CBlackboard* self, const CStrId& key, const CStrId& name);
bool fn_7100a68fb0(CBlackboard::CSection* section, const void* name, u32* out);

s32 fn_71010823f0(lua_State* L) {
    const char* key_name;
    const char* section_name;
    if (fn_71010c0270(L, "ss", &section_name, &key_name)) {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        UnkStrIdHandle key;
        {
            CStrId id(key_name, true);
            fn_7100ae986c(&key, id);
        }
        CStrId section_id(section_name, true);
        if (fn_71010474f4(blackboard, section_id, key.id)) {
            switch (fn_71010475d0(blackboard, section_id, key.id)) {
            case 'i':
            case 'u': {
                u32 value = 0xffffffff;
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                    fn_7100a68fb0(s, &key, &value);
                return fn_71010c0b40(L, "i", value);
            }
            case 'b': {
                bool value = false;
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                    fn_710006d11c(s, &key.id, &value);
                return fn_71010c0b40(L, "b", value);
            }
            case 'f': {
                f32 value = -1.0f;
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                    fn_7100395b34(s, &key, &value);
                return fn_71010c0b40(L, "f", (f64)value);
            }
            }
        } else {
            return 0;
        }
    }
    return 0;
}
