// From unit 2735 of tools/tu_map.py (0x710119ea2c-0x71011a4eb4; reflected classes CScenario and
// CScenario::CTunableScenario): the current scenario, the CGameObject (0x930 bytes) the game
// manager keeps at +0x608 (game/UnkGameManager.h's UnkGameManagerSub_0x608, the type the other
// hand-written files pass it as). Its reflected ids are sLevelID (+0x10) and sScenarioID (+0x18);
// the scenario's blackboard section is named by the second, mapped through the game manager.
// Names and layouts are placeholders.
// Holds fn_71011a2fbc (the section's id), fn_71011a224c (loads the scenario's collision objects), fn_71011a2190 (reads an object file through unit
// 2660's reader), fn_71011a2500 (switches to the active collider the blackboard names, or to the
// default one) and fn_71011a4050 (the scenario's start).
#include "base/UnkStrIdPathPair.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

// The members of CScenario this file reads (the reflected ids).
struct UnkScenarioIds {
    u8 pad_0x0[0x10];
    CStrId sLevelID;
    CStrId sScenarioID;
};
static inline UnkScenarioIds* Ids(UnkGameManagerSub_0x608* self) {
    return reinterpret_cast<UnkScenarioIds*>(self);
}

// The scenario's blackboard section id: sScenarioID mapped through the game manager.
CStrId fn_71011a2fbc(UnkGameManagerSub_0x608* self) {
    return fn_7100dfbda4(g_unk_7101d47310, Ids(self)->sScenarioID);
}

// The id and path pair (base/UnkStrIdPathPair.h) fn_710008d680 builds from a path and
// fn_710008d8e8 destroys (as src/game/UnkObjectFileRead.cpp names it).
struct UnkStrPair_0x10 {
    void* a;
    void* b;
};
void fn_710008d680(UnkStrPair_0x10* s, const char* path);

// Unit 2660's reader (src/game/UnkObjectFileRead.cpp): no state, and an empty constructor and
// destructor.
struct UnkObjectFileReadOp {
    u8 unk_0x0;
};
void fn_7101062390(UnkObjectFileReadOp* self);
void fn_7101062394(UnkObjectFileReadOp* self);
void fn_7101062398(void*, void*, const UnkStrPair_0x10* path);

// Reads the object file at `path` (fn_7101062398; `arg` is passed through).
void fn_71011a2190(void* arg, const char* path) {
    UnkObjectFileReadOp op;
    fn_7101062390(&op);
    {
        UnkStrPair_0x10 s;
        fn_710008d680(&s, path);
        fn_7101062398(&op, arg, &s);
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    fn_7101062394(&op);
}

// A blackboard property seen as a definition whose qualified id (game/UnkGameBlackboard.h) is
// built on first use (as src/game/CGameManager.cpp's UnkGameManagerDef).
struct UnkScenarioDef : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};
static inline const UnkQualifiedId& Qid(UnkScenarioDef& def) {
    if (def.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}
// "ScenarioActiveCollider", a property of type CStrId (src/game_s7/fn_7100ae39e0.cpp builds it).
extern UnkPropertyEx<> g_unk_7172759808;

// The id table (fn_7100031894 builds it; the same definition as src/game/CGameManager.cpp's and
// src/game/UnkSubareaSetupState.cpp's). Its +0xb40 is the id of the default scenario setup.
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x17e0];
    u8 field_0x17e0[8];
    u8 pad_0x17e8[0x2578 - 0x17e8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// Reads the string id under the property `name` names (refl_GetKeyedValue for CStrId).
bool fn_710051d464(CBlackboard::CSection* section, const void* name, CStrId* out);
void fn_71011a2fd0(UnkGameManagerSub_0x608* self, const CStrId& id, bool arg2);

// A string id built empty inline: the pool is created first when there is none.
struct UnkScenarioEmptyStrId {
    union {
        CStrId id;
    };

    UnkScenarioEmptyStrId() {
        id.data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        id.data = &g_unk_7101d47108->empty;
    }
    ~UnkScenarioEmptyStrId() { id.~CStrId(); }
};

// Switches to the collider the scenario's blackboard section names under "ScenarioActiveCollider"
// (fn_71011a2fd0), or to the default one from the id table when there is none.
void fn_71011a2500(UnkGameManagerSub_0x608* self) {
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    UnkScenarioEmptyStrId setup;
    {
        CStrId section = fn_7100dfbda4(g_unk_7101d47310, Ids(self)->sScenarioID);
        const UnkQualifiedId& name = Qid(reinterpret_cast<UnkScenarioDef&>(g_unk_7172759808));
        if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
            fn_710051d464(s, &name, &setup.id);
    }
    if (setup.id.data == &g_unk_7101d47108->empty)
        setup.id = *reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(&GameManagerIdTable()) + 0xb40);
    fn_71011a2fd0(self, setup.id, false);
}

