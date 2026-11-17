// lua_bind's unit (unit 2674), fn_710109f610: a script binding that reads the property named by the
// second argument in the section named by the first, from the game manager's blackboard at +0x820,
// and returns it to the script as a bool, number, integer or string (nothing when it is missing).
// Names and layouts are placeholders.
#include "base/UnkTunableLine.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU500.h"
#include "types.h"

using base::global::CStrId;

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
    UnkStrIdHandle(const CStrId& id);  // fn_7100ae986c
};

s32 fn_71010475d0(CBlackboard* self, const CStrId& key, const CStrId& name);

int fn_710109f610(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* section;
    const char* name;
    if (fn_71010c0270(L, "ss", &section, &name)) {
        CBlackboard* blackboard = &manager->field_0x820;
        CStrId section_id(section, true);
        CStrId name_id(name, true);
        UnkStrIdHandle handle(name_id);
        if (fn_71010474f4(blackboard, section_id, handle.id)) {
            switch (fn_71010475d0(blackboard, section_id, handle.id)) {
            case 's': {
                UnkTunableLine value(g_unk_710162e2ed);
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                    fn_7100395c38(s, &handle.id, value.Str());
                return fn_71010c0b40(L, "s", value.CStr());
            }
            case 'i': {
                s32 value = 0;
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                    fn_7100395a30(s, &handle.id, &value);
                return fn_71010c0b40(L, "i", value);
            }
            case 'f': {
                f32 value = 0.0f;
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                    fn_7100395b34(s, &handle.id, &value);
                return fn_71010c0b40(L, "f", value);
            }
            case 'b': {
                bool value = false;
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                    fn_710006d11c(s, &handle.id, &value);
                return fn_71010c0b40(L, "b", value);
            }
            }
        }
    }
    return 0;
}
