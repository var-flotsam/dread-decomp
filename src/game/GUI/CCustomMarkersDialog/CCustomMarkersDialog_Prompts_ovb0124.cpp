// GUI::CCustomMarkersDialog's unit (tools/tu_map.py unit 1660): the dialog's delete and delete-all prompts
// (asked through the GUI composition, the game manager's +0x2258 and its +0x10, as
// src/game/GUI/CLoadCheckPointButton/CLoadCheckPointButton_Prompt_ovb0118.cpp does), its opening (unless a
// marker is selected it plays its event, tells the GUI sound selector, and waits on its "Panel" child's set) and
// its destructor. Names and layouts are placeholders.
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkCustomMarkersDialog_ovb0124;
void fn_7100afe31c(UnkCustomMarkersDialog_ovb0124* self);
void fn_7100afe380(UnkCustomMarkersDialog_ovb0124* self);

// A callback: an object and a function to call on it.
struct UnkDialogCallback_ovb0124 {
    UnkCustomMarkersDialog_ovb0124* object;
    void (*function)(UnkCustomMarkersDialog_ovb0124*);
};

struct UnkGuiComposition_ovb0090;
struct UnkMapCallback_ovb0090;
void fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const CStrId* text, const CStrId* accept, const CStrId* back,
                   const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode,
                   f32 time);

struct UnkGuiRef_ovb0124 {
    u8 pad_0x0[0x10];
    UnkGuiComposition_ovb0090* composition;  // 0x10
};

struct UnkCustomMarkersDialog_ovb0124 {
    u8 pad_0x0[0xb4];
    bool active;  // 0xb4
    u8 pad_0xb5[0x294 - 0xb5];
    u32 selected;  // 0x294
    u32 pending;   // 0x298
    bool has_markers;  // 0x29c
    u8 pad_0x29d;
    bool busy;  // 0x29e
};

void fn_7100afd294(UnkCustomMarkersDialog_ovb0124* self) {
    if (!self->active || self->busy)
        return;
    self->pending = self->selected;
    UnkGuiRef_ovb0124* ref = static_cast<UnkGuiRef_ovb0124*>(g_unk_7101d47310->field_0x2258);
    if (!ref)
        return;
    UnkGuiComposition_ovb0090* gui = ref->composition;
    if (!gui)
        return;
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    CStrId text("#GUI_MAPMENU_CUSTOMMARKER_WARNING_DELETE", true);
    UnkDialogCallback_ovb0124 accept_callback = {self, fn_7100afe31c};
    UnkDialogCallback_ovb0124 back_callback = {self, fn_7100afe380};
    fn_7100b5fde0(gui, &text, &accept, &back, reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 2, -1.0f);
}

struct UnkMarkersInput_ovb0124 {
    u8 pad_0x0[0x1a];
    bool handled;  // 0x1a
};

void fn_71011cf754(UnkCustomMarkersDialog_ovb0124* self, UnkMarkersInput_ovb0124* input);

static inline void AskDeleteAll_ovb0124(UnkCustomMarkersDialog_ovb0124* self) {
    UnkGuiRef_ovb0124* ref = static_cast<UnkGuiRef_ovb0124*>(g_unk_7101d47310->field_0x2258);
    if (!ref)
        return;
    UnkGuiComposition_ovb0090* gui = ref->composition;
    if (!gui)
        return;
    CStrId back("#GUI_B_BACK", true);
    CStrId accept("#GUI_A_ACCEPT", true);
    CStrId text("#GUI_MAPMENU_CUSTOMMARKER_WARNING_DELETEALL", true);
    UnkDialogCallback_ovb0124 accept_callback = {self, fn_7100afe31c};
    UnkDialogCallback_ovb0124 back_callback = {nullptr, nullptr};
    fn_7100b5fde0(gui, &text, &accept, &back, reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 0, -1.0f);
}

void fn_7100afdf5c(UnkCustomMarkersDialog_ovb0124* self, UnkMarkersInput_ovb0124* input) {
    fn_71011cf754(self, input);
    if (self->active && self->has_markers && !self->busy) {
        self->pending = -1;
        AskDeleteAll_ovb0124(self);
        input->handled = true;
    }
}

struct UnkStateSink;
struct UnkStateKey;
void fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);
extern UnkStateSink* g_unk_717275c380;
extern UnkStateKey g_unk_71729f0180;
struct UnkMarkersEvent_ovb0124;
extern UnkMarkersEvent_ovb0124 g_unk_717275d460 __attribute__((visibility("hidden")));

struct UnkMarkersHashed_ovb0124 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkMarkersPanelState_ovb0124 {
    u8 pad_0x0[0x11];
    bool closed;  // 0x11
};

