// Unit 2733 of tools/tu_map.py (the language singleton of src/game/UnkLanguage.cpp): loading the current language's
// texts once, under the unit's mutex (fn_710119ca8c), and adding a credits text (fn_710119ce68: a key ending in
// "_TITLE", "_SUBTITLE" or "_END" marks the entry as a title, a subtitle or the end, the entries after the end
// carrying a flag). The entry goes after the 0x400 cached texts' count. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringEntry;

#define HIDDEN __attribute__((visibility("hidden")))

struct UnkLanguageText {
    u8 unk_0x0[0x20];
    UnkLanguageText(const UnkLanguageText& other) { fn_7100066cac(this, &other); }
};

struct UnkLanguage {
    u8 unk_0x0[0x48];
    bool field_0x48;
    u8 unk_0x49[0xf];
    s32 field_0x58;
    bool field_0x5c;
    bool field_0x5d;
    UnkLanguageText* cache[1024];  // 0x60
    s32 language;                  // 0x2060
    s32 field_0x2064;
};
static_assert(__builtin_offsetof(UnkLanguage, field_0x2064) == 0x2064, "layout");

extern u8 g_unk_7172a0be50[] HIDDEN;  // the unit's mutex
extern const char* g_unk_7101cb5438[];  // the language files' paths
s32 fn_710119c41c();
void fn_710119cb44(UnkLanguage* self, const char* path, bool keep_texts);

void fn_710119ca8c(UnkLanguage* self) {
    nn::os::LockMutex(reinterpret_cast<nn::os::MutexType*>(g_unk_7172a0be50));
    if (!self->field_0x5d) {
        u32 language = self->field_0x2064;
        if (language == 0x7fffffff) {
            language = fn_710119c41c();
            self->field_0x2064 = language;
        }
        if (self->language != language) {
            self->language = language;
            fn_710119cb44(self, g_unk_7101cb5438[language < 15 ? language : 0], false);
        }
        self->field_0x5d = true;
    }
    fn_7100066920(reinterpret_cast<nn::os::MutexType*>(g_unk_7172a0be50));
}

// A credits entry: the text and three flags.
struct UnkCreditEntry_ovb0215 : UnkLanguageText {
    bool after_end;  // 0x20
    bool title;      // 0x21
    bool subtitle;   // 0x22

    UnkCreditEntry_ovb0215(const UnkLanguageText& text) : UnkLanguageText(text) {}
};

// What a string id points at: the string at +0x10 and a count of the reads at +0x44 (a union member: the original
// reloads the id after the increment).
struct UnkIdEntry_ovb0215 {
    u8 unk_0x0[0x10];
    CRntString buffer;
    u8 unk_0x40[4];
    union {
        u32 reads;
    };
};

// The id's string (counting the use), null for the empty id.
static inline const CRntString* IdString(const CStrId& id) {
    ++reinterpret_cast<UnkIdEntry_ovb0215*>(id.data)->reads;
    UnkIdEntry_ovb0215* e = reinterpret_cast<UnkIdEntry_ovb0215*>(id.data);
    return e == reinterpret_cast<UnkIdEntry_ovb0215*>(&g_unk_7101d47108->empty) ? nullptr : &e->buffer;
}

static inline UnkCreditEntry_ovb0215* NewEntry(UnkLanguage* self, const UnkLanguageText& text) {
    void* p = fn_71000839a0(0x28, 5);
    UnkCreditEntry_ovb0215* e = static_cast<UnkCreditEntry_ovb0215*>(p);
    fn_7100066cac(e, &text);
    e->after_end = false;
    e->title = false;
    e->subtitle = false;
    if (self->field_0x5c)
        e->after_end = true;
    return e;
}

void fn_710119ce68(UnkLanguage* self, const CStrId& key, const UnkLanguageText& text) {
    UnkCreditEntry_ovb0215* e;
    const CRntString* s = IdString(key);
    if (s && fn_710007fb9c(s, "_TITLE")) {
        e = NewEntry(self, text);
        e->title = true;
        e->subtitle = false;
    } else {
        s = IdString(key);
        if (s && fn_710007fb9c(s, "_SUBTITLE")) {
            e = NewEntry(self, text);
            e->title = false;
            e->subtitle = true;
        } else {
            s = IdString(key);
            if (s && fn_710007fb9c(s, "_END")) {
                self->field_0x5c = true;
                e = NewEntry(self, text);
                e->title = false;
                e->subtitle = false;
            } else {
                e = NewEntry(self, text);
                e->title = false;
                e->subtitle = false;
            }
        }
    }
    self->cache[self->field_0x58++] = e;
    self->field_0x48 = false;
}
