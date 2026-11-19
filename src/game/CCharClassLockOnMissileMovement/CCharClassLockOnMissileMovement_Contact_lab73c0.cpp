// CCharClassLockOnMissileMovement's unit (tools/tu_map.py, unit 1616): handles a contact. It is tried
// against the owner's "missile" collider (fn_7100abb448), then, unless that settled it, against the
// "explosion" collider; when neither takes it the flag at +0x868 is cleared. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkLockOnOwner_lab73c0 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkLockOnMissileMovement_lab73c0 {
    u8 pad_0x0[0x30];
    UnkLockOnOwner_lab73c0* owner;  // 0x30
    u8 pad_0x38[0x525 - 0x38];
    bool exploded;  // 0x525
    u8 pad_0x526[0x868 - 0x526];
    bool armed;  // 0x868
};

// A counted reference to a weak handle, handed over by value.
struct UnkWeakRef_lab73c0 {
    UnkWeakHandle* handle;

    explicit UnkWeakRef_lab73c0(UnkWeakHandle* p) : handle(nullptr) {
        if (p) {
            ++p->ref_count;
            handle = p;
        }
    }
    ~UnkWeakRef_lab73c0() {
        UnkWeakHandle* p = handle;
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

// Handles the contact against one collider; true when it was handled.
bool fn_7100abb448(UnkLockOnMissileMovement_lab73c0* self, void* other, void* contact, bool flag, bool check,
                   UnkWeakRef_lab73c0 collider);

// The owner's collider with this name, or null.
static inline UnkWeakTarget* FindCollider(UnkLockOnMissileMovement_lab73c0* self, const char* name) {
    UnkCollisionContainer* collision = self->owner->collision;
    CStrId id(name, true);
    return reinterpret_cast<UnkWeakTarget*>(fn_71008640a4(collision, &id));
}

// The weak handle of `target`, or null.
static inline UnkWeakHandle* HandleOf(UnkWeakTarget* target) {
    return target ? GetWeakHandle(target) : nullptr;
}

bool fn_7100ab73c0(UnkLockOnMissileMovement_lab73c0* self, void* other, void* contact, bool flag) {
    UnkWeakTarget* missile = FindCollider(self, "missile");
    if (!fn_7100abb448(self, other, contact, flag, true, UnkWeakRef_lab73c0(HandleOf(missile))) && !self->armed)
        return false;
    if (self->exploded)
        return true;
    UnkWeakTarget* explosion = FindCollider(self, "explosion");
    if (fn_7100abb448(self, other, contact, flag, true, UnkWeakRef_lab73c0(HandleOf(explosion))))
        return true;
    self->armed = false;
    return false;
}