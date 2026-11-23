// CModelUpdaterComponent's unit (tools/tu_map.py unit 2719): when the owner's component is a
// CCharClassComponent, loads the model of each of its entries of one kind, with the two strings the
// entry's value bag holds (fn_7101171a08). Names and layouts are placeholders.
#include <algorithm>

#include "../../game_aca57c/UnkBufferString.h"
#include "../../game_s7/UnkIter.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkBufferStringPool.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntString;
using base::global::CStrId;
using unk_prop_bag::UnkPropBag;

struct UnkModelUpdater_ovb0032;
u32 fn_7101171a08(UnkModelUpdater_ovb0032* self, const void* path, const base::global::CRntString* extra);

// As src/game/CAIAttackComponent/CAIAttackComponent_Attack_ovb0050.cpp declares it.
void fn_710007ca94(CRntFile* file, char* buf);

typedef UnkBufferString<0x80, g_unk_7101a1b750> UnkModelString_l1715d0;

// A value in the bag: where it is in the bag's stream, the hash of its name and its type.
struct UnkBagValue_l1715d0 {
    u64 offset;
    u64 key;
    s32 type;  // 's' for a string
};

struct UnkIdHash_l1715d0 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkModelEntry_l1715d0 {
    CStrId kind;
    UnkPropBag* values;
    u8 pad_0x10[0x88];
};

struct UnkCharClassComponent_l1715d0 {
    virtual const base::reflection::CClass* vf_0x0() const;
    u8 pad_0x8[0x28];
    UnkModelEntry_l1715d0* entries;  // 0x30
    s32 count;                       // 0x38
};

struct UnkModelUpdater_l1715d0 {
    u8 pad_0x0[0x10];
    UnkCharClassComponent_l1715d0* component;  // 0x10
};

static inline const CStrId& Id_l1715d0(u32 index) {
    return base::reflection::refl_GetStrIdTable<0x7100031894>().ids[index];
}

static inline UnkBagValue_l1715d0* Values_l1715d0(UnkPropBag* bag) {
    return reinterpret_cast<UnkBagValue_l1715d0*>(bag->values.data);
}

// The value named `id`, or the end of the values when there is none.
static inline UnkBagValue_l1715d0* FindValue_l1715d0(UnkPropBag* bag, const CStrId& id) {
    UnkBagValue_l1715d0* values = Values_l1715d0(bag);
    u64 key = reinterpret_cast<UnkIdHash_l1715d0*>(id.data)->hash;
    UnkIter<UnkBagValue_l1715d0> first{values};
    UnkIter<UnkBagValue_l1715d0> last{&values[bag->values.count]};
    return std::find_if(first, last, [key](const UnkBagValue_l1715d0& v) { return v.key == key; }).p;
}

static inline bool HasValue_l1715d0(UnkPropBag* bag, const CStrId& id) {
    UnkBagValue_l1715d0* end = &Values_l1715d0(bag)[bag->values.count];
    return FindValue_l1715d0(bag, id) != end;
}

// Reads the string value named `id` into `out`, when the bag has one.
static inline void ReadString_l1715d0(UnkPropBag* bag, const CStrId& id, UnkModelString_l1715d0& out) {
    UnkBagValue_l1715d0* value = FindValue_l1715d0(bag, id);
    if (value != &Values_l1715d0(bag)[bag->values.count] && value->type == 's') {
        char text[0x100];
        CRntFile_Seek(&bag->lock, value->offset, 0);
        fn_710007ca94(&bag->lock, text);
        fn_710007e77c(out.Str(), text);
    }
}

bool fn_71011715d0(UnkModelUpdater_l1715d0* self) {
    UnkCharClassComponent_l1715d0* component = self->component;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_71011aca94()))
        return true;

    bool loaded = true;
    s32 count = component->count;
    for (s32 i = 0; i < count; ++i) {
        UnkModelEntry_l1715d0& entry = component->entries[i];
        if (!(entry.kind == Id_l1715d0(0x2cd)))
            continue;
        UnkModelString_l1715d0 path;
        UnkModelString_l1715d0 extra;
        UnkPropBag* values = entry.values;
        ReadString_l1715d0(values, Id_l1715d0(0x36c), path);
        if (HasValue_l1715d0(values, Id_l1715d0(0x36d)))
            ReadString_l1715d0(values, Id_l1715d0(0x36d), extra);
        loaded = fn_7101171a08(reinterpret_cast<UnkModelUpdater_ovb0032*>(self), path.Str(), extra.Str()) & 1;
    }
    return loaded;
}
