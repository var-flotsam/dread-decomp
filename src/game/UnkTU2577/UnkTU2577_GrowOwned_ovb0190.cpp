// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eef244: the element accessor of the owned-entry array at +0x888 (CRntVector::GrowAt, out of line): past the end it grows the array to the next multiple of the step and then to index + 1, moving the owned entries and filling the new ones with null. Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/global/CRntVector.h"
#include "game/UnkOwnedEntry_b190.h"
#include "game/UnkTU2577.h"
#include "types.h"

struct UnkNamedIds;

UnkOwnedEntry_b190& fn_7100eef244(base::global::CRntVector<UnkOwnedEntry_b190>* v, u32 index) {
    return v->GrowAt(index);
}
