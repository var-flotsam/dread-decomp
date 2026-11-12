// CPickableItemComponent's unit (tools/tu_map.py), fn_71009cf870: after the base read (fn_710024fc0c), when the
// file has data left, reads a string, a u32 and a string and drops them (an older format's fields). Names
// are placeholders.
#include "base/global/CRntFile.h"
#include "game/CActorComponent.h"
#include "types.h"

using base::global::CRntFile;

// As src/game/CTargetComponent/CTargetComponent_c2_2.cpp declares it.
void fn_710007ca94(CRntFile* file, char* buf);

struct UnkPickableItem_A4_b76;

void fn_71009cf870(UnkPickableItem_A4_b76* self, CRntFile* file) {
    fn_710024fc0c(self, file);
    if (file->position < file->size) {
        char first[0x200] = {};
        char second[0x200] = {};
        u32 value = 0;
        fn_710007ca94(file, first);
        fn_710007c714(file, &value, 4, 1);
        fn_710007ca94(file, second);
    }
}