// The scenario's start: resets the blackboard's NumTanksPickedUpMax counter when it is set, shows
// the level's name (a label with the text the label table gives for the scenario, and a sprite with
// the image the sprite table gives) in the GUI root's menu, tells the navigation mesh, and calls
// fn_71002326d0 on every object in the scenario's three groups.
#include "base/math/CVector.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkGuiSprite.h"
#include "game/UnkNavMeshManager.h"

// The scenario's members this part reads.
struct UnkScenarioNode {
    u8 pad_0x0[0x8];
    void* object;
    UnkScenarioNode* next;
};
struct UnkScenarioGroup {
    u8 pad_0x0[0x50];
    UnkScenarioNode* nodes;
    u8 pad_0x58[0x8];
};
static_assert(sizeof(UnkScenarioGroup) == 0x60, "UnkScenarioGroup placeholder layout");
struct UnkScenarioGroups {
    UnkScenarioGroup* data;
    void* keys;
    u32 count;
    u8 pad_0x14[0xb4];
};
static_assert(sizeof(UnkScenarioGroups) == 0xc8, "UnkScenarioGroups placeholder layout");
struct UnkScenarioView {
    u8 pad_0x0[0x10];
    CStrId sLevelID;
    CStrId sScenarioID;
    u8 pad_0x20[0x8];
    UnkScenarioGroups groups[3];
    u8 pad_0x280[0x5d8];
    u8 field_0x858[0xc0];
    UnkGuiDisplayObject* level_label;
    UnkGuiDisplayObject* level_sprite;
};
static_assert(__builtin_offsetof(UnkScenarioView, field_0x858) == 0x858, "UnkScenarioView placeholder layout");
static_assert(__builtin_offsetof(UnkScenarioView, level_sprite) == 0x920, "UnkScenarioView placeholder layout");

// "NumTanksPickedUpMax", a property of type s32 (src/game_s7/fn_7100ae39e0.cpp builds it).
extern UnkPropertyEx<> g_unk_7172759138;

bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);
// A typed value as the section setter takes it (as in src/game/CGameManager.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
const base::global::CStrId* fn_7100de9bf8(UnkGameManager* self);

// The language singleton (as src/game/UnkAdamDialog.cpp declares it): created on first use, it
// gives localized texts.
struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
void fn_710119c020(UnkLanguage* language);
bool fn_710119c9fc(UnkLanguage* language);
s32 fn_710119c9f4(UnkLanguage* language);
void fn_7100066e44(void* text);
// The text the label is given (game/UnkGuiLabel.h), with its destructor.
struct UnkDialogText : UnkGuiText {
    ~UnkDialogText() { fn_7100066e44(this); }
};
UnkDialogText fn_710119d040(UnkLanguage* language, const CStrId& key);
static inline UnkLanguage* Language() {
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    return language;
}

// The scenario's tunable (src/reflection/CScenario.cpp registers it): where the level's name is shown.
// A base::math::CVector2D copied member by member (as src/game_s7/UnkGuiS7.h's colour).
struct UnkVec2f : base::math::CVector2D {
    UnkVec2f(const UnkVec2f& other) {
        x = other.x;
        y = other.y;
    }
};
// The address of `tmp`: given a temporary copy, the copy lives until the end of the call's full
// expression it is made in.
static inline const UnkVec2f* Addr(const UnkVec2f& tmp) {
    return &tmp;
}
struct UnkScenarioTunableView {
    u8 pad_0x0[0x24];
    UnkVec2f label_pos;
    UnkVec2f sprite_pos;
};
extern UnkTunable g_unk_7172a0bea8;
static inline const UnkScenarioTunableView& ScenarioTunable() {
    return reinterpret_cast<const UnkScenarioTunableView&>(g_unk_7172a0bea8);
}

// The label texts and the sprite images by scenario (string ids by string id).
extern UnkGameManagerIdMap g_unk_7172a0bf10;
extern UnkGameManagerIdMap g_unk_7172a0bee8;

bool fn_71011ceac0(UnkGuiDisplayObject* self);
bool fn_71011ceb34(UnkGuiDisplayObject* self);
void fn_71011b5598(UnkGuiRoot* root, UnkGuiDisplayObject* object);

