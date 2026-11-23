// CDemolitionBlockLifeComponent unit (tools/tu_map.py unit 1374): restores the block from the blackboard. A dead
// block shows its debris, turns its debris colliders on and finishes dying; a living one takes its phase colour,
// and a removed husk hides the parts named after the husk's suffix. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkStrIdPathPair.h"
#include "base/UnkTunableLine.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/CCollisionComponent.h"
#include "game/CCoreXAIComponent.h"
#include "game/CDemolitionBlockLifeComponent.h"
#include "game/CDropComponent.h"
#include "game/CInputComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU253.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU341.h"
#include "game/UnkTU372.h"
#include "game/UnkTU57.h"
#include "game/UnkTU883.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineComponent.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrIdBase;
using base::global::UnkStringData;
using base::math::CVector4D;

// The unit's data (its statics, merged into one block).
struct UnkDemolitionBlockLifeTU {
    UnkTUHeader header;
    CStrId destroyed;               // 0x18
    CStrId explosion;               // 0x20
    CStrId material;                // 0x28
    CFilePathStrId dissolve_fx;     // 0x30
    u64 field_0x38;
    CRntString phase_one_two_sound;  // 0x40
    CRntString phase_one_one_sound;  // 0x70
    CRntString phase_two_one_sound;  // 0xa0
    UnkReflType type;  // 0xd0
};

extern UnkDemolitionBlockLifeTU g_unk_7172730d30 __attribute__((visibility("hidden")));
extern UnkProperty g_unk_7172730cc0;
extern const u64 g_unk_710164b010;
extern const u64 g_unk_710164b018;
extern UnkDesc_SpitSend g_unk_710164b958;

struct UnkStruct_7101176ea8;
bool fn_7101176e78(UnkStruct_7101176ea8* self, void* key, void** out);

struct UnkPowerBombCollider_ovb0127;
struct UnkPowerBombColliders_ovb0127;
struct UnkPowerBombPartList_ovb0127;
struct UnkPowerBombPart_ovb0127;

struct UnkDemolitionPart {
    UnkStringData* name;
};

struct UnkDemolitionNodeTarget {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkDemolitionNode {
    u8 pad_0x0[0x240];
    CVector4D value;  // 0x240
    u8 pad_0x250[0x2e0 - 0x250];
    UnkDemolitionNodeTarget* target;  // 0x2e0
};

struct UnkDemolitionSlots {
    u8 pad_0x0[0x48];
    UnkDemolitionNode** nodes;  // 0x48
};

struct UnkDemolitionModelResource {
    u8 pad_0x0[0x78];
    UnkDemolitionSlots* slots;  // 0x78
};

struct UnkDemolitionModelData {
    u8 pad_0x0[0x138];
    UnkDemolitionModelResource* resource;  // 0x138

