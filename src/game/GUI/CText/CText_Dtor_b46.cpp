// GUI::CText unit (tools/tu_map.py unit 1724): the destructor and the deleting destructor. Two
// wide strings (+0x218, +0x1f8; fn_7100066e44), the string at +0x1c0 (CRntString, fn_710007ecd4)
// and the string id at +0x1a8 are released, then the display object's base destructor
// (fn_71011ccfb4) runs. Member types are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkTU2747.h"

struct UnkTag_7100b86668;
extern char g_unk_7101a9fa90[];  // the class's vtable

template <>
struct base::UnkDtorTraits<UnkTag_7100b86668> {
    static char* vtable() { return g_unk_7101a9fa90; }
    static void base(char* p) { fn_71011ccfb4(p); }
};

template <u32 Offset>
struct UnkWideStringAt_b46a3 {
    static void Release(char* self) { fn_7100066e44(self + Offset); }
};

template <u32 Offset>
struct UnkRntStringAt_b46a3 {
    static void Release(char* self) { fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(self + Offset)); }
};

using UnkTextMembers_b46a3 = base::UnkMembers<UnkWideStringAt_b46a3<0x218>, UnkWideStringAt_b46a3<0x1f8>,
                                              UnkRntStringAt_b46a3<0x1c0>, base::UnkStrIdAt<0x1a8>>;

void fn_7100b86668(char* self) { base::refl_Dtor<UnkTag_7100b86668, UnkTextMembers_b46a3>(self); }
__attribute__((flatten)) void fn_7100b866fc(char* self) { base::refl_DeletingDtor<UnkTag_7100b86668, UnkTextMembers_b46a3>(self); }
