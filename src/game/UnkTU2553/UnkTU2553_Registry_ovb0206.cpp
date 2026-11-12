// The step callback array's element accessor is inlined (base/UnkVector.h).
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))

// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): the game manager's registries, load and start-up
// steps (batch b0206). Each function is introduced by a comment. Names and layouts are placeholders.
#include <stdio.h>
#include <stdlib.h>

#include <nn/os.h>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDefaultAllocator.h"
#include "base/UnkDestructors.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPool.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/CBombGun.h"
#include "game/CBossAIComponent.h"
#include "game/CDroppableLifeComponent.h"
#include "game/CGameManager/CTunableGameManager.h"
#include "game/GUI/CLoadCheckPointButton.h"
#include "game/GUI/CTutorialComposition.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkItemProperties.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2559.h"
#include "game/UnkTU2571.h"
#include "game/UnkTU2733.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU341.h"
#include "game/UnkTU57.h"
#include "game/UnkTU63.h"
#include "game/UnkTU83.h"
#include "game/UnkTUProperty.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// ---- fn_7100df5cec ----
// CGameManager::CTunableGameManager's unit: replaces the id at +0x2238; when the old one was not empty the
// music manager's channel 1 is faded out and its bank (+0x2b8) reset first. Names are placeholders.
using base::global::CStrId;

struct UnkMgr_0xf18;
void fn_7100fd0e74(void* bank);
extern UnkMgr_0xf18* g_unk_71726bc560;

static inline char* MusicManager_df5cec() {
    char* object = reinterpret_cast<char*>(g_unk_71726bc560);
    if (!object) {
        object = static_cast<char*>(fn_71000839a0(0xf18, 0x3f));
        fn_7100fd3804(object);
        reinterpret_cast<void*&>(g_unk_71726bc560) = object;
    }
    return object;
}

void fn_7100df5cec(UnkGameManager* self, void* target) {
    CStrId id = *static_cast<const CStrId*>(target);
    if (id.data != self->field_0x2238.data) {
        if (self->field_0x2238.data != &g_unk_7101d47108->empty) {
            fn_7100fdb9e0(reinterpret_cast<UnkMgr_0xf18*>(MusicManager_df5cec()), 1, 0, -1.0f, 0.0f);
            fn_7100fd0e74(MusicManager_df5cec() + 0x2b8);
        }
        if (self->field_0x2238.data != id.data)
            self->field_0x2238 = id;
    }
}

// ---- fn_7100e01d98 ----
// CGameManager::CTunableGameManager's unit: the scenario file path "<root>maps/levels/<scenario>/<name>/<name>.<ext>"
// as a file-path id (the root folder is the game manager's string at +0x620). Names are placeholders.
using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

CRntString fn_710007ef90(const CRntString* a, const char* b);

// The pooled data behind an id as its text accessor sees it: the text at +0x10 and a count of the
// reads at +0x44.
struct UnkIdTextData_e01d98 {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 reads;
    };
};

static inline const CRntString* Text_e01d98(const CStrId& id) {
    UnkIdTextData_e01d98* data = reinterpret_cast<UnkIdTextData_e01d98*>(id.data);
    ++data->reads;
    return &reinterpret_cast<UnkIdTextData_e01d98*>(id.data)->str;
}

// A file-path id built from a string out of line, returned in memory.
struct UnkPathResult_e01d98 {
    union {
        CFilePathStrId id;
    };
    explicit UnkPathResult_e01d98(const CRntString& str) { fn_710001a270(&id, str, true); }
    UnkPathResult_e01d98(const UnkPathResult_e01d98& other) : id(other.id) {}
    ~UnkPathResult_e01d98() { id.~CFilePathStrId(); }
};

static inline CRntString Cat_e01d98(const CRntString& a, const CRntString& b) { return fn_710007eee0(&a, &b); }
static inline CRntString Cat_e01d98(const CRntString& a, const char* b) { return fn_710007ef90(&a, b); }

UnkPathResult_e01d98 fn_7100e01d98(UnkGameManager* self, const CStrId& scenario, const CStrId& name,
                                   const CStrId& ext) {
    const CRntString& root = *reinterpret_cast<const CRntString*>(reinterpret_cast<char*>(self) + 0x620);
    return UnkPathResult_e01d98(Cat_e01d98(Cat_e01d98(Cat_e01d98(Cat_e01d98(Cat_e01d98(Cat_e01d98(Cat_e01d98(Cat_e01d98(root, "maps/levels/"), *Text_e01d98(scenario)), "/"), *Text_e01d98(name)), "/"), *Text_e01d98(name)), "."), *Text_e01d98(ext)));
}

// ---- fn_7100decc8c ----
// CGameManager::CTunableGameManager's unit: resets the game manager's object registries: leaves the
// script environment, lets go of the objects on the list at +0x2778 and empties it, resets the pack
// sets, and switches the objects on the list at +0x27e8 that the lookup knows (empties it too).
// Names and layouts are placeholders.
struct UnkEntityState;

void fn_71011a2e1c(UnkGameManagerSub_0x608* env);
void fn_71011a2dac(UnkGameManagerSub_0x608* env);
void fn_7100dea304(UnkGameManager* self, void* object);
void fn_7100089894(base::file::UnkFileDevice* self);
void fn_7100dec088(UnkGameManager* self);
void fn_7100232864(UnkEntityState* entity, bool value);
extern base::file::UnkFileDevice* g_unk_7101d4d828;

struct UnkNode_decc8c {
    struct UnkEntry {
        u8 pad_0x0[0x30];
        u64 id;
    };
    struct UnkObject {
        u8 pad_0x0[0x30];
        UnkEntry* entry;
    };
    UnkObject* object;
    UnkNode_decc8c* next;
};

struct UnkNodeList_decc8c {
    u32 count;
    UnkNode_decc8c* head;
    UnkNode_decc8c* tail;
    base::UnkAllocator* allocator;

