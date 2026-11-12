// Unit 241: finds the index of the first entry whose name equals the given name, compared
// case-insensitively (the name is lower-cased into a string with inline storage), bumping the
// entry's use count while it looks; -1 when none. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "game/UnkTU57.h"
#include "types.h"

#include <string.h>

using base::global::CRntString;

struct UnkInlineString_71001b47b4 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_71001b47b4() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        char* vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
        storage_vtable = vtable;
    }
    ~UnkInlineString_71001b47b4() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

struct UnkNamedData_71001b47b4 {
    u8 pad_0x0[0x10];
    UnkCompared_0x30* name;  // +0x10, inline
    u8 pad_0x18[0x2c];
    u32 use_count;  // +0x44
};

struct UnkNamedEntry_71001b47b4 {
    u8 pad_0x0[8];
    UnkNamedData_71001b47b4* data;
};

struct UnkNameList_71001b47b4 {
    u8 pad_0x0[0x80];
    u8 entries[0x38];
    u32 count;  // +0xb8
};

UnkNamedEntry_71001b47b4** fn_71001b459c(void* entries, u32 index);

u32 fn_71001b47b4(UnkNameList_71001b47b4* self, const char* name) {
    char lower[0x100];
    lower[0] = 0;
    u64 len = static_cast<u32>(strlen(name));
    if (len != 0) {
        for (u64 i = 0; i < len; i++) {
            char c = name[i];
            if (c >= 'A' && c <= 'Z')
                c += 0x20;
            lower[i] = c;
        }
        lower[len] = 0;
    }

    UnkInlineString_71001b47b4 key;
    fn_710007e77c(key.Str(), lower);

    u32 found = 0xffffffff;
    if (self->count != 0) {
        u32 i = 0;
        do {
            UnkNamedEntry_71001b47b4* entry = *fn_71001b459c(self->entries, i);
            u32 uses = entry->data->use_count + 1;
            __builtin_memcpy(&entry->data->use_count, &uses, sizeof(uses));
            bool same = fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(key.Str()),
                                      reinterpret_cast<const UnkCompared_0x30*>(
                                          reinterpret_cast<u8*>(entry->data) + 0x10));
            found = same ? i : 0xffffffff;
            i += !same;
        } while ((found == 0xffffffff) & (i < self->count));
    }
    return found;
}
