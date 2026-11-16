// CShotManager::CTunableShotManager's unit (tools/tu_map.py, unit 2576): the copy-assignment of a record of
// eight file-path ids, which does nothing when assigned to itself. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
struct UnkSlot_0x40;  // scratch: header declaration with the result changed to UnkSlot_0x40*
// fn_7100edbef4 is declared in game/CShotManager/CTunableShotManager.h.
#include "types.h"

using base::global::CFilePathStrId;

struct UnkFilePathRecord_ledbef4 {
    CFilePathStrId field_0x0;
    CFilePathStrId field_0x8;
    CFilePathStrId field_0x10;
    CFilePathStrId field_0x18;
    CFilePathStrId field_0x20;
    CFilePathStrId field_0x28;
    CFilePathStrId field_0x30;
    CFilePathStrId field_0x38;
};

UnkSlot_0x40* fn_7100edbef4(UnkSlot_0x40* selfSlot, UnkSlot_0x40* otherSlot) {
    UnkFilePathRecord_ledbef4* self = reinterpret_cast<UnkFilePathRecord_ledbef4*>(selfSlot);
    const UnkFilePathRecord_ledbef4* other = reinterpret_cast<const UnkFilePathRecord_ledbef4*>(otherSlot);
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        self->field_0x8 = other->field_0x8;
        self->field_0x10 = other->field_0x10;
        self->field_0x18 = other->field_0x18;
        self->field_0x20 = other->field_0x20;
        self->field_0x28 = other->field_0x28;
        self->field_0x30 = other->field_0x30;
        self->field_0x38 = other->field_0x38;
    }
    return selfSlot;
}
