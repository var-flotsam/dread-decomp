// CAnimationComponent's unit (tools/tu_map.py): resets the component from its character class
// (CCharClassAnimationComponent, +0x10): clears its lists, takes the class's default action names and,
// when it has tracks, requests the class's start action (or the default one) on the main track; a
// request that is taken clears a flag of the owner's +0x218 object. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU39.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

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

// A time on an animation's clock (src/game/UnkAnimTime.cpp): unit 1 is seconds.
struct UnkAnimTime_ovb0009 {
    f32 value;
    s32 unit;
};

// A key and a time.
struct UnkS16 {
    u64 key;
    UnkAnimTime_ovb0009 time;
};

// A blend descriptor (g_unk_7172769740, g_unk_7172769770).
struct UnkS12 {
    u32 a, b, c;
};

struct UnkAnimList_l176214;
void fn_71002df844(UnkTrackSet* tracks);
void fn_710070f098(UnkAnimList_l176214* list, u32 value, bool flag);

// A linked list of nodes from its own allocator.
struct UnkAnimListNode_l176214 {
    u8 pad_0x0[0x8];
    UnkAnimListNode_l176214* next;
};

struct UnkAnimList_l176214 {
    u8 pad_0x0[0x8];
    u32 count;
    UnkAnimListNode_l176214* head;
    UnkAnimListNode_l176214* tail;
    base::UnkAllocator* allocator;

    void Clear() {
        for (UnkAnimListNode_l176214* node = head; node;) {
            UnkAnimListNode_l176214* next = node->next;
            allocator->vf_0x48(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkAnimCharClass_l176214 : UnkReflObject {
    u8 pad_0x8[0x58 - 0x8];
    CStrId start_action;    // 0x58
    CStrId default_action;  // 0x60
    u8 pad_0x68[0x8];
    u32 field_0x70;
    u8 pad_0x74[4];
    u32 field_0x78;
};

struct UnkAnimOwnerPart_l176214 {
    u8 pad_0x0[0x1c09];
    bool field_0x1c09;
};

struct UnkAnimOwner_l176214 {
    u8 pad_0x0[0x218];
    UnkAnimOwnerPart_l176214* field_0x218;
};

struct UnkAnimComponent_l176214 {
    u8 pad_0x0[0x10];
    UnkReflObject* char_class;
    u8 pad_0x18[0x30 - 0x18];
    UnkAnimOwner_l176214* owner;
    u8 pad_0x38[0x8];
    UnkTrackSet* tracks;  // 0x40
    u8 pad_0x48[0x2b4 - 0x48];
    u32 field_0x2b4;
    CStrId action_0x2b8;
    CStrId action_0x2c0;
    u8 pad_0x2c8[0x8];
    u8 list_0x2d0[0xb0];
    u8 list_0x380[0x138];
    bool field_0x4b8;
    bool field_0x4b9;
    u8 pad_0x4ba[6];
    UnkAnimList_l176214 list_0x4c0;
};
static_assert(__builtin_offsetof(UnkAnimComponent_l176214, list_0x4c0) == 0x4c0, "layout");

// The name hash of the actor's main track.
static const u64 kMainTrack = 0xf4061ff25a2163d6;

static inline const UnkS16& DefaultTime() { return reinterpret_cast<const UnkS16&>(g_unk_710164b968); }

static inline bool IsEmpty(const CStrId& id) {
    return &g_unk_7101d47108->empty == id.data;
}

static inline void* TrackData(UnkAnimComponent_l176214* self, u64 key) {
    UnkTrackSet* set = self->tracks;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    return (it != set->field_0xc8.End() && *it) ? (*it)->field_0x70 : nullptr;
}

// fn_7101176690 inlined: the action `id` names to the track whose name hash is `key`.
static inline bool SendAction(UnkAnimComponent_l176214* self, u64 id, u64 key, u32 flags, const UnkS16& a,
                              const UnkS12& b, u32 mask) {
    UnkStruct_7101176ea8* target = reinterpret_cast<UnkStruct_7101176ea8*>(self);
    void* r = fn_7101176d34(target, reinterpret_cast<void*>(id));
    return fn_7101177044(target, r, -1, TrackData(self, key), flags, a, b, mask);
}

void fn_7101176214(UnkAnimComponent_l176214* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    UnkAnimCharClass_l176214* char_class = static_cast<UnkAnimCharClass_l176214*>(self->char_class);
    if (char_class && !fn_710009d50c(char_class->vf_0x0(), fn_71007105e4()))
        char_class = nullptr;

    fn_710002fa90(self->list_0x2d0);
    fn_710002fa90(self->list_0x380);
    self->field_0x4b8 = false;
    self->list_0x4c0.Clear();
    self->field_0x4b9 = false;
    self->field_0x2b4 = 0;
    self->action_0x2b8 = char_class->default_action;
    self->action_0x2c0 = char_class->default_action;

    if (!self->tracks)
        return;
    fn_71002df844(self->tracks);
    if (char_class->field_0x70)
        fn_710070f098(&self->list_0x4c0, char_class->field_0x70, false);

    // The start action, or the default one when the class names none.
    u32 blend_value = char_class->field_0x78;
    bool requested;
    if (!IsEmpty(char_class->start_action))
        requested = SendAction(self, char_class->start_action.data->Key(), kMainTrack, 1, DefaultTime(),
                               UnkS12{0, 0, blend_value}, UnkAll_SpitSend().value);
    else
        requested = SendAction(self, self->action_0x2b8.data->Key(), kMainTrack, 1, DefaultTime(),
                               UnkS12{0, 0, blend_value}, UnkAll_SpitSend().value);
    if (requested) {
        if (UnkAnimOwnerPart_l176214* part = self->owner->field_0x218)
            part->field_0x1c09 = false;
    }
}