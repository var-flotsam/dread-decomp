// CAnimationComponent's unit (tools/tu_map.py): an event starting. Adds its key to the key lists it
// names, tells the queued listeners (kind 3), then copies or re-anchors the owner's transform and
// restarts the owner's effect as the event's flags ask. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkAnimEventData_a190;
void fn_7100d7c5c0(void* owner, UnkAnimEventData_a190* data);

struct UnkAnimEffect;
void fn_7100143618(UnkAnimEffect* self, bool flag, f32 value);

__attribute__((visibility("hidden"))) extern u32 g_unk_7172a0ae60;  // flag bits a new matrix clears

// The component's listener lists (+0x438), as UnkAnimListeners_b52 (CAnimationComponent_b52.cpp).
struct UnkAnimListeners_a190 {
    void** active;
    u32 active_count;
    u8 pad_0xc[0x1c];
    void** pending;
    u32 pending_count;
    u8 pad_0x34[0x1c];
    void** queued;    // 0x50
    u32 queued_count; // 0x58
    u8 pad_0x5c[0x1c];
    union {
        s32 lock;  // 0x78
    };
};

struct UnkAnimListenersLock_a190 {
    UnkAnimListeners_a190* listeners;

    explicit UnkAnimListenersLock_a190(UnkAnimListeners_a190* l);
    ~UnkAnimListenersLock_a190();
};

struct UnkAnimListenersLock;
void fn_710117ecb0(UnkAnimListenersLock* self);

inline UnkAnimListenersLock_a190::UnkAnimListenersLock_a190(UnkAnimListeners_a190* l) {
    listeners = l;
    ++l->lock;
}

inline UnkAnimListenersLock_a190::~UnkAnimListenersLock_a190() {
    fn_710117ecb0(reinterpret_cast<UnkAnimListenersLock*>(this));
}

struct UnkAnimListeners_b52;
void fn_710117eb1c(UnkAnimListeners_b52* self, void* value);
struct UnkPairOwner;
struct UnkValueSource;
UnkValueSource** fn_71002dba48(UnkPairOwner* self);

// A copied string id released in two steps, as CStrId::Reset does.
struct UnkMsgName_a190 : base::global::UnkStrIdBase {
    UnkMsgName_a190(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkMsgName_a190() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// What the listeners are told: kind 3, the event's data and the source's name.
struct UnkAnimEventMsg_a190 {
    u32 kind;
    UnkAnimEventData_a190* info;
    UnkMsgName_a190 name;

    UnkAnimEventMsg_a190(UnkAnimEventData_a190* info_, const CStrId& name_)
        : kind(3), info(info_), name(name_) {}
};

struct UnkAnimListener_a190 {
    u8 pad_0x0[8];
    void* context;
    bool (*callback)(void* context, UnkAnimEventMsg_a190* msg);
};

struct UnkAnimEventInfo_a190 {
    u8 pad_0x0[0x34];
    u32 keys;  // bit 0: the first key list, bit 1: the second
    CStrId key;
};

struct UnkAnimEventData_a190 {
    u8 pad_0x0[0x10];
    UnkAnimEventInfo_a190* info;
};

enum : u32 {
    kEventRestartEffect = 1u << 7,
    kEventCopyXform = 1u << 11,
    kEventKeepPosition = 1u << 12,
};

struct UnkAnimEvent_a190 {
    u32 flags;
    u8 pad_0x4[0xc];
    UnkAnimEventData_a190* data;  // 0x10
};

struct UnkAnimSource_a190 {
    u8 pad_0x0[0x7e];
    bool adds_keys;
};

struct UnkAnimEffectHolder_a190 {
    u8 pad_0x0[0x50];
    UnkAnimEffect* effect;
};

struct UnkAnimOwner_a190 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x228 - 0x94];
    UnkAnimEffectHolder_a190* effects;  // 0x228
};

struct UnkAnimComponent_a190 {
    u8 pad_0x0[0x30];
    UnkAnimOwner_a190* owner;  // 0x30
    u8 pad_0x38[0x258 - 0x38];
    UnkXform xform;  // 0x258
    u8 pad_0x2b4[0x2d0 - 0x2b4];
    UnkGuiKeyList keys_0x2d0;
    u8 pad_0x300[0x80];
    UnkGuiKeyList keys_0x380;
    u8 pad_0x3b0[0x88];
    UnkAnimListeners_a190 listeners;  // 0x438
};
static_assert(__builtin_offsetof(UnkAnimComponent_a190, listeners) == 0x438, "layout");

// Word-by-word assignment of a whole transform.
static inline void AssignXform(UnkXform* dst, const UnkXform& src) {
    if (dst == &src)
        return;
    UnkCopyMatrix(reinterpret_cast<UnkMatrix44*>(dst->m), *reinterpret_cast<const UnkMatrix44*>(src.m));
    dst->angle = src.angle;
    dst->scale = src.scale;
    dst->flags = src.flags;
}

void fn_710117a190(UnkAnimComponent_a190* self, UnkAnimSource_a190* source, UnkAnimEvent_a190* event) {
    fn_7100d7c5c0(self->owner, event->data);
    {
        UnkAnimEventMsg_a190 msg(
            event->data, *reinterpret_cast<CStrId*>(fn_71002dba48(reinterpret_cast<UnkPairOwner*>(source))));
        UnkAnimListenersLock_a190 lock(&self->listeners);
        void** end = self->listeners.queued + self->listeners.queued_count;
        for (void** it = self->listeners.queued; it != end; ++it) {
            UnkAnimListener_a190* listener = static_cast<UnkAnimListener_a190*>(*it);
            if (!listener || !listener->callback || !listener->callback(listener->context, &msg))
                fn_710117eb1c(reinterpret_cast<UnkAnimListeners_b52*>(&self->listeners), listener);
        }
    }

    if (source->adds_keys) {
        if (event->data->info->keys & 1)
            fn_710002fd10(&self->keys_0x2d0, &event->data->info->key);
        if (event->data->info->keys & 2)
            fn_710002fd10(&self->keys_0x380, &event->data->info->key);
    }

    if (event->flags & kEventCopyXform) {
        AssignXform(&self->xform, self->owner->xform);
    } else if (event->flags & kEventKeepPosition) {
        // The component's own matrix, moved to the owner's position.
        const UnkMatrix44& from = self->owner->xform.GetMatrix();
        f32 x = from.m[3];
        f32 y = from.m[7];
        f32 z = from.m[11];
        UnkMatrix44 m = self->xform.GetMatrix();
        m.m[3] = x;
        m.m[7] = y;
        m.m[11] = z;
        __builtin_memcpy(self->xform.m, &m, sizeof(m));
        self->xform.flags = (self->xform.flags | 8) & ~g_unk_7172a0ae60;
    }

    if (event->flags & kEventRestartEffect) {
        UnkAnimEffectHolder_a190* effects = self->owner->effects;
        if (effects && effects->effect)
            fn_7100143618(effects->effect, true, 0.0f);
    }
}
