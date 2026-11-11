// CLuaCallsLogicAction unit (tools/tu_map.py, the name from reflection): the action's step. After the base
// step (fn_7100a55ac8), when the function name (+0x48) isn't empty: with the flag at +0x10 set it looks up
// the object named by +0x18 (its hash with the kind g_unk_71727bebb8) in the game manager and calls the
// function on that object's script component with ("oob": the action's entity, the object, the flag at
// +0x78) when the component has a script; otherwise it calls the function on the entity's script
// (+0x1e0) with ("oo": the entity, `arg`), or logs it when the entity has none. Names and layouts are
// placeholders.
#include "base/global/CRntString.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CLuaCallsLogicAction.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;

struct UnkTutoExitAction_b47;

u64* fn_7100066630(u64* out, const CRntString* str);
struct UnkScriptCaller;
bool fn_71009cde54(void* script, const char* name, const char* fmt, ...);

struct UnkLuaCallsEntity_b53 {
    u8 pad_0x0[0x1e0];
    void* script;
};

struct UnkLuaCallsScript_b53 {
    u8 pad_0x0[0x98];
    const char* name;
};

struct UnkLuaCallsAction_b53 {
    void** vtable;
    UnkLuaCallsEntity_b53* entity;  // 0x8
    bool on_object;                 // 0x10
    u8 pad_0x11[7];
    CRntString object;    // 0x18
    CRntString function;  // 0x48
    bool flag;            // 0x78
};

void fn_7100a50598(UnkLuaCallsAction_b53* self, void* arg) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    const char* function = self->function.data;
    if (!function || !*function)
        return;
    if (self->on_object) {
        UnkGameManager* manager = g_unk_7101d47310;
        asm("" : "+r"(manager));  // the manager is read before the kind
        u64 hash;
        u32 kind_value = g_unk_71727bebb8;
        fn_7100066630(&hash, &self->object);
        alignas(8) u32 kind = kind_value;
        void* object = fn_7100deba8c(manager, &hash, &kind);
        if (!object)
            return;
        auto* script = static_cast<UnkLuaCallsScript_b53*>(fn_7100a506a8(object));
        if (!script || !script->name || !*script->name)
            return;
        fn_7100305088(reinterpret_cast<UnkScriptCaller*>(script), self->function.data, "oob", self->entity, object,
                      self->flag);
    } else {
        UnkLuaCallsEntity_b53* entity = self->entity;
        if (entity->script)
            fn_71009cde54(entity->script, function, "oo", entity, arg);
        else
            fn_7100000790(function, "");
    }
}
