// CShotManager::CTunableShotManager's unit (tools/tu_map.py, unit 2576): the destructor of a record of
// eight file-path ids, which releases them last to first. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/CShotManager/CTunableShotManager.h"
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

void fn_7100ed977c(UnkSlot_0x40* self) {
    UnkFilePathRecord_ledbef4* record = reinterpret_cast<UnkFilePathRecord_ledbef4*>(self);
    record->field_0x38.~CFilePathStrId();
    record->field_0x30.~CFilePathStrId();
    record->field_0x28.~CFilePathStrId();
    record->field_0x20.~CFilePathStrId();
    record->field_0x18.~CFilePathStrId();
    record->field_0x10.~CFilePathStrId();
    record->field_0x8.~CFilePathStrId();
    record->field_0x0.~CFilePathStrId();
}
