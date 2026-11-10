// CEmmyForestAIComponent's unit (tools/tu_map.py unit 784): sets the byte at +0x2a700, looks up the unit's
// two named entries in the list owner reached through +0x29558 (fn_7100772cb0) and passes each one found
// (+0x58 of it) to fn_710110f8ac; while the flag at +0x27720 is clear, mode 0 (+0x29ef0) sends the first
// and mode 1 the second to fn_710110f744 instead. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkNamedEntry_7100772cb0;
struct UnkNamedList_7100772cb0;
UnkNamedEntry_7100772cb0* fn_7100772cb0(UnkNamedList_7100772cb0* self, const CStrId* id);
void fn_710110f8ac(void* self, void* object);
void fn_710110f744(void* self, void* object);
extern CStrId g_unk_71726dbe18 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726dbe20 __attribute__((visibility("hidden")));

struct UnkEmmyForestHolder_ovb0120 {
    u8 pad_0x0[0x10];
    UnkNamedList_7100772cb0* list;
};

struct UnkEmmyForestLink_ovb0120 {
    UnkEmmyForestHolder_ovb0120* holder;
    UnkNamedList_7100772cb0* List() const { return holder ? holder->list : nullptr; }
};

struct UnkEmmyForestAI_ovb0120 {
    u8 pad_0x0[0x27720];
    bool flag;  // 0x27720
    u8 pad_0x27721[0x1e37];
    UnkEmmyForestLink_ovb0120 link;  // 0x29558
    u8 pad_0x29560[0x990];
    s32 mode;  // 0x29ef0
    u8 pad_0x29ef4[0x80c];
    bool active;  // 0x2a700
};
static_assert(__builtin_offsetof(UnkEmmyForestAI_ovb0120, active) == 0x2a700, "UnkEmmyForestAI_ovb0120 layout");

static inline void Apply(UnkEmmyForestAI_ovb0120* self, char* a, char* b, bool altA, bool altB) {
    if (a) {
        if (altA)
            fn_710110f744(self, a + 0x58);
        else
            fn_710110f8ac(self, a + 0x58);
    }
    if (b) {
        if (altB)
            fn_710110f744(self, b + 0x58);
        else
            fn_710110f8ac(self, b + 0x58);
    }
}

void fn_71004f2fa8(UnkEmmyForestAI_ovb0120* self) {
    self->active = true;
    if (!self->link.List())
        return;
    char* a = reinterpret_cast<char*>(fn_7100772cb0(self->link.List(), &g_unk_71726dbe18));
    char* b = reinterpret_cast<char*>(fn_7100772cb0(self->link.List(), &g_unk_71726dbe20));
    if (self->flag) {
        Apply(self, a, b, false, false);
        return;
    }
    switch (self->mode) {
    case 0:
        Apply(self, a, b, true, false);
        break;
    case 1:
        Apply(self, a, b, false, true);
        break;
    default:
        Apply(self, a, b, false, false);
        break;
    }
}
