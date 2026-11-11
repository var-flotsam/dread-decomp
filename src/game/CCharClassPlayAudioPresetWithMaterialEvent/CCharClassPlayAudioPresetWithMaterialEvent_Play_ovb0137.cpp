// CCharClassPlayAudioPresetWithMaterialEvent's unit (tools/tu_map.py): the event's run, as CCharClassPlayAudioPresetEvent's (fn_7100bc3164) but played only when the owner has a material source (fn_710094c9ac), with the material fn_7100798e88 gives.
// Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "game/UnkTU2523.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

CRntString fn_710007f050(const char* a, const CRntString* b);

struct UnkPathIds;
struct UnkInlineString;

struct UnkSoundHandle;
struct UnkEntityWithSound;

// A pooled string's record: the text at +0x10 and a use count at +0x44.
struct UnkStringRecord_m_ovb0137 {
    u8 pad_0x0[0x10];
    CRntString text;
    u8 pad_0x40[4];
    union {
        u32 uses;  // 0x44
    };
};

struct UnkBuiltPathIds_m_ovb0137 {
    union {
        CStrId id;
    };
    union {
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    explicit UnkBuiltPathIds_m_ovb0137(const UnkInlineString* source) {
        fn_710001f074(reinterpret_cast<UnkPathIds*>(this), source);
    }
    ~UnkBuiltPathIds_m_ovb0137() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

struct UnkAudioPresetParams_m_ovb0137 {
    u32 field_0x0;
    u32 field_0x4;
    bool field_0x8;
};

struct UnkAudioPresetEvent_m_ovb0137 {
    u8 pad_0x0[0x10];
    UnkStringData* name;          // 0x10
    UnkStrIdPathResource preset;  // 0x18
    UnkAudioPresetParams_m_ovb0137 params;  // 0x30
};

struct UnkSoundHandle_m_ovb0137 {
    void* ref;
    s64 serial;
};

struct UnkEventOwner_m_ovb0137 {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
};

struct UnkEventContext_m_ovb0137 {
    UnkEventOwner_m_ovb0137* owner;
};

void* fn_710094c9ac(UnkEventOwner_m_ovb0137* owner);  // the owner's material source, if any
u32 fn_7100798e88();

static inline bool IsSet(const UnkStrIdPathResource& r) {
    return r.ids.id.data != &g_unk_7101d47108->empty && r.ids.path.data != &g_unk_7101d47200->empty;
}

void fn_7100bc3de0(UnkAudioPresetEvent_m_ovb0137* self, UnkEventContext_m_ovb0137* ctx) {
    if (!ctx->owner || ctx->owner->field_0x1b5)
        return;
    if (!IsSet(self->preset) && self->name != &g_unk_7101d47108->empty) {
        UnkStringRecord_m_ovb0137* record = reinterpret_cast<UnkStringRecord_m_ovb0137*>(self->name);
        ++record->uses;
        alignas(8) u8 storage[sizeof(UnkBuiltPathIds_m_ovb0137)];
        CRntString path = fn_710007f050("audiopreset:system/snd/presets/",
                                        &reinterpret_cast<UnkStringRecord_m_ovb0137*>(self->name)->text);
        UnkBuiltPathIds_m_ovb0137* ids =
            new (storage) UnkBuiltPathIds_m_ovb0137(reinterpret_cast<const UnkInlineString*>(&path));
        fn_71000267f8(&self->preset, reinterpret_cast<const UnkStrIdPathPair&>(*ids));
        ids->~UnkBuiltPathIds_m_ovb0137();
    }
    if (IsSet(self->preset)) {
        u32 a = self->params.field_0x0;
        u32 b = self->params.field_0x4;
        UnkAudioPresetParams_m_ovb0137 params;
        params.field_0x0 = a;
        params.field_0x4 = b;
        params.field_0x8 = true;
        if (fn_710094c9ac(ctx->owner)) {
            using Play = UnkSoundHandle_m_ovb0137 (*)(UnkEntityWithSound*, void*, void*, u32);
            reinterpret_cast<Play>(&fn_7100d7c45c)(reinterpret_cast<UnkEntityWithSound*>(ctx->owner),
                                                   &self->preset, &params, fn_7100798e88());
        }
    }
}
