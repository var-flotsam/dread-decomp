#pragma once

#include "game/GUI/CLabel.h"
#include "game/UnkDlgIdData_ovb0069.h"

struct UnkDlgId_ovb0069 {
    UnkDlgIdData_ovb0069* data;

    const char* Text() const {
        ++data->reads;
        return data->text ? data->text : g_unk_710162e2ed;
    }
};
