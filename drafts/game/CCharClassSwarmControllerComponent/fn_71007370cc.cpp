// CCharClassSwarmControllerComponent's unit: the component's read. After the base read, each of its three
// id lists is emptied and refilled from the file (a count, then that many strings), the two id-pair groups
// are built from their three strings each, and +0xdc caches the square of +0xd8. Names and layouts are
// placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntString;
using base::global::CStrId;

struct UnkCollisionClassDef;
bool fn_71011aced4(UnkCollisionClassDef* self, CRntFile* file);  // the base read

struct UnkSwarmIdPairs_l7370cc;
void fn_7100737450(UnkSwarmIdPairs_l7370cc* pairs, CRntString* first, CRntString* second, CRntString* third);

// Up to ten ids stored in place after their count.
struct UnkSwarmIdList_l7370cc {
    u32 count;
    u8 storage[10 * sizeof(CStrId)];

    CStrId* items() { return reinterpret_cast<CStrId*>(storage); }

    void Clear() {
        for (u64 i = 0; i < count; ++i)
            items()[i].Reset();
        count = 0;
    }

    // Takes the id into the next free slot, leaving it empty.
    void Add(CStrId& id) {
        CStrId* slot = &items()[count];
        slot->data = id.data;
        id.data = &g_unk_7101d47108->empty;
        ++count;
    }

    // Appends the ids the file lists: a count, then that many strings.
    void Read(CRntFile* file) {
        s32 n = 0;
        fn_710007c714(file, &n, 4, 1);
        for (s32 i = 0; i < n; ++i) {
            const char* str;
            fn_710007cee4(file, &str);
            CStrId id(str, true);
            Add(id);
        }
    }
};
static_assert(sizeof(UnkSwarmIdList_l7370cc) == 0x54, "UnkSwarmIdList_l7370cc layout");

struct UnkSwarmIdPairs_l7370cc {
    u8 pad_0x0[0x48];
};

struct UnkSwarmController_l7370cc {
    u8 pad_0x0[0xd8];
    f32 field_0xd8;
    f32 field_0xdc;
    u8 pad_0xe0[0x180 - 0xe0];
    CRntString field_0x180;
    CRntString field_0x1b0;
    CRntString field_0x1e0;
    CRntString field_0x210;
    CRntString field_0x240;
    CRntString field_0x270;
    UnkSwarmIdPairs_l7370cc field_0x2a0;
    UnkSwarmIdPairs_l7370cc field_0x2e8;
    u8 pad_0x330[0x4];
    UnkSwarmIdList_l7370cc field_0x334;
    UnkSwarmIdList_l7370cc field_0x388;
    UnkSwarmIdList_l7370cc field_0x3dc;
};
static_assert(__builtin_offsetof(UnkSwarmController_l7370cc, field_0x2a0) == 0x2a0, "layout");
static_assert(__builtin_offsetof(UnkSwarmController_l7370cc, field_0x334) == 0x334, "layout");

bool fn_71007370cc(UnkSwarmController_l7370cc* self, CRntFile* file) {
    bool result = fn_71011aced4(reinterpret_cast<UnkCollisionClassDef*>(self), file);
    if (result) {
        self->field_0x334.Clear();
        self->field_0x334.Read(file);
        self->field_0x388.Clear();
        self->field_0x388.Read(file);
        self->field_0x3dc.Clear();
        self->field_0x3dc.Read(file);
        fn_7100737450(&self->field_0x2a0, &self->field_0x180, &self->field_0x1b0, &self->field_0x1e0);
        fn_7100737450(&self->field_0x2e8, &self->field_0x210, &self->field_0x240, &self->field_0x270);
        self->field_0xdc = self->field_0xd8 * self->field_0xd8;
    }
    return result;
}
