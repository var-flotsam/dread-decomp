// Out-of-line initialisers of reflected functions (UnkReflFunction). A class's member-field
// function adds each function with fn_7100097550 and then initialises it, inline for getters
// and through one of these for setters.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"

using base::global::UnkStrId;
using base::reflection::CType;
using base::reflection::UnkReflFunction;

extern base::reflection::CType g_unk_7101d5af48;  // return type: probably void
void fn_7100a199a0();                            // the invoker (reached through the GOT)

// CEmmySpawnPointComponent's "AddPatrolShape": one CStrId parameter.
void fn_7100a19530(UnkReflFunction* fn, const char* name) {
    fn->name = UnkStrId(name, true);
    fn->return_type = &g_unk_7101d5af48;
    fn->field_0xb2 = 0;
    fn->field_0x40 = nullptr;
    fn->invoker = reinterpret_cast<void*>(&fn_7100a199a0);
    fn->params.PushBack(fn_7100009658());
}