struct UnkMarkersRegView_ovb0124 {
    base::UnkDeferredSet<void>* set;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkMarkersDialog_ovb0124 {
    void** vtable;
    u8 pad_0x8[0x294 - 0x8];
    s32 selected;  // 0x294
    u32 pending;   // 0x298
    u8 pad_0x29c;
    bool silent;  // 0x29d
    bool busy;    // 0x29e
    u8 pad_0x29f;
    UnkMarkersRegView_ovb0124 registration;  // 0x2a0
};

void fn_7100afd4c0(UnkMarkersDialog_ovb0124* self) {
    self->busy = true;
    if (self->selected != -1) {
        self->busy = false;
        fn_71011cdce8(reinterpret_cast<UnkGuiDisplayObject*>(self), false);
        return;
    }
    reinterpret_cast<void (*)(UnkMarkersDialog_ovb0124*, UnkMarkersEvent_ovb0124*)>(self->vtable[0x1f0 / 8])(
        self, &g_unk_717275d460);
    if (!self->silent)
        fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0180, self);
    UnkGuiDisplayObject* panel;
    {
        CStrId name("Panel", true);
        panel = fn_71011d98ec(reinterpret_cast<UnkGuiDisplayObject*>(self),
                              reinterpret_cast<UnkMarkersHashed_ovb0124*>(name.data)->hash);
    }
    if (panel) {
        auto* state = reinterpret_cast<UnkMarkersPanelState_ovb0124* (*)(UnkGuiDisplayObject*)>(
            (*reinterpret_cast<void***>(panel))[0x1f8 / 8])(panel);
        if (state && !state->closed) {
            UnkMarkersRegView_ovb0124* r = &self->registration;
            if (r->set) {
                fn_71004f3ea4(r->set, r);
                r->set = nullptr;
            }
            r->set = reinterpret_cast<base::UnkDeferredSet<void>*>(reinterpret_cast<char*>(state) + 0x18);
            fn_71004f3cf0(r->set, r);
            return;
        }
    }
    self->busy = false;
    fn_71011cdce8(reinterpret_cast<UnkGuiDisplayObject*>(self), false);
}

struct UnkIdPair;
void fn_7100026f70(UnkIdPair*);
extern void* g_unk_7101a99520[];

// A counted reference released (not cleared) on destruction.
struct UnkMarkersRef_ovb0124 {
    UnkRefObject* ptr;

    ~UnkMarkersRef_ovb0124() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// An array of counted references.
struct UnkMarkersRefs_ovb0124 {
    UnkRefObject** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    ~UnkMarkersRefs_ovb0124() {
        if (data) {
            for (u64 i = 0; i < count; ++i) {
                if (UnkRefObject* p = data[i]) {
                    if (--p->ref_count == 0)
                        p->vf_0x10();
                }
            }
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// A registration in a deferred set, removed on destruction.
struct UnkMarkersReg_ovb0124 {
    base::UnkDeferredSet<void>* set;
    u64 field_0x8;
    u64 field_0x10;

    ~UnkMarkersReg_ovb0124() {
        if (set) {
            fn_71004f3ea4(set, this);
            set = nullptr;
        }
        field_0x10 = 0;
        field_0x8 = 0;
    }
};

struct UnkMarkersIdPair_ovb0124 {
    u8 data[0x18];

    ~UnkMarkersIdPair_ovb0124() { fn_7100026f70(reinterpret_cast<UnkIdPair*>(data)); }
};

struct UnkMarkersDialogFull_ovb0124 {
    void** vtable;
    u8 pad_0x8[0x228];
    UnkMarkersRef_ovb0124 refs_0x230[6];  // 0x230
    UnkMarkersRefs_ovb0124 list;  // 0x260
    base::UnkStrIdMember name;    // 0x288
    u8 pad_0x290[0x10];
    UnkMarkersReg_ovb0124 registration;  // 0x2a0
    UnkMarkersIdPair_ovb0124 field_0x2b8;
    UnkMarkersIdPair_ovb0124 field_0x2d0;
    UnkMarkersIdPair_ovb0124 field_0x2e8;
};
static_assert(__builtin_offsetof(UnkMarkersDialogFull_ovb0124, field_0x2e8) == 0x2e8, "layout");

void fn_7100afc540(UnkMarkersDialogFull_ovb0124* self) {
    self->vtable = g_unk_7101a99520 + 2;
    self->field_0x2e8.~UnkMarkersIdPair_ovb0124();
    self->field_0x2d0.~UnkMarkersIdPair_ovb0124();
    self->field_0x2b8.~UnkMarkersIdPair_ovb0124();
    self->registration.~UnkMarkersReg_ovb0124();
    self->name.~UnkStrIdMemberOf();
    self->list.~UnkMarkersRefs_ovb0124();
    for (s32 i = 5; i >= 0; --i)
        self->refs_0x230[i].~UnkMarkersRef_ovb0124();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
}
