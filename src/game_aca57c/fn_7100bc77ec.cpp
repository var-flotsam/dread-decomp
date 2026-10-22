// Four conditions on an actor's mode (the condition's mode at `Offset`: 0 always holds, 1 and
// 2 must equal the current value). The value comes from the global manager while it is in a
// given state (fn_7100df5c38), else from the actor's component found by fn_7100bc7938, else
// from the object at 0x240 through vf_0x300 (when vf_0x2f8 allows it).
#include "types.h"

struct UnkModeManager {
    u8 unk_0x0[0xc0];
    s32 mode;
    s32 field_0xc4;
    s32 field_0xc8;
    u8 unk_0xcc[0x40];
    bool field_0x10c;
};

struct UnkModeComponent {
    u8 unk_0x0[0x579];
    bool enabled;
    u8 unk_0x57a[0x2];
    s32 mode;
};

// Only two slots of a large vtable are used: vf_0x2f8 and vf_0x300.
struct UnkModeSource {
    void** vtable;

    bool Allows() { return reinterpret_cast<bool (*)(UnkModeSource*)>(vtable[0x2f8 / 8])(this); }
    s32 Mode() { return reinterpret_cast<s32 (*)(UnkModeSource*)>(vtable[0x300 / 8])(this); }
};

struct UnkModeActor {
    u8 unk_0x0[0x240];
    UnkModeSource* source;
};

extern UnkModeManager* g_unk_7101d47310;
bool fn_7100df5c38(UnkModeManager* manager);
UnkModeComponent* fn_7100bc7938(UnkModeActor* actor);

template <u32 Offset>
struct UnkModeCondition {
    u8 unk_0x0[Offset];
    s32 mode;

    __attribute__((always_inline)) bool Matches(const s32& value) const {
        switch (mode) {
        case 1:
            if (value != 1)
                return false;
            break;
        case 2:
            if (value != 2)
                return false;
            break;
        }
        return true;
    }

    __attribute__((always_inline)) bool Check(UnkModeActor* actor) const {
        if (mode == 0)
            return true;
        if (fn_7100df5c38(g_unk_7101d47310)) {
            UnkModeManager* manager = g_unk_7101d47310;
            if (manager->field_0xc8 == 0 && !manager->field_0x10c)
                return false;
            return Matches(manager->mode);
        }
        if (UnkModeComponent* component = fn_7100bc7938(actor)) {
            if (!component->enabled)
                return false;
            return Matches(component->mode);
        }
        UnkModeSource* source = actor->source;
        if (!source)
            return true;
        if (!source->Allows())
            return false;
        if (mode == 1 && source->Mode() != 1)
            return false;
        if (mode == 2 && source->Mode() != 2)
            return false;
        return true;
    }
};

bool fn_7100bc77ec(const UnkModeCondition<0x18>* self, UnkModeActor* actor) { return self->Check(actor); }
bool fn_7100bc8e88(const UnkModeCondition<0x18>* self, UnkModeActor* actor) { return self->Check(actor); }
bool fn_7100bc9f90(const UnkModeCondition<0x30>* self, UnkModeActor* actor) { return self->Check(actor); }
bool fn_7100c45dc8(const UnkModeCondition<0x30>* self, UnkModeActor* actor) { return self->Check(actor); }
