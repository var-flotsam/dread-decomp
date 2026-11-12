// GUI::CSubtitleComposition's unit (tools/tu_map.py): shows a subtitle. The composition is made
// visible and active, and its GUI label (fn_71011e8dfc), found by the path a global string id holds
// (whose pooled data counts its reads at +0x44 and holds the text at +0x10), gets mode 3
// (fn_71011eec60, and +0x2f8), the text, and is refreshed (fn_71011ef0c8). Names and layouts are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "types.h"

using base::global::CRntString;

using UnkGuiObj = UnkGuiDisplayObject;
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);

// A string id's pooled data as these reads see it.
struct UnkSubtitleIdData_ovb0114 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 reads;  // 0x44
    };
};

struct UnkSubtitleId_ovb0114 {
    UnkSubtitleIdData_ovb0114* data;

    const char* Text() const {
        ++data->reads;
        return data->text ? data->text : g_unk_710162e2ed;
    }
};

extern UnkSubtitleId_ovb0114 g_unk_71727686a0 __attribute__((visibility("hidden")));

struct UnkSubtitleLabel_ovb0114 {
    u8 pad_0x0[0x2f8];
    u32 mode;  // 0x2f8
};

static inline UnkGuiObj* FindLabel(void* self, const UnkSubtitleId_ovb0114& id) {
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

bool fn_7100b8524c(UnkGuiObj* self, const UnkGuiText& text) {
    UnkGuiObj* label = FindLabel(self, g_unk_71727686a0);
    fn_71011cdce8(self, true);
    fn_71011cdc30(self, true);
    fn_71011cf2a4(self, false);
    fn_71011eec60(label, 3);
    fn_71011ec468(label, text, false);
    reinterpret_cast<UnkSubtitleLabel_ovb0114*>(label)->mode = 3;
    fn_71011ef0c8(label, true);
    return true;
}
