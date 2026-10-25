// Two flag setters of a GUI display object (game/UnkGuiDisplayObject.h). fn_71011cdce8 sets the
// flag at +0xb4: marks the object stale and, if its derived visibility changed, sends a change
// event. fn_71011cf2a4 sets the flag at +0x181: when the object is shown and active and the flag
// changed, tells the GUI root that the object gained or lost it.
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

struct UnkEvent_0x28 {
    char data[0x28];
};

void fn_71011cee40(UnkGuiDisplayObject* self);
void fn_710120235c(UnkEvent_0x28* ev, UnkGuiDisplayObject* sender, const s32* type, void* extra);
void fn_7101202488(UnkEvent_0x28* ev);
void fn_71011b6e0c(void* mgr, UnkEvent_0x28* ev);
void fn_71011b5d94(void* root, UnkGuiDisplayObject* object);
void fn_71011b7514(void* root, UnkGuiDisplayObject* object);
extern void* g_unk_717271e8f8;

static inline u8 IsShown_71011cdce8(UnkGuiDisplayObject* self) {
    if (!self->field_0x74)
        return 0;
    if (self->field_0x183)
        fn_71011cee40(self);
    return self->field_0x76;
}

static inline void Notify_71011cdce8(UnkGuiDisplayObject* self, s32 kind) {
    UnkEvent_0x28 ev;
    fn_710120235c(&ev, self, &kind, nullptr);
    fn_71011b6e0c(g_unk_717271e8f8, &ev);
    fn_7101202488(&ev);
}

void fn_71011cdce8(UnkGuiDisplayObject* self, bool value) {
    if (value == self->field_0xb4)
        return;
    if (self->field_0x183)
        fn_71011cee40(self);
    u8 before = IsShown_71011cdce8(self);
    self->field_0xb4 = value;
    if (!self->field_0x184) {
        self->field_0x184 = true;
        self->vf_0xa0();
    }
    self->field_0x183 = true;
    self->field_0x144 |= 0x40;
    fn_71011cee40(self);
    bool after = IsShown_71011cdce8(self) != 0;
    if (before == after)
        return;
    if (after)
        Notify_71011cdce8(self, 0);
    else
        Notify_71011cdce8(self, 1);
}

// The flag at +0x181 as the root sees it: only while the object is shown and active.
static inline bool IsActiveFlag_71011cf2a4(UnkGuiDisplayObject* self) {
    return self->field_0x76 && self->field_0x74 && self->field_0x181;
}

// NON_MATCHING(minor): same behaviour; the original keeps one store-and-compare tail for the shown
// and the not-shown paths (comparing two zeros in the latter) and computes the new state as a
// bitwise and of the active byte and the value, where ours folds the not-shown and not-active
// paths into an early store-and-return.
void fn_71011cf2a4(UnkGuiDisplayObject* self, bool value) {
    if (self->field_0x181 == value)
        return;
    if (self->field_0x183)
        fn_71011cee40(self);
    bool before = IsActiveFlag_71011cf2a4(self);
    self->field_0x181 = value;
    bool after = IsActiveFlag_71011cf2a4(self);
    if (before == after)
        return;
    if (after)
        fn_71011b7514(g_unk_717271e8f8, self);
    else
        fn_71011b5d94(g_unk_717271e8f8, self);
}
