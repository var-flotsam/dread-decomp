// CAnimationComponent's unit (tools/tu_map.py): a track has started an action. The component's listeners
// (+0x438) hear of it first (a listener that is gone or answers false is removed); then, while the track
// still plays that action, an action of kind 2 or 3 is followed by its own request to the track. Names and
// layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkCountedDeferredSet.h"
#include "base/global/CStrId.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimTime_ovb0009.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkS12.h"
#include "game/UnkS16.h"
#include "game/animtree/CAnimTreeElement.h"
#include "types.h"

using base::global::CStrId;

// A request's mode (passed by value, through memory).
struct UnkFlags_7101176ea8 {
    u32 value;
    UnkFlags_7101176ea8(u32 v) : value(v) {}
    UnkFlags_7101176ea8(const UnkFlags_7101176ea8& o) : value(o.value) {}
    UnkFlags_7101176ea8& operator=(const UnkFlags_7101176ea8& o) {
        value = o.value;
        return *this;
    }
};

// What an action of a track was started with.
struct UnkActionInfo_l17a620 {
    u8 pad_0x0[0x14];
    s32 kind;
    u8 pad_0x18[4];
    s32 time_unit;  // 0x1c
    u8 pad_0x20[0x10];
    f32 time;       // 0x30
    u32 flags;      // 0x34
    u8 pad_0x38[0x10];
    void* name;     // 0x48: the action to request
};

struct UnkStartedAction_l17a620 {
    u8 pad_0x0[0x10];
    UnkActionInfo_l17a620* info;
};

// The track and the action it started.
struct UnkActionStarted_l17a620 {
    UnkTrack* track;
    UnkTrackAction* action;
};

static inline UnkActionInfo_l17a620* Info(UnkTrackAction* action) {
    return reinterpret_cast<UnkStartedAction_l17a620*>(action)->info;
}

// What the listeners are told.
struct UnkAnimEvent_l17a620 {
    s32 kind;
    UnkTrackAction* action;
    CStrId name;

    UnkAnimEvent_l17a620(UnkTrackAction* a, const CStrId& n) : kind(1), action(a), name(n) {}
};

struct UnkAnimListener_l17a620 {
    u8 pad_0x0[0x8];
    void* context;
    bool (*callback)(void* context, UnkAnimEvent_l17a620* event);
};

struct UnkClassActions_l17a620 {
    u8 pad_0x0[0x30];
    void* name;
};

struct UnkCharClass_l17a620 {
    u8 pad_0x0[0x2b8];
    UnkClassActions_l17a620* actions;
};

struct UnkAnimComponent_l17a620 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x438 - 0x38];
    base::UnkCountedDeferredSet<void> listeners;
};

static inline const UnkS12& DefaultBlend() { return reinterpret_cast<const UnkS12&>(g_unk_7172769740); }

static inline UnkS16 ActionTime(const UnkActionStarted_l17a620* started) {
    UnkS16 at;
    at.key = static_cast<u64>(-1);
    at.time.value = Info(started->action)->time;
    at.time.unit = Info(started->action)->time_unit;
    return at;
}

void fn_710117a620(UnkAnimComponent_l17a620* self, UnkPairOwner* source, UnkActionStarted_l17a620* started) {
    {
        UnkAnimEvent_l17a620 event(started->action, *reinterpret_cast<CStrId*>(fn_71002dba48(source)));
        base::UnkCountedDeferredWalk<void> walk(&self->listeners);
        for (void* item : self->listeners.items) {
            auto* listener = static_cast<UnkAnimListener_l17a620*>(item);
            if (!listener || !listener->callback || !listener->callback(listener->context, &event))
                fn_710117eb1c(&self->listeners, listener);
        }
        fn_710117ecb0(&walk);
    }

    UnkTrackAction* playing = fn_710070d500(started->track);
    if (playing != started->action)
        return;
    UnkStruct_7101176ea8* target = reinterpret_cast<UnkStruct_7101176ea8*>(self);
    switch (Info(playing)->kind) {
    case 3: {
        auto* char_class = static_cast<UnkCharClass_l17a620*>(self->owner->field_0x220);
        void* action = fn_7101176d34(target, char_class->actions->name);
        fn_7101177044(target, action, -1, started->track, UnkFlags_7101176ea8(0x201), ActionTime(started),
                      DefaultBlend(), UnkFlags_7101176ea8(UnkAll_SpitSend().value));
        break;
    }
    case 2: {
        u32 mode = (Info(playing)->flags & 4) ? 0x381 : 0x301;
        void* action = fn_7101176d34(target, Info(playing)->name);
        fn_7101177044(target, action, -1, started->track, UnkFlags_7101176ea8(mode), ActionTime(started),
                      DefaultBlend(), UnkFlags_7101176ea8(UnkAll_SpitSend().value));
        break;
    }
    }
}