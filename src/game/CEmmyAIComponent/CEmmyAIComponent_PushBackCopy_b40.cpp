// CEmmyAIComponent unit (tools/tu_map.py unit 811): appends a copy of an inner array to an array of
// arrays, the same code as fn_7100d43e70 (src/game/CCollider/CCollider_CopyArrays.cpp); its caller is
// fn_71006fb3f4 (unit 1115). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkInnerVec.h"
#include "types.h"

typedef base::global::CRntVector<UnkInnerVec> UnkOuterVec;

void CEmmyAIComponent_PushBackInnerArrayCopy(UnkOuterVec* self, const UnkInnerVec* value) {
    self->PushBackCopy(*value);
}
