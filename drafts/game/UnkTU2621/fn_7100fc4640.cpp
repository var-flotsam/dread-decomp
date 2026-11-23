// Unit 2621 (the save system): loads a save slot. Records the slot's path (without its "savedata://"
// prefix) and the path itself, reads the slot's file and the "common" file of its directory into two
// game blackboards, and when both read, takes the profile name from the second and reads the slot's
// values from the two. Names and layouts are placeholders.
#include "../../../src/game_aca57c/UnkBufferString.h"
#include "base/UnkHashStream.h"
#include "base/file/nx/CDeviceNX.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU56.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

namespace base::reflection {
struct UnkReflObject;
}
struct UnkGameBlackboard_ovb0064;
struct UnkOwner_7100fc4b34;
struct UnkTuning_7100fc4b34;

void fn_7101046974(CBlackboard* blackboard);
void fn_7100aebde8(UnkGameBlackboard_ovb0064* self);
bool fn_71010477f0(base::reflection::UnkReflObject* self, const base::global::CRntFile* file);
bool fn_7100fc4b34(UnkOwner_7100fc4b34* self, CBlackboard* a, CBlackboard* b, UnkTuning_7100fc4b34* out);
extern void* g_unk_7101a978b8[];
extern const char* g_unk_7101aa99b0;
extern const char* g_unk_7101aa99b8;
extern const char g_unk_710164784b;  // the path separator
extern u8 g_unk_71729f0020[] __attribute__((visibility("hidden")));

// A string on 0x40 bytes of inline storage: the first `length` characters of `str`.
struct UnkString40_fc4640 : UnkBufferString<0x40, g_unk_7101a1b678> {
    UnkString40_fc4640(const char* str, u32 length) { CRntString_AssignPrefix(Str(), str, length); }
    CRntString&& Take() { return static_cast<CRntString&&>(*Str()); }
};

// Assigns another string's text.
static inline void Assign(CRntString* self, const CRntString& other) { CRntString_Assign(self, &other); }

// A string on 0x100 bytes of inline storage.
using UnkString100_fc4640 = UnkBufferString<0x100, g_unk_7101a194b8>;

// A game blackboard: a CBlackboard (fn_7101046974) with the game's vtable.
struct UnkGameBlackboardBase_fc4640 {
    void** vtable;
    u8 unk_0x8[0xe8];

    UnkGameBlackboardBase_fc4640() {
        fn_7101046974(Blackboard());
        vtable = &g_unk_7101a978b8[2];
    }
    CBlackboard* Blackboard() { return reinterpret_cast<CBlackboard*>(this); }
};

// One of its two dictionaries (base::global::CRntSmallDictionary's fields; the blackboard's
// destructor frees them).
struct UnkDictionary_fc4640 {
    void* values = nullptr;
    void* keys = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step = 10;
    u32 flags;
    base::UnkAllocator* allocator = fn_7100083990();

    explicit UnkDictionary_fc4640(u32 flags) : flags(flags) {}
};

struct UnkGameBlackboard_fc4640 : UnkGameBlackboardBase_fc4640 {
    UnkDictionary_fc4640 sets{2};     // 0xf0
    UnkDictionary_fc4640 tables{0xe};  // 0x118
    u64 field_0x140;

    ~UnkGameBlackboard_fc4640() { fn_7100aebde8(reinterpret_cast<UnkGameBlackboard_ovb0064*>(this)); }
};
static_assert(sizeof(UnkGameBlackboard_fc4640) == 0x148, "UnkGameBlackboard_fc4640 layout");

// A file read whole into a buffer, which its owner deletes through its vtable.
struct UnkSaveBuffer_fc4640 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkOwnedSaveBuffer_fc4640 {
    UnkSaveBuffer_fc4640* buffer = nullptr;

    ~UnkOwnedSaveBuffer_fc4640() {
        if (buffer) {
            buffer->vf_0x8();
            buffer = nullptr;
        }
    }
};

// Reads the file at `path` whole into `owner`; 0 when it could.
static inline u32 ReadFile(CRntString* path, UnkOwnedSaveBuffer_fc4640* owner) {
    return fn_710002ed98(g_unk_71729f0020, path, &owner->buffer, 1, nullptr, 8, 0x16);
}

// Parses a file's contents into the blackboard; whether it parsed.
static inline bool Parse(UnkGameBlackboard_fc4640* blackboard, UnkSaveBuffer_fc4640* buffer) {
    base::global::CRntFile file;
    fn_710007c080(&file, buffer, 0x16, 0);
    bool ok = fn_71010477f0(reinterpret_cast<base::reflection::UnkReflObject*>(blackboard), &file);
    fn_710007c144(&file);
    fn_710007c0e4(&file);
    return ok;
}

// An item's qualified id, built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

struct UnkSaveSystem_fc4640;

struct UnkSaveSlot_fc4640 {
    CRntString local_path;  // 0x0: the path without its "savedata://" prefix
    CRntString path;        // 0x30
    CRntString field_0x60;
    CRntString field_0x90;
    CRntString name;  // 0xc0
    u8 pad_0xf0[0x68];
    bool valid;  // 0x158: the first of the values fn_7100fc4b34 reads
};

bool fn_7100fc4640(UnkSaveSystem_fc4640* self, const CRntString* path, UnkSaveSlot_fc4640* slot) {
    static const CRntString s_prefix("savedata://");
    Assign(&slot->local_path,
           CRntString_StartsWith(path, &s_prefix)
               ? CRntString(UnkString40_fc4640(path->data + s_prefix.length, path->length - s_prefix.length).Take())
               : CRntString(*path));
    CRntString_Assign(&slot->path, path);

    CRntString file_name(g_unk_7101aa99b0);
    CRntString_AppendCString(&file_name, g_unk_7101aa99b8);
    CRntString directory(*path);
    CRntString_AppendChar(&directory, g_unk_710164784b);

    UnkGameBlackboard_fc4640 slot_data;
    UnkGameBlackboard_fc4640 common_data;
    bool ok = false;
    {
        CRntString file = CRntString_Concat(&directory, &file_name);
        UnkOwnedSaveBuffer_fc4640 buffer;
        if (ReadFile(&file, &buffer) == 0)
            ok = Parse(&slot_data, buffer.buffer);
    }
    if (ok) {
        ok = false;
        CRntString file = CRntString_ConcatCString(&directory, "common");
        CRntString_AppendCString(&file, g_unk_7101aa99b8);
        UnkOwnedSaveBuffer_fc4640 buffer;
        if (ReadFile(&file, &buffer) == 0)
            ok = Parse(&common_data, buffer.buffer);
    }
    slot->valid = ok;
    if (ok) {
        CRntString_Assign(&slot->field_0x60, &g_unk_7101d58d10);
        CRntString_Assign(&slot->field_0x90, &g_unk_7101d58d10);
        UnkString100_fc4640 name;
        const base::global::CStrId& section = GameManagerIdTable().field_0x1148;
        const UnkQualifiedId& id = Qid(g_unk_7172758cb0);
        if (CBlackboard::CSection* s = fn_7101047270(common_data.Blackboard(), section))
            fn_7100395c38(s, &id, name.Str());
        CRntString_Assign(&slot->name, name.Str());
        fn_7100fc4b34(nullptr, slot_data.Blackboard(), common_data.Blackboard(),
                      reinterpret_cast<UnkTuning_7100fc4b34*>(&slot->valid));
    }
    return ok;
}
