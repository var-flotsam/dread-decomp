// Destructor of a string id followed by an array of string ids (four copies).
#include "base/global/CStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkIdVector.h"
#include "game/UnkNamedIds.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_71009ba4ac(UnkNamedIds* self) {
    self->~UnkNamedIds();
}

void fn_7100a72af4(UnkNamedIds* self) {
    self->~UnkNamedIds();
}

void fn_7100a73894(UnkNamedIds* self) {
    self->~UnkNamedIds();
}

void fn_7100a73ec8(UnkNamedIds* self) {
    self->~UnkNamedIds();
}
