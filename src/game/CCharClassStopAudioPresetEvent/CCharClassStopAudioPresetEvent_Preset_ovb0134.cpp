// CCharClassStopAudioPresetEvent's unit (tools/tu_map.py unit 1870): fn_7100be4760 runs the event on an
// actor. Unless the event's preset ids (+0x18, with the resource they resolve to) are both set, they
// are resolved from the preset name (+0x10) as "audiopreset:system/snd/presets/<name>" (fn_710007f050,
// built into ids by fn_710001f074, assigned by fn_71000267f8). With both ids set, the preset is
// stopped on the actor (fn_7100d7c548) with the event's two times (+0x30, +0x34). Names and layouts
// are placeholders. fn_7100be45dc sets the preset name and resolves the ids from it the same way.
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "game/UnkTU25.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkPathIds;
struct UnkInlineString;

// The ids the builder fills in place, destroyed in reverse order (as in CDroppableComponent_b50.cpp).
struct UnkPresetIds_ovb0134 {
    union {
        CStrId id;
    };
    union {
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    explicit UnkPresetIds_ovb0134(const CRntString& source) {
        fn_710001f074(reinterpret_cast<UnkPathIds*>(this), reinterpret_cast<const UnkInlineString*>(&source));
    }
    ~UnkPresetIds_ovb0134() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

// The preset name: a pooled string whose text is at +0x10 and use count at +0x44.
struct UnkPresetName_ovb0134 {
    u8 pad_0x0[0x10];
    CRntString text;  // 0x10
    u8 pad_0x10[0x44 - 0x10 - sizeof(CRntString)];
    union {
        s32 use_count;  // 0x44
    };
};

struct UnkStopAudioPresetEvent_ovb0134 {
    u8 pad_0x0[0x10];
    base::UnkStrIdMemberOf<g_unk_7101d47108> name;  // 0x10
    UnkPresetName_ovb0134* Name() const { return reinterpret_cast<UnkPresetName_ovb0134*>(name.data); }
    UnkStrIdPathResource preset;   // 0x18
    f32 blend;                     // 0x30
    f32 start;                     // 0x34
};

struct UnkActorRef_ovb0134 {
    UnkActor* actor;
};

static inline bool IsSet(const UnkStrIdPathResource* preset) {
    return preset->ids.id.data != &g_unk_7101d47108->empty && preset->ids.path.data != &g_unk_7101d47200->empty;
}

// Resolves the preset ids from the name.
__attribute__((always_inline)) static inline void Resolve(UnkStopAudioPresetEvent_ovb0134* self,
                                                          UnkStrIdPathResource* preset, UnkPresetName_ovb0134* name) {
    ++name->use_count;
    fn_71000267f8(preset, reinterpret_cast<const UnkStrIdPathPair&>(static_cast<const UnkPresetIds_ovb0134&>(
                                     UnkPresetIds_ovb0134(fn_710007f050("audiopreset:system/snd/presets/",
                                                                        &self->Name()->text)))));
}

// Sets the preset name and resolves the ids from it.
void fn_7100be45dc(UnkStopAudioPresetEvent_ovb0134* self, const base::UnkStrIdMemberOf<g_unk_7101d47108>* name) {
    UnkStrIdPathResource* preset = &self->preset;
    self->name = *name;
    if (self->Name() != reinterpret_cast<UnkPresetName_ovb0134*>(&g_unk_7101d47108->empty))
        Resolve(self, preset, self->Name());
}

void fn_7100be4760(UnkStopAudioPresetEvent_ovb0134* self, UnkActorRef_ovb0134* ref) {
    UnkStrIdPathResource* preset = &self->preset;
    if (!IsSet(preset)) {
        if (self->Name() != reinterpret_cast<UnkPresetName_ovb0134*>(&g_unk_7101d47108->empty))
            Resolve(self, preset, self->Name());
    }
    if (IsSet(preset))
        fn_7100d7c548(ref->actor, &preset->ids, self->blend, self->start);
}