    void Clear() {
        for (UnkNode_decc8c* node = head; node;) {
            UnkNode_decc8c* next = node->next;
            allocator->vf_0x48(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkGameManagerLists_decc8c {
    u8 pad_0x0[0x2770];
    UnkNodeList_decc8c taken;  // 0x2770
    u8 pad_0x2790[0x50];
    UnkNodeList_decc8c pending;  // 0x27e0
};
static_assert(__builtin_offsetof(UnkGameManagerLists_decc8c, pending) == 0x27e0, "layout");

static inline base::file::UnkFileDevice* Singleton_decc8c() {
    base::file::UnkFileDevice* object = g_unk_7101d4d828;
    if (!object) {
        object = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(object);
        g_unk_7101d4d828 = object;
    }
    return object;
}

void fn_7100decc8c(UnkGameManager* self) {
    UnkGameManagerLists_decc8c* lists = reinterpret_cast<UnkGameManagerLists_decc8c*>(self);
    if (self->field_0x608) {
        fn_71011a2e1c(self->field_0x608);
        fn_71011a2dac(self->field_0x608);
    }
    fn_7100de6ee8(self);
    for (UnkNode_decc8c* node = lists->taken.head; node; node = node->next)
        fn_7100dea304(self, node->object);
    lists->taken.Clear();
    fn_7100089894(Singleton_decc8c());
    fn_7100dec088(self);
    for (UnkNode_decc8c* node = lists->pending.head; node; node = node->next) {
        UnkNode_decc8c::UnkObject* object = node->object;
        if (!object)
            continue;
        u64 id = object->entry->id;
        alignas(8) u32 flags = g_unk_71727bebb8;
        if (fn_7100deba8c(self, &id, &flags))
            fn_7100232864(reinterpret_cast<UnkEntityState*>(object), true);
    }
    lists->pending.Clear();
}

// ---- fn_7100de92e8 ----
// CGameManager::CTunableGameManager's unit: empties a map of lists of reference-counted objects by
// string id (parallel arrays of lists and ids, as UnkIdMap): releases every id, drops and frees every
// list, and zeroes the count. Names and layouts are placeholders.
using base::global::CStrId;

struct UnkRefList_de92e8 {
    UnkRefObject** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkRefListMap_de92e8 {
    UnkRefList_de92e8* values;
    CStrId* keys;
    u32 count;
};

void fn_7100de92e8(UnkRefListMap_de92e8* self) {
    if (self->keys && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].Reset();
    }
    if (self->values && self->count) {
        for (u64 i = 0; i < self->count; ++i) {
            UnkRefList_de92e8* list = &self->values[i];
            if (list->data) {
                for (u64 j = 0; j < list->count; ++j) {
                    if (UnkRefObject* object = list->data[j]) {
                        if (--object->ref_count == 0)
                            object->vf_0x10();
                    }
                }
                UnkRefObject** data = list->data;
                list->count = 0;
                list->allocator->vf_0x48(data);
                list->capacity = 0;
            }
        }
    }
    self->count = 0;
}

// ---- fn_7100deb914 ----
// CGameManager::CTunableGameManager's unit: the first object of the script environment's list (+0x2b0 of the
// object behind +0x608) whose name is `id` and whose second component's state is not 2; null when
// there is none. A pool list of 0x10-byte nodes is built and destroyed on the way. Names and layouts
// are placeholders.
using base::global::CStrId;

struct UnkEntity_b0165;
struct UnkPoolListTag_deb914;
template <>
struct base::UnkPoolListTraits<UnkPoolListTag_deb914> {
    static char* vtable() { return g_unk_7101a20a30; }
    static char* base_vtable() { return g_unk_7101a20a50; }
};
using UnkPoolList_deb914 = base::UnkPoolList<UnkPoolListTag_deb914, 0x8, 0x10>;

struct UnkNameHolder_b0161 {
    CStrId id;
};
struct UnkObjectNode_deb914 {
    void* object;
    UnkObjectNode_deb914* next;
};
struct UnkEnvView_deb914 {
    u8 pad_0x0[0x2a8];
    u32 field_0x2a8;
    u8 pad_0x2ac[4];
    UnkObjectNode_deb914* objects;  // 0x2b0
};

void fn_7100de5f98(UnkPoolList_deb914* self, u32 n, u32 a, u32 flags, base::UnkAllocator* allocator);

struct UnkSecond_deb914 {
    u8 pad_0x0[0x70];
    s32 state;
};

void* fn_7100deb914(UnkGameManager* self, const CStrId* id) {
    UnkEnvView_deb914* env = reinterpret_cast<UnkEnvView_deb914*>(self->field_0x608);
    UnkPoolList_deb914 list;
    fn_7100de5f98(&list, env->field_0x2a8, 0, 5, nullptr);
    for (UnkObjectNode_deb914* node = reinterpret_cast<UnkEnvView_deb914*>(self->field_0x608)->objects; node;
         node = node->next) {
        void* object = node->object;
        if (fn_7100232fcc(object)->id.data != id->data)
            continue;
        UnkSecond_deb914* second = static_cast<UnkSecond_deb914*>(fn_71007bb940(static_cast<UnkEntity_b0165*>(object)));
        if (second && second->state != 2)
            return object;
    }
    return nullptr;
}

// ---- fn_7100e00b48 ----
// CGameManager::CTunableGameManager's unit: hands the pack set path at +0x610 to the sound manager, one
// file path at a time (the files fn_7100089eec lists under it), and then unmounts the set. Names and
// layouts are placeholders.
using base::UnkStrIdMemberOf;

struct UnkSoundMgr;

// UnkListedPath and UnkFilePathList as src/base/file/CPackManagerDevice.cpp defines them.
struct UnkListedPath {
    UnkStrIdMemberOf<g_unk_7101d47200> id;
    UnkListedPath* next;
};
struct UnkFilePathList {
    void** vtable;
    u32 count;
    UnkListedPath* head;
    UnkListedPath* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    UnkFilePathList() {
        vtable = &g_unk_7101a1b030[2];
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkFilePathList() {
        vtable = &g_unk_7101a1b030[2];
        for (UnkListedPath* n = head; n;) {
            UnkListedPath* next = n->next;
            n->id.~UnkStrIdMemberOf();
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

extern base::file::UnkFileDevice* g_unk_7101d4d828;
extern UnkSoundMgr* g_unk_7101d4d820;
void fn_7100089eec(base::file::UnkFileDevice* self, const CFilePathStrId* path, UnkFilePathList* out);
bool fn_7100089ef4(base::file::UnkFileDevice* self, const base::global::UnkFilePathStrIdBase* path);
void fn_7100108c94(UnkSoundMgr* self, const CFilePathStrId* path);

static inline base::file::UnkFileDevice* PackSets_e00b48() {
    base::file::UnkFileDevice* object = g_unk_7101d4d828;
    if (!object) {
        object = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(object);
        g_unk_7101d4d828 = object;
    }
    return object;
}

static inline const CFilePathStrId* PackPath_e00b48(UnkGameManager* self) {
    return reinterpret_cast<const CFilePathStrId*>(reinterpret_cast<char*>(self) + 0x610);
}

void fn_7100e00b48(UnkGameManager* self) {
    UnkFilePathList list;
    fn_7100089eec(PackSets_e00b48(), PackPath_e00b48(self), &list);
    for (UnkListedPath* n = list.head; n; n = n->next)
        fn_7100108c94(g_unk_7101d4d820, reinterpret_cast<const CFilePathStrId*>(n));
    fn_7100089ef4(PackSets_e00b48(), PackPath_e00b48(self));
}

// ---- fn_7100dfb1b0 ----
// CGameManager::CTunableGameManager's unit: the numbered title for an entry named "<7 characters><number>":
// the localized text of "#GUI_MISC023" followed by the number plus one (1 when the name has no
// number). Names and layouts are placeholders.
using base::global::CRntString;
using base::global::CStrId;

struct UnkLanguage;
extern UnkLanguage* g_unk_7101d47358;
// The text the language gives (as src/game/CScenario.cpp declares it).
struct UnkDialogText : UnkGuiText {
    ~UnkDialogText() { fn_7100066e44(this); }
    const u32* Chars() const { return *reinterpret_cast<const u32* const*>(unk_0x0 + 8); }
};
UnkDialogText fn_710119d040(UnkLanguage* language, const CStrId& key);
extern const u32 g_unk_7101648434[] __attribute__((visibility("hidden")));

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkLuaDoString_b50.cpp.
struct UnkInlineString_dfb1b0 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_dfb1b0() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    ~UnkInlineString_dfb1b0() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

CRntString fn_7100dfb1b0(UnkGameManager* manager, const base::UnkNamedEntry* key) {
    const CRntString* name = reinterpret_cast<const CRntString*>(key);
    s32 number;
    u32 length = name->length;
    if (length >= 7) {
        const char* text = name->data + 7;
        UnkInlineString_dfb1b0 digits;
        fn_710007e7f8(digits.Str(), text, length - 7);
        number = atoi(digits.Str()->data) + 1;
    } else {
        number = 1;
    }
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    char buffer[0x100];
    {
        CStrId id("#GUI_MISC023", true);
        UnkDialogText text = fn_710119d040(language, id);
        const u32* chars = text.Chars();
        sprintf(buffer, "%ls %d", chars ? chars : g_unk_7101648434, number);
    }
    return CRntString(buffer);
}

// ---- fn_7100df703c ----
// CGameManager::CTunableGameManager's unit: a copy of one of three texts the game manager's id table
// keeps (at +0x1ca8, +0x1cb0 and +0x1cb8) by a kind: 1 and 2 pick the second and the third, any
// other the first. Names and layouts are placeholders.
using base::global::CRntString;
using base::global::CStrId;

struct UnkGameManagerIdTable {
    u8 pad_0x0[0x2578];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};

// The pooled data behind an id as its text accessor sees it: the text at +0x10 and a count of the
// reads at +0x44.
struct UnkIdTextData_df703c {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 reads;
    };
};

inline UnkGameManagerIdTable& IdTable_df703c() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table;
}

inline CRntString TableText_df703c(u32 offset) {
    const CStrId& id = *reinterpret_cast<const CStrId*>(IdTable_df703c().pad_0x0 + offset);
    UnkIdTextData_df703c* data = reinterpret_cast<UnkIdTextData_df703c*>(id.data);
    ++data->reads;
    return CRntString(reinterpret_cast<UnkIdTextData_df703c*>(id.data)->str);
}

CRntString fn_7100df703c(u32 kind) {
    u8 k = kind;
    if (k == 1)
        return TableText_df703c(0x1cb0);
    if (k == 2)
        return TableText_df703c(0x1cb8);
    return TableText_df703c(0x1ca8);
}

// ---- fn_7100df2dd4 ----
// CGameManager::CTunableGameManager's unit: files a request in the game manager: an id (+0x550), five flags
// (+0x4a2..+0x4a7), two more ids (+0x558, +0x560) and a vector (+0x568). Names and layouts are
// placeholders.
using base::global::CStrId;

struct UnkLuaVec3_b0164 {
    f32 x, y, z;
};

// The members of the game manager this function writes that the shared layout leaves as padding.
struct UnkGameManagerRequestView_df2dd4 {
    u8 pad_0x0[0x4a1];
    bool field_0x4a1;
    bool field_0x4a2;
    bool field_0x4a3;
    bool field_0x4a4;
    bool field_0x4a5;
    bool field_0x4a6;
    bool field_0x4a7;
    u8 pad_0x4a8[0xa8];
    CStrId field_0x550;
    CStrId field_0x558;
    CStrId field_0x560;
    u32 field_0x568;
    u32 field_0x56c;
    u32 field_0x570;
};
static_assert(__builtin_offsetof(UnkGameManagerRequestView_df2dd4, field_0x570) == 0x570, "layout");

void fn_7100df2dd4(UnkGameManager* self, const CStrId& a, u8 b1, u8 b2, u8 b3, u8 b4, u8 b5, const CStrId& c,
                   const CStrId& d, const UnkLuaVec3_b0164& v) {
    UnkGameManagerRequestView_df2dd4* view = reinterpret_cast<UnkGameManagerRequestView_df2dd4*>(self);
    bool f1 = b1 & 1;
    bool f2 = b2 & 1;
    bool f3 = b3 & 1;
    bool f4 = b4 & 1;
    bool f5 = b5 & 1;
    view->field_0x550 = a;
    view->field_0x4a2 = f1;
    view->field_0x4a3 = f2;
    view->field_0x4a4 = f3;
    view->field_0x4a1 = true;
    view->field_0x4a5 = false;
    view->field_0x4a6 = f4;
    view->field_0x4a7 = f5;
    view->field_0x558 = c;
    view->field_0x560 = d;
    const u32* words = reinterpret_cast<const u32*>(&v);
    view->field_0x568 = words[0];
    view->field_0x56c = words[1];
    view->field_0x570 = words[2];
}

// ---- fn_7100dfc428 ----
// CGameManager::CTunableGameManager's unit: the callback at `index` of a step-callback array, which first
// grows (zero-filled) to cover it. Names and layouts are placeholders.

struct UnkStepCallback {
    // Union members: no type-based alias info, so the array's count is reloaded after each copy.
    union {
        void* context;
    };
    union {
        void (*fn)(void* context);
    };
    UnkStepCallback() = default;
    UnkStepCallback(const UnkStepCallback& other) : context(other.context), fn(other.fn) {}
};
struct UnkStepCallbacks {
    base::UnkVector<UnkStepCallback> callbacks;
};

UnkStepCallback* fn_7100dfc428(UnkStepCallbacks* self, u32 index) { return &self->callbacks.GrowAt(index); }

// ---- fn_7100dfbba8 ----
// CGameManager::CTunableGameManager's unit: the percentage of the tanks picked up
// ("NumTanksPickedUp" / "NumTanksPickedUpMax", read from the blackboard section that field_0x1000
// maps `id` to), 0 when there is no maximum. Names and layouts are placeholders.
using base::global::CStrId;

bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);

extern UnkPropertyEx<> g_unk_7172759138;
extern UnkPropertyEx<> g_unk_71727590f8;

// The property's qualified id, built on first use.
static inline const UnkQualifiedId& Qid_dfbba8(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// fn_7100dfbda4 (src/game/CGameManager.cpp), inlined.
static inline __attribute__((always_inline)) CStrId Mapped_dfbba8(UnkGameManager* self, const CStrId& id) {
    CStrId* it = self->field_0x1000.Find(id);
    if (it != self->field_0x1000.values + self->field_0x1000.count)
        return *it;
    return id;
}

s32 fn_7100dfbba8(void* unused, const CStrId* id) {
    UnkGameManager* manager = g_unk_7101d47310;
    CStrId section_id = Mapped_dfbba8(manager, *id);
    CBlackboard* blackboard = &manager->field_0x968;
    const UnkQualifiedId& max_name = Qid_dfbba8(g_unk_7172759138);
    const UnkQualifiedId& picked_name = Qid_dfbba8(g_unk_71727590f8);
    s32 max = 0;
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        fn_7100395a30(section, &max_name, &max);
    s32 picked = 0;
    s32 percent = 0;
    if (max >= 1) {
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
            fn_7100395a30(section, &picked_name, &picked);
        if (max >= 1)
            percent = static_cast<s32>(static_cast<f32>(picked) / static_cast<f32>(max) * 100.0f);
    }
    return percent;
}

// ---- fn_7100e04b00 ----
// CGameManager::CTunableGameManager's unit: applies the saved gamma: reads "GammaValue" from the user data's
// "USER_DATA" section when it is there (2.2 otherwise) and gives it to the render manager and to the
// settings object. Names and layouts are placeholders.
using base::global::CStrId;

// The user data's owner, created on first use (as src/game/CBossRushManager.cpp has it).
struct UnkUserDataOwner;
bool fn_71010474f4(CBlackboard* self, const CStrId& section, const CStrId& name);
bool fn_7100395b34(CBlackboard::CSection* section, const void* name, f32* out);
extern u8 g_unk_71727be8a8[];

static inline UnkUserDataOwner* GetOwner_e04b00() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

void fn_7100e04b00(UnkGameManager* self) {
    f32 gamma = 2.2f;
    CBlackboard* data = fn_710120384c(GetOwner_e04b00());
    bool known;
    {
        CStrId section("USER_DATA", true);
        CStrId name("GammaValue", true);
        known = fn_71010474f4(data, section, name);
    }
    if (known) {
        CStrId section("USER_DATA", true);
        CStrId name("GammaValue", true);
        if (CBlackboard::CSection* s = fn_7101047270(data, section))
            fn_7100395b34(s, &name, &gamma);
    }
    fn_7100219294(g_unk_7101d47300, 0xffa4d6b88c29ee76, &gamma, 0);
    *reinterpret_cast<f32*>(g_unk_71727be8a8 + 0x24) = gamma;
}

// ---- fn_7100e02498 ----
// CGameManager::CTunableGameManager's unit: puts an object on the list of objects the lookup treats as
// taken (+0x2778) when it is not on it yet, and takes it off the other two lists (+0x2698, +0x27e8). Does
// nothing for no object or one with its flag at +0x1b5 set. Names and layouts are placeholders.
struct UnkObjectListTag_e02498;
using UnkObjectList_e02498 = base::UnkSList<UnkObjectListTag_e02498>;

// Removes the first node holding `value` and frees it (as src/game/CScenario/CScenario_Actors_ovb0032.cpp).
static inline __attribute__((always_inline)) void Remove_e02498(UnkObjectList_e02498& list, u64 value) {
    UnkObjectList_e02498::Node* node = list.head;
    for (; node; node = node->next) {
        if (node->value == value)
            break;
    }
    if (!node)
        return;
    UnkObjectList_e02498::Node* prev = nullptr;
    UnkObjectList_e02498::Node* cur = list.head;
    UnkObjectList_e02498::Node* next;
    UnkObjectList_e02498::Node* last = nullptr;
    for (;;) {
        if (!cur)
            return;
        prev = last;
        next = cur->next;
        if (cur == node)
            break;
        last = cur;
        cur = next;
    }
    if (prev) {
        prev->next = next;
        if (list.tail == node)
            list.tail = prev;
    } else {
        list.head = next;
    }
    if (--list.count == 0) {
        list.head = nullptr;
        list.tail = nullptr;
    }
    list.allocator->vf_0x48(node);
}

static inline __attribute__((always_inline)) s32 IndexOf_e02498(const UnkObjectList_e02498& list, u64 value) {
    u32 i = 0;
    for (UnkObjectList_e02498::Node* n = list.head; n; n = n->next, ++i)
        if (n->value == value)
            return (s32)i;
    return -1;
}

struct UnkListsView_e02498 {
    u8 pad_0x0[0x2688];
    UnkObjectList_e02498 field_0x2688;
    u8 pad_0x26b8[0xb0];
    UnkObjectList_e02498 field_0x2768;
    u8 pad_0x2798[0x40];
    UnkObjectList_e02498 field_0x27d8;
};
static_assert(__builtin_offsetof(UnkListsView_e02498, field_0x27d8) == 0x27d8, "layout");

struct UnkObject_e02498 {
    u8 pad_0x0[0x1b5];
    u8 field_0x1b5;
};

void fn_7100e02498(UnkGameManager* self, UnkObject_e02498* object) {
    if (!object || object->field_0x1b5)
        return;
    UnkListsView_e02498* view = reinterpret_cast<UnkListsView_e02498*>(self);
    u64 value = reinterpret_cast<u64>(object);
    if (IndexOf_e02498(view->field_0x2768, value) < 0)
        view->field_0x2768.PushBack(value);
    Remove_e02498(view->field_0x2688, value);
    Remove_e02498(view->field_0x27d8, value);
}

// ---- fn_7100dea304 ----
// CGameManager::CTunableGameManager's unit: takes an entity out of the manager's registries: erases its key
// (+0x30) from the two hash maps (+0x25f8, +0x2640), takes it off the lists at +0x2698 and +0x27e8 and,
// when it belongs to a character class whose kind at +0x130 is not 1, hands it to the per-kind
// object at +0x2848 + (kind << 8). Names and layouts are placeholders.
using base::reflection::CClass;

struct UnkKeyMap_b60;
struct UnkKeyNode_b60;
void fn_7100d8ba60(void* holder, void* entity);

struct UnkObjectListTag_dea304;
using UnkObjectList_dea304 = base::UnkSList<UnkObjectListTag_dea304>;

// Removes the first node holding `value` and frees it (as src/game/CScenario/CScenario_Actors_ovb0032.cpp).
static inline __attribute__((always_inline)) void Remove_dea304(UnkObjectList_dea304& list, u64 value) {
    UnkObjectList_dea304::Node* node = list.head;
    for (; node; node = node->next) {
        if (node->value == value)
            break;
    }
    if (!node)
        return;
    UnkObjectList_dea304::Node* prev = nullptr;
    UnkObjectList_dea304::Node* cur = list.head;
    UnkObjectList_dea304::Node* next;
    UnkObjectList_dea304::Node* last = nullptr;
    for (;;) {
        if (!cur)
            return;
        prev = last;
        next = cur->next;
        if (cur == node)
            break;
        last = cur;
        cur = next;
    }
    if (prev) {
        prev->next = next;
        if (list.tail == node)
            list.tail = prev;
    } else {
        list.head = next;
    }
    if (--list.count == 0) {
        list.head = nullptr;
        list.tail = nullptr;
    }
    list.allocator->vf_0x48(node);
}

struct UnkListsView_dea304 {
    u8 pad_0x0[0x25f8];
    u8 field_0x25f8[0x48];
    u8 field_0x2640[0x48];
    UnkObjectList_dea304 field_0x2688;
    u8 pad_0x26b8[0x120];
    UnkObjectList_dea304 field_0x27d8;
};
static_assert(__builtin_offsetof(UnkListsView_dea304, field_0x27d8) == 0x27d8, "layout");

struct UnkClassed_dea304 {
    virtual const CClass* vf_0x0();
};

struct UnkCharClass_dea304 {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x128];
    u32 kind;  // 0x130
};

struct UnkEntity_dea304 {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x20];
    UnkCharClass_dea304* field_0x28;
    u8 pad_0x30[0x4];
};

void fn_7100dea304(UnkGameManager* self, void* entity) {
    UnkListsView_dea304* view = reinterpret_cast<UnkListsView_dea304*>(self);
    UnkEntity_dea304* object = static_cast<UnkEntity_dea304*>(entity);
    fn_71002430e8(reinterpret_cast<UnkKeyMap_b60*>(view->field_0x25f8), reinterpret_cast<UnkKeyNode_b60*>(reinterpret_cast<char*>(entity) + 0x30));
    fn_71002430e8(reinterpret_cast<UnkKeyMap_b60*>(view->field_0x2640), reinterpret_cast<UnkKeyNode_b60*>(reinterpret_cast<char*>(entity) + 0x30));
    Remove_dea304(view->field_0x2688, reinterpret_cast<u64>(entity));
    Remove_dea304(view->field_0x27d8, reinterpret_cast<u64>(entity));
    if (object && fn_710009d50c(object->vf_0x0(), fn_7100d70c20())) {
        UnkCharClass_dea304* char_class = object->field_0x28;
        if (char_class && fn_710009d50c(char_class->vf_0x0(), fn_71011abbd4()) && char_class->kind != 1)
            fn_7100d8ba60(reinterpret_cast<char*>(self) + 0x2848 + ((u64)char_class->kind << 8), entity);
    }
}

// ---- fn_7100de6ee8 ----
// CGameManager::CTunableGameManager's unit: unregisters every entity on the list at +0x2708 (their keys leave the
// two hash maps, they go to the per-kind objects and the GUI, are told to stop, and are released) and
// empties the list. Names and layouts are placeholders.
using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkKeyMap_b60;
struct UnkKeyNode_b60;
struct UnkActorTable_b43a3;
struct UnkActorWithDef_b43a3;
struct UnkActor_71002329e8;
struct UnkBoundObj;
void fn_7100d8ba60(void* holder, void* entity);
void fn_7101198450(void* gui, UnkReflObject* entity);
bool fn_710119dd94(UnkActorTable_b43a3* self, UnkActorWithDef_b43a3* actor);
extern void* g_unk_7101d47330;

struct UnkEntityNode_de6ee8 {
    void* object;
    UnkEntityNode_de6ee8* next;
};

struct UnkEntityList_de6ee8 {
    u32 count;
    UnkEntityNode_de6ee8* head;
    UnkEntityNode_de6ee8* tail;
    base::UnkAllocator* allocator;
};

struct UnkCharClass_de6ee8 {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x128];
    u32 kind;  // 0x130
};

struct UnkEntity_de6ee8 {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x20];
    UnkCharClass_de6ee8* field_0x28;
    u8 pad_0x30[0x182];
    u8 field_0x1b2;
};

struct UnkManagerView_de6ee8 {
    u8 pad_0x0[0x25f8];
    u8 field_0x25f8[0x48];
    u8 field_0x2640[0xc0];
    UnkEntityList_de6ee8 field_0x2700;
};
static_assert(__builtin_offsetof(UnkManagerView_de6ee8, field_0x2700) == 0x2700, "layout");

void fn_7100de6ee8(UnkGameManager* self) {
    UnkManagerView_de6ee8* view = reinterpret_cast<UnkManagerView_de6ee8*>(self);
    for (UnkEntityNode_de6ee8* node = view->field_0x2700.head; node; node = node->next) {
        UnkEntity_de6ee8* object = static_cast<UnkEntity_de6ee8*>(node->object);
        UnkKeyNode_b60* key = reinterpret_cast<UnkKeyNode_b60*>(reinterpret_cast<char*>(object) + 0x30);
        fn_71002430e8(reinterpret_cast<UnkKeyMap_b60*>(view->field_0x2640), key);
        fn_71002430e8(reinterpret_cast<UnkKeyMap_b60*>(view->field_0x25f8), key);
        if (object && fn_710009d50c(object->vf_0x0(), fn_7100d70c20())) {
            UnkCharClass_de6ee8* char_class = object->field_0x28;
            if (char_class && fn_710009d50c(char_class->vf_0x0(), fn_71011abbd4()) && char_class->kind != 1)
                fn_7100d8ba60(reinterpret_cast<char*>(self) + ((u64)char_class->kind << 8) + 0x2848, object);
            fn_7101198450(g_unk_7101d47330, reinterpret_cast<UnkReflObject*>(object));
        }
        if (object->field_0x1b2)
            fn_71002329e8(reinterpret_cast<UnkActor_71002329e8*>(object));
        UnkSubAreaGuiCache* cache = g_unk_71727254f8;
        if (!cache) {
            cache = static_cast<UnkSubAreaGuiCache*>(fn_71000839a0(0x48, 0x3f));
            fn_710119d484(cache);
            g_unk_71727254f8 = cache;
        }
        if (!fn_710119dd94(reinterpret_cast<UnkActorTable_b43a3*>(cache), reinterpret_cast<UnkActorWithDef_b43a3*>(object)))
            fn_7100232124(reinterpret_cast<UnkBoundObj*>(object));
    }
    UnkEntityList_de6ee8& list = view->field_0x2700;
    for (UnkEntityNode_de6ee8* node = list.head; node;) {
        UnkEntityNode_de6ee8* next = node->next;
        list.allocator->vf_0x48(node);
        node = next;
    }
    list.count = 0;
    list.head = nullptr;
    list.tail = nullptr;
}

// ---- fn_7100dfe36c ----
// CGameManager::CTunableGameManager's unit: puts the game manager into its in-game state: the mode
// holder at +0x498 is replaced by "INGAME" (kept as the mode name at +0x5d0) unless it already is, then the
// profile, the GUI and the sound are brought up to date. Names and layouts are placeholders.
using base::global::CRntString;

struct UnkUserDataOwner;
struct UnkLoadCheckPointSingleton_ovb0118;
struct UnkMgr_0xf18;
extern void* g_unk_7101d47330;
extern UnkMgr_0xf18* g_unk_71726bc560;
void fn_7101198978(void* gui);
void fn_7101198c0c(void* gui);
void fn_7101047068(CBlackboard* a, CBlackboard* b);
void fn_7100dfe560(UnkGameManager* self, bool flag);
void fn_7100ec7364(u8* self);
void fn_7100fdd250(UnkMgr_0xf18* manager, u32 kind, f32 time);

// The members of the game manager this step touches that the shared layout leaves as padding.
struct UnkGameManagerIngame_dfe36c {
    u8 pad_0x0[0x438];
    CRntString field_0x438;
    u8 pad_0x468[0x160];
    u8 field_0x5c8;
    u8 pad_0x5c9[7];
    CRntString field_0x5d0;
    u64 field_0x600;
    u8 pad_0x608[0x360];
    CBlackboard field_0x968;
    u8 pad_0xa58[0x58];
    CBlackboard field_0xab0;
    u8 pad_0xba0[0x1e18];
    u8* field_0x29b8;
};
static_assert(__builtin_offsetof(UnkGameManagerIngame_dfe36c, field_0x600) == 0x600, "layout");
static_assert(__builtin_offsetof(UnkGameManagerIngame_dfe36c, field_0x968) == 0x968, "layout");
static_assert(__builtin_offsetof(UnkGameManagerIngame_dfe36c, field_0xab0) == 0xab0, "layout");
static_assert(__builtin_offsetof(UnkGameManagerIngame_dfe36c, field_0x29b8) == 0x29b8, "layout");

template <class T, u64 Size, void (*Ctor)(T*)>
static inline T* Singleton_dfe36c(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

void fn_7100dfe36c(UnkGameManager* manager) {
    UnkGameManagerIngame_dfe36c* self = reinterpret_cast<UnkGameManagerIngame_dfe36c*>(manager);
    char* mode = static_cast<char*>(manager->field_0x498);
    if (!mode || fn_710007f240(mode + 8, "INGAME")) {
        CRntString ingame("INGAME");
        fn_710007eaa0(&self->field_0x5d0, &ingame);
        self->field_0x5c8 = 1;
        self->field_0x600 = 0x200000000;
    } else {
        CRntString* current = reinterpret_cast<CRntString*>(static_cast<char*>(manager->field_0x498) + 8);
        fn_710007eaa0(&self->field_0x438, current);
    }
    fn_7100def888(reinterpret_cast<char*>(manager));
    fn_7101203768(Singleton_dfe36c<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308));
    fn_7100dee46c(manager);
    fn_7101198978(g_unk_7101d47330);
    fn_7101047068(&self->field_0x968, &self->field_0xab0);
    {
        if (Singleton_dfe36c<u8, 0x10, fn_7100ec65fc>(g_unk_71726bc558)[8])
            fn_7100ec7364(Singleton_dfe36c<u8, 0x10, fn_7100ec65fc>(g_unk_71726bc558));
    }
    fn_7100dfe560(manager, false);
    fn_7100e19954(Singleton_dfe36c<UnkLoadCheckPointSingleton_ovb0118, 0x28, fn_7100e17cc8>(g_unk_7172763578), false);
    if (u8* a = self->field_0x29b8) {
        if (u8* b = *reinterpret_cast<u8**>(a + 0x10)) {
            if (u8* c = *reinterpret_cast<u8**>(b + 0x1c0))
                c[0x324] = 1;
        }
    }
    fn_7101198c0c(g_unk_7101d47330);
    char* music = reinterpret_cast<char*>(g_unk_71726bc560);
    if (!music) {
        music = static_cast<char*>(fn_71000839a0(0xf18, 0x3f));
        fn_7100fd3804(music);
        reinterpret_cast<void*&>(g_unk_71726bc560) = music;
    }
    fn_7100fdd250(reinterpret_cast<UnkMgr_0xf18*>(music), 0, 0.2f);
}

// ---- fn_7100dfa010 ----
// CGameManager::CTunableGameManager's unit: handles a load request: one that carries a name (+0x40) empties the
// scenario name and the blackboards; otherwise, once the request's source is far enough along
// (+0xf8 >= 2) with no text at +0x120, it closes the two GUI flag objects (+0x2258, +0x2270) and, when
// the named object the lookup finds for the scenario reports it (+0x88), goes on (fn_7100dfa1e4).
// Names and layouts are placeholders.
using base::global::CRntString;
using base::global::CStrId;

struct UnkPopUpComposition_ovb0085;
struct UnkPopUpEvent_ovb0085;
struct UnkTutorialComposition;
struct UnkTutorialRequest;
void fn_7100b5ed84(UnkPopUpComposition_ovb0085* self, UnkPopUpEvent_ovb0085* event);
void fn_7101047ea8(void* self);

// A blackboard seen through its vtable (called through a pointer, so the call stays virtual).
struct UnkBoardCall_dfa010 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
};
static inline void Reset_dfa010(CBlackboard& board) { reinterpret_cast<UnkBoardCall_dfa010*>(&board)->vf_0x18(); }

struct UnkLoadRequest_dfa010;
void fn_7100dfa1e4(UnkGameManager* self, UnkLoadRequest_dfa010* request);

// Named objects (a CRntString at +8), in a list at +0x478.
struct UnkNamedObject {
    u8 pad_0x0[0x8];
    CRntString name;
};
struct UnkNamedNode {
    UnkNamedObject* value;
    UnkNamedNode* next;
};
struct UnkNamedUser {
    u8 pad_0x0[0x88];
    bool field_0x88;
};
// String equality (the declaration is src/game_s1/fn_71000d4d50.cpp's, over a record of CRntString's size).
struct UnkCompared_0x30;
static inline bool Equal(const CRntString* a, const CRntString* b) {
    return fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(a), reinterpret_cast<const UnkCompared_0x30*>(b));
}
UnkNamedUser* fn_7101190890(UnkNamedObject* self, CRntString name);

