// Unit 2661 (CCutscene::CTunableCutscene): move helper of the iterator value over a reflected array of 0x30-byte polymorphic elements,
// as base::refl_IterValueMove. Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "game/UnkElem2661.h"
#include "game/UnkElemData2661.h"
#include "game/UnkTU2661.h"
#include "types.h"

void fn_710106ea40(void* dst, const void* src) {
    base::refl_IterValueMove<UnkElem2661>(dst, src);
}
