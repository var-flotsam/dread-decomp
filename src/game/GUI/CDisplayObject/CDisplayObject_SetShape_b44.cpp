// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): the reflected setter of the
// object's shape by name: the string id picked from the argument's holder (inline, or behind a
// pointer for 9 or more bytes or kind 1); unless it is the global empty id (g_unk_7101d470e0), the GUI
// root looks the shape up by the id's key (fn_71011ba9dc) into +0x148 and the id's text is copied to
// +0x150. Returns whether the object has a shape (true for the empty id) as a bool value. The id's
// pooled data holds the text at +0x10 (its key at +0x30) and a count of reads at +0x44, as in
// src/game/CGameManager.cpp. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkGuiMapOwner;
void* fn_71011ba9dc(UnkGuiMapOwner* self, u64 id);  // src/game/GUI/CGUIManager/CGUIManager_FindById.cpp

struct UnkGuiShapeIdData_b44a3 {
    u8 pad_0x0[0x10];
    CRntString text;  // its key (+0x20) is the id's key
    u8 pad_0x40[0x4];
    s32 reads;  // 0x44
};

struct UnkGuiShapeView_b44a3 {
    u8 pad_0x0[0x148];
    void* shape;      // 0x148
    CRntString name;  // 0x150
};

struct UnkGuiShapeOwner_b44a3 {
    u8 pad_0x0[8];
    UnkGuiShapeView_b44a3* target;
};

UnkReflValue fn_71011d403c(UnkGuiShapeOwner_b44a3* owner, UnkReflValue** source) {
    UnkReflValue* src = *source;
    UnkGuiShapeView_b44a3* target = owner->target;
    if (src->field_0x14 == 1 || *reinterpret_cast<const u64*>(src->type + 0x18) > 8)
        src = static_cast<UnkReflValue*>(src->field_0x0);
    CStrId id(*reinterpret_cast<const CStrId*>(src));
    bool has;
    if (id.data == g_unk_7101d470e0.data) {
        has = true;
    } else {
        auto* d = reinterpret_cast<UnkGuiShapeIdData_b44a3*>(id.data);
        target->shape = fn_71011ba9dc(reinterpret_cast<UnkGuiMapOwner*>(g_unk_717271e8f8), d->text.key);
        ++d->reads;
        fn_710007eaa0(&target->name, &d->text);
        has = target->shape != nullptr;
    }
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &has, false);
    return r;
}
