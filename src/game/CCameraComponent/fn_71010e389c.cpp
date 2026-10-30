// A lookup in the CCameraComponent unit (unit 2691 of tools/tu_map.py): the camera controller an id
// names in the holder's map of parallel arrays at +0x80 (base/UnkParallelFind.h), or null.
#include "base/UnkParallelFind.h"
#include "game/UnkCameraCtrl.h"

struct UnkCameraCtrlMap {
    u8 pad_0x0[0x80];
    UnkCameraCtrl** values;      // 0x80
    base::global::CStrId* ids;   // 0x88
    u32 count;                   // 0x90
};

UnkCameraCtrl* fn_71010e389c(void* holder, void* key) {
    return UnkParallelFind(static_cast<UnkCameraCtrlMap*>(holder), *static_cast<const base::global::CStrId*>(key));
}
