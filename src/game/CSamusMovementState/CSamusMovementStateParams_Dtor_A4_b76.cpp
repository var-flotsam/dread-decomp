// CSamusMovementState::CParams's unit (tools/tu_map.py), fn_71009b0800: a destructor. Its vtable (0x7101a87850),
// the linked secondary base at +0x168 (refl_ReleaseLink), the refcounted pointer at +0x160, the member at +0x48
// (fn_71009b08c8), then the inlined base: its vtable (0x7101a879b8) and the reflected value at +0x18
// (fn_71000a1c60). Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a87850[];
extern void* g_unk_7101a879b8[];
extern void* g_unk_7101a22d48[];

struct UnkSamusParamsMember_A4_b76;
void fn_71009b08c8(UnkSamusParamsMember_A4_b76* self);

struct UnkSamusParamsDtor_A4_b76;

template <>
struct base::UnkDtorTraits<UnkSamusParamsDtor_A4_b76> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a87850); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a879b8); }
};

template <>
struct base::UnkLinkTraits<UnkSamusParamsDtor_A4_b76> {
    static char* link_vtable() { return reinterpret_cast<char*>(g_unk_7101a22d48); }
    static void unlink(char* list, char* link) { fn_710117b3f0(reinterpret_cast<UnkWeakTarget*>(list), link); }
};

struct UnkSamusParamsLink_A4_b76 {
    static void Release(char* self) { base::refl_ReleaseLink<UnkSamusParamsDtor_A4_b76, 0x168>(self); }
};

struct UnkSamusParamsMember48_A4_b76 {
    static void Release(char* self) { fn_71009b08c8(reinterpret_cast<UnkSamusParamsMember_A4_b76*>(self + 0x48)); }
};

struct UnkSamusParamsValue_A4_b76 {
    static void Release(char* self) {
        fn_71000a1c60(reinterpret_cast<base::reflection::UnkReflValue*>(self + 0x18));
    }
};

// As src/game/CSamusHangMovementState/CSamusHangMovementState_Dtor_b49a4.cpp declares it.
__attribute__((flatten)) void fn_71009b0800(void* object) {
    char* self = static_cast<char*>(object);
    base::refl_DtorInlineBase<
        UnkSamusParamsDtor_A4_b76,
        base::UnkMembers<UnkSamusParamsLink_A4_b76, base::UnkRefPtrAt<0x160>, UnkSamusParamsMember48_A4_b76>,
        UnkSamusParamsValue_A4_b76>(self);
}
