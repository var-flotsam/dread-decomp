// Unit 1120 (tools/tu_map.py, no reflected class): the parameter-block handler. After the shared step
// (fn_71007038f4), a block of kind 0 or 7 is merged into the object's own (+0xb0, fn_7100388570); a changed
// target or frame source, or a raised restart flag, restarts the path query, and the optional frame and
// ranges are applied. Names and layouts are placeholders.
#include "game/UnkTU1124.h"
#include "types.h"

struct UnkSelf_nq41;
void fn_71007071e0(UnkSelf_nq41* self);

template <typename T>
struct UnkOptional_l701714 {
    T value;
    bool has_value;

    T ValueOr(T fallback) const { return has_value ? value : fallback; }
};

// A one-shot restart request: reading it lowers it. True while no restart was asked for.
struct UnkRequest_l701714 {
    UnkOptional_l701714<bool> flag;

    bool Settled() {
        bool settled = true;
        if (flag.has_value) {
            settled = !flag.value;
            flag.value = false;
            flag.has_value = true;
        }
        return settled;
    }
};

struct UnkFrameWords_l701714 {
    u32 x;
    u32 y;
    u32 z;
};

// A frame source, as fn_7100701560 steps through it.
struct UnkFrameSource_l701714 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual const UnkFrameWords_l701714* GetWords();      // 0x18
    virtual void vf_0x20();
    virtual u32 GetCount();                               // 0x28
    virtual UnkFrameSource_l701714* GetChild(u32 index);  // 0x30
};

struct UnkRange_l701714 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
};

// The kind-7 parameter block (layout as game_s2/UnkParams7.h).
struct UnkParams_l701714 {
    void** vtable;
    u8 pad_0x8[0x2c];
    u32 kind;                                                  // 0x34
    UnkOptional_l701714<void*> target;                         // 0x38
    UnkOptional_l701714<UnkFrameSource_l701714*> source;       // 0x48
    u8 pad_0x58[0x3];
    UnkRequest_l701714 restart_target;                         // 0x5b
    UnkRequest_l701714 restart_source;                         // 0x5d
    u8 pad_0x5f[0x9];
    UnkOptional_l701714<UnkRange_l701714> range_a;             // 0x68
    u8 pad_0x7c[0x4];
    UnkOptional_l701714<UnkRange_l701714> range_b;             // 0x80
    UnkOptional_l701714<u32> frame;                            // 0x94
};
static_assert(__builtin_offsetof(UnkParams_l701714, restart_target) == 0x5b, "layout");
static_assert(__builtin_offsetof(UnkParams_l701714, range_a) == 0x68, "layout");
static_assert(__builtin_offsetof(UnkParams_l701714, frame) == 0x94, "layout");

void fn_7100388570(UnkParams_l701714* self, UnkParams_l701714* other, bool reset);

struct UnkUnit1120Object_l701714 {
    u8 pad_0x0[0x38];
    UnkFrameWords_l701714 words;  // 0x38
    u8 pad_0x44[0x6c];
    UnkParams_l701714 params;     // 0xb0
    u8 query[0x38040];            // 0x150
    u32 frame;                    // 0x38190
    u8 pad_0x38194[4];
    UnkRange_l701714 range_a;     // 0x38198
    UnkRange_l701714 range_b;     // 0x381a8

    void Restart() {
        fn_71007071e0(reinterpret_cast<UnkSelf_nq41*>(query));
        frame = -1;
    }
};
static_assert(__builtin_offsetof(UnkUnit1120Object_l701714, query) == 0x150, "layout");
static_assert(__builtin_offsetof(UnkUnit1120Object_l701714, range_b) == 0x381a8, "layout");

// Whether a pointer changed between two values, either of them possibly null.
template <typename T>
static inline bool Changed(T* before, T* after) {
    return (before != nullptr || after != nullptr) & (before != after);
}

void fn_7100701714(UnkUnit1120Object_l701714* self, UnkParams_l701714* params, bool reset) {
    fn_71007038f4(self, params, reset);
    if (params->kind != 7 && params->kind != 0)
        return;

    void* old_target = self->params.target.ValueOr(nullptr);
    UnkFrameSource_l701714* old_source = self->params.source.ValueOr(nullptr);
    fn_7100388570(&self->params, params, reset);

    void* target = self->params.target.ValueOr(nullptr);
    bool settled = self->params.restart_target.Settled();
    if (Changed(old_target, target) || !settled)
        self->Restart();

    UnkFrameSource_l701714* source = self->params.source.ValueOr(nullptr);
    settled = self->params.restart_source.Settled();
    if (Changed(old_source, source) || !settled)
        self->Restart();

    if (self->params.frame.has_value) {
        self->frame = self->params.frame.value;
        const UnkFrameWords_l701714* words = source->GetChild(0)->GetChild(self->frame)->GetWords();
        self->words.x = words->x;
        self->words.y = words->y;
        self->words.z = words->z;
    }
    if (self->params.range_a.has_value)
        self->range_a = self->params.range_a.value;
    if (self->params.range_b.has_value)
        self->range_b = self->params.range_b.value;
}
