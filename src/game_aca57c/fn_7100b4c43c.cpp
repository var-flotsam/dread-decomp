// Passes a value to a menu (at 0x270) and, when the menu is in state 3, drops the GUI object
// held at 0x18, sets parameter 0xede73d1953baf654 to -1, flags the three panels at 0x28..0x38
// and clears their two values; then calls fn_7100b364dc on the menu.
#include "types.h"

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    void* field_0x10;
};

struct UnkPanel {
    u8 unk_0x0[0x290];
    u64 field_0x290;
    u64 field_0x298;
    bool field_0x2a0;
};

struct UnkMenuSource {
    u8 unk_0x0[0x8];
    u64 field_0x8;
    u8 unk_0x10[0x8];
    UnkRefObject* field_0x18;
    u8 unk_0x20[0x8];
    UnkPanel* field_0x28;
    UnkPanel* field_0x30;
    UnkPanel* field_0x38;
};

struct UnkMenuTarget {
    u8 unk_0x0[0x270];
    u64 field_0x270;
    u8 unk_0x278[0x8];
    s32 field_0x280;
};

struct UnkGuiRoot;
struct UnkParamTarget;
extern UnkGuiRoot* g_unk_717271e8f8;
extern UnkParamTarget* g_unk_7101d47300;
void fn_71011b57c4(UnkGuiRoot* root, void* object);
void fn_7100219294(UnkParamTarget* target, u64 hash, const f32* value, u32 mode);
void fn_7100b364dc(UnkMenuTarget* menu);

// A byte store, which may alias the panel pointer (the original reloads it).
static inline void StoreFlag(bool* field, bool value) { *reinterpret_cast<u8*>(field) = value; }

static inline void ResetPanel(UnkPanel* const& panel) {
    if (panel) {
        StoreFlag(&panel->field_0x2a0, true);
        panel->field_0x298 = 0;
        panel->field_0x290 = 0;
    }
}

void fn_7100b4c43c(UnkMenuSource* self, UnkMenuTarget* menu) {
    menu->field_0x270 = self->field_0x8;
    if (menu->field_0x280 == 3) {
        if (self->field_0x18 && self->field_0x18->field_0x10) {
            fn_71011b57c4(g_unk_717271e8f8, self->field_0x18->field_0x10);
            if (UnkRefObject* p = self->field_0x18) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->field_0x18 = nullptr;
            }
        }
        f32 value = -1.0f;
        fn_7100219294(g_unk_7101d47300, 0xede73d1953baf654, &value, 2);
        ResetPanel(self->field_0x28);
        ResetPanel(self->field_0x30);
        ResetPanel(self->field_0x38);
    }
    fn_7100b364dc(menu);
}
