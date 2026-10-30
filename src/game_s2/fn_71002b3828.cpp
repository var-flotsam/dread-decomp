// Refreshes the record from the manager: when the record names an entry (a u16 at +0x1ec and a u8 at
// +0x1ee), reports it through fn_71002b3aa0 and takes over the entry's list at +0x1f0 (registered
// through fn_71002b4538). Then looks every 24-byte entry at +0x128 up by its key, keeps a weak
// reference to what it finds in the entry, and sets one of the flags at +0x1e8..+0x1ea from the kind
// of the object's component.
#include "game/UnkGameManager.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkKeyed {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkEntry {
    u8 pad_0x0[0x128];
    UnkKeyed* field_0x128;
    UnkWeakHandle* field_0x130;
    u32 field_0x138;
};

struct UnkLinkList;

struct UnkRecord {
    u8 pad_0x0[0x124];
    u32 field_0x124;
};

struct UnkEntryInfo {
    u8 pad_0x0[0x38];
    s32 field_0x38;
};

struct UnkOwner {
    u8 pad_0x0[0x1e8];
    u8 field_0x1e8;
    u8 field_0x1e9;
    u8 field_0x1ea;
    u8 pad_0x1eb[0x1];
    u16 field_0x1ec;
    u8 field_0x1ee;
    u8 pad_0x1ef[0x1];
    UnkLinkList* field_0x1f0;
};

extern u32 g_unk_71727bebb8;
extern char g_unk_71726a4bc0[] __attribute__((visibility("hidden")));

void* fn_7100ca719c(void* manager, u32 id);
UnkEntryInfo* fn_7100ca1ea8(void* entry, u8 sub);
void fn_71002b3aa0(UnkOwner* self, bool flag, void* arg);
UnkLinkList* fn_7100ca7a64(void* manager, u16 id);
void fn_71002b46ec(UnkLinkList* list, UnkLinkList** slot);
void fn_71002b4538(UnkLinkList* list, UnkLinkList** slot);
UnkWeakTarget* fn_7100debbc8(UnkGameManager* manager, const u64* key, const u32* kind);
void* fn_71002b3b84(UnkWeakTarget* object);
void fn_71009c1130(void* component, u32 value, void* owner);
s32 fn_71009c0b8c(void* component);

// NON_MATCHING(minor): the entry loop is strength-reduced to a pointer induction here; the original keeps
// a byte offset (and spills the weak handle vtable address). Same behaviour.
void fn_71002b3828(UnkOwner* self) {
    if (self->field_0x1ec) {
        void* entries = g_unk_7101d47310->field_0x1af8;
        if (void* entry = fn_7100ca719c(entries, self->field_0x1ec)) {
            if (UnkEntryInfo* info = fn_7100ca1ea8(entry, self->field_0x1ee)) {
                fn_71002b3aa0(self, info->field_0x38 == 2, g_unk_71726a4bc0);
                UnkLinkList** slot = &self->field_0x1f0;
                UnkLinkList* list = fn_7100ca7a64(entries, self->field_0x1ec);
                if (*slot) {
                    fn_71002b46ec(*slot, slot);
                    *slot = nullptr;
                }
                *slot = list;
                fn_71002b4538(list, slot);
            }
        }
    }
    UnkRecord* record = reinterpret_cast<UnkRecord*>(self);
    u64 size = (u64)record->field_0x124 * 24;
    for (u64 offset = 0; offset != size; offset += 24) {
        UnkEntry* entry = reinterpret_cast<UnkEntry*>(reinterpret_cast<char*>(record) + offset);
        u64 key = entry->field_0x128->field_0x30;
        u32 kind = g_unk_71727bebb8;
        UnkWeakTarget* object = fn_7100debbc8(g_unk_7101d47310, &key, &kind);
        if (!object)
            continue;
        UnkWeakHandle* handle = GetWeakHandle(object);
        AssignRef(&entry->field_0x130, handle);
        void* component = fn_71002b3b84(object);
        if (!component)
            continue;
        fn_71009c1130(component, entry->field_0x138, self);
        if (fn_71009c0b8c(component) == 1)
            self->field_0x1e9 = 1;
        else if (fn_71009c0b8c(component) == 2)
            self->field_0x1ea = 1;
        else if (fn_71009c0b8c(component) == 0)
            self->field_0x1e8 = 1;
    }
}
