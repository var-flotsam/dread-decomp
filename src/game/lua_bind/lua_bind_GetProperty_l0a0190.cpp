// lua_bind's unit (unit 2675): the script binding that reads a property (second argument) of a section
// (first argument) of the blackboard at +0xeb8 of the game manager and pushes it as a bool, float, integer or string, by its type.
// Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU500.h"
#include "types.h"

#include "../../game_aca57c/UnkBufferString.h"

using base::global::CStrId;

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
    UnkStrIdHandle(const CStrId& id);  // fn_7100ae986c
};

// The blackboard at +0xeb8 of the game manager (include/game/UnkGameManager.h does not name it yet).
static inline CBlackboard* Blackboard_l0a0190(UnkGameManager* manager) {
    return reinterpret_cast<CBlackboard*>(reinterpret_cast<u8*>(manager) + 0xeb8);
}

int fn_71010a0190(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* section;
    const char* name;
    if (fn_71010c0270(L, "ss", &section, &name)) {
        CBlackboard* blackboard = Blackboard_l0a0190(manager);
        CStrId sectionId(section, true);
        CStrId nameId(name, true);
        UnkStrIdHandle handle(nameId);
        if (fn_71010474f4(blackboard, sectionId, handle.id)) {
            s32 type = fn_71010475d0(blackboard, sectionId, handle.id);
            if (type == 'b') {
                bool value = false;
                if (CBlackboard::CSection* found = fn_7101047270(blackboard, sectionId))
                    fn_710006d11c(found, &handle.id, &value);
                return fn_71010c0b40(L, "b", value);
            } else if (type == 'f') {
                f32 value = 0.0f;
                if (CBlackboard::CSection* found = fn_7101047270(blackboard, sectionId))
                    fn_7100395b34(found, &handle.id, &value);
                return fn_71010c0b40(L, "f", value);
            } else if (type == 'i') {
                s32 value = 0;
                if (CBlackboard::CSection* found = fn_7101047270(blackboard, sectionId))
                    fn_7100395a30(found, &handle.id, &value);
                return fn_71010c0b40(L, "i", value);
            } else if (type == 's') {
                UnkBufferString<0x100, g_unk_7101a194b8> value(g_unk_710162e2ed);
                if (CBlackboard::CSection* found = fn_7101047270(blackboard, sectionId))
                    fn_7100395c38(found, &handle.id, value.Str());
                const char* text = value.Str()->data;
                return fn_71010c0b40(L, "s", text ? text : g_unk_710162e2ed);
            }
        }
    }
    return 0;
}
