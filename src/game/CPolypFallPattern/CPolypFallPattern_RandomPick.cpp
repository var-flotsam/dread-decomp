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

void* CCharClassHydrogigaPolypsAttack_PickUnbreakableFarPattern(CPolypFallPatternLists* self) { return PickRandom(self->list_0x100); }
void* CCharClassHydrogigaPolypsAttack_PickUnbreakableMiddlePattern(CPolypFallPatternLists* self) { return PickRandom(self->list_0x128); }
void* CCharClassHydrogigaPolypsAttack_PickUnbreakableClosePattern(CPolypFallPatternLists* self) { return PickRandom(self->list_0x150); }
