// CAIManager::CTunableAIManager's unit: starts or ends the record's tracking of `target`. Starting
// clears the timer and keeps a weak reference to the target; ending notes whether the target changed
// too soon, keeps the previous and the final target, the elapsed time, and tells every registered
// listener. Names and layouts are placeholders.
#include "game_s2/UnkChargeComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkListener_daec80 {
    void** vtable;

    void vf_0xe0() { reinterpret_cast<void (*)(UnkListener_daec80*)>(vtable[0xe0 / 8])(this); }
};

struct UnkListenerOwner_daec80 {
    u8 pad_0x0[0x18];
    UnkListener_daec80* listener;
};

struct UnkListenerEntry_daec80 {
    u8 pad_0x0[0x10];
    UnkListenerOwner_daec80* owner;
};

struct UnkListenerList_daec80 {
    UnkListenerEntry_daec80** data;
    u32 count;

    UnkListenerEntry_daec80** begin() { return data; }
    UnkListenerEntry_daec80** end() { return data + count; }
};

UnkListenerList_daec80* fn_7100502868();

struct UnkAIRecord_daec80 {
    u8 unk_0x0[0x118];
    UnkWeakHandle* target;  // 0x118
    f32 time;               // 0x120
    bool changed_early;     // 0x124
    UnkWeakHandle* previous_target;  // 0x128
    UnkWeakHandle* final_target;     // 0x130
    f32 final_time;                  // 0x138
};
static_assert(__builtin_offsetof(UnkAIRecord_daec80, final_time) == 0x138, "layout");

struct UnkEmmyRegistry_b55;

void CTunableAIManager_StartOrEndEmmyDoorWalkThrough(UnkEmmyRegistry_b55* registry, void* actor, bool start) {
    auto* self = reinterpret_cast<UnkAIRecord_daec80*>(registry);
    auto* target = static_cast<UnkWeakTarget*>(actor);
    if (start) {
        self->time = 0.0f;
        self->changed_early = false;
        AssignRef(&self->target, target ? GetWeakHandle(target) : nullptr);
        return;
    }
    UnkWeakHandle* current = self->target;
    if ((current ? current->target : nullptr) != target &&
        self->time < g_unk_71726e0aa8.field_0xfc)
        self->changed_early = true;
    CopyWeakRef(&self->previous_target, current);
    AssignRef(&self->final_target, target ? GetWeakHandle(target) : nullptr);
    self->final_time = self->time;
    self->time = 0.0f;
    ResetRef(&self->target);
    for (UnkListenerEntry_daec80* entry : *fn_7100502868()) {
        if (UnkListener_daec80* listener = entry->owner->listener)
            listener->vf_0xe0();
    }
}