    UnkDemolitionNode* Node(u32 index) const { return resource->slots->nodes[index]; }
};

struct UnkDemolitionModel {
    u8 pad_0x0[0x50];
    UnkDemolitionModelData* data;  // 0x50
};

struct UnkDemolitionCollider {
    u8 pad_0x0[0x20];
    UnkStringData* name;  // 0x20
};

// The husk's collider: its name is a string id the block keeps a copy of.
struct UnkDemolitionHuskCollider {
    u8 pad_0x0[0x20];
    CStrId name;  // 0x20
};

// The block's owner (actor) as this function sees it.
struct UnkDemolitionLoadOwner {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* colliders;  // 0x2a0
};

// The phase the block's definition starts in.
struct UnkDemolitionPhaseDef {
    u8 pad_0x0[0x12c];
    s32 phase;          // 0x12c
    bool husk_removed;  // 0x130
};

struct UnkDemolitionBlockValue_l8c5ad0 {
    CStrId id;
    u64 field_0x8;
};

struct UnkDemolitionBlockLoad {
    u8 pad_0x0[0x30];
    UnkDemolitionLoadOwner* owner;  // 0x30
    u8 pad_0x38[0x268 - 0x38];
    UnkDemolitionBlockValue_l8c5ad0* values;  // 0x268
    u64* keys;                                // 0x270
    u32 count;                                // 0x278
    u8 pad_0x27c[0x290 - 0x27c];
    UnkDemolitionPhaseDef* def;  // 0x290
    u8 pad_0x298[0x2b0 - 0x298];
    UnkStrIdPathPair dissolve;  // 0x2b0
    u8 pad_0x2c0[0x2c8 - 0x2c0];
    UnkStrIdPathPair dissolve_sound;  // 0x2c8
    u8 pad_0x2d8[0x300 - 0x2d8];
    UnkSoundHandle sound;  // 0x300
    u8 pad_0x310[0x350 - 0x310];
    UnkHitInfo_A5_b63* drop;  // 0x350
    s32 level;                // 0x358
    u8 pad_0x35c[4];
    UnkDemolitionNode* node;  // 0x360
    u8 pad_0x368[0x3a0 - 0x368];
    s32 phase;  // 0x3a0
    u8 pad_0x3a4[0x3e8 - 0x3a4];
    u8 listeners[8];  // 0x3e8

    // Where the property's value sits in the block's values (count when it has none).
    u64 IndexOf(const UnkProperty& property) const {
        u64 index = 0;
        u32 n = count;
        if (n) {
            const u64* list = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (list[index] == key)
                    break;
            }
        }
        return index;
    }
};
static_assert(__builtin_offsetof(UnkDemolitionBlockLoad, sound) == 0x300, "layout");
static_assert(__builtin_offsetof(UnkDemolitionBlockLoad, listeners) == 0x3e8, "layout");

// The colour of each phase, from the first to the third.
static const CVector4D kPhaseColors[3] = {
    {7.5f, 12.0f, 2.0f, 0.0f},
    {5.0f, 10.0f, 2.0f, 0.0f},
    {2.5f, 0.0f, 2.0f, 0.0f},
};

// The model's named parts (fn_71001c1440 counts them, fn_71001c1450 returns one).
static inline u32 PartCount(UnkDemolitionModelResource* resource) {
    return fn_71001c1440(reinterpret_cast<UnkPowerBombPartList_ovb0127*>(resource));
}
static inline UnkDemolitionPart* PartAt(UnkDemolitionModelResource* resource, u32 index) {
    return reinterpret_cast<UnkDemolitionPart*>(
        fn_71001c1450(reinterpret_cast<UnkPowerBombPartList_ovb0127*>(resource), index));
}

static inline UnkDemolitionModel* ModelOf(UnkDemolitionLoadOwner* owner) {
    return reinterpret_cast<UnkDemolitionModel*>(fn_7100233684(reinterpret_cast<UnkStruct_7100233684*>(owner)));
}

// Whether the name contains `str` (an empty name doesn't); reading it counts a use of the string.
static inline bool NameHas(UnkStringData* const& name, const char* str) {
    ++name->reads;
    if (name == &g_unk_7101d47108->empty)
        return false;
    return fn_710007fb9c(&name->String(), str);
}

// The same, for the text of a string id, read only once the name is known not to be empty.
static inline bool NameHas(UnkStringData* const& name, const CStrId& sub) {
    ++name->reads;
    if (name == &g_unk_7101d47108->empty)
        return false;
    return fn_710007fb9c(&name->String(), sub.Text());
}

// The last of a name's `length` characters; reading it counts a use of the string.
static inline char LastChar(UnkStringData* const& name, u32 length) {
    ++name->reads;
    const char* text = name->text ? name->text : g_unk_710162e2ed;
    return text[length - 1];
}

