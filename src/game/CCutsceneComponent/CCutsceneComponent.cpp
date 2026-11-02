// The reflected End of an engine array (base/UnkVector.h) of 0x88-byte elements destroyed out of
// line by fn_71007a8eec; the same shape as fn_71000d5f4c (src/game_s1/fn_71000d5888.cpp).
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "types.h"

using base::UnkVector;

struct UnkStruct_71007a8eec;
void fn_71007a8eec(void* self);

struct UnkCutsceneElement {
    u64 unk_0x0[0x88 / 8];
    ~UnkCutsceneElement() { fn_71007a8eec(this); }
};

using UnkCutsceneElementIter = base::UnkCollectionIter<UnkCutsceneElement>;

void fn_71007acf3c(const UnkVector<UnkCutsceneElement>* v, UnkCutsceneElementIter* out) {
    *out = UnkCutsceneElementIter(v->count, v->data + v->count);
}
