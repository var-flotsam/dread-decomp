// CCharClassDaivoAIComponent unit (tools/tu_map.py unit 765): the destructor and the deleting
// destructor of a class there. The member at +0x650 is destroyed out of line (fn_71004dbc70); the
// base's destructor (fn_71006c54dc in src/game/CCharClassStartPointComponent/fn_710116baf4.cpp) is
// inlined: its vtable, the string id at +0x640, the member at +0x510 (fn_71006c0074), then its own
// base destructor (fn_7100416e80). Names and member types are placeholders.
#include "base/UnkDestructors.h"

struct UnkTag_71004dea0c;
extern char g_unk_7101a40860[];  // the class's vtable
extern void* g_unk_7101a64f60[];  // the base's vtable

struct UnkStruct_7100416e80;
void fn_7100416e80(UnkStruct_7100416e80* self);
void fn_71006c0074(void* self);
void fn_71004dbc70(void* self);

template <>
struct base::UnkDtorTraits<UnkTag_71004dea0c> {
    static char* vtable() { return g_unk_7101a40860; }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a64f60); }
};

template <void (*Destroy)(void*), u32 Offset>
struct UnkCalledAt_71004dea0c {
    static void Release(char* self) { Destroy(self + Offset); }
};

struct UnkBaseDtor_71004dea0c {
    static void Release(char* self) { fn_7100416e80(reinterpret_cast<UnkStruct_7100416e80*>(self)); }
};

using UnkDerivedMembers_71004dea0c = base::UnkMembers<UnkCalledAt_71004dea0c<fn_71004dbc70, 0x650>>;
using UnkBaseMembers_71004dea0c =
    base::UnkMembers<base::UnkStrIdAt<0x640>, UnkCalledAt_71004dea0c<fn_71006c0074, 0x510>, UnkBaseDtor_71004dea0c>;

void fn_71004dea0c(char* self) {
    base::refl_DtorInlineBase<UnkTag_71004dea0c, UnkDerivedMembers_71004dea0c, UnkBaseMembers_71004dea0c>(self);
}
void fn_71004deaa8(char* self) {
    base::refl_DeletingDtorInlineBase<UnkTag_71004dea0c, UnkDerivedMembers_71004dea0c, UnkBaseMembers_71004dea0c>(self);
}
