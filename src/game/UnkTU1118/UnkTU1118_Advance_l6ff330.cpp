// Unit 1118: steps the state's selection by its step, wrapping or bouncing at the ends of the list as
// its mode says, records whether the item there is ready and selects it. Names and layouts are placeholders.
#include "game/UnkListObject_b0169.h"
#include "game/UnkReadyObject_b0169.h"
#include "types.h"

struct UnkAIStateSelect_b0169;
bool fn_71006fe16c(UnkAIStateSelect_b0169* self, s32 index);

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

bool fn_71006ff330(UnkTU1118Stepper* self) {
    s32 step = self->step;
    s32 target;
    if (self->current < 0) {
        target = self->current + 1;
        if (target == 0)
            target = self->field_0xb4;
    } else {
        target = self->current + step;
        s32 count = UnkTU1118Stepper::Count(self->List());
        if (target < 0 || target >= count) {
            if (!self->has_mode || self->mode == UnkTU1118StepMode_Wrap) {
                if (self->step > 0)
                    target = 0;
                else if (count > 0)
                    target = count - 1;
            } else if (self->mode == UnkTU1118StepMode_Bounce) {
                step = -self->step;
                target = self->current + step;
            }
        }
        UnkListObject_b0169* list = self->List();
        UnkReadyObject_b0169* item;
        if (step >= 0)
            item = self->Item(list, self->current);
        else
            item = self->Item(list, target);
        self->item_ready = item->vf_0x20();
    }
    self->step = step;
    return fn_71006fe16c(reinterpret_cast<UnkAIStateSelect_b0169*>(self), target);
}
