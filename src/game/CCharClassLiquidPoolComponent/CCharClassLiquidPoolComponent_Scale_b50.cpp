// CCharClassLiquidPoolComponent unit (tools/tu_map.py, the name from reflection): the Y scale of the
// actor's model (the transform at +0x38 of the object at +0x50 of its model updater, fn_7100233684),
// kept within [0, 1]; 1 when there is no model updater. Names and layouts are placeholders.
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkStruct_7100233684;
namespace base::reflection {
class UnkReflObject;
}
base::reflection::UnkReflObject* fn_7100233684(UnkStruct_7100233684* self);

struct UnkLiquidModel {
    u8 unk_0x0[0x38];
    UnkXform xform;  // 0x38
};

struct UnkLiquidModelUpdater {
    u8 unk_0x0[0x50];
    UnkLiquidModel* model;  // 0x50
};

f32 fn_7100902abc(char* self) {
    auto* updater = reinterpret_cast<UnkLiquidModelUpdater*>(
        fn_7100233684(*reinterpret_cast<UnkStruct_7100233684**>(self + 0x30)));
    f32 y = 1.0f;
    if (updater != nullptr) {
        y = updater->model->xform.GetScale().y;
        if (y <= 0.0f)
            return 0.0f;
    }
    return 1.0f < y ? 1.0f : y;
}
