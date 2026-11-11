// ICharClassPlayAudioPresetEvent's unit (tools/tu_map.py): setting the event's preset name (+0x10). A
// non-empty name also resolves "audiopreset:system/snd/presets/<name>" into the id and path pair at
// +0x18 (fn_710001f074, fn_71000267f8). Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU25.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

CRntString fn_710007f050(const char* a, const CRntString* b);
struct UnkPathIds;
struct UnkInlineString;

// The ids the builder fills in place, destroyed in reverse order (as in CDroppableComponent_b50.cpp).
struct UnkPresetBuiltIds_ovb0135 {
    union {
        CStrId id;
    };
    union {
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    explicit UnkPresetBuiltIds_ovb0135(const CRntString& source) {
        fn_710001f074(reinterpret_cast<UnkPathIds*>(this), reinterpret_cast<const UnkInlineString*>(&source));
    }
    ~UnkPresetBuiltIds_ovb0135() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

struct UnkAudioPresetEvent_ovb0135 {
    u8 pad_0x0[0x10];
    CStrId name;                // 0x10
    UnkStrIdPathResource path;  // 0x18
};

void fn_7100beeadc(UnkAudioPresetEvent_ovb0135* self, const CStrId* name) {
    self->name = *name;
    if (self->name.data == &g_unk_7101d47108->empty)
        return;
    fn_71000267f8(&self->path,
                  reinterpret_cast<const UnkStrIdPathPair&>(static_cast<const UnkPresetBuiltIds_ovb0135&>(
                      UnkPresetBuiltIds_ovb0135(fn_710007f050("audiopreset:system/snd/presets/",
                                                              &base::reflection::refl_RntStringOf(self->name))))));
}
