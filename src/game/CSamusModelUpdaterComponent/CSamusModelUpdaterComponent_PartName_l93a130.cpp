// CSamusModelUpdaterComponent's unit (tools/tu_map.py unit 1427): the name of a model part for the current
// suit. A name the model has a part for is kept; one that starts with a suit's mesh prefix ("Mesh_Power",
// "Mesh_Varia", ...) gets the prefix of the suit model in use (+0x210). Names and layouts are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/UnkTU57.h"
#include "types.h"
#include "../CAccessPointCommanderComponent/UnkStrIdHashedInit.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkModelUpdater_ovb0032;
bool fn_71011743bc(UnkModelUpdater_ovb0032* self, const s64* id);

struct UnkSamusModelUpdater_l93a130 {
    u8 pad_0x0[0x210];
    CStrId model;  // 0x210: the suit model in use
};

// A model name: a string id built from a literal whose length and hash the caller computed
// (CStrId's layout).
struct UnkModelName_l93a130 {
    UnkStringData* data;

    UnkModelName_l93a130(const char* str, u64 length, u64 hash, bool flag);  // fn_71000406dc
    ~UnkModelName_l93a130() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
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

constexpr u64 kDefaultHash = StrIdHash("Default", 7);
constexpr u64 kVariaHash = StrIdHash("Varia", 5);
constexpr u64 kGravityHash = StrIdHash("Gravity", 7);
constexpr u64 kHyperHash = StrIdHash("Hyper", 5);

// The id's string, counting the read.
static inline const CRntString* Str_l93a130(const CStrId& id) {
    ++id.data->reads;
    return &id.data->String();
}

static inline bool StartsWith_l93a130(const CStrId& name, const CStrId& prefix) {
    return CRntString_StartsWith(Str_l93a130(name), Str_l93a130(prefix));
}

static inline bool IsModel_l93a130(const CStrId& model, const UnkModelName_l93a130& name) {
    return model.data == name.data;
}

void fn_710093a130(char* self, char* entry, CRntString* out) {
    UnkSamusModelUpdater_l93a130* updater = reinterpret_cast<UnkSamusModelUpdater_l93a130*>(self);
    const CStrId& name = *reinterpret_cast<const CStrId*>(entry);
    CRntString_Assign(out, Str_l93a130(name));
    {
        s64 key = name.data->Key();
        if (fn_71011743bc(reinterpret_cast<UnkModelUpdater_ovb0032*>(self), &key))
            return;
    }

    static CStrId s_power("Mesh_Power", true);
    static CStrId s_varia("Mesh_Varia", true);
    static CStrId s_gravity("Mesh_Gravity", true);
    static CStrId s_hyper("Mesh_Hyper", true);

    const CStrId* prefix;
    if (StartsWith_l93a130(name, s_power))
        prefix = &s_power;
    else if (StartsWith_l93a130(name, s_varia))
        prefix = &s_varia;
    else if (StartsWith_l93a130(name, s_gravity))
        prefix = &s_gravity;
    else if (StartsWith_l93a130(name, s_hyper))
        prefix = &s_hyper;
    else
        return;

    u32 skip = prefix->data->String().length;
    const CRntString* text = Str_l93a130(name);
    u32 rest_length = text->length - skip;
    const char* rest = text->data + skip;
    UnkBufferString<0x40, g_unk_7101a1b678> suffix;
    CRntString_AssignPrefix(suffix.Str(), rest, rest_length);

    const CStrId* mesh;
    if (IsModel_l93a130(updater->model, UnkModelName_l93a130("Default", 7, kDefaultHash, true)))
        mesh = &s_power;
    else if (IsModel_l93a130(updater->model, UnkModelName_l93a130("Varia", 5, kVariaHash, true)))
        mesh = &s_varia;
    else if (IsModel_l93a130(updater->model, UnkModelName_l93a130("Gravity", 7, kGravityHash, true)))
        mesh = &s_gravity;
    else if (IsModel_l93a130(updater->model, UnkModelName_l93a130("Hyper", 5, kHyperHash, true)))
        mesh = &s_hyper;
    else
        return;

    CRntString_Assign(out, Str_l93a130(*mesh));
    CRntString_Append(out, suffix.Str());
}
