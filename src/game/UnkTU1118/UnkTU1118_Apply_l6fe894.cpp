// Unit 1118 (no class hint): an AI state takes a parameter block; for blocks of kind 2 or 4 that carry a
// ready target, it resets its tracked indices and path, and when the owner's navigation record names this
// state it hands its own ready object on (fn_71006fc7c4). Names and layouts are placeholders.
#include "game/UnkAIComponent.h"
#include "game/UnkTU1124.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

// An object asked whether it is ready (its slot at +0x20).
struct UnkReadyObject_l6fe894 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
};

// The parameter block the state receives.
struct UnkParams_l6fe894 {
    u8 pad_0x0[0x34];
    u32 kind;  // 0x34
    UnkReadyObject_l6fe894* target;  // 0x38
    bool has_target;  // 0x40
};
static_assert(__builtin_offsetof(UnkParams_l6fe894, has_target) == 0x40, "layout");

struct UnkNavEntry_l6fe894 {
    u8 pad_0x0[0x10];
    void* state;  // 0x10
};

struct UnkAIState_l6fe894 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x74];
    s32 current;  // 0xac
    s32 previous;  // 0xb0
    s32 field_0xb4;
    u8 pad_0xb8[0x48];
    u8 params[0x38];  // 0x100
    UnkReadyObject_l6fe894* ready;  // 0x138
    bool has_ready;  // 0x140
    u8 pad_0x141[0xc7];
    u8 path[8];  // 0x208
};
static_assert(__builtin_offsetof(UnkAIState_l6fe894, ready) == 0x138, "layout");
static_assert(__builtin_offsetof(UnkAIState_l6fe894, path) == 0x208, "layout");

struct UnkFlyingPathParams_l387b8c;
void fn_7100387b8c(UnkFlyingPathParams_l387b8c* self, const UnkFlyingPathParams_l387b8c* other, bool reset);
void fn_71006fc7c4(UnkAIState_l6fe894* self, UnkReadyObject_l6fe894* ready);

// The state the owner's navigation currently runs (null without one).
static inline void* NavigationState(UnkActor* owner) {
    if (!owner)
        return nullptr;
    UnkAIComponent* ai = static_cast<UnkAIComponent*>(owner->field_0x240);
    if (!ai)
        return nullptr;
    UnkNavigation* navigation = fn_71010fccf8(ai);
    if (!navigation)
        return nullptr;
    UnkNavEntry_l6fe894* entry = *static_cast<UnkNavEntry_l6fe894**>(navigation->field_0xb0);
    return entry ? entry->state : nullptr;
}

// Declared by its callers with these types (the block is the event type of unit 1119).
struct UnkUnit1119Event_b54;

void fn_71006fe894(char* state, UnkUnit1119Event_b54* event, bool reset) {
    UnkAIState_l6fe894* self = reinterpret_cast<UnkAIState_l6fe894*>(state);
    UnkParams_l6fe894* params = reinterpret_cast<UnkParams_l6fe894*>(event);
    fn_71007038f4(self, params, reset);
    if (params->kind == 0 || params->kind == 2 || params->kind == 4) {
        fn_7100387b8c(reinterpret_cast<UnkFlyingPathParams_l387b8c*>(self->params),
                      reinterpret_cast<const UnkFlyingPathParams_l387b8c*>(params), reset);
        if (params->kind != 4 && params->kind != 2)
            return;
        if (!params->has_target)
            return;
        UnkReadyObject_l6fe894* target = params->target;
        if (!target || !target->vf_0x20())
            return;
        self->current = 0x80000001;
        self->previous = 0x80000001;
        self->field_0xb4 = 0x80000001;
        fn_71002adc8c(self->path);
        if (NavigationState(self->owner) != self)
            return;
        UnkReadyObject_l6fe894* ready = nullptr;
        if (self->has_ready) {
            ready = self->ready;
            if (ready && !ready->vf_0x20())
                ready = nullptr;
        }
        fn_71006fc7c4(self, ready);
    }
}
