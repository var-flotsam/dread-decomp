#include "game/UnkTU2772.h"
#include "types.h"

struct CPolypFallPatternList {
    void** data;
    u32 count;
    u8 unk_0xc[0x1c];
};

struct CPolypFallPatternLists {
    u8 unk_0x0[0x100];
    CPolypFallPatternList list_0x100;
    CPolypFallPatternList list_0x128;
    CPolypFallPatternList list_0x150;
};

static inline __attribute__((always_inline)) void* PickRandom(const CPolypFallPatternList& list) {
    u32 count = list.count;
    u32 index = count != 0 ? (u32)(fn_71012395a0() & 0x7fff) % count : 0;
    return list.data[index];
}

void* fn_71005af994(CPolypFallPatternLists* self) { return PickRandom(self->list_0x100); }
void* fn_71005af9d8(CPolypFallPatternLists* self) { return PickRandom(self->list_0x128); }
void* fn_71005afa1c(CPolypFallPatternLists* self) { return PickRandom(self->list_0x150); }
