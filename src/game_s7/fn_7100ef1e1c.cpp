// Copies of fn_7100a72af4 (src/game/CChangeStageNavMeshItemComponent/fn_71009ba4ac.cpp), which the binary
// repeats at other addresses: fn_7100ef1e1c.
// Destructor of a string id followed by an array of string ids (four copies).
#include "base/global/CStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkIdVector.h"
#include "game/UnkNamedIds.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_7100ef1e1c(UnkNamedIds* self) {
    self->~UnkNamedIds();
}

