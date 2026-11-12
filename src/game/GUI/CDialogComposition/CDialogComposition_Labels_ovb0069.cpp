// GUI::CDialogComposition's unit (tools/tu_map.py): the dialog's label accessors and its page close. Each
// accessor looks a GUI label (fn_71011e8dfc) up by a path held in a global string id (whose pooled data
// counts its reads at +0x44 and holds the text at +0x10) and reads or sets it: the progress value
// (+0x31c, clamped to [0, 1], with +0x318 reset to -1), the continue arrow (shown, looping) and the text.
// The page close sends the current page's (+0x260) event, reports the state key g_unk_71729f0180
// (fn_7100fc9d78) unless the page is "OnPageClose", and moves the watch at +0x248 to the set at +0x18 of
// the owner (+0x30). Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

using UnkGuiObj = UnkGuiDisplayObject;
UnkGuiObj* fn_71011d9508(UnkGuiObj* self, const char* const* path);
struct UnkWideStr_0x20;
void fn_71011ef0c8(UnkGuiDisplayObject*, bool);

// A string id's pooled data as these reads see it.
struct UnkDlgIdData_ovb0069 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 reads;  // 0x44
    };
};

struct UnkDlgId_ovb0069 {
    UnkDlgIdData_ovb0069* data;

    const char* Text() const {
        ++data->reads;
        return data->text ? data->text : g_unk_710162e2ed;
    }
};

extern UnkDlgId_ovb0069 g_unk_717275db70 __attribute__((visibility("hidden")));
extern UnkDlgId_ovb0069 g_unk_717275db78 __attribute__((visibility("hidden")));
extern UnkDlgId_ovb0069 g_unk_717275db90 __attribute__((visibility("hidden")));

struct UnkDlgLabel_ovb0069 {
    u8 pad_0x0[0x318];
    f32 field_0x318;
    f32 field_0x31c;
};

// A text of 32-bit characters built from a string out of line (fn_7100066b38).
struct UnkDlgText_ovb0069 {
    UnkGuiText text;

    explicit UnkDlgText_ovb0069(const char* str) { fn_7100066b38(reinterpret_cast<UnkWideStr_0x20*>(this), str); }
    ~UnkDlgText_ovb0069() { fn_7100066e44(this); }
};

static inline UnkGuiObj* FindLabel(void* self, const UnkDlgId_ovb0069& id) {
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

f32 fn_7100b01dc0(void* self) {
    return reinterpret_cast<UnkDlgLabel_ovb0069*>(FindLabel(self, g_unk_717275db70))->field_0x31c;
}

void fn_7100b01cfc(void* self, f32 value) {
    f32 v = !(value <= 0.0f) ? (!(value >= 1.0f) ? value : 1.0f) : 0.0f;
    auto* label = reinterpret_cast<UnkDlgLabel_ovb0069*>(FindLabel(self, g_unk_717275db70));
    label->field_0x31c = v;
    label->field_0x318 = -1.0f;
}

bool fn_7100b02530(void* self) {
    UnkGuiObj* label = FindLabel(self, g_unk_717275db78);
    fn_71011cdce8(label, true);
    fn_71011cdc30(label, true);
    reinterpret_cast<void (*)(UnkGuiObj*, const char*, bool)>((*reinterpret_cast<void***>(label))[0x1e8 / 8])(
        label, "ContinueArrow_Loop", false);
    return true;
}

void fn_7100b01c08(void* self, const UnkDlgId_ovb0069* text) {
    UnkGuiObj* label = FindLabel(self, g_unk_717275db90);
    {
        UnkDlgText_ovb0069 t(text->Text());
        fn_71011ec468(label, t.text, false);
    }
    fn_71011ef0c8(label, true);
}

struct UnkStateSink;
struct UnkStateKey;
extern UnkStateSink* g_unk_717275c380;
extern UnkStateKey g_unk_71729f0180;

struct UnkDialogClose_ovb0069 {
    void** vtable;
    u8 pad_0x8[0x28];
    char* owner;  // 0x30
    u8 pad_0x38[0x210];
    base::UnkDeferredSet<void>* watch;  // 0x248
    u8 pad_0x250[0x10];
    CStrId page;  // 0x260

    void SendEvent(const char* name, bool flag) {
        reinterpret_cast<void (*)(UnkDialogClose_ovb0069*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, flag);
    }
};
static_assert(__builtin_offsetof(UnkDialogClose_ovb0069, page) == 0x260, "layout");

static inline bool Differ(const CStrId& a, const CStrId& b) {
    return a.data != b.data;
}

void fn_7100b02404(UnkDialogClose_ovb0069* self) {
    if (self->page.data != &g_unk_7101d47108->empty) {
        auto* d = reinterpret_cast<UnkDlgIdData_ovb0069*>(self->page.data);
        ++d->reads;
        d = reinterpret_cast<UnkDlgIdData_ovb0069*>(self->page.data);
        self->SendEvent(d->text ? d->text : g_unk_710162e2ed, false);
    }
    if (Differ(self->page, CStrId("OnPageClose", true)))
        fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0180, self);
    if (char* owner = self->owner) {
        base::UnkDeferredSet<void>** slot = &self->watch;
        if (*slot) {
            fn_71004f3ea4(*slot, slot);
            *slot = nullptr;
        }
        *slot = reinterpret_cast<base::UnkDeferredSet<void>*>(owner + 0x18);
        fn_71004f3cf0(*slot, slot);
    }
}
