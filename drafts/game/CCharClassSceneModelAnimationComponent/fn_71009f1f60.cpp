// CCharClassSceneModelAnimationComponent unit (tools/tu_map.py unit 1503): the definition's read. After
// the base read it replaces the 0x50-byte entries at +0x60 with groups read from the file: a group name
// and a count, then per entry a second id and a file path whose resource is loaded. Names and layouts
// are placeholders.
#include <functional>
#include <memory>
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassSceneModelAnimationComponent.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntFile;
using base::global::CStrId;

struct UnkCollisionClassDef;
struct UnkEntry_b0129;

bool fn_71011aced4(UnkCollisionClassDef* self, CRntFile* file);  // the base read
void CCharClassSceneModelAnimationComponent_ReserveEntryArray(base::global::CRntVector<UnkEntry_b0129>* self, u32 n);
void CCharClassSceneModelAnimationComponent_ConstructEntry(UnkEntry_b0129* self);
std::unique_ptr<void, std::function<void(void*)>> fn_7100013484(const CFilePathStrId& path);

// An entry's members: the group name, a second id and the loaded resource with its deleter.
struct UnkAnimEntryBody_l9f1f60 {
    CStrId name;
    base::global::UnkStrId id;
    std::unique_ptr<void, std::function<void(void*)>> resource;
};
static_assert(sizeof(UnkAnimEntryBody_l9f1f60) == 0x50, "entry size");

// Entries are built and destroyed out of line (fn_71009f2814, fn_71009f26a0).
struct UnkAnimEntryLifetime_l9f1f60 {
    UnkAnimEntryLifetime_l9f1f60() { CCharClassSceneModelAnimationComponent_ConstructEntry(reinterpret_cast<UnkEntry_b0129*>(this)); }
    ~UnkAnimEntryLifetime_l9f1f60() { CCharClassSceneModelAnimationComponent_DestroyEntry(reinterpret_cast<UnkEntry_b50a5*>(this)); }
};

struct UnkAnimEntry_l9f1f60 : UnkAnimEntryLifetime_l9f1f60 {
    u8 raw[0x50];

    UnkAnimEntryBody_l9f1f60& Body() { return *reinterpret_cast<UnkAnimEntryBody_l9f1f60*>(this); }
};
static_assert(sizeof(UnkAnimEntry_l9f1f60) == 0x50, "entry size");

struct UnkAnimEntries_l9f1f60 : base::global::CRntVector<UnkAnimEntry_l9f1f60> {
    void Reserve(u32 n) { CCharClassSceneModelAnimationComponent_ReserveEntryArray(reinterpret_cast<base::global::CRntVector<UnkEntry_b0129>*>(this), n); }

    // Appends a value-initialised entry and returns it.
    UnkAnimEntry_l9f1f60& Add() {
        u32 n = count + 1;
        Reserve(n);
        for (u32 i = count; i < n; ++i)
            new (&data[i]) UnkAnimEntry_l9f1f60();
        count = n;
        UnkAnimEntry_l9f1f60* end = &data[n];
        return n ? end[-1] : *end;
    }
};

struct UnkSceneModelAnimDef_l9f1f60 {
    u8 pad_0x0[0x60];
    UnkAnimEntries_l9f1f60 entries;  // 0x60
};
static_assert(__builtin_offsetof(UnkSceneModelAnimDef_l9f1f60, entries) == 0x60, "layout");

bool fn_71009f1f60(UnkSceneModelAnimDef_l9f1f60* self, CRntFile* file) {
    bool result = fn_71011aced4(reinterpret_cast<UnkCollisionClassDef*>(self), file);
    if (!result)
        return result;

    const char* str;
    s32 group_count = 0;
    CRntFile_Read(file, &group_count, 4, 1);
    self->entries.Clear();
    for (s32 group = 0; group < group_count; ++group) {
        CRntFile_ReadCString(file, &str);
        s32 count;
        CRntFile_Read(file, &count, 4, 1);
        CStrId name(str, true);
        self->entries.Reserve(count);
        for (s32 i = 0; i < count; ++i) {
            UnkAnimEntryBody_l9f1f60& entry = self->entries.Add().Body();
            entry.name = name;
            CRntFile_ReadCString(file, &str);
            entry.id = base::global::UnkStrId(str, true);
            CRntFile_ReadCString(file, &str);
            CFilePathStrId path(base::global::UnkStrIdInline(), str, true);
            entry.resource = fn_7100013484(path);
        }
    }
    return result;
}