// The pooled data behind a string id as its text accessor sees it.
struct UnkIdTextData {
    u8 pad_0x0[0x10];
    CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 reads;
    };
};
template <class Id>
static inline const CRntString& IdText(const Id& id) {
    ++reinterpret_cast<UnkIdTextData*>(id.data)->reads;
    return reinterpret_cast<UnkIdTextData*>(id.data)->str;
}

struct UnkRequestSource_dfa010 {
    u8 pad_0x0[0xf8];
    u32 field_0xf8;
    u8 pad_0xfc[0x24];
    const char* field_0x120;
};
struct UnkLoadRequest_dfa010 {
    u8 pad_0x0[0x40];
    u32 field_0x40;
    u8 pad_0x44[0x14];
    UnkRequestSource_dfa010* field_0x58;
};

struct UnkGameManagerRequest_dfa010 {
    u8 pad_0x0[0x478];
    UnkNamedNode* field_0x478;
    u8 pad_0x480[0x370];
    CRntString field_0x7f0;
    CBlackboard field_0x820;
    u8 pad_0x910[0x58];
    CBlackboard field_0x968;
    u8 pad_0xa58[0x58];
    CBlackboard field_0xab0;
    u8 pad_0xba0[0x58];
    CBlackboard field_0xbf8;
    u8 pad_0xce8[0x58];
    CBlackboard field_0xd40;
    u8 pad_0xe30[0x1428];
    u8* field_0x2258;
    u8 pad_0x2260[0x10];
    u8* field_0x2270;
};
static_assert(__builtin_offsetof(UnkGameManagerRequest_dfa010, field_0x2270) == 0x2270, "layout");

