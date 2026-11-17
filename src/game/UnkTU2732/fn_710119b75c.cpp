// Unit 2732 (tools/tu_map.py): records the state of one controller, or of every connected one, into the
// next free slots of a capture table (up to 60 slots). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU0.h"
#include "game/nn/time.h"
#include "types.h"

struct UnkSoundGroup;
struct UnkSoundHolder {
    UnkSoundGroup* group;
};

// The controller source: 32 buttons per controller, each with three states and an analog value.
struct UnkPadSource_l19b75c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool IsHeld(u32 button, u32 pad);
    virtual bool IsPressed(u32 button, u32 pad);
    virtual bool IsReleased(u32 button, u32 pad);
    virtual f32 Value(u32 button, u32 pad);
    virtual void vf_0x50();
    virtual u32 PadCount();
    virtual bool IsConnected(u32 pad);

    u32 pad;
};

struct UnkPadCapture_l19b75c {
    u32 held;
    u32 released;
    u32 pressed;
    f32 values[32];
    u32 pad;
};
static_assert(sizeof(UnkPadCapture_l19b75c) == 0x90, "");

struct UnkPadRecorder_l19b75c {
    u8 pad_0x0[0x30];
    UnkPadCapture_l19b75c captures[60];
    u32 count;
    bool enabled;

    void Capture(UnkPadSource_l19b75c* source, u32 pad) {
        UnkPadCapture_l19b75c& capture = captures[count];
        capture = UnkPadCapture_l19b75c{0, 0, 0, {}, pad};
        for (s32 i = 0; i < 32; i++) {
            u32 button = 1u << i;
            if (source->IsHeld(button, pad))
                capture.held |= button;
            if (source->IsPressed(button, pad))
                capture.pressed |= button;
            if (source->IsReleased(button, pad))
                capture.released |= button;
            capture.values[i] = source->Value(button, pad);
        }
        count++;
    }
};
static_assert(__builtin_offsetof(UnkPadRecorder_l19b75c, count) == 0x21f0, "");

template <class T, u64 Size, void (*Ctor)(T*), u32 Flags = 0x3f>
static inline T* Singleton(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, Flags));
        Ctor(p);
        instance = p;
    }
    return instance;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710119b75c(UnkPadRecorder_l19b75c* self) {
    if (!self->enabled || self->count >= 60)
        return;
    UnkPadSource_l19b75c* source = reinterpret_cast<UnkPadSource_l19b75c*>(
        Singleton<UnkSoundHolder, 0x48, fn_710006cd9c>(g_unk_7101d47340)->group);
    u32 pad = source->pad;
    bool all = pad == 0xff || pad == 0xfe;
    u32 count = source->PadCount();
    if (all) {
        for (u32 i = 0; i < count; i = i + 1, count = source->PadCount()) {
            if (source->IsConnected(i))
                self->Capture(source, i);
        }
    } else if (pad < count && source->IsConnected(pad)) {
        self->Capture(source, pad);
    }
}
