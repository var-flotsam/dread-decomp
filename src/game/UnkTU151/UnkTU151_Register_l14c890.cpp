// Unit 151 (tools/tu_map.py): registers the object under the shader path its name gives, once (the handle
// at +0x38 is still unset): the id at +0x30 is set from that path, the resource the path names is loaded
// and handed to the registry, and the registry's handle is kept. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkResKey.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU204.h"
#include "game/msapi/res/CRenderRes.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrIdBase;

struct UnkRegistry_710014c790;
extern UnkRegistry_710014c790* unk_710b372d68;  // the registry (reached through the GOT)


// An id whose move-assignment releases the old string as the destructor does before taking the new one.
struct UnkRegisteredId_l14c890 : CStrId {
    UnkRegisteredId_l14c890& operator=(CStrId&& other) {
        ReleaseAndTake(other);
        return *this;
    }
};

struct UnkRegistered_l14c890 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkRegisteredId_l14c890 id;  // 0x30
    void* handle;  // 0x38
};

// An id's text as the string it was pooled from, counting the read.
static inline const CRntString& RntText(const UnkStrIdBase& id) {
    ++id.data->reads;
    return *reinterpret_cast<const CRntString*>(&id.data->text);
}
static inline const CRntString& RntText(const CFilePathStrId& id) {
    ++id.data->reads;
    return *reinterpret_cast<const CRntString*>(&id.data->text);
}

// Ids built out of line from a pooled string's text (fn_7100000850, fn_710001a270).
struct UnkTextId_l14c890 {
    union {
        CStrId id;
    };
    explicit UnkTextId_l14c890(const CRntString& text) { CStrId_ConstructFromRntString(&id, text, true); }
    ~UnkTextId_l14c890() { id.~CStrId(); }
    CStrId&& Take() { return static_cast<CStrId&&>(id); }
};
struct UnkTextPathId_l14c890 {
    union {
        CFilePathStrId id;
    };
    explicit UnkTextPathId_l14c890(const CRntString& text) { fn_710001a270(&id, text, true); }
    ~UnkTextPathId_l14c890() { id.~CFilePathStrId(); }
};

void fn_710014c890(UnkRegistered_l14c890* self, const char* name) {
    if (self->handle)
        return;
    self->id = UnkTextId_l14c890(RntText(fn_7100169c74(CStrId(name, true)))).Take();
    UnkResKey key(g_unk_716e453ef8.name, UnkTextPathId_l14c890(RntText(self->id)).id);
    UnkResRef resource = fn_71000ead80(g_unk_7101d472f8, &key, 1, nullptr);
    fn_7100169d28(unk_710b372d68, self->id, resource.ptr);
    self->handle = fn_7100169be4(unk_710b372d68, &self->id);
}
