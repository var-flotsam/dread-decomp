// Unit 112 (the tunables' registry): finds a tunable object by name, either by its full name or, when
// `qualified` is false, by the part of each stored name after its last '|'. Names and layouts are
// placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU112.h"
#include "game/UnkTU57.h"
#include "../../game_aca57c/UnkBufferString.h"
#include "types.h"

using base::global::CRntString;

// Objects and their names in two parallel arrays sharing a count.
struct UnkTunableRegistry {
    UnkTunableObject** objects;
    CRntString* names;
    u32 count;
};

typedef UnkBufferString<0x80, g_unk_7101a1b750> UnkNameString_l13c5cc;

static inline bool SameName(const CRntString* a, const CRntString* b) {
    return fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(a),
                         reinterpret_cast<const UnkCompared_0x30*>(b));
}

// The slot of the object named `name`, or the end of the array.
static inline UnkTunableObject** FindByFullName(UnkTunableRegistry* self, const CRntString* name) {
    for (u64 i = 0; i < self->count; ++i) {
        if (SameName(&self->names[i], name))
            return &self->objects[i];
    }
    return &self->objects[self->count];
}

UnkTunableObject* FindTunableObjectByName(UnkTunableRegistry* self, const char* name, bool qualified) {
    UnkNameString_l13c5cc key(name);
    if (qualified) {
        UnkTunableObject** it = FindByFullName(self, key.Str());
        if (it == &self->objects[self->count])
            return nullptr;
        return *it;
    }
    UnkNameString_l13c5cc last;
    u32 count = self->count;
    for (u32 i = 0; i < count; ++i) {
        const CRntString* full = &self->names[i];
        s32 bar = CRntString_FindLastChar(full, '|', -1);
        CRntString_AssignCString(last.Str(), (full->data ? full->data : g_unk_710162e2ed) + bar + 1);
        if (SameName(last.Str(), key.Str()))
            return self->objects[i];
    }
    return nullptr;
}
