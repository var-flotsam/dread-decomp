// CCameraComponent's unit (tools/tu_map.py unit 2691): the unit's static initialiser. The header
// data, the "DefaultNear" and "DefaultFar" ids, four reflected types, then the named string-id
// value "vPos" copied into a one-value list. Names and layouts are placeholders.
#include <new>

#include "base/UnkErasedVector.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"
#include "types.h"

// The unit's statics: the header ids, the two camera names and the class object's storage.
struct UnkStatics_l0e7d30 {
    UnkTUHeader header;
    CStrId default_near;  // 0x18
    CStrId default_far;   // 0x20
    u8 pad_0x28[8];
    u8 field_0x30[8];
};
extern UnkStatics_l0e7d30 g_unk_7172a045d8 __attribute__((visibility("hidden")));
extern char g_unk_7172a04268[];
extern char g_unk_7172a04269[];
extern char g_unk_7172a0426a[];
extern char g_unk_7172a0426b[];
extern UnkValue g_unk_7172a04270;
extern UnkErasedVector g_unk_7172a04280;

const base::reflection::CType* fn_71010df6c0();
// The pointer types CCameraComponent* and the other the unit describes (generated getters).
const base::reflection::CType* fn_71010e17f4();
const base::reflection::CType* fn_71010e18d0();

struct UnkStruct_71729e0ab0;
namespace unk_static_init {
struct UnkNamedValueTmp;
}
using unk_static_init::UnkNamedValueTmp;
void fn_7100ad3404(UnkStruct_71729e0ab0*, UnkNamedValueTmp*, u64);

// A value's type as its copies see it: the hooks to copy and to tear down a value.
struct UnkValueOps_l0e7d30 {
    u8 pad_0x0[0x30];
    void (*copy)(void* value, const void* from);
    void (*init)(void* value, const void* src);
    void (*destroy)(void* value);
};

// A temporary copy of a value: its type is set only when the type can copy.
struct UnkValueCopy_l0e7d30 {
    u64 storage;
    UnkValueOps_l0e7d30* type;

    explicit UnkValueCopy_l0e7d30(const UnkValue& from) {
        type = nullptr;
        UnkValueOps_l0e7d30* t = reinterpret_cast<UnkValueOps_l0e7d30*>(from.type);
        if (t->copy) {
            type = t;
            t->copy(this, &from);
        }
    }
    ~UnkValueCopy_l0e7d30() {
        if (type) {
            type->destroy(this);
            type = nullptr;
        }
    }
};

void fn_71010e7d30() {
    InitHeader(g_unk_7172a045d8.header);
    new (&g_unk_7172a045d8.default_near) CStrId("DefaultNear", true);
    fn_7100000250(fn_710000051c, &g_unk_7172a045d8.default_near, &g_unk_7101a19000);
    new (&g_unk_7172a045d8.default_far) CStrId("DefaultFar", true);
    fn_7100000250(fn_710000051c, &g_unk_7172a045d8.default_far, &g_unk_7101a19000);

    fn_710009ec00(g_unk_7172a04268, const_cast<base::reflection::CType*>(fn_71010df6c0()));
    fn_71010df7a0();
    fn_710009ec00(g_unk_7172a04269, g_unk_7172a045d8.field_0x30);
    fn_710009ec00(g_unk_7172a0426a, const_cast<base::reflection::CType*>(fn_71010e17f4()));
    fn_710009ec00(g_unk_7172a0426b, const_cast<base::reflection::CType*>(fn_71010e18d0()));

    new (&g_unk_7172a04270) UnkValue(CStrId("vPos", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_7172a04270, &g_unk_7101a19000);
    {
        UnkValueCopy_l0e7d30 copy(g_unk_7172a04270);
        fn_7100ad3404(reinterpret_cast<UnkStruct_71729e0ab0*>(&g_unk_7172a04280),
                      reinterpret_cast<UnkNamedValueTmp*>(&copy), 1);
    }
    fn_7100000250(fn_7100d8ce3c, &g_unk_7172a04280, &g_unk_7101a19000);
}
