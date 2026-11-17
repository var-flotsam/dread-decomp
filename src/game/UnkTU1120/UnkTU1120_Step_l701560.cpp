// Unit 1120 (tools/tu_map.py, no reflected class): steps the object's frame along a source's frame list
// (once, back and forth, or looping), copies the new frame's three words and updates the path query at
// +0x150. Names and layouts are placeholders.
#include "types.h"

struct UnkSelf_nq41;
void fn_71007071e0(UnkSelf_nq41* self);

struct UnkFrameWords_l701560 {
    u32 x;
    u32 y;
    u32 z;
};

// A frame source: its frame lists, their lengths and each frame's words.
struct UnkFrameSource_l701560 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkFrameWords_l701560* GetWords();          // 0x18
    virtual void vf_0x20();
    virtual u32 GetCount();                                   // 0x28
    virtual UnkFrameSource_l701560* GetChild(u32 index);      // 0x30
};

// A callback with its context, cleared once it has run.
struct UnkCallback_l701560 {
    void* ctx;
    void (*fn)(void* ctx, void* arg);

    bool IsSet() const { return fn != nullptr; }
    void Invoke(void* arg) { fn(ctx, arg); }
    void Clear() {
        ctx = nullptr;
        fn = nullptr;
    }
};

enum UnkStepMode_l701560 : s32 {
    kStepOnce = 1,
    kStepPingPong = 2,
    kStepLoop = 3,
};

struct UnkUnit1120Object_l701560 {
    u8 pad_0x0[0x30];
    void* field_0x30;
    UnkFrameWords_l701560 words;  // 0x38
    u8 pad_0x44[0x14];
    UnkCallback_l701560 finished;                      // 0x58
    u8 pad_0x68[0x9c];
    s32 mode;                                          // 0x104
    bool playing;                                      // 0x108
    u8 pad_0x109[0x47];
    u8 query[0x38040];                                 // 0x150
    u32 frame;                                         // 0x38190
    u8 pad_0x38194[0x24];
    bool reverse;                                      // 0x381b8
};
static_assert(__builtin_offsetof(UnkUnit1120Object_l701560, mode) == 0x104, "layout");
static_assert(__builtin_offsetof(UnkUnit1120Object_l701560, frame) == 0x38190, "layout");
static_assert(__builtin_offsetof(UnkUnit1120Object_l701560, reverse) == 0x381b8, "layout");

static inline u32 FrameCount(UnkFrameSource_l701560* source) {
    return source->GetChild(0)->GetCount();
}

bool fn_7100701560(UnkUnit1120Object_l701560* self, UnkFrameSource_l701560* source) {
    if (!source)
        return false;
    if (!self->playing || self->mode == kStepOnce) {
        u32 next = ++self->frame;
        if (next >= FrameCount(source)) {
            UnkCallback_l701560& finished = self->finished;
            if (finished.IsSet()) {
                finished.Invoke(self->field_0x30);
                finished.Clear();
            }
            return false;
        }
    } else if (self->mode == kStepPingPong) {
        u32 next = self->frame + (self->reverse ? -1 : 1);
        if (next >= FrameCount(source))
            self->reverse = !self->reverse;
        self->frame += self->reverse ? -1 : 1;
    } else if (self->mode == kStepLoop) {
        u32 next = ++self->frame;
        if (next >= FrameCount(source))
            self->frame = 0;
    }
    const UnkFrameWords_l701560* words = source->GetChild(0)->GetChild(self->frame)->GetWords();
    self->words.x = words->x;
    self->words.y = words->y;
    self->words.z = words->z;
    fn_71007071e0(reinterpret_cast<UnkSelf_nq41*>(self->query));
    return true;
}
