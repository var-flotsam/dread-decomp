// CSpawnPointComponent's unit (tools/tu_map.py unit 2704): tests whether the component's name id is on the
// id list or one of four fixed names. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2704.h"
#include "types.h"


struct UnkNameData {
    u8 pad_0x0[0x10];
    base::global::CRntString name;  // 0x10
    u8 pad_0x40[4];
    union {
        s32 ref_count;  // 0x44
    };
};

struct __attribute__((packed)) UnkNameList {
    u32 count;
    UnkNameData* ids[4];  // 0x4
};

struct UnkNamed {
    u8 pad_0x0[0x48];
    UnkNameData* id;  // 0x48
};

static_assert(__builtin_offsetof(UnkNameData, ref_count) == 0x44, "UnkNameData layout");
static_assert(__builtin_offsetof(UnkNamed, id) == 0x48, "UnkNamed layout");

static inline bool NameIs(UnkNamed* self, const char* str) {
    ++self->id->ref_count;
    UnkNameData* id = self->id;
    UnkNameData* empty = reinterpret_cast<UnkNameData*>(&g_unk_7101d47108->empty);
    return id != empty && fn_710007fb9c(&id->name, str);
}


bool fn_710112a3bc(UnkNamed* self) {
    UnkNameList& list = reinterpret_cast<UnkNameList&>(g_unk_7172a0768c);
    u32 count = list.count;
    UnkNameData* id = self->id;
    if (count != 0) {
        bool found = false;
        for (u32 i = 0; i < count; ++i)
            found |= (id == list.ids[i]);
        if (__builtin_expect(found, 0))
            return true;
    }
    if (NameIs(self, "centralunit"))
        return true;
    if (NameIs(self, "emmy"))
        return true;
    if (NameIs(self, "xparasite"))
        return true;
    if (NameIs(self, "big_x"))
        return true;
    return false;
}
