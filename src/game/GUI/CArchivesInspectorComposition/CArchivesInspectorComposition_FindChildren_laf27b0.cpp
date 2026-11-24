// GUI::CArchivesInspectorComposition's unit (tools/tu_map.py unit 1651): fn_7100af27b0 runs the base
// composition's set-up and, the first time, finds its children: the "Content" container and in it the
// "Image" sprite and the "Label-Number", "Label-Left" and "Label-Right" labels. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;

struct UnkArchivesInspector_laf27b0 {
    u8 pad_0x0[0x2e0];
    UnkGuiDisplayObject* image;         // 0x2e0
    UnkGuiDisplayObject* label_number;  // 0x2e8
    UnkGuiDisplayObject* content;       // 0x2f0
    UnkGuiDisplayObject* label_left;    // 0x2f8
    UnkGuiDisplayObject* label_right;   // 0x300
};

// The object when it is of the class `Type` returns (or derives from it), else null.
template <const base::reflection::CClass* (*Type)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* o) {
    return o && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(o->GetClass()),
                              reinterpret_cast<const base::reflection::CType*>(Type()))
               ? o
               : nullptr;
}

void CArchivesInspectorComposition_SetUpAndFindContentChildren(UnkArchivesInspector_laf27b0* self) {
    fn_71011d9b3c(reinterpret_cast<UnkPanel_71011dfeb0*>(self));
    if (self->content)
        return;
    self->content = As<fn_71011d7b84>(
        fn_71011d98ec(reinterpret_cast<UnkGuiDisplayObject*>(self), CStrId("Content", true).data->Key()));
    self->image = As<fn_71011f6890>(fn_71011d94d4(self->content, CStrId("Image", true)));
    self->label_number = As<fn_71011e8dfc>(fn_71011d94d4(self->content, CStrId("Label-Number", true)));
    self->label_left = As<fn_71011e8dfc>(fn_71011d94d4(self->content, CStrId("Label-Left", true)));
    self->label_right = As<fn_71011e8dfc>(fn_71011d94d4(self->content, CStrId("Label-Right", true)));
}
