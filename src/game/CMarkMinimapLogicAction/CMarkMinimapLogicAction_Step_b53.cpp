// CMarkMinimapLogicAction unit (tools/tu_map.py, the name from reflection): the action's step. After the
// base step (fn_7100a55ac8) it resolves its two lazily resolved actor references (sources +0x18 and
// +0x58, states +0x40 and +0x80, handles +0x48 and +0x88; the shape of
// src/game/CTutoExitLogicAction/CTutoExitLogicAction_b47.cpp), and hands the +0x30 object of each
// referenced actor's tracked component to the world (game manager +0x2228): the first through
// fn_7100e950b4, the second through fn_7100e95038. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2566.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTutoExitAction_b47;

struct UnkRefl_fn_710028aca8;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710028aca8>(void*);

struct UnkMarkMinimapRef_b53 {
    void* vtable;
    u8 source[0x28];
    u32 state;
    u8 pad_0x34[4];
    base::UnkRefHandle handle;

    // The referenced actor's tracked component's +0x30 object, or null.
    __attribute__((always_inline)) void* Resolve() {
        if (state == 1) {
            {
                UnkRefPtr fresh = fn_710023b580(source);
                fn_7100234f78(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        if (base::UnkRefCounted* target = handle.ptr) {
            void* actor = *reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10);
            if (actor) {
                asm("" : "+r"(target));
                char* comp = reinterpret_cast<char*>(base::reflection::refl_FindTyped<UnkRefl_fn_710028aca8>(
                    *reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10)));
                if (comp)
                    return *reinterpret_cast<void**>(comp + 0x30);
            }
        }
        return nullptr;
    }
};

struct UnkMarkMinimapAction_b53 {
    void* vtable;
    u8 pad_0x8[0x8];
    UnkMarkMinimapRef_b53 first;   // 0x10
    UnkMarkMinimapRef_b53 second;  // 0x50
};

void fn_7100a3ef6c(UnkMarkMinimapAction_b53* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    if (void* object = self->first.Resolve())
        fn_7100e950b4(g_unk_7101d47310->field_0x2228, object);
    if (void* object = self->second.Resolve())
        fn_7100e95038(g_unk_7101d47310->field_0x2228, object);
}
