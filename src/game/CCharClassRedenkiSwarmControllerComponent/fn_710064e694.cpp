// CCharClassRedenkiSwarmControllerComponent (unit 976): the constructor. After the base come twelve
// empty strings and twelve entries of a string id, a file path id and a word, each id copied from the
// empty default. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "types.h"

void fn_7100732834(void* self);
extern void* g_unk_7101a5ae60[];

struct UnkRedenkiSwarmEntry_l64e694 {
    base::global::CStrId id;
    base::global::CFilePathStrId path;
    u64 field_0x10;

    UnkRedenkiSwarmEntry_l64e694() : id(g_unk_7101d470e0), path(g_unk_7101d470e8), field_0x10(0) {}
};

struct UnkCharClassRedenkiSwarmController_l64e694 {
    void** vtable;
    u8 pad_0x8[0x4c0];
    u8 strings[12][0x30];  // 0x4c8: CRntStrings
    UnkRedenkiSwarmEntry_l64e694 entries[12];  // 0x708
};
static_assert(__builtin_offsetof(UnkCharClassRedenkiSwarmController_l64e694, entries) == 0x708, "layout");

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_710064e694(UnkCharClassRedenkiSwarmController_l64e694* self) {
    fn_7100732834(self);
    self->vtable = g_unk_7101a5ae60 + 2;
    for (int i = 0; i < 12; i++)
        fn_710007e6c4(self->strings[i]);
    for (int i = 0; i < 12; i++)
        new (&self->entries[i]) UnkRedenkiSwarmEntry_l64e694();
}
