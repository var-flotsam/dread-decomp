// GUI::CLegendMenuEntryItemRenderer's unit (tools/tu_map.py): binding the renderer's widgets. After the
// base's (fn_71011d9328), the child named by the id table's id at +0x1588 is kept at +0x230 when it is a
// GUI::CLabel and the one at +0x1390 at +0x238 when it is a GUI::CSprite. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;

static inline const CStrId& TableId(u32 offset) {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<u8*>(&GameManagerIdTable()) + offset);
}

struct UnkLegendEntryRenderer_ovb0135 : UnkGuiDisplayObject {};

void fn_71011d9328(UnkLegendEntryRenderer_ovb0135* self);

struct UnkLegendEntryRendererFields_ovb0135 {
    u8 pad_0x0[0x230];
    UnkGuiDisplayObject* label;   // 0x230
    UnkGuiDisplayObject* sprite;  // 0x238
};

template <const base::reflection::CClass* (*Getter)()>
static inline UnkGuiDisplayObject* CastTo(UnkGuiDisplayObject* obj) {
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->GetClass(), Getter()) ? obj : nullptr;
}

void fn_7100b333cc(UnkLegendEntryRenderer_ovb0135* self) {
    fn_71011d9328(self);
    UnkLegendEntryRendererFields_ovb0135* fields = reinterpret_cast<UnkLegendEntryRendererFields_ovb0135*>(self);
    fields->label = CastTo<fn_71011e8dfc>(fn_71011d94d4(self, TableId(0x1588)));
    fields->sprite = CastTo<fn_71011f6890>(fn_71011d94d4(self, TableId(0x1390)));
}