// A string id's text; reading it counts a use of the string.
static inline const CRntString& TextOf(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

// The text from `start` to the end.
static inline UnkTunableLine Tail(const CRntString& text, u32 start) {
    u32 length = text.length - start;
    return UnkTunableLine(text.data + start, length);
}

// The text in upper case (ASCII letters only).
static inline UnkTunableLine ToUpper(const UnkTunableLine& text) {
    char upper[256];
    upper[0] = '\0';
    const char* src = text.Data();
    u32 length = strlen(src);
    if (length != 0) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            upper[i] = (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
        }
        upper[length] = '\0';
    }
    return UnkTunableLine(upper);
}

// A string id built from a string's text (fn_7100000850).
struct UnkTextStrId {
    UnkStringData* data;

    explicit UnkTextStrId(const UnkTunableLine& text) {
        CStrId_ConstructFromRntString(reinterpret_cast<UnkStrIdBase*>(this), *text.Str(), true);
    }
    ~UnkTextStrId() { reinterpret_cast<CStrId*>(this)->Reset(); }
    const CStrId& Id() const { return *reinterpret_cast<const CStrId*>(this); }
};

static inline UnkCollisionObject* FindCollider(UnkCollisionContainer* colliders, const CStrId& name) {
    return fn_7100864194(colliders, &name);
}

// Shows the colour of the block's level on its tracked node.
static inline void ApplyLevelColor(UnkDemolitionBlockLoad* self) {
    if (UnkDemolitionNode* node = self->node) {
        s32 level = self->level;
        level = level < 3 ? level : 3;
        level = level > 1 ? level : 1;
        u32 index = level - 1;
        const CVector4D& phase_color = kPhaseColors[index];
        CVector4D color = {phase_color.x, phase_color.y, phase_color.z, phase_color.w};
        fn_710007b518(&node->value, &color);
        if (UnkDemolitionNodeTarget* target = node->target)
            *target->value = color;
    }
}

static inline void SetPhaseTwo(UnkDemolitionBlockLoad* self) {
    self->level = 2;
    self->phase = 3;
    ApplyLevelColor(self);
}

