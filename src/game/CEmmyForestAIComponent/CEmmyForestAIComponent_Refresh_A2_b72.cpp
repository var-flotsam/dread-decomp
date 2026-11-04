// CEmmyForestAIComponent's unit (tools/tu_map.py unit 784): a reflected invoker on the component (+8 of the
// object): clears the byte at +0x2a700, then looks up two named entries (the unit's static ids) in the list
// owner reached through +0x29558 (+0x10 of it; fn_7100772cb0) and passes each one found (+0x58 of it) to
// fn_710110f8ac. Returns the empty value. Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkNamedEntry_7100772cb0;
struct UnkNamedList_7100772cb0;
UnkNamedEntry_7100772cb0* fn_7100772cb0(UnkNamedList_7100772cb0* self, const CStrId* id);
void fn_710110f8ac(void* self, void* object);
extern CStrId g_unk_71726dbe18 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726dbe20 __attribute__((visibility("hidden")));

struct UnkEmmyForestHolder_A2_b72 {
    u8 pad_0x0[0x10];
    UnkNamedList_7100772cb0* list;
};

struct UnkEmmyForestLink_A2_b72 {
    UnkEmmyForestHolder_A2_b72* holder;
    UnkNamedList_7100772cb0* List() const { return holder ? holder->list : nullptr; }
};

UnkReflValue fn_71004f33b0(void*, char* obj) {
    char* self = *reinterpret_cast<char**>(obj + 8);
    UnkEmmyForestLink_A2_b72& link = *reinterpret_cast<UnkEmmyForestLink_A2_b72*>(self + 0x29558);
    *reinterpret_cast<bool*>(self + 0x2a700) = false;
    if (link.List()) {
        char* a = reinterpret_cast<char*>(fn_7100772cb0(link.List(), &g_unk_71726dbe18));
        char* b = reinterpret_cast<char*>(fn_7100772cb0(link.List(), &g_unk_71726dbe20));
        if (a)
            fn_710110f8ac(self, a + 0x58);
        if (b)
            fn_710110f8ac(self, b + 0x58);
    }
    return base::reflection::refl_EmptyValue();
}
