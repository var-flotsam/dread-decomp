// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): the per-frame update, after
// stale derived state is brought up to date (fn_71011cee40): while active, each condition (+0x100
// many, from +0x104, an unaligned array of pointers, as in CDisplayObject.cpp) gets vf_0x18 with the
// time step; the state object at +0x30 is updated (fn_7100b03c88) and, when its flag at +0x11 is set,
// the string id at +0x20 is emptied and the flag at +0x10 cleared; then vf_0x60 when +0x184 is set
// and vf_0x68 when +0x186 is. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "types.h"

struct UnkGuiCondition_b44a3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(f32 dt);
};
struct UnkGuiObjectSub_b44a3 {
    u8 pad_0x0[0x11];
    bool field_0x11;
};

void fn_7100b03c88(UnkGuiObjectSub_b44a3* sub, UnkGuiDisplayObject* object, f32 dt);

struct UnkGuiUpdateView_b44a3 {
    char* vtable;
    u8 pad_0x8[0x8];
    bool field_0x10;
    u8 pad_0x11[0xf];
    base::UnkStrIdMember field_0x20;
    u8 pad_0x28[0x8];
    UnkGuiObjectSub_b44a3* field_0x30;
    u8 pad_0x38[0xc8];
    u32 condition_count;  // 0x100
    u8 conditions[1];     // 0x104

    UnkGuiCondition_b44a3** begin() { return reinterpret_cast<UnkGuiCondition_b44a3**>(conditions); }
    UnkGuiCondition_b44a3** end() { return begin() + condition_count; }
};

void fn_71011cdfa8(UnkGuiDisplayObject* self, f32 dt) {
    if (self->field_0x183)
        fn_71011cee40(self);
    UnkGuiUpdateView_b44a3* view = reinterpret_cast<UnkGuiUpdateView_b44a3*>(self);
    if (self->field_0x74)
        for (UnkGuiCondition_b44a3** it = view->begin(); it != view->end(); ++it)
            (*it)->vf_0x18(dt);
    if (view->field_0x30) {
        fn_7100b03c88(view->field_0x30, self, dt);
        if (view->field_0x30->field_0x11) {
            using Id = base::UnkStrIdMember;
            view->field_0x20.~Id();
            view->field_0x10 = false;
        }
    }
    if (self->field_0x184)
        self->vf_0x60();
    if (reinterpret_cast<bool*>(self)[0x186])
        self->vf_0x68();
}