// The display object's members read here.
struct UnkGuiObjectView {
    u8 pad_0x0[0x130];
    CStrId name;
    u8 pad_0x138[0x4c];
    bool field_0x184;
    u8 pad_0x185[0x2];
    bool field_0x187;
    bool field_0x188;
    u8 pad_0x189[0x16f];
    // A union member: no type-based alias info, so the object's pointer is reloaded after the store.
    union {
        u32 field_0x2f8;
    };
};
static_assert(__builtin_offsetof(UnkGuiObjectView, field_0x2f8) == 0x2f8, "UnkGuiObjectView placeholder layout");
static inline UnkGuiObjectView* View(UnkGuiDisplayObject* object) {
    return reinterpret_cast<UnkGuiObjectView*>(object);
}
static inline void Vf18(UnkGuiDisplayObject* object) {
    reinterpret_cast<void (*)(UnkGuiDisplayObject*)>((*reinterpret_cast<void***>(object))[0x18 / 8])(object);
}
static inline void Vf78(UnkGuiDisplayObject* object, s32 a, u32 b) {
    reinterpret_cast<void (*)(UnkGuiDisplayObject*, s32, u32)>((*reinterpret_cast<void***>(object))[0x78 / 8])(
        object, a, b);
}

// Names a newly built object and hands it to the root when it is to be shown.
static inline void Setup(UnkGuiRoot* root, UnkGuiDisplayObject* object, const CStrId& name) {
    Vf18(object);
    View(object)->name = name;
    UnkGuiObjectView* v = View(object);
    if (!v->field_0x187 &&
        (v->field_0x188 || v->field_0x184 ||
         (fn_71011cde14(object)->field_0x3c && (fn_71011ceac0(object) || fn_71011ceb34(object)))))
        fn_71011b5598(root, object);
}

// A GUI::CLabel (0x340 bytes) and a GUI::CSprite (0x1f0 bytes): their constructors.
struct UnkStruct_71011ebdb4;
void fn_71011ebdb4(UnkStruct_71011ebdb4* self);
void fn_71011f890c(UnkGuiDisplayObject* self);
void fn_71011ed150(UnkGuiDisplayObject* label, const CStrId& font);
void fn_71011ef0c8(UnkGuiDisplayObject* label, bool value);
void fn_71011f9878(UnkGuiDisplayObject* sprite);

void fn_7100e148a0(void* self);
void fn_710027a24c(UnkNavMesh* navmesh);
void fn_71002326d0(void* object);

void fn_71011a4050(UnkGameManagerSub_0x608* scenario) {
    UnkScenarioView* self = reinterpret_cast<UnkScenarioView*>(scenario);
    s32 tanks = -1;
    UnkGameManager* gm = g_unk_7101d47310;
    CBlackboard* blackboard = &gm->field_0x968;
    CStrId section = fn_7100dfbda4(gm, *fn_7100de9bf8(gm));
    UnkScenarioDef& def = reinterpret_cast<UnkScenarioDef&>(g_unk_7172759138);
    {
        const UnkQualifiedId& name = Qid(def);
        if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
            fn_7100395a30(s, &name, &tanks);
    }
    if (tanks > 0) {
        const UnkQualifiedId& name = Qid(def);
        s32 zero = 0;
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
            if (!blackboard->field_0xe8) {
                UnkTypedValue v;
                v.type = fn_7100014768();
                v.value = &zero;
                fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
            }
        }
    }
    UnkGuiRoot* root = g_unk_717271e8f8;
    if (UnkGuiDisplayObject* menu = root->menu) {
        bool shown = fn_710119c9fc(Language()) || fn_710119c9f4(Language()) == 0xe;
        UnkGameManagerIdMap* labels = &g_unk_7172a0bf10;
        if (!self->level_label && shown && labels->Find(self->sScenarioID) != labels->values + labels->count) {
            {
                CStrId name("LevelNameLabel", true);
                UnkGuiDisplayObject* label = static_cast<UnkGuiDisplayObject*>(fn_71000839a0(0x340, 0xc));
                fn_71011ebdb4(reinterpret_cast<UnkStruct_71011ebdb4*>(label));
                if (label)
                    Setup(root, label, name);
                self->level_label = label;
            }
            if (UnkGuiDisplayObject* label = self->level_label) {
                fn_71011d968c(menu, label);
                {
                    UnkGuiDisplayObject* l = self->level_label;
                    CStrId font("digital_big", true);
                    fn_71011ed150(l, font);
                }
                {
                    UnkGuiDisplayObject* l = self->level_label;
                    UnkLanguage* language = Language();
                    UnkDialogText text = fn_710119d040(language, *labels->Find(self->sScenarioID));
                    fn_71011ec468(l, text, false);
                }
                fn_71011cde64(self->level_label, Addr(UnkVec2f(ScenarioTunable().label_pos)), 1, 1);
                fn_71011cde44(self->level_label, 2);
                Vf78(self->level_label, -2000, 0);
                fn_71011d0088(self->level_label, 0);
                fn_71011ed018(self->level_label, 1);
                View(self->level_label)->field_0x2f8 = 3;
                fn_71011ef0c8(self->level_label, true);
                fn_71011cdce8(self->level_label, false);
            }
        }
        if (!self->level_sprite) {
            {
                CStrId name("LevelNameSprite", true);
                UnkGuiDisplayObject* sprite = static_cast<UnkGuiDisplayObject*>(fn_71000839a0(0x1f0, 0xc));
                fn_71011f890c(sprite);
                if (sprite)
                    Setup(root, sprite, name);
                self->level_sprite = sprite;
            }
            UnkGameManagerIdMap* sprites = &g_unk_7172a0bee8;
            if (UnkGuiDisplayObject* sprite = self->level_sprite) {
                if (sprites->Find(self->sScenarioID) != sprites->values + sprites->count) {
                    fn_71011d968c(menu, sprite);
                    fn_71011cde44(self->level_sprite, 2);
                    Vf78(self->level_sprite, -2000, 0);
                    fn_71011f8bdc(self->level_sprite, *sprites->Find(self->sScenarioID));
                    fn_71011cdc30(self->level_sprite, true);
                    fn_71011cde64(self->level_sprite, Addr(UnkVec2f(ScenarioTunable().sprite_pos)), 1, 1);
                    fn_71011f9878(self->level_sprite);
                    fn_71011cdce8(self->level_sprite, false);
                }
            }
        }
    }
    fn_7100e148a0(self->field_0x858);
    UnkNavMeshManager* nav = g_unk_71726a4510;
    if (!nav) {
        nav = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(nav);
        g_unk_71726a4510 = nav;
    }
    if (nav->navmesh)
        fn_710027a24c(nav->navmesh);
    for (UnkScenarioGroups& groups : self->groups) {
        UnkScenarioGroup* end = groups.data + groups.count;
        for (UnkScenarioGroup* g = groups.data; g != end; ++g) {
            for (UnkScenarioNode* n = g->nodes; n;) {
                void* object = n->object;
                n = n->next;
                if (object)
                    fn_71002326d0(object);
            }
        }
    }
}