void fn_7100dfa010(UnkGameManager* manager, UnkLoadRequest_dfa010* request) {
    UnkGameManagerRequest_dfa010* self = reinterpret_cast<UnkGameManagerRequest_dfa010*>(manager);
    if (request->field_0x40) {
        CRntString empty("");
        fn_710007eaa0(&self->field_0x7f0, &empty);
        fn_7101047ea8(&self->field_0x820);
        Reset_dfa010(self->field_0x968);
        Reset_dfa010(self->field_0xab0);
        Reset_dfa010(self->field_0xbf8);
        Reset_dfa010(self->field_0xd40);
        return;
    }
    UnkRequestSource_dfa010* source = request->field_0x58;
    if (source->field_0xf8 < 2)
        return;
    if (source->field_0x120 && source->field_0x120[0])
        return;
    if (self->field_0x2258) {
        if (u8* inner = *reinterpret_cast<u8**>(self->field_0x2258 + 0x10))
            fn_7100b5ed84(reinterpret_cast<UnkPopUpComposition_ovb0085*>(inner), nullptr);
    }
    if (self->field_0x2270) {
        if (u8* inner = *reinterpret_cast<u8**>(self->field_0x2270 + 0x10))
            fn_7100b8903c(reinterpret_cast<UnkTutorialComposition*>(inner), nullptr);
    }
    const CRntString& text = IdText(*reinterpret_cast<CStrId*>(IdTable_df703c().pad_0x0 + 0x16a8));
    UnkNamedObject* found = nullptr;
    for (UnkNamedNode* n = self->field_0x478; n; n = n->next) {
        if (Equal(&n->value->name, &text)) {
            found = n->value;
            break;
        }
    }
    bool known = fn_7101190890(found, CRntString(self->field_0x7f0))->field_0x88;
    if (known)
        fn_7100dfa1e4(manager, request);
}

