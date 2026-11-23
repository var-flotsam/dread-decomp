// Static initialiser: constructs two string-id globals in place (registering their destructors
// with atexit), then registers 1 class: "EExecutionReturnValue".
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/UnkTUInit.h"
#include "base/reflection/TypeObject.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

extern CStrId g_unk_7101d5bdb0[2] HIDDEN;
enum class EExecutionReturnValue : u32;
template <> u8 base::reflection::TypeObject<EExecutionReturnValue, u8[]>::object[] asm("g_unk_7101d5b720");
extern u8 g_unk_7101d5b838[];
void fn_71000a2184(void*);
void fn_71000a2270(void* obj, CStrId* name, void (*fn)(void*), void* a);
void fn_71000a4470() {
    new (&g_unk_7101d5bdb0[0]) CStrId("sources", true);
    fn_7100000250(CStrId_Destroy, &g_unk_7101d5bdb0[0], &g_unk_7101a19000);
    new (&g_unk_7101d5bdb0[1]) CStrId("binaries", true);
    fn_7100000250(CStrId_Destroy, &g_unk_7101d5bdb0[1], &g_unk_7101a19000);
    {
        CStrId name("EExecutionReturnValue", true);
        fn_71000a2270(base::reflection::TypeObject<EExecutionReturnValue, u8[]>::object, &name, fn_71000a2184, nullptr);
    }
    fn_7100000250(fn_7100099468, base::reflection::TypeObject<EExecutionReturnValue, u8[]>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d5b838, base::reflection::TypeObject<EExecutionReturnValue, u8[]>::object);
}