// ---- fn_71011a224c: loading the scenario's collision objects ----

#include <new>

#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"

// The collision file's reader (as src/game/CSubAreaManager.cpp declares it).
struct UnkCollisionFileReader {
    u8 unk_0x0[0x18];
};
void fn_7100d45fc4(UnkCollisionFileReader* self, void* arg);
void fn_7100d45fd4(UnkCollisionFileReader* self);
void fn_7100860830(UnkCollisionFileReader* self, const char* path, base::UnkVector<UnkCollisionRecord>* out,
                   bool a, void* b);

// The scenario's array of weak references to its collision objects (+0x828; base::UnkVector's
// layout, Reserve out of line as in src/game/logic/collision/CCollider/fn_7100d407c0.cpp).
template <class Tag>
struct UnkRefPtrArray {
    UnkRefPtr* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};
struct UnkTag_71007ed25c;
void fn_71007ed25c(UnkRefPtrArray<UnkTag_71007ed25c>* self, u32 capacity);

struct UnkScenarioColliders {
    u8 pad_0x0[0x828];
    UnkRefPtrArray<UnkTag_71007ed25c> colliders;
};

// Appends a counted copy of `value` (PushBack's growth, including its hang).
static inline void Append(UnkRefPtrArray<UnkTag_71007ed25c>* self, const UnkRefPtr& value) {
    u32 n = self->count;
    u32 c = self->capacity;
    if (c <= n && self->step != 0)
        c = (n / self->step + 1) * self->step;
    if ((s32)c <= (s32)n)
        for (;;) {
        }
    fn_71007ed25c(self, c);
    UnkRefPtr* slot = &self->data[self->count];
    slot->field_0x0 = nullptr;
    AssignRef(&slot->field_0x0, value.field_0x0);
    ++self->count;
}

void fn_71011a1d00();

// Clears the scenario's collision objects, reads those of the file at `path` and keeps a weak
// reference to each, then switches to the active collider (fn_71011a2500).
// NON_MATCHING(minor): registers and scheduling (the original reloads the array's count before each
// increment and keeps one more callee-saved register).
void fn_71011a224c(UnkGameManagerSub_0x608* self, const char* path) {
    fn_71011a1d00();
    UnkCollisionFileReader reader;
    fn_7100d45fc4(&reader, self);
    base::UnkVector<UnkCollisionRecord> records(7);
    fn_7100860830(&reader, path, &records, false, nullptr);
    auto* colliders = &reinterpret_cast<UnkScenarioColliders*>(self)->colliders;
    for (u64 i = 0; i < records.count; ++i) {
        auto* object = reinterpret_cast<UnkWeakTarget*>(records.data[i].field_0x28);
        UnkRefPtr ref;
        UnkWeakHandle* handle = GetWeakHandle(object);
        if (handle)
            ++handle->ref_count;
        ref.field_0x0 = handle;
        Append(colliders, ref);
    }
    fn_71011a2500(self);
    records.Free();
    fn_7100d45fd4(&reader);
}
