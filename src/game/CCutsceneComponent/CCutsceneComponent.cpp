// The reflected End of an engine array (base/global/CRntVector.h) of 0x88-byte elements destroyed out of
// line by fn_71007a8eec; the same shape as fn_71000d5f4c (src/game_s1/fn_71000d5888.cpp).
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "game/CCutsceneComponent.h"
#include "types.h"

using base::global::CRntVector;

struct UnkStruct_71007a8eec;

struct UnkCutsceneElement {
    u64 unk_0x0[0x88 / 8];
    ~UnkCutsceneElement() { fn_71007a8eec(this); }
};

using UnkCutsceneElementIter = base::UnkCollectionIter<UnkCutsceneElement>;

void fn_71007acf3c(const CRntVector<UnkCutsceneElement>* v, UnkCutsceneElementIter* out) {
    *out = UnkCutsceneElementIter(v->count, v->data + v->count);
}
