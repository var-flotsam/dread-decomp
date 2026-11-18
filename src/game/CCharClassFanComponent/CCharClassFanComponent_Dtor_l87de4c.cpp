// CCharClassFanComponent's unit (tools/tu_map.py unit 1336): the fan component's destructor. Its two pooled
// lists (+0xb08 and +0xb78) are destroyed in reverse order, then the base destructor fn_7100a46b64 runs.
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "types.h"

extern void* g_unk_7101a7b368[];  // the class's vtable
extern char g_unk_7101a7b5b0[];
extern char g_unk_7101a7b5d0[];
extern char g_unk_7101a7b5f0[];
extern char g_unk_7101a7b610[];

void fn_7100a46b64(char*);

struct UnkFanListA_l87de4c;
struct UnkFanListB_l87de4c;
template <>
struct base::UnkPoolListTraits<UnkFanListA_l87de4c> {
    static char* vtable() { return g_unk_7101a7b5b0; }
    static char* base_vtable() { return g_unk_7101a7b5d0; }
};
template <>
struct base::UnkPoolListTraits<UnkFanListB_l87de4c> {
    static char* vtable() { return g_unk_7101a7b5f0; }
    static char* base_vtable() { return g_unk_7101a7b610; }
};

using UnkFanList0x38_l87de4c = base::UnkPoolList<UnkFanListA_l87de4c, 0x30, 0x38>;
using UnkFanList0x18_l87de4c = base::UnkPoolList<UnkFanListB_l87de4c, 0x10, 0x18>;

struct UnkFanComponent_l87de4c {
    void** vtable;
    u8 pad_0x8[0xb00];
    UnkFanList0x38_l87de4c list_a;  // 0xb08
    UnkFanList0x18_l87de4c list_b;  // 0xb78
};
static_assert(__builtin_offsetof(UnkFanComponent_l87de4c, list_b) == 0xb78, "layout");

void fn_710087de4c(UnkFanComponent_l87de4c* self) {
    self->vtable = g_unk_7101a7b368 + 2;
    self->list_b.~UnkFanList0x18_l87de4c();
    self->list_a.~UnkFanList0x38_l87de4c();
    fn_7100a46b64(reinterpret_cast<char*>(self));
}
