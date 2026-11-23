// GUI::CButton unit: whether the list a button's label names is shown. The label for the given state
// (one of the four ids at +0x230: states 0, 1, 3, 2) is a path when it contains a "."; the child it names in the button's
// container (+0x270, its object at +0x10) holds a list at +0x120, whose state flag +0x3c is the result.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGuiDisplayObject.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkButtonContainerRef_l1c06a4 {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* object;  // 0x10
};

struct UnkButtonLabels_l1c06a4 {
    u8 pad_0x0[0x230];
    // The label for each state: 0, 1, 3 and 2 in that order.
    CStrId label_0;  // 0x230
    CStrId label_1;  // 0x238
    CStrId label_3;  // 0x240
    CStrId label_2;  // 0x248
    u8 pad_0x250[0x20];
    UnkButtonContainerRef_l1c06a4* container;  // 0x270
};
static_assert(__builtin_offsetof(UnkButtonLabels_l1c06a4, container) == 0x270, "layout");

// The child the path names, as far as this function reads it.
struct UnkButtonChild_l1c06a4 {
    u8 pad_0x0[0x120];
    UnkGuiDisplayObject* list;  // 0x120
};

// A string id held for the length of the call: empty at first, then a reference to a label.
struct UnkLabelId_l1c06a4 {
    UnkStringData* data;

    UnkLabelId_l1c06a4() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkLabelId_l1c06a4() {
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool && data != &pool->empty) {
            if (--data->ref_count == 0)
                StringPool_Release(pool, data);
        }
    }
    void Take(const CStrId& id) {
        if (data != id.data) {
            data = id.data;
            ++data->ref_count;
        }
    }
};

// The object's class (vtable slot 0) is read before the getter runs.
template <auto Getter>
inline bool IsKindOf(UnkGuiDisplayObject* obj) {
    const base::reflection::CClass* cls = obj->GetClass();
    return fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(cls),
                         reinterpret_cast<const base::reflection::CType*>(Getter()));
}

bool fn_71011c06a4(UnkButtonLabels_l1c06a4* self, u32 state) {
    UnkLabelId_l1c06a4 label;
    switch (state) {
    case 0:
        label.Take(self->label_0);
        break;
    case 1:
        label.Take(self->label_1);
        break;
    case 2:
        label.Take(self->label_2);
        break;
    case 3:
        label.Take(self->label_3);
        break;
    }
    if (!self->container)
        return false;
    UnkGuiDisplayObject* container = self->container->object;
    if (!container || !IsKindOf<fn_71011d7b84>(container))
        return false;
    ++label.data->reads;
    if (label.data == &g_unk_7101d47108->empty || !CRntString_Contains(&label.data->String(), "."))
        return false;
    ++label.data->reads;
    auto* child = reinterpret_cast<UnkButtonChild_l1c06a4*>(fn_71011d9508(container, &label.data->text));
    if (!child)
        return false;
    UnkGuiDisplayObject* list = child->list;
    if (!list || !IsKindOf<fn_71011fe904>(list))
        return false;
    return fn_71011cde14(list)->field_0x3c;
}
