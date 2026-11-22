// CWeaponMovement unit (tools/tu_map.py unit 1624): the value a hit object's name gives. The collision
// container names the object's kind; for a "HUSK" the number after "husk" in the object's name is
// returned negated, for a "HIT" the number after "hit" (1 when there is none). Names and layouts are
// placeholders.
#include <stdlib.h>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/CCollisionComponent.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// The object a hit refers to.
struct UnkHitObject_lac6ed0 {
    u8 pad_0x0[0x20];
    CStrId name;  // 0x20
};

// What the hit handle points at: the hit object at +0x10.
struct UnkHitSource_lac6ed0 : base::UnkRefCounted {
    UnkHitObject_lac6ed0* object;  // 0x10
};

struct UnkWeaponMovement_lac6ed0 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

// A string on 0x100 bytes of inline storage, copied from a string or from part of one.
struct UnkText_lac6ed0 : UnkBufferString<0x100, g_unk_7101a194b8> {
    explicit UnkText_lac6ed0(const CRntString& text) { CRntString_Assign(Str(), &text); }
    UnkText_lac6ed0(const char* str, u32 length) { CRntString_AssignPrefix(Str(), str, length); }
};

// The id's whole string, counting the read.
static inline const CRntString& StringOf(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// The number a string reads as; 1 when it is empty.
static inline s32 NumberOr1(const UnkText_lac6ed0& text) {
    const char* chars = text.Str()->data;
    return chars && chars[0] ? atoi(chars) : 1;
}

s32 fn_7100ac6ed0(UnkWeaponMovement_lac6ed0* self, const base::UnkRefHandle* hit) {
    UnkCollisionContainer* collision = self->collision;
    if (!collision)
        return -0x7fffffff - 1;
    UnkHitObject_lac6ed0* object = hit->ptr ? static_cast<UnkHitSource_lac6ed0*>(hit->ptr)->object : nullptr;
    CStrId kind = *fn_7100864018(collision, object);
    s32 value;
    if (kind == CStrId("HUSK", true)) {
        UnkText_lac6ed0 text(StringOf(object->name));
        u32 start = CRntString_Find(text.Str(), "husk", 0) + 4;
        u32 length = text.Str()->length - start;
        const char* rest = text.Str()->data + start;
        UnkText_lac6ed0 digits(rest, length);
        value = -NumberOr1(digits);
    } else if (kind == CStrId("HIT", true)) {
        UnkText_lac6ed0 text(StringOf(object->name));
        u32 start = CRntString_Find(text.Str(), "hit", 0) + 3;
        u32 length = text.Str()->length - start;
        const char* rest = text.Str()->data + start;
        UnkText_lac6ed0 digits(rest, length);
        value = NumberOr1(digits);
    } else {
        value = -0x7fffffff - 1;
    }
    return value;
}