// ---- fn_7100df916c ----
// CGameManager::CTunableGameManager's unit: starts a load of the scenario named at +0x7f0 in mode 0, asking
// for `name` as the second string; when it is accepted merges the blackboards (+0xbf8 into +0xab0 into
// +0x968) and refreshes the GUI. 5 when no scenario is named. Names and layouts are placeholders.
using base::global::CRntString;

struct UnkUserDataOwner;

// A list of pointers (vtable g_unk_7101aa9ac0), its nodes from `allocator` with `flags`.
struct UnkPtrListNode {
    void* value;
    UnkPtrListNode* next;
};
struct UnkPtrList {
    void** vtable;
    // Union members: the original reloads the count, the tail and the allocator after storing a new
    // node's links.
    union {
        u32 count;
    };
    UnkPtrListNode* head;
    union {
        UnkPtrListNode* tail;
    };
    union {
        base::UnkAllocator* allocator;
    };
    union {
        u32 flags;
    };

    UnkPtrList() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkPtrList() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
        for (UnkPtrListNode* n = head; n;) {
            UnkPtrListNode* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
    }
    void PushBack(void* value) {
        auto* node = static_cast<UnkPtrListNode*>(allocator->vf_0x40(sizeof(UnkPtrListNode), 8, flags));
        if (!node)
            return;
        node->value = value;
        node->next = nullptr;
        if (count == 0) {
            head = node;
            tail = node;
            count = 1;
        } else {
            tail->next = node;
            node->next = nullptr;
            tail = node;
            ++count;
        }
    }
};

