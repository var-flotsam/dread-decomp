// Unit 2525 (base::reflection): the unit's static initialiser. The header data, the
// CEnvironmentManager class type and its pointer type, then three named string-id values
// ("pVisualPresets", "pSoundPresets", "pMusicPresets"), each copied into a one-value list.
// Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "game/UnkTUInit.h"
#include "types.h"

namespace base::reflection {
class CType;
}

extern UnkTUHeaderFnsClass g_unk_71727b8ce8 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_71727b87d0;  // CEnvironmentManager
extern char g_unk_71727b8908[];
extern char g_unk_71727b8909[];
extern UnkValue g_unk_71727b8910;
extern UnkValue g_unk_71727b8920;
extern UnkValue g_unk_71727b8930;
extern UnkErasedVector g_unk_71727b8940;
extern UnkErasedVector g_unk_71727b8968;
extern UnkErasedVector g_unk_71727b8990;

void fn_7100d8ca4c(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(),
                   void* b);
void fn_7100d8c56c();
struct UnkStruct_71729e0ab0;
namespace unk_static_init {
struct UnkNamedValueTmp;
}
using unk_static_init::UnkNamedValueTmp;
void fn_7100ad3404(UnkStruct_71729e0ab0*, UnkNamedValueTmp*, u64);
void fn_7100d8ce3c(void* self);

// A value's type as its copies see it: the hooks to copy and to tear down a value.
struct UnkValueOps_d90cd0 {
    u8 pad_0x0[0x30];
    void (*copy)(void* value, const void* from);
    void (*init)(void* value, const void* src);
    void (*destroy)(void* value);
};

// A temporary copy of a value: its type is set only when the type can copy.
struct UnkValueCopy_d90cd0 {
    u64 storage;
    UnkValueOps_d90cd0* type;

    explicit UnkValueCopy_d90cd0(const UnkValue& from) {
        type = nullptr;
        UnkValueOps_d90cd0* t = reinterpret_cast<UnkValueOps_d90cd0*>(from.type);
        if (t->copy) {
            type = t;
            t->copy(this, &from);
        }
    }
    ~UnkValueCopy_d90cd0() {
        if (type) {
            type->destroy(this);
            type = nullptr;
        }
    }
};

// A list of the one value `from` (destroyed at exit).
inline void InitValueList_d90cd0(UnkErasedVector* list, const UnkValue& from) {
    {
        UnkValueCopy_d90cd0 copy(from);
        fn_7100ad3404(reinterpret_cast<UnkStruct_71729e0ab0*>(list), reinterpret_cast<UnkNamedValueTmp*>(&copy), 1);
    }
    fn_7100000250(fn_7100d8ce3c, list, &g_unk_7101a19000);
}

void fn_7100d90cd0() {
    InitHeaderFns(g_unk_71727b8ce8);
    InitClass(reinterpret_cast<UnkReflType*>(&g_unk_71727b87d0), "CEnvironmentManager", fn_7100d8ca4c, fn_7100d8c56c);
    fn_710009ec00(g_unk_71727b8908, &g_unk_71727b87d0);
    fn_710009ec00(g_unk_71727b8909, const_cast<base::reflection::CType*>(fn_7100ad62d4()));

    new (&g_unk_71727b8910) UnkValue(CStrId("pVisualPresets", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_71727b8910, &g_unk_7101a19000);
    new (&g_unk_71727b8920) UnkValue(CStrId("pSoundPresets", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_71727b8920, &g_unk_7101a19000);
    new (&g_unk_71727b8930) UnkValue(CStrId("pMusicPresets", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_71727b8930, &g_unk_7101a19000);

    InitValueList_d90cd0(&g_unk_71727b8940, g_unk_71727b8910);
    InitValueList_d90cd0(&g_unk_71727b8968, g_unk_71727b8920);
    InitValueList_d90cd0(&g_unk_71727b8990, g_unk_71727b8930);
}
