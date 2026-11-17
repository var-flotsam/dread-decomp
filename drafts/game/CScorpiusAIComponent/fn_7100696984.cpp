// CScorpiusAIComponent unit (tools/tu_map.py, unit 1027): fn_7100696984 walks six timeline resources in
// order and returns the first that is present and active, storing the hash that names it in `out`.
// Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern const u64 g_unk_710164a5e0;  // reached through the GOT
extern const u64 g_unk_710164a5d8;  // reached through the GOT
extern const u64 g_unk_710164a5d0;  // reached through the GOT
extern const u64 g_unk_710164a5c8;  // reached through the GOT
extern const u64 g_unk_710164a5c0;  // reached through the GOT
extern const u64 g_unk_710164a5b8;  // reached through the GOT

struct UnkScorpius_l696984 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

struct UnkScorpiusLookup_l696984 {
    UnkTimelineResourceEntry* entry;
    bool active;
};

// Looks the resource `id` names up, and whether it is active; `out` gets `hash`.
static inline UnkScorpiusLookup_l696984 Lookup(UnkTimelineComponent* timeline, const u64* id, u64 hash, u64* out) {
    UnkScorpiusLookup_l696984 result;
    result.entry = fn_7100a2c13c(timeline, id);
    result.active = fn_7100a2be94(timeline, id);
    *out = hash;
    return result;
}

// The first of six resources that is present and active, or null; `out` gets the hash of the last one tried.
UnkTimelineResourceEntry* fn_7100696984(UnkScorpius_l696984* self, u64* out) {
    UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    UnkScorpiusLookup_l696984 found = {nullptr, false};
    if (timeline) {
        found = Lookup(timeline, &g_unk_710164a5e0, 0xfe90e4d8ffb24565, out);
        if (!found.entry || !found.active)
            found = Lookup(timeline, &g_unk_710164a5d8, 0x93056405f1e08978, out);
        if (!found.entry || !found.active)
            found = Lookup(timeline, &g_unk_710164a5d0, 0xbe9d1a28c7f36296, out);
        if (!found.entry || !found.active)
            found = Lookup(timeline, &g_unk_710164a5c8, 0x5e50dee6f235fa0a, out);
        if (!found.entry || !found.active)
            found = Lookup(timeline, &g_unk_710164a5c0, 0x5be336e9b14bc980, out);
        if (!found.entry || !found.active)
            found = Lookup(timeline, &g_unk_710164a5b8, 0x8e9b0ed0c1d73ef6, out);
    }
    return found.active ? found.entry : nullptr;
}