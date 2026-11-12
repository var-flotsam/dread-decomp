// Reflected functions of the same shape (fn_7100f8a4c0 and fn_7100f8a918 in unit 2602, no name hint;
// fn_7100fdd74c and fn_7100fdda0c in the sound::CMusicManager unit, 2637): look the string-id
// argument up in a map of the object at 8 (buckets at +0x10, count at +0x18, modulus at +0x1c);
// when found, erase it (fn_7100f87480 / fn_7100f89fd8) and tell the music manager (created on
// first use, game/sound/CMusicManager.h) through fn_7100fd9bf8 / fn_7100fd9c00 /
// fn_7100fd9e94 / fn_7100fd9e9c. Return whether it was found. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkMgr_0xf18;
extern UnkMgr_0xf18* g_unk_71726bc560;

struct UnkKeyData_b41a3m {
    u8 unk_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

// A map node: the key, the value, then the next node at NextOffset.
template <u32 NextOffset>
struct UnkMusicNode_b41a3 {
    UnkKeyData_b41a3m* key;
    u8 value[NextOffset - 8];
    UnkMusicNode_b41a3* next;
};

template <u32 NextOffset>
struct UnkMusicMap_b41a3 {
    u8 unk_0x0[0x10];
    UnkMusicNode_b41a3<NextOffset>** buckets;
    u32 count;
    u32 modulus;
};

void* fn_7100f87480(UnkMusicMap_b41a3<0x80>* map, const CStrId* key);
void* fn_7100f89fd8(UnkMusicMap_b41a3<0x68>* map, const CStrId* key);
void fn_7100fd9bf8(char* manager, const CStrId* key);
void fn_7100fd9c00(char* manager, const CStrId* key);
void fn_7100fd9e94(char* manager, const CStrId* key);
void fn_7100fd9e9c(char* manager, const CStrId* key);

static inline __attribute__((always_inline)) char* MusicManager_b41a3() {
    if (!reinterpret_cast<void*&>(g_unk_71726bc560)) {
        void* m = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(m);
        reinterpret_cast<void*&>(g_unk_71726bc560) = m;
    }
    return reinterpret_cast<char*>(g_unk_71726bc560);
}

template <u32 MapOffset, u32 NextOffset, void* (*Erase)(UnkMusicMap_b41a3<NextOffset>*, const CStrId*),
          void (*Notify)(char*, const CStrId*), u32 NotifyOffset>
static inline __attribute__((always_inline)) UnkReflValue RemoveMusicEntry_b41a3(char* obj,
                                                                                  UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    const CStrId* id;
    if (!arg->type)
        id = nullptr;
    else if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        id = static_cast<const CStrId*>(arg->field_0x0);
    else
        id = reinterpret_cast<const CStrId*>(arg);
    UnkMusicMap_b41a3<NextOffset>* map = reinterpret_cast<UnkMusicMap_b41a3<NextOffset>*>(target + MapOffset);
    bool removed = false;
    if (map->count != 0) {
        UnkKeyData_b41a3m* key = reinterpret_cast<UnkKeyData_b41a3m*>(id->data);
        u32 index = (key->field_0x30 ^ key->field_0x34) % map->modulus;
        UnkMusicNode_b41a3<NextOffset>** bucket = map->buckets + index;
        if (bucket) {
            for (UnkMusicNode_b41a3<NextOffset>* node = *bucket; node; node = node->next) {
                if (node->key == key) {
                    Erase(map, id);
                    Notify(MusicManager_b41a3() + NotifyOffset, id);
                    removed = true;
                    break;
                }
            }
        }
    }
    bool found = removed;
    const base::reflection::CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &found, false);
    return r;
}

UnkReflValue fn_7100f8a4c0(void*, char* obj, UnkReflValue* const* args) {
    return RemoveMusicEntry_b41a3<0, 0x80, fn_7100f87480, fn_7100fd9bf8, 0>(obj, args);
}

UnkReflValue fn_7100fdd74c(void*, char* obj, UnkReflValue* const* args) {
    return RemoveMusicEntry_b41a3<0x1c0, 0x80, fn_7100f87480, fn_7100fd9c00, 0x130>(obj, args);
}

UnkReflValue fn_7100f8a918(void*, char* obj, UnkReflValue* const* args) {
    return RemoveMusicEntry_b41a3<0x48, 0x68, fn_7100f89fd8, fn_7100fd9e94, 0>(obj, args);
}

UnkReflValue fn_7100fdda0c(void*, char* obj, UnkReflValue* const* args) {
    return RemoveMusicEntry_b41a3<0x208, 0x68, fn_7100f89fd8, fn_7100fd9e9c, 0x178>(obj, args);
}

// Two identical reflected functions (fn_7100f8a77c in unit 2602, fn_7100fdd870 in unit 2637): take an
// object and a bool; when the bool is set, pass the object's part at +8 to fn_7100fed650 and to the
// music manager's fn_7100fd9aa0. Return an empty value.
void fn_7100fed650(char* part, char* object);
void fn_7100fd9aa0(char* manager, char* part);

static inline __attribute__((always_inline)) UnkReflValue AddMusicPart_b41a3(UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* object;
    if (!a[0].type)
        object = nullptr;
    else if (a[0].field_0x14 == 1 || *reinterpret_cast<const u64*>(a[0].type + 0x18) > 8)
        object = static_cast<char*>(a[0].field_0x0);
    else
        object = reinterpret_cast<char*>(&a[0]);
    if (*base::reflection::refl_ArgValue<bool>(&a[1])) {
        char* part = object + 8;
        fn_7100fed650(part, object);
        fn_7100fd9aa0(MusicManager_b41a3(), part);
    }
    return base::reflection::refl_EmptyValue();
}

UnkReflValue fn_7100f8a77c(void*, char*, UnkReflValue* const* args) {
    return AddMusicPart_b41a3(args);
}

UnkReflValue fn_7100fdd870(void*, char*, UnkReflValue* const* args) {
    return AddMusicPart_b41a3(args);
}

// Two identical reflected functions (fn_7100f8abd8 in unit 2602, fn_7100fddb30 in unit 2637): take an
// object and a bool; when the bool is set, pass the object to the music manager's fn_7100fd9d80.
// Return an empty value.
void fn_7100fd9d80(char* manager, char* object);

static inline __attribute__((always_inline)) UnkReflValue SendMusicObject_b41a3(UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* object;
    if (!a[0].type)
        object = nullptr;
    else if (a[0].field_0x14 == 1 || *reinterpret_cast<const u64*>(a[0].type + 0x18) > 8)
        object = static_cast<char*>(a[0].field_0x0);
    else
        object = reinterpret_cast<char*>(&a[0]);
    if (*base::reflection::refl_ArgValue<bool>(&a[1]))
        fn_7100fd9d80(MusicManager_b41a3(), object);
    return base::reflection::refl_EmptyValue();
}

UnkReflValue fn_7100f8abd8(void*, char*, UnkReflValue* const* args) {
    return SendMusicObject_b41a3(args);
}

UnkReflValue fn_7100fddb30(void*, char*, UnkReflValue* const* args) {
    return SendMusicObject_b41a3(args);
}
