// Unit 2626: builds the lower-case file-path id of a sound stream: "streams/localization/", then the
// current language's code (language 7 when fn_710119c9fc says so) or "common", then "/", the stream's
// name, and ".wav" unless the name has it. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2733.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

const char* fn_710119bdac(s32 language);  // a language's code

static inline UnkLanguage* Language() {
    UnkLanguage* language = g_unk_7101d47358;
    if (!language) {
        language = static_cast<UnkLanguage*>(fn_71000839a0(0x2068, 0x3f));
        fn_710119c020(language);
        g_unk_7101d47358 = language;
    }
    return language;
}

// A string with 0x100 bytes of inline storage (0x148 bytes).
struct UnkInlineString_lfcd380 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_lfcd380() {
        CRntString_InitWithAllocator(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
    }
    explicit UnkInlineString_lfcd380(const char* text) : UnkInlineString_lfcd380() { CRntString_AssignCString(Str(), text); }
    ~UnkInlineString_lfcd380() {
        CRntString_Clear(Str());
        CRntString_Destroy(Str());
    }
};

// A file-path id built from a string; returned in memory.
struct UnkPathId_lfcd380 {
    base::global::UnkFilePathStrIdBase id;
    explicit UnkPathId_lfcd380(const CRntString& str) { fn_710001a270(&id, str, true); }
    UnkPathId_lfcd380(const UnkPathId_lfcd380& other);
};

// Copies `src` into `dst` with A-Z turned into a-z.
static inline void CopyLower(char* dst, const char* src) {
    dst[0] = 0;
    u32 length = static_cast<u32>(__builtin_strlen(src));
    if (length) {
        for (u32 i = 0; i < length; ++i) {
            char c = src[i];
            if (static_cast<u32>(c - 'A') < 26)
                c += 0x20;
            dst[i] = c;
        }
        dst[length] = 0;
    }
}

// The text of a string id, counting the read.
static inline const CRntString* Text(const CStrId& id) {
    ++id.data->reads;
    return &id.data->String();
}

struct UnkStream_lfcd380 {
    CStrId name;
};

UnkPathId_lfcd380 fn_7100fcd380(UnkStream_lfcd380* self, bool localized) {
    char lower[0x100];  // declared first: it sits above both strings in the frame
    UnkInlineString_lfcd380 path("streams/localization/");
    if (localized) {
        s32 language = fn_710119c9fc(Language()) ? 7 : fn_710119c9f4(Language());
        Language();
        CopyLower(lower, fn_710119bdac(language));
        UnkInlineString_lfcd380 code(lower);
        CRntString_Append(path.Str(), code.Str());
    } else {
        CRntString_AppendCString(path.Str(), "common");
    }
    CRntString_AppendCString(path.Str(), "/");
    CopyLower(lower, Text(self->name)->data);
    {
        UnkInlineString_lfcd380 name(lower);
        CRntString_Append(path.Str(), name.Str());
    }
    if (!CRntString_Contains(Text(self->name), ".wav"))
        CRntString_AppendCString(path.Str(), ".wav");
    return UnkPathId_lfcd380(*path.Str());
}
