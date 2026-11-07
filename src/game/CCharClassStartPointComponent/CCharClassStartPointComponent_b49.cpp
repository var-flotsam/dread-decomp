// CCharClassStartPointComponent: after the base step (fn_71010c41bc) succeeds, copies the byte at 0x58 of the
// object at 0x10 to 0x79 when that object is a CCharClassStartPointComponent (its class descriptor is the
// static the getter returns). Names are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;

// The base step; it reads the component, still in x0 (as the other sources declare it).
bool fn_71010c41bc();
extern char g_unk_7172a0a650[] __attribute__((visibility("hidden")));

// `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
bool fn_710116bb80(char* self, void* arg) {
    bool r = reinterpret_cast<bool (*)(char*, void*)>(fn_71010c41bc)(self, arg);
    if (r) {
        UnkReflObject* o = *reinterpret_cast<UnkReflObject**>(self + 0x10);
        const CType* t = o->vf_0x0();
        fn_710116aaa0();
        char* p = fn_710009d50c(t, reinterpret_cast<const CType*>(g_unk_7172a0a650)) ? reinterpret_cast<char*>(o) : nullptr;
        self[0x79] = p[0x58];
    }
    return r;
}
