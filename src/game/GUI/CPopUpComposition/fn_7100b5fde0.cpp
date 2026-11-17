// GUI::CPopUpComposition's unit (tools/tu_map.py): opens the pop-up. Looks up its four labels by
// path (the text, the hidden one, the accept and back prompts), sets the mode, shows and focuses the
// composition, fills the labels, keeps the accept and back callbacks and starts the timer (1 when
// `time` is negative). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CLabel.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkGuiComposition_ovb0090;
struct UnkMapCallback_ovb0090;
struct UnkPopUpComposition_ovb0085;

// The pop-up's own steps (src/game/GUI/CPopUpComposition/CPopUpComposition_Members_ovb0085.cpp
// defines fn_7100b5effc; fn_7100b5f938 is a placeholder signature).
void fn_7100b5f938(UnkPopUpComposition_ovb0085* self, u32 mode);
void fn_7100b5effc(UnkPopUpComposition_ovb0085* self, f32 alpha);

// A string id's pooled data as these reads see it.
struct UnkPopUpIdData_b5fde0 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 reads;  // 0x44
    };
};

// The label paths (string ids' data): the text, the hidden one, the accept and the back prompt.
extern u8* g_unk_71727653a0 __attribute__((visibility("hidden")));
extern u8* g_unk_71727653b0 __attribute__((visibility("hidden")));
extern u8* g_unk_71727653b8 __attribute__((visibility("hidden")));

// The id's text, counting a read.
static inline const char* IdText(u8* id) {
    UnkPopUpIdData_b5fde0* data = reinterpret_cast<UnkPopUpIdData_b5fde0*>(id);
    ++data->reads;
    return data->text ? data->text : g_unk_710162e2ed;
}

// The object at the game manager's +0x498: set up through vf_0x70 unless vf_0x68 says it is.
struct UnkPopUpService_b5fde0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual bool vf_0x68();
    virtual void vf_0x70();
};

// A callback: an object and a function to call on it.
struct UnkPopUpCallback_b5fde0 {
    void* object;
    void (*function)(void*);

    void Clear() {
        function = nullptr;
        object = nullptr;
    }
};

struct UnkPopUp_b5fde0 {
    u8 pad_0x0[0x2e0];
    bool field_0x2e0;
    bool field_0x2e1;
    u8 pad_0x2e2[0x6];
    u32 field_0x2e8;
    u8 pad_0x2ec[0x3c];
    CStrId field_0x328;
    UnkPopUpCallback_b5fde0 field_0x330;
    UnkPopUpCallback_b5fde0 on_accept;  // 0x340
    UnkPopUpCallback_b5fde0 on_back;    // 0x350
    u8 pad_0x360[0x44];
    f32 time;  // 0x3a4
};
static_assert(__builtin_offsetof(UnkPopUp_b5fde0, time) == 0x3a4, "UnkPopUp_b5fde0 layout");

// The label at the path held in `id`, or null.
static inline UnkGuiDisplayObject* FindLabel(UnkPopUp_b5fde0* self, u8* id) {
    UnkGuiDisplayObject* obj;
    {
        CRntString path(IdText(id));
        obj = fn_71011d9508(reinterpret_cast<UnkGuiDisplayObject*>(self), reinterpret_cast<const char* const*>(&path));
        if (obj && !fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                                  reinterpret_cast<const base::reflection::CType*>(fn_71011e8dfc())))
            obj = nullptr;
    }
    return obj;
}

// Returns true. The shared declaration (game/GUI/CPopUpComposition.h) still gives it no result.
bool fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const base::global::CStrId* text, const base::global::CStrId* accept, const base::global::CStrId* back,
                   const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode,
                   f32 time) {
    UnkPopUp_b5fde0* self = reinterpret_cast<UnkPopUp_b5fde0*>(gui);
    UnkPopUpService_b5fde0* service = static_cast<UnkPopUpService_b5fde0*>(g_unk_7101d47310->field_0x498);
    if (!service->vf_0x68()) {
        self->field_0x2e0 = true;
        service->vf_0x70();
    }

    UnkGuiDisplayObject* text_label = FindLabel(self, g_unk_71727653a0);
    UnkGuiDisplayObject* hidden_label = FindLabel(self, g_unk_71727653a8);
    UnkGuiDisplayObject* accept_label = FindLabel(self, g_unk_71727653b0);
    UnkGuiDisplayObject* back_label = FindLabel(self, g_unk_71727653b8);

    fn_7100b5f938(reinterpret_cast<UnkPopUpComposition_ovb0085*>(gui), mode);
    UnkGuiDisplayObject* object = reinterpret_cast<UnkGuiDisplayObject*>(self);
    fn_71011cf2a4(object, true);
    fn_71011b6bbc(g_unk_717271e8f8, object, false);
    self->field_0x2e1 = true;

    fn_71011cdce8(hidden_label, false);
    fn_71011ecfbc(text_label, reinterpret_cast<const char32_t*>(text));
    fn_71011cdce8(back_label, true);
    fn_71011ecfbc(back_label, reinterpret_cast<const char32_t*>(back));
    fn_71011cdce8(accept_label, true);
    fn_71011ecfbc(accept_label, reinterpret_cast<const char32_t*>(accept));

    self->field_0x2e8 = 1;
    self->field_0x328 = g_unk_7101d470e0;
    self->on_back.Clear();
    self->on_accept.Clear();
    self->field_0x330.Clear();
    self->on_accept = *reinterpret_cast<const UnkPopUpCallback_b5fde0*>(on_accept);
    self->on_back = *reinterpret_cast<const UnkPopUpCallback_b5fde0*>(on_back);
    self->time = time;
    fn_7100b5effc(reinterpret_cast<UnkPopUpComposition_ovb0085*>(gui), time < 0.0f ? 1.0f : time);
    return true;
}
