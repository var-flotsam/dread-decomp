// GUI::CTutorialComposition's unit (tools/tu_map.py unit 1727): fn_7100b88f10 shows the composition, sets
// the label named by g_unk_7172768b08 (mode 3, the given text, shown), reports the state key
// g_unk_71729f0178 and sends "OnEnter". The label lookup is the one in
// src/game/GUI/CDialogComposition/CDialogComposition_Labels_ovb0069.cpp. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;

using UnkGuiObj = UnkGuiDisplayObject;
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);
void fn_71011ef0c8(UnkGuiDisplayObject*, bool);
void fn_71011eec60(UnkGuiDisplayObject* label, u32 mode);
void fn_71011ecfbc(UnkGuiObj*, const char32_t*);

struct UnkStateSink;
struct UnkStateKey;
extern UnkStateSink* g_unk_717275c380;
extern UnkStateKey g_unk_71729f0178;

// A string id's pooled data as these reads see it.
struct UnkTutoIdData_ovb0125 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 reads;  // 0x44
    };
};

struct UnkTutoId_ovb0125 {
    UnkTutoIdData_ovb0125* data;

    const char* Text() const {
        ++data->reads;
        return data->text ? data->text : g_unk_710162e2ed;
    }
};

extern UnkTutoId_ovb0125 g_unk_7172768b08 __attribute__((visibility("hidden")));

static inline UnkGuiObj* FindLabel(void* self, const UnkTutoId_ovb0125& id) {
    UnkGuiObj* obj;
    {
        CRntString path(id.Text());
        obj = fn_71011d9508(static_cast<UnkGuiObj*>(self), reinterpret_cast<const char* const*>(&path));
        if (obj && !fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                  reinterpret_cast<const base::reflection::CType*>(fn_71011e8dfc())))
            obj = nullptr;
    }
    return obj;
}

struct UnkTutorialComposition_ovb0125 {
    void** vtable;

    void SendEvent(const char* name, bool flag) {
        reinterpret_cast<void (*)(UnkTutorialComposition_ovb0125*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, flag);
    }
};

bool fn_7100b88f10(UnkTutorialComposition_ovb0125* self, const char32_t* text) {
    UnkGuiObj* label = FindLabel(self, g_unk_7172768b08);
    auto* obj = reinterpret_cast<UnkGuiDisplayObject*>(self);
    fn_71011cdce8(obj, true);
    fn_71011cdc30(obj, true);
    fn_71011cf2a4(obj, false);
    fn_71011eec60(label, 3);
    fn_71011ecfbc(label, text);
    fn_71011ef0c8(label, true);
    fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0178, self);
    self->SendEvent("OnEnter", false);
    return true;
}
