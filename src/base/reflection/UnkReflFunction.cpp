// Out-of-line initialisers of reflected functions (CFunction). A class's member-field
// function adds each function with fn_7100097550 and then initialises it, inline for getters
// and through one of these for setters.
#include "base/reflection/CClass.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkReflAttributes.h"

using base::global::UnkStrId;
using base::reflection::CType;
using base::reflection::CFunction;
#include "base/reflection/TypeObject.h"
template <> base::reflection::CType base::reflection::TypeObject<void>::object asm("g_unk_7101d5af48");

void fn_7100a199a0();                            // the invoker (reached through the GOT)

// CEmmySpawnPointComponent's "AddPatrolShape": one CStrId parameter.
void fn_7100a19530(CFunction* fn, const char* name) {
    fn->name = UnkStrId(name, true);
    fn->return_type = &base::reflection::TypeObject<void>::object;
    fn->field_0xb2 = 0;
    fn->field_0x40 = nullptr;
    fn->invoker = reinterpret_cast<void*>(&fn_7100a199a0);
    fn->params.PushBack(fn_7100009658());
}
