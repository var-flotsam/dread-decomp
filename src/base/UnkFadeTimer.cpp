// From unit 50 of tools/tu_map.py (0x7100071dac-0x7100072564, no reflected class): an object with a
// value that runs down to zero (+0x94, clamped to 10) and the time it was last set (+0x8c), a hook run
// once (+0x60) and a listener (+0x58) told when the value runs out or a flag changes. Names and
// layouts are placeholders inferred from the code.
#include "types.h"

struct UnkFadeListener {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(void* fade, f32 time);  // told that the value has run out
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(void* fade, bool flag);  // told that the flag at +0xa0 changed
};

struct UnkFadeHook {
    virtual void vf_0x0(void* fade);  // run once, then dropped
};

struct UnkFade {
    u8 pad_0x0[0x58];
    UnkFadeListener* listener;  // 0x58
    UnkFadeHook* hook;          // 0x60
    u8 pad_0x68[0x24];
    f32 field_0x8c;
    u8 pad_0x90[4];
    f32 field_0x94;
    u8 pad_0x98[4];
    bool field_0x9c;  // active
    bool field_0x9d;  // running
    u8 pad_0x9e[2];
    bool field_0xa0;
    u8 pad_0xa1[2];
    bool field_0xa3;  // the listener is not told
};

// Sets the value at `time` (the listener is told when it runs out), first running the hook.
void fn_7100072328(void* self, f32 time, f32 value) {
    UnkFade* fade = static_cast<UnkFade*>(self);
    if (!fade->field_0x9c)
        return;
    if (UnkFadeHook* hook = fade->hook) {
        hook->vf_0x0(fade);
        fade->hook = nullptr;
    }
    if (!fade->field_0x9d)
        return;
    if (value <= 0.0f) {
        fade->field_0x94 = 0.0f;
        fade->listener->vf_0x20(fade, time);
        fade->field_0x9d = false;
    } else if (value >= 10.0f) {
        fade->field_0x94 = 10.0f;
    } else {
        fade->field_0x94 = value;
    }
    fade->field_0x8c = time;
}

// Sets the flag at +0xa0, telling the listener when it changes.
void fn_71000723cc(UnkFade* fade, bool flag) {
    if (fade->field_0x9c && fade->field_0xa0 != flag) {
        if (!fade->field_0xa3) {
            if (UnkFadeListener* listener = fade->listener)
                listener->vf_0x40(fade, flag);
        }
        fade->field_0xa0 = flag;
    }
}
