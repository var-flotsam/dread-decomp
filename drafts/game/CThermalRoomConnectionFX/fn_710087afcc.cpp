// CThermalRoomConnectionFX unit: while the factor (+0x7c) is above zero, plays the timeline ids that
// follow the connection's state (+0x100) and the linked actor's state (+0x108), then shows or hides the
// actor's (+0x30) effect from the other actor (+0x110). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CActorComponent.h"
#include "game/CCollisionMaterialCacheComponent.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

#include <atomic>

struct UnkStruct_710024fa0c;
extern const u64 g_unk_710164ae10;  // reached through the GOT
extern const u64 g_unk_710164ae18;  // reached through the GOT
extern const u64 g_unk_710164ae20;  // reached through the GOT
extern const u64 g_unk_710164ae28;  // reached through the GOT

struct UnkThermalConnState {
    u8 pad_0x0[0x10];
    std::atomic<s32> state;  // 0x10
};

struct UnkThermalConn {
    u8 pad_0x0[0x430];
    std::atomic<UnkThermalConnState*> state;  // 0x430
};

struct UnkThermalLinkInfo {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[0x3];
    bool active;  // 0x1b4
};

struct UnkThermalLink {
    u8 pad_0x0[0x30];
    UnkThermalLinkInfo* info;  // 0x30
    u8 pad_0x38[0x38];
    s32 state;  // 0x70
};

template <class T>
struct UnkThermalHandle {
    u8 pad_0x0[0x10];
    std::atomic<T*> object;  // 0x10
};

template <class T>
inline T* Get(UnkThermalHandle<T>* handle) {
    return handle != nullptr ? handle->object.load(std::memory_order_relaxed) : nullptr;
}

struct UnkThermalRoomConnectionFXUpdate {
    u8 pad_0x0[0x30];
    void* actor;  // 0x30
    u8 pad_0x38[0x44];
    f32 factor;  // 0x7c
    u8 pad_0x80[0x80];
    UnkThermalHandle<UnkThermalConn>* connection;      // 0x100
    UnkThermalHandle<UnkThermalLink>* linked;          // 0x108
    UnkThermalHandle<UnkThermalLink>* other;           // 0x110
    UnkThermalHandle<UnkTimelineComponent>* timeline;  // 0x118
    u8 open;            // 0x120
    u8 started;         // 0x121
    u8 linkedActive;    // 0x122
    u8 linkedSeen;      // 0x123
};
static_assert(__builtin_offsetof(UnkThermalRoomConnectionFXUpdate, open) == 0x120, "layout");

static inline bool IsInState(UnkThermalConn* connection, s32 state) {
    UnkThermalConnState* s = connection->state.load(std::memory_order_relaxed);
    return s != nullptr && s->state.load(std::memory_order_relaxed) == state;
}

static inline bool IsActive(UnkThermalLinkInfo* info) {
    return info->active && (!info->field_0x1b0 || info->field_0x1a8 != nullptr);
}

inline void PlayTimeline(UnkTimelineComponent* timeline, const u64* id) {
    base::reflection::UnkReflTypeIdArg value;
    value.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, id, &value.id, false, -1.0f);
}

void fn_710087afcc(UnkThermalRoomConnectionFXUpdate* self) {
    if (self->factor <= 1e-5f)
        return;
    if (Get(self->timeline) != nullptr) {
        if (!self->started) {
            if (Get(self->connection) != nullptr) {
                self->started = true;
                if (UnkThermalConnState* s = Get(self->connection)->state.load(std::memory_order_relaxed)) {
                    if (s->state.load(std::memory_order_relaxed) == 1) {
                        PlayTimeline(Get(self->timeline), &g_unk_710164ae10);
                        self->open = true;
                    } else if (s->state.load(std::memory_order_relaxed) == 2) {
                        PlayTimeline(Get(self->timeline), &g_unk_710164ae18);
                        self->open = false;
                    }
                }
            }
        }
        if (Get(self->connection) == nullptr && !self->open) {
            PlayTimeline(Get(self->timeline), &g_unk_710164ae10);
        } else if (UnkThermalConn* connection = Get(self->connection)) {
            if (IsInState(connection, 1) && !self->open) {
                PlayTimeline(Get(self->timeline), &g_unk_710164ae10);
                self->open = true;
            } else if (IsInState(connection, 2) && self->open) {
                PlayTimeline(Get(self->timeline), &g_unk_710164ae18);
                self->open = false;
            }
        }
        if (!self->linkedSeen) {
            if (Get(self->linked) != nullptr) {
                self->linkedSeen = true;
                s32 state = Get(self->linked)->state;
                UnkTimelineComponent* timeline = Get(self->timeline);
                if (state == 2) {
                    PlayTimeline(timeline, &g_unk_710164ae20);
                    self->linkedActive = true;
                } else {
                    PlayTimeline(timeline, &g_unk_710164ae28);
                    self->linkedActive = false;
                }
            }
        }
    }
    if (UnkThermalLink* other = Get(self->other)) {
        if (other->state != 2 && IsActive(other->info)) {
            if (UnkStruct_710024fa0c* fx = fn_7100798eec(self->actor))
                fn_710024fa0c(fx, false);
            return;
        }
        if (UnkStruct_710024fa0c* fx = fn_7100798eec(self->actor))
            fn_710024fa0c(fx, true);
        if (Get(self->connection) == nullptr && !self->linkedSeen) {
            self->linkedActive = true;
            self->linkedSeen = true;
            fn_7100d7c828(reinterpret_cast<UnkActor*>(self->actor), &g_unk_710164ae20, false);
        }
    }
}
