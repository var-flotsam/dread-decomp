// GUI::CPopUpComposition's unit (tools/tu_map.py): sets the pop-up up. Registers its input handler
// for events 0x11 and 0x12, then keeps weak references to two children: the one the static id names
// (told to start its fade) and "FadeScreen". Names and layouts are placeholders.
#include <functional>

#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkPopUpEvent_b5ea20;

// The child's object as the fade call sees it.
struct UnkPopUpFader_b5ea20 : UnkWeakTarget {
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78(s32 value, bool start);
};

struct UnkPopUpComposition_b5ea20 {
    u8 pad_0x0[0x318];
    UnkWeakHandle* field_0x318;  // the child the static id names
    UnkWeakHandle* field_0x320;  // "FadeScreen"

    void OnInput(UnkPopUpEvent_b5ea20* event);
};

// Adds `handler` for `event` to the composition's input handlers.
void fn_7100b60900(void* owner, u32* event, const std::function<void(UnkPopUpEvent_b5ea20*)>& handler, void* a3, u32 a4);

// The name of the first child the pop-up keeps.
extern CStrId g_unk_7172765398 __attribute__((visibility("hidden")));

static inline void Listen(UnkPopUpComposition_b5ea20* self, u32* event) {
    if (self) {
        fn_7100b60900(self, event,
                      std::bind(&UnkPopUpComposition_b5ea20::OnInput, self, std::placeholders::_1), nullptr, 2);
    }
}

static inline UnkWeakHandle* HandleOf(UnkGuiDisplayObject* object) {
    return GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(object));
}

void fn_7100b5ea20(UnkPopUpComposition_b5ea20* self) {
    u32 event = 0x11;
    Listen(self, &event);
    event = 0x12;
    Listen(self, &event);

    UnkGuiDisplayObject* container = reinterpret_cast<UnkGuiDisplayObject*>(self);
    if (UnkGuiDisplayObject* child = fn_71011d98ec(container, g_unk_7172765398.data->Key())) {
        AssignRef(&self->field_0x318, HandleOf(child));
        static_cast<UnkPopUpFader_b5ea20*>(self->field_0x318->target)->vf_0x78(-3010, true);
    }
    UnkGuiDisplayObject* fade;
    {
        CStrId name("FadeScreen", true);
        fade = fn_71011d98ec(container, name.data->Key());
    }
    if (fade)
        AssignRef(&self->field_0x320, HandleOf(fade));
}
