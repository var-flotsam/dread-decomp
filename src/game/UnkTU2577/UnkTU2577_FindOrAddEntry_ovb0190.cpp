// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eef488: finds the entry of the array at +0x888 (owned entries of 0x30 bytes) whose key is the argument's, else builds one (fn_7100f9e7fc) and inserts it at the given index (fn_7100ef1f24), or appends it when the key's count at +0x30 equals the array's. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkNamedIds;
void fn_7100ef1e1c(UnkNamedIds* self);

struct UnkOwnedEntry_b190 {
    union {
        UnkNamedIds* ptr;
    };
    explicit UnkOwnedEntry_b190(UnkNamedIds* p) : ptr(p) {}
    UnkOwnedEntry_b190(UnkOwnedEntry_b190&& o) {
        UnkNamedIds* moved = o.ptr;
        o.ptr = nullptr;
        ptr = moved;
    }
    ~UnkOwnedEntry_b190() {
        UnkNamedIds* old = ptr;
        ptr = nullptr;
        if (old) {
            fn_7100ef1e1c(old);
            operator delete(old);
        }
    }
};

struct UnkKeyData_b190 {
    u8 pad_0x0[0x30];
    u64 index;  // 0x30
};
struct UnkKeyRef_b190 {
    UnkKeyData_b190* data;
};

struct UnkEntry_b190 {
    UnkKeyData_b190* key;
    u8 pad_0x8[0x28];
};

struct UnkSubAreaMgr_b190 {
    u8 pad_0x0[0x888];
    base::UnkVector<UnkOwnedEntry_b190> entries;  // 0x888
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, entries) == 0x888, "layout");

UnkOwnedEntry_b190& fn_7100eef244(base::UnkVector<UnkOwnedEntry_b190>* v, u32 index);
void fn_7100f9e7fc(UnkEntry_b190* self, const UnkKeyRef_b190* key);
void fn_7100ef1f24(base::UnkVector<UnkOwnedEntry_b190>* v, UnkOwnedEntry_b190&& value, u32 index);

void* fn_7100eef488(char* self, u32 index, void* arg) {
    UnkSubAreaMgr_b190* mgr = reinterpret_cast<UnkSubAreaMgr_b190*>(self);
    const UnkKeyRef_b190* key = static_cast<const UnkKeyRef_b190*>(arg);
    for (u32 i = 0; i < mgr->entries.count; ++i) {
        UnkEntry_b190* e = reinterpret_cast<UnkEntry_b190*>(fn_7100eef244(&mgr->entries, i).ptr);
        if (e->key == key->data)
            return e;
    }
    UnkEntry_b190* e = static_cast<UnkEntry_b190*>(fn_71000839a0(0x30, 0x53));
    fn_7100f9e7fc(e, key);
    if (!e)
        return e;
    UnkKeyData_b190* data = key->data;
    u32 n = mgr->entries.count;
    if (data->index != n) {
        UnkOwnedEntry_b190 owned(reinterpret_cast<UnkNamedIds*>(e));
        fn_7100ef1f24(&mgr->entries, static_cast<UnkOwnedEntry_b190&&>(owned), index);
    } else {
        mgr->entries.EmplaceBack(UnkOwnedEntry_b190(reinterpret_cast<UnkNamedIds*>(e)));
    }
    return e;
}
