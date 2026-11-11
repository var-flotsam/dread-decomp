// CInventoryComponent unit (tools/tu_map.py unit 2710): a destructor shaped like
// fn_71005ca278/fn_71005ca304 (src/game/CCharClassInfesterAIComponent/CCharClassInfesterAIComponent_Dtor_b44.cpp).
// fn_7101141378 runs on the object and fn_71009168e0 on the member at +0x90, the string id at +0x88 is
// released, the member at +0x40 is destroyed (fn_7101141468) and the component base destructor (fn_71010c4194) runs. Member
// types and offsets are placeholders.
#include "base/UnkDestructors.h"
#include "game/CInventoryComponent.h"
#include "game/UnkComponent.h"

struct UnkTag_71011412e4;
struct UnkObject_7101141558;

void fn_71009168e0(char*);
void fn_7101141468(char*);

template <>
struct base::UnkDtorTraits<UnkTag_71011412e4> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101cb33c8); }
    static void base(char* p) { fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(p)); }
};

template <void (*Destroy)(char*), u32 Offset>
struct UnkCalledAt_71011412e4 {
    static void Release(char* self) { Destroy(self + Offset); }
};

static void Cleanup_71011412e4(char* self) {
    fn_7101141378(reinterpret_cast<UnkObject_7101141558*>(self));
}

void fn_71011412e4(char* self) {
    base::refl_Dtor<UnkTag_71011412e4, UnkCalledAt_71011412e4<Cleanup_71011412e4, 0>,
                    UnkCalledAt_71011412e4<fn_71009168e0, 0x90>, base::UnkStrIdAt<0x88>,
                    UnkCalledAt_71011412e4<fn_7101141468, 0x40>>(self);
}
