// CQuetzoaAIComponent's unit (tools/tu_map.py unit 973): the component's destructor (vtable 0x7101a5a240):
// the linked listener at +0x14b00 (vtable 0x7101a22d48), refcounted pointers at +0x14ac8 and +0x14ac0, two
// callback lists (base::UnkCallbackListShort) at +0x14a78 and +0x14a30, a refcounted pointer at +0x14a20,
// two members destroyed by fn_71003606c0, then the base's destructor fn_71003ac388. Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/CAbilityGhostAura.h"
#include "game/CBossAIComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a5a240[];
void fn_71003606c0(void*);  // as src/game_5821e4/CAttackDtor.h declares it

struct UnkQuetzoaDtor_A4_b70;

template <>
struct base::UnkDtorTraits<UnkQuetzoaDtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a5a240); }
    static void base(char* self) { fn_71003ac388(self); }
};

template <>
struct base::UnkCallbackTraits<UnkQuetzoaDtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

template <>
struct base::UnkLinkTraits<UnkQuetzoaDtor_A4_b70> {
    static char* link_vtable() { return reinterpret_cast<char*>(g_unk_7101a22d48); }
    static void unlink(char* list, char* link) { fn_710117b3f0(reinterpret_cast<UnkWeakTarget*>(list), link); }
};

template <u32 Offset>
struct UnkQuetzoaList_A4_b70 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkCallbackListShort<UnkQuetzoaDtor_A4_b70>*>(self + Offset)->~UnkCallbackListShort();
    }
};

struct UnkQuetzoaLink_A4_b70 {
    static void Release(char* self) { base::refl_ReleaseLink<UnkQuetzoaDtor_A4_b70, 0x14b00>(self); }
};

template <u32 Offset>
struct UnkQuetzoaPart_A4_b70 {
    static void Release(char* self) { fn_71003606c0(self + Offset); }
};

__attribute__((flatten)) void fn_7100649ca8(char* self) {
    base::refl_Dtor<UnkQuetzoaDtor_A4_b70, UnkQuetzoaLink_A4_b70, base::UnkRefPtrAt<0x14ac8>,
                    base::UnkRefPtrAt<0x14ac0>, UnkQuetzoaList_A4_b70<0x14a78>, UnkQuetzoaList_A4_b70<0x14a30>,
                    base::UnkRefPtrAt<0x14a20>, UnkQuetzoaPart_A4_b70<0x149e0>, UnkQuetzoaPart_A4_b70<0x149a0>>(self);
}
