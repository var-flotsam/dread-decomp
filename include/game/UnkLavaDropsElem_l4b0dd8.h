#pragma once

#include "game/CCooldownXBossLavaDropsMovementDef.h"
#include "game/UnkLavaDropsData_l4b0dd8.h"
#include "game/UnkLavaDropsElemVtable_l4b0dd8.h"
#include "types.h"

struct UnkLavaDropsElem_l4b0dd8;

struct UnkLavaDropsElem_l4b0dd8 {
    const UnkLavaDropsElemVtable_l4b0dd8* vtable;
    UnkLavaDropsData_l4b0dd8 data;  // 0x8

    UnkLavaDropsElem_l4b0dd8(const UnkLavaDropsElem_l4b0dd8& other)
        : vtable(reinterpret_cast<const UnkLavaDropsElemVtable_l4b0dd8*>(g_unk_7101a3e068 + 0x10)), data(other.data) {}
    UnkLavaDropsElem_l4b0dd8(UnkLavaDropsElem_l4b0dd8&& other)
        : vtable(reinterpret_cast<const UnkLavaDropsElemVtable_l4b0dd8*>(g_unk_7101a3e068 + 0x10)), data(other.data) {}
    ~UnkLavaDropsElem_l4b0dd8() { vtable->dtor(this); }
};
