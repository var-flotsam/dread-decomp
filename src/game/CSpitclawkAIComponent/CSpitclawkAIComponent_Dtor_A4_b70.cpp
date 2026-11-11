// CSpitclawkAIComponent's unit (tools/tu_map.py unit 1067): the destructor of a class with vtable
// 0x7101a664a0 and no base destructor: path ids (CFilePathStrId's pool) at +0x1d8, +0x150 and +0x148,
// string ids at +0x1b0 and +0x138, a refcounted pointer at +0x188, a runtime string at +0x158
// (fn_710007ecd4), then the member at +8 (fn_71008c1558). Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "game/UnkTU1370.h"
#include "types.h"

extern void* g_unk_7101a664a0[];
struct UnkBase_71003cc474;

struct UnkSpitclawkDtor_A4_b70;

template <>
struct base::UnkDtorTraits<UnkSpitclawkDtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a664a0); }
};

template <u32 Offset>
using UnkSpitclawkPathAt_A4_b70 = base::UnkMemberAt<base::UnkStrIdMemberOf<g_unk_7101d47200>, Offset>;

struct UnkSpitclawkString_A4_b70 {
    static void Release(char* self) { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(self + 0x158)); }
};

struct UnkSpitclawkBase_A4_b70 {
    static void Release(char* self) { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(self + 8)); }
};

__attribute__((flatten)) void fn_71006c9404(char* self) {
    base::refl_DtorNoBase<UnkSpitclawkDtor_A4_b70, UnkSpitclawkPathAt_A4_b70<0x1d8>, base::UnkStrIdAt<0x1b0>,
                          base::UnkRefPtrAt<0x188>, UnkSpitclawkString_A4_b70, UnkSpitclawkPathAt_A4_b70<0x150>,
                          UnkSpitclawkPathAt_A4_b70<0x148>, base::UnkStrIdAt<0x138>, UnkSpitclawkBase_A4_b70>(self);
}
