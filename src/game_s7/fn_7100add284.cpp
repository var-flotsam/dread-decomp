// Reflection functions of CTunableMenuHelperClass. Written from the refl_attrs draft; the class's
// own getter, used as a return type, lives in this translation unit.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflField;
using base::reflection::UnkReflFunction;
using base::reflection::UnkTunable;
using base::reflection::UnkTunableRegistry;
extern const base::reflection::CType g_unk_71727586d0 __attribute__((visibility("hidden")));
void fn_7100adeaf4();
void fn_7100add688(UnkReflFunction* fn, const char* name);
extern base::reflection::CType g_unk_7101d5af48;
void fn_7100adec94();
void fn_7100500260(UnkReflFunction* fn);

void fn_7100add284(CClass* cls) {
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("AddElement", true))) {
        fn->name = base::global::UnkStrId("AddElement", true);
        fn->return_type = (fn_7100add190(), &g_unk_71727586d0);
        fn->field_0xb2 = 0x1;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_7100adeaf4);
    }
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("SetKey", true)))
        fn_7100add688(fn, "SetKey");
    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId("SetInfo", true))) {
        fn->name = base::global::UnkStrId("SetInfo", true);
        fn->return_type = &g_unk_7101d5af48;
        fn->field_0xb2 = 0;
        fn->field_0x40 = nullptr;
        fn->invoker = reinterpret_cast<void*>(&fn_7100adec94);
        fn_7100500260(fn);
    }
}
