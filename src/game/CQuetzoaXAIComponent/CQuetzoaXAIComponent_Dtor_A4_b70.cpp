// CQuetzoaXAIComponent's unit (tools/tu_map.py unit 962): the component's destructor (vtable 0x7101a58f80):
// the linked listener at +0x14c08 (vtable 0x7101a22d48), a callback list (base::UnkCallbackListShort) at
// +0x14b88, the member at +0x14b28 (vtable 0x7101a58d90: its own linked listener at +0x40, then
// fn_71003606c0), then CQuetzoaAIComponent's destructor fn_7100649ca8. Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a58f80[];
extern void* g_unk_7101a58d90[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a22d48[];
void fn_71003606c0(void*);  // as src/game_5821e4/CAttackDtor.h declares it
void fn_7100649ca8(char* self);  // CQuetzoaAIComponent's destructor

struct UnkQuetzoaXDtor_A4_b70;
struct UnkQuetzoaXPart_A4_b70;

template <>
struct base::UnkDtorTraits<UnkQuetzoaXDtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a58f80); }
    static void base(char* self) { fn_7100649ca8(self); }
};

template <>
struct base::UnkDtorTraits<UnkQuetzoaXPart_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a58d90); }
    static void base(char* self) { fn_71003606c0(self); }
};

template <>
struct base::UnkCallbackTraits<UnkQuetzoaXDtor_A4_b70> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a20950); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};

template <>
struct base::UnkLinkTraits<UnkQuetzoaXDtor_A4_b70> {
    static char* link_vtable() { return reinterpret_cast<char*>(g_unk_7101a22d48); }
    static void unlink(char* list, char* link) { fn_710117b3f0(reinterpret_cast<UnkWeakTarget*>(list), link); }
};

template <u32 Offset>
struct UnkQuetzoaXLink_A4_b70 {
    static void Release(char* self) { base::refl_ReleaseLink<UnkQuetzoaXDtor_A4_b70, Offset>(self); }
};

struct UnkQuetzoaXList_A4_b70 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkCallbackListShort<UnkQuetzoaXDtor_A4_b70>*>(self + 0x14b88)->~UnkCallbackListShort();
    }
};

struct UnkQuetzoaXMember_A4_b70 {
    static void Release(char* self) {
        base::refl_Dtor<UnkQuetzoaXPart_A4_b70, UnkQuetzoaXLink_A4_b70<0x40>>(self + 0x14b28);
    }
};

__attribute__((flatten)) void fn_710063f310(char* self) {
    base::refl_Dtor<UnkQuetzoaXDtor_A4_b70, UnkQuetzoaXLink_A4_b70<0x14c08>, UnkQuetzoaXList_A4_b70,
                    UnkQuetzoaXMember_A4_b70>(self);
}
