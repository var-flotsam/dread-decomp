// CAudioComponent's unit: placing the sounds marked for it. Each one takes the point the owner's
// component at +0x228 gives for the sound's name (when the name is set and is not one of two ids of
// the game's table), or else the owner's position, and its sound is moved there (fn_7100105ebc).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringEntry;

void fn_7100105ebc(char* p0, char* p1);

struct UnkAudioPlaceTarget_l71ddac {
    void* sound;
    s64 id;
};

// A sound slot of the component (0xd0 bytes).
struct UnkAudioPlaceSound_l71ddac {
    CStrId name;  // 0x0
    u8 pad_0x8[0x30 - 0x8];
    UnkVec3f position;  // 0x30
    u8 pad_0x3c[0x78 - 0x3c];
    UnkAudioPlaceTarget_l71ddac* target;  // 0x78: the sound's handle, current while target->id == id
    s64 id;                               // 0x80
    u8 pad_0x88[0xc4 - 0x88];
    bool field_0xc4_0 : 1;
    bool restart : 1;  // 0xc4 bit 1
    u8 pad_0xc5[0xd0 - 0xc5];

    // The sound while the handle is current.
    void* Get() const {
        if (!target || target->id != id || id == -1)
            return nullptr;
        return target->sound;
    }
};
static_assert(sizeof(UnkAudioPlaceSound_l71ddac) == 0xd0, "UnkAudioPlaceSound_l71ddac layout");

struct UnkAudioPlaceOwner_l71ddac {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x228 - 0x68];
    void* field_0x228;
};

struct UnkAudioPlaceComponent_l71ddac {
    u8 pad_0x0[0x30];
    UnkAudioPlaceOwner_l71ddac* owner;  // 0x30
    u8 pad_0x38[0x48 - 0x38];
    UnkAudioPlaceSound_l71ddac* sounds;  // 0x48
    u32 count;                           // 0x50
};

// The id table's slot at +0x2128 (a string id; UnkGameManagerIdTable has no member for it yet).
static inline const CStrId& IdTableSlot2128() {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const u8*>(&GameManagerIdTable()) + 0x2128);
}

// Whether two string ids are the same.
template <class A, class B>
static inline bool SameId(const A& a, const B& b) {
    return a.data == b.data;
}

void fn_710071ddac(UnkAudioPlaceComponent_l71ddac* self) {
    UnkAudioPlaceSound_l71ddac* it = self->sounds;
    for (u32 left = self->count; left != 0; --left, ++it) {
        if (!it->restart)
            continue;
        UnkVec3f point(0.0f, 0.0f, 0.0f);
        void* target = self->owner->field_0x228;
        bool found = false;
        // The name gives a point of its own when it is set and is neither of two ids of the table.
        if (target && !SameId(it->name, g_unk_7101d470e0) && !SameId(it->name, GameManagerIdTable().field_0xd40) &&
            !SameId(it->name, IdTableSlot2128())) {
            u64 hash = reinterpret_cast<const UnkStringEntry*>(it->name.data)->buffer.key;
            found = fn_710117364c(target, &hash, reinterpret_cast<UnkGrappleAnswer_b50*>(&point));
        }
        if (found) {
            it->position = point;
        } else {
            UnkAudioPlaceOwner_l71ddac* owner = self->owner;
            it->position = UnkVec3f(owner->x, owner->y, owner->z);
        }
        if (void* sound = it->Get())
            fn_7100105ebc(static_cast<char*>(sound), reinterpret_cast<char*>(&it->position));
    }
}