struct UnkStrVector;
s32 fn_71012034b0(UnkUserDataOwner* self, u32 kind, const CRntString* scenario, const CRntString* name,
                  UnkPtrList* blackboards, bool a, void (*fn)(), bool b, const CRntString* c);

template <class T, u64 Size, void (*Ctor)(T*)>
static inline T* Singleton_df916c(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

void fn_7101046e78(CBlackboard* self, CBlackboard* other, bool a, bool b);
void fn_7101198c0c(void* gui);
extern void* g_unk_7101d47330;

// The game manager's members this step uses (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerLoad_df916c {
    u8 pad_0x0[0x7f0];
    CRntString field_0x7f0;
    u8 pad_0x820[0x148];
    CBlackboard field_0x968;
    u8 pad_0xa58[0x58];
    CBlackboard field_0xab0;
    u8 pad_0xba0[0x58];
    CBlackboard field_0xbf8;
};
static_assert(__builtin_offsetof(UnkGameManagerLoad_df916c, field_0xbf8) == 0xbf8, "layout");

s32 fn_7100df916c(UnkGameManager* manager, const CRntString& name) {
    UnkGameManagerLoad_df916c* self = reinterpret_cast<UnkGameManagerLoad_df916c*>(manager);
    fn_7101203768(Singleton_df916c<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308));
    fn_7100dee46c(manager);
    s32 result = 5;
    if (self->field_0x7f0.data && self->field_0x7f0.data[0]) {
        UnkPtrList blackboards;
        blackboards.PushBack(&self->field_0xbf8);
        CRntString scenario(self->field_0x7f0);
        UnkUserDataOwner* owner = Singleton_df916c<UnkUserDataOwner, 0x2b8, fn_710120320c>(g_unk_7101d47308);
        result = fn_71012034b0(owner, 0, &self->field_0x7f0, &scenario, &blackboards, true,
                               reinterpret_cast<void (*)()>(&fn_7100df7468), true, &name);
        fn_7100dee46c(manager);
        if (result == 0) {
            CBlackboard* game = &self->field_0x968;
            CBlackboard* middle = &self->field_0xab0;
            fn_7101046e78(middle, &self->field_0xbf8, false, false);
            fn_7101046e78(game, middle, false, false);
            fn_7101198c0c(g_unk_7101d47330);
        }
    }
    return result;
}
