// behaviortree::CVulkranChaseAction's unit (tools/tu_map.py unit 1082): the handler of an animation event
// (kinds 1 and 2; fn_71006d808c): when the event names the action's animation (+0x40) and the owner's AI
// (+0x30) and the animation set (+0x38) are there, starts the chase (fn_71006d7f78), tells the AI
// (fn_71006dcdc8) and sets the delay at +0x60: a random one between the set's bounds (+0x6b8, +0x6bc)
// for the set's two animations (fn_71006dac40, fn_71006dac4c), else 0.002. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkVulkranChaseAction;
bool fn_71006d7f78(UnkVulkranChaseAction* self);

struct UnkVulkranTarget_ovb0120;

using base::global::UnkStringData;

extern "C" int rand();
struct UnkVulkranAnims_ovb0120;
void fn_71006dcdc8(UnkVulkranTarget_ovb0120* target);
UnkStringData** fn_71006dac40(UnkVulkranAnims_ovb0120* anims);
UnkStringData** fn_71006dac4c(UnkVulkranAnims_ovb0120* anims);

struct UnkVulkranAnimsRange_ovb0120 {
    u8 pad_0x0[0x6b8];
    f32 min_delay;  // 0x6b8
    f32 max_delay;  // 0x6bc
};

struct UnkVulkranEventSource_ovb0120 {
    u8 pad_0x0[0x38];
    UnkStringData* name;  // 0x38
};

struct UnkVulkranEventHolder_ovb0120 {
    u8 pad_0x0[0x10];
    UnkVulkranEventSource_ovb0120* source;
};

struct UnkVulkranEvent_ovb0120 {
    s32 kind;
    UnkVulkranEventHolder_ovb0120* holder;
};

struct UnkVulkranChaseEv_ovb0120 {
    u8 pad_0x0[0x30];
    UnkVulkranTarget_ovb0120* target;  // 0x30
    UnkVulkranAnims_ovb0120* anims;  // 0x38
    UnkStringData* name;  // 0x40
    u8 pad_0x48[0x12];
    bool field_0x5a;
    u8 pad_0x5b[5];
    f32 delay;  // 0x60
};

static inline bool SameId(UnkStringData* const* a, UnkStringData* const* b) { return *a == *b; }

bool fn_71006d808c(UnkVulkranChaseEv_ovb0120* self, const UnkVulkranEvent_ovb0120* event) {
    if (event->kind == 1 || event->kind == 2) {
        UnkStringData* const* name = &g_unk_7101d470e0.data;
        if (UnkVulkranEventHolder_ovb0120* holder = event->holder)
            name = holder->source ? &holder->source->name : &g_unk_7101d470e0.data;
        if (*name != &g_unk_7101d47108->empty && self->target && self->anims && *name == self->name) {
            self->field_0x5a = true;
            fn_71006d7f78(reinterpret_cast<UnkVulkranChaseAction*>(self));
            fn_71006dcdc8(self->target);
            f32 delay;
            if (SameId(name, fn_71006dac40(self->anims)) || SameId(name, fn_71006dac4c(self->anims))) {
                if (UnkVulkranAnimsRange_ovb0120* range = reinterpret_cast<UnkVulkranAnimsRange_ovb0120*>(self->anims)) {
                    f32 lo = range->min_delay;
                    f32 hi = range->max_delay;
                    f32 r = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
                    delay = lo + (hi - lo) * r;
                } else {
                    delay = -1.0f;
                }
            } else {
                delay = 0.002f;
            }
            self->delay = delay;
            *reinterpret_cast<s32*>(reinterpret_cast<u8*>(self->target) + 0x146e0) = 0;
        }
    }
    return true;
}