void fn_71008c5ad0(UnkDemolitionBlockLoad* self, CBlackboard* blackboard) {
    // The animation request sent when the block is dead: its mode and the type id's mask.
    base::reflection::UnkReflTypeIdArg mode, mask;
    CBlackboard::CSection* section;
    {
        CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
        section = fn_71010472e0(blackboard, section_id);
    }
    if (!section)
        return;

    bool dead = false;
    u64 index = self->IndexOf(g_unk_7172730c98);
    bool found = fn_710006d11c(section, &self->values[index].id, &dead);
    if (dead && found) {
        // Dead: show the debris and hide the rest (but the root).
        bool has_debris = false;
        if (UnkDemolitionModel* model = ModelOf(self->owner)) {
            if (UnkDemolitionModelData* data = model->data) {
                for (u32 i = 0; i < PartCount(data->resource); ++i) {
                    UnkDemolitionPart* part = PartAt(data->resource, i);
                    if (NameHas(part->name, "_DEBRIS")) {
                        u64 key = part->name->Key();
                        fn_7101172764(reinterpret_cast<UnkFlagTarget*>(model), reinterpret_cast<const char*>(&key));
                        has_debris = true;
                    } else if (!NameHas(part->name, "Root")) {
                        u64 key = part->name->Key();
                        fn_7101172780(reinterpret_cast<UnkFlagTarget*>(model), reinterpret_cast<const char*>(&key));
                    }
                }
            }
        }
        if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
            fn_71009c0f1c(target, g_unk_7172730d30.destroyed.data->Key());
        if (UnkCollisionContainer* colliders = self->owner->colliders) {
            for (u32 i = 0; i < colliders->records.count; ++i) {
                auto* collider = reinterpret_cast<UnkDemolitionCollider*>(
                    fn_710086420c(reinterpret_cast<UnkPowerBombColliders_ovb0127*>(colliders), i));
                UnkStringData* const& name = collider->name;
                ++name->reads;
                bool enabled;
                if (name != &g_unk_7101d47108->empty && fn_710007fb9c(&name->String(), "_DEBRIS"))
                    enabled = true;
                else
                    enabled = false;
                fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), enabled);
            }
        }
        if (void* animation = self->owner->animation) {
            u64 hash = 0xc567047b7a4bf0cf;
            if (fn_7101176e78(static_cast<UnkStruct_7101176ea8*>(animation), reinterpret_cast<void*>(hash), nullptr)) {
                mode.id = 0;
                mask.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
                fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_717276974c,
                              &mask);
            }
        }
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b010, false);
        bool held = fn_71005c1008(reinterpret_cast<u64>(self->owner)) != nullptr;
        if (has_debris | held)
            return;
        fn_7100232864(reinterpret_cast<UnkEntityState*>(self->owner), false);
        if (char* drops = fn_71004dad10(reinterpret_cast<UnkActor*>(self->owner))) {
            if (self->drop) {
                fn_71007c1ce0(drops, self->drop);
                self->drop = nullptr;
            }
        }
        fn_7100874460(self->listeners, reinterpret_cast<UnkWeakTarget**>(&self->owner));
        return;
    }

    // Alive: take the definition's phase.
    s32 phase = self->def->phase;
    if (phase != 0) {
        self->phase = phase;
        if (phase == 2) {
            self->level = 2;
            self->phase = 3;
        } else if (phase == 3) {
            self->level = 1;
            fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b018, false);
            self->phase = 4;
        }
        ApplyLevelColor(self);
    }

    // Whether the husk was removed, from the current scenario's section of the game's blackboard.
    bool husk_removed = false;
    {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* game_blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 husk_index = self->IndexOf(g_unk_7172730cc0);
        UnkDemolitionBlockValue_l8c5ad0* values = self->values;
        if (CBlackboard::CSection* game_section = fn_7101047270(game_blackboard, section_id))
            fn_710006d11c(game_section, &values[husk_index].id, &husk_removed);
    }
    if (self->def->husk_removed || husk_removed) {
        SetPhaseTwo(self);
        CStrId name = g_unk_7101d470e0;
        if (UnkCollisionObject* husk = FindCollider(self->owner->colliders, CStrId("HUSK", true))) {
            name = reinterpret_cast<UnkDemolitionHuskCollider*>(husk)->name;
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(husk), false);
            fn_7100d7c548(reinterpret_cast<UnkActor*>(self->owner), &self->dissolve, 0.2f, 0.0f);
            self->sound = fn_7100d7c45c(reinterpret_cast<UnkEntityWithSound*>(self->owner), &self->dissolve_sound,
                                        nullptr, nullptr);
        }
        if (name.data != &g_unk_7101d47108->empty) {
            // Hide the parts whose names contain the husk's suffix (from its last '_'), upper-cased.
            UnkTextStrId suffix(ToUpper(Tail(TextOf(name), fn_710007fb10(&TextOf(name), '_', -1))));
            UnkStringData* text = suffix.data;
            u32 length = text->String().length;
            if (length != 0) {
                char last = LastChar(text, length);
                if (UnkDemolitionModel* model = ModelOf(self->owner)) {
                    if (UnkDemolitionModelData* data = model->data) {
                        for (u32 i = 0; i < PartCount(data->resource); ++i) {
                            UnkDemolitionPart* part = PartAt(data->resource, i);
                            u32 part_length = part->name->String().length;
                            if (part_length == 0 || last != LastChar(part->name, part_length))
                                continue;
                            if (NameHas(part->name, suffix.Id())) {
                                u64 key = part->name->Key();
                                fn_7101172780(reinterpret_cast<UnkFlagTarget*>(model),
                                              reinterpret_cast<const char*>(&key));
                            }
                        }
                    }
                }
            }
        }
    }

    UnkDemolitionPhaseDef* def = self->def;
    if (def->phase == 0 && !def->husk_removed) {
        if (!FindCollider(self->owner->colliders, CStrId("HUSK", true)) || husk_removed)
            SetPhaseTwo(self);
    }
}
