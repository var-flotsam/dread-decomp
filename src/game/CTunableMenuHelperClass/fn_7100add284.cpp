// Reflection functions of CTunableMenuHelperClass. Written from the refl_attrs draft; the class's
// own getter, used as a return type, lives in this translation unit.
#include "base/reflection/CType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CVariable;
using base::reflection::CFunction;
using base::reflection::UnkTunable;
using base::reflection::UnkTunableRegistry;
#include "base/reflection/TypeObject.h"
template <> base::reflection::CType base::reflection::TypeObject<void>::object asm("g_unk_7101d5af48");
extern const base::reflection::CType g_unk_71727586d0 __attribute__((visibility("hidden")));
void fn_7100adeaf4();
void fn_7100add688(CFunction* fn, const char* name);
base::reflection::UnkReflValue fn_7100adec94(void*, char* obj, base::reflection::UnkReflValue* const* args);
void fn_7100500260(CFunction* fn);

void fn_7100add284(CClass* cls) {
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("AddElement", true))) {
        fn->name = base::global::UnkStrId("AddElement", true);
        fn->return_type = (fn_7100add190(), &g_unk_71727586d0);
        fn->field_0xb2 = 0x1;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_7100adeaf4);
    }
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("SetKey", true)))
        fn_7100add688(fn, "SetKey");
    if (CFunction* fn = AddReflectedFunction(cls, CStrId("SetInfo", true))) {
        fn->name = base::global::UnkStrId("SetInfo", true);
        fn->return_type = &base::reflection::TypeObject<void>::object;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_7100adec94);
        fn_7100500260(fn);
    }
}
