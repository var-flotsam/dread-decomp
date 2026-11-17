// Unit 1118: finds the item a step away from `index`, wrapping or bouncing at the ends of the list as
// the state's mode says, and reports the index it settled on. Names and layouts are placeholders.
#include "types.h"

// An object the state asks whether it is ready.
struct UnkReadyObject_b0169 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
};

// A list-like object: ready, size and element accessors.
struct UnkListObject_b0169 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    virtual s32 vf_0x28();
    virtual UnkReadyObject_b0169* vf_0x30(s32 index);
};

// How the selection behaves past the ends of the list.
enum UnkTU1118StepMode : s32 {
    UnkTU1118StepMode_Bounce = 2,
    UnkTU1118StepMode_Wrap = 3,
};

struct UnkTU1118Stepper {
    u8 pad_0x0[0xac];
    s32 current;                      // 0xac
    u8 pad_0xb0[4];
    s32 field_0xb4;                   // 0xb4
    u8 pad_0xb8[4];
    s32 step;                         // 0xbc
    u8 pad_0xc0[0x78];
    UnkListObject_b0169* list;        // 0x138
    u8 has_list;                      // 0x140
    u8 pad_0x141[3];
    UnkTU1118StepMode mode;           // 0x144
    u8 has_mode;                      // 0x148
    u8 pad_0x149[0xc7];
    u8 has_alt;                       // 0x210
    u8 pad_0x211[7];
    u8 alt_list[8];                   // 0x218, a list object held in place
    u8 pad_0x220[0x12a7c];
    u8 item_ready;                    // 0x12c9c

    // The list, when there is one and it is ready.
    UnkListObject_b0169* List() {
        UnkListObject_b0169* result = nullptr;
        if (has_list) {
            result = list;
            if (result && !result->vf_0x20())
                result = nullptr;
        }
        return result;
    }

    // The number of items in the list's first entry.
    static s32 Count(UnkListObject_b0169* list) {
        if (!list || !list->vf_0x20() || !list->vf_0x28())
            return 0;
        return reinterpret_cast<UnkListObject_b0169*>(list->vf_0x30(0))->vf_0x28();
    }

    // The item at `index` (counted from the end of the second list when negative), or null.
    UnkReadyObject_b0169* Item(UnkListObject_b0169* list, s32 index) {
        if (index >= 0) {
            if (!list || !list->vf_0x20() || !list->vf_0x28())
                return nullptr;
            list->vf_0x28();
            UnkListObject_b0169* sub = reinterpret_cast<UnkListObject_b0169*>(list->vf_0x30(0));
            if (sub->vf_0x28() <= index)
                return nullptr;
            return sub->vf_0x30(index);
        }
        if (!has_alt)
            return nullptr;
        UnkListObject_b0169* alt = reinterpret_cast<UnkListObject_b0169*>(alt_list);
        s32 at = alt->vf_0x28() + index;
        if (at < 1 || at >= alt->vf_0x28())
            return nullptr;
        return alt->vf_0x30(at);
    }
};
static_assert(__builtin_offsetof(UnkTU1118Stepper, alt_list) == 0x218, "");
static_assert(__builtin_offsetof(UnkTU1118Stepper, item_ready) == 0x12c9c, "");

// The item at `index`, reporting the index looked at.
static inline UnkReadyObject_b0169* ItemAt(UnkTU1118Stepper* state, UnkListObject_b0169* list, s32 index,
                                           s32* found_index) {
    UnkReadyObject_b0169* item = state->Item(list, index);
    if (found_index)
        *found_index = index;
    return item;
}

void* fn_71006fe430(void* self, void* target, s32 index, s32 arg, void* extra) {
    UnkTU1118Stepper* state = static_cast<UnkTU1118Stepper*>(self);
    UnkListObject_b0169* list = static_cast<UnkListObject_b0169*>(target);
    s32 step = arg;
    s32* found_index = static_cast<s32*>(extra);

    if (found_index)
        *found_index = -1;
    s32 next;
    if (index >= 0) {
        next = index + step;
    } else {
        next = index + 1;
        if (next == 0) {
            next = state->field_0xb4;
        } else {
            UnkReadyObject_b0169* item = ItemAt(state, list, next, found_index);
            if (item)
                return item;
        }
    }
    s32 count = UnkTU1118Stepper::Count(list);
    if (count >= 1) {
        if (next >= 0 && next < count)
            return ItemAt(state, list, next, found_index);
        if (!state->has_mode)
            return nullptr;
        if (state->mode == UnkTU1118StepMode_Wrap) {
            if (step >= 1)
                return ItemAt(state, list, 0, found_index);
            return ItemAt(state, list, count - 1, found_index);
        }
        if (state->mode == UnkTU1118StepMode_Bounce)
            return ItemAt(state, list, index - step, found_index);
        return nullptr;
    }
    return ItemAt(state, list, next, found_index);
}
