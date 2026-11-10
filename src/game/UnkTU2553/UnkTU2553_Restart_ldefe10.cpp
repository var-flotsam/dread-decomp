// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): restarts the current level: quiets the
// sound, rumble and effects, tears the game down (fn_7100def930), rebuilds the level's .bmssd path and drops
// the pending file path. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkSoundMgr;
struct UnkRumbleManager;
struct UnkSoundSystem_b65a1;
struct UnkOwner_b0188;
struct CSubAreaManager;
struct UnkSoundHolder;
struct UnkRefPtrArray;

void fn_7100108030(UnkSoundSystem_b65a1* self, bool a, bool b);
void fn_7100073f48(UnkRumbleManager* self, u32 a, u32 b);
void fn_7101196bf4(UnkOwner_b0188* self, bool release);
void fn_710105383c(void* effects);
void fn_7101053968(void* effects, UnkVec3f* a, UnkVec3f* b, f32 x, f32 y, f32 z);
void fn_7100ee1f34(void* self);
void fn_7100ee34f0(CSubAreaManager* self);
void fn_71010e3704(void* self);
void fn_7100debf3c(UnkGameManager* self);
void fn_7100de6ee8(UnkGameManager* self);
void fn_7100089894(base::file::UnkFileDevice* self);
void fn_710008989c(base::file::UnkFileDevice* self);
void fn_710006cd9c(UnkSoundHolder* holder);
void fn_710006cea8(UnkSoundHolder* holder, bool value);
void fn_7100def930(UnkGameManager* self, u32 flags);
void fn_71002191a4(UnkRenderManager* self, u32 value);
CRntString fn_710007f050(const char* a, const CRntString* b);
CRntString fn_710007ef90(const CRntString* a, const char* b);
void fn_7100e00b48(UnkGameManager* self);
void fn_7100da201c(void* self);
void fn_7100da2774(UnkRefPtrArray* self);

extern UnkSoundMgr* g_unk_7101d4d820;
extern UnkRumbleManager* g_unk_7101d47348;
extern void* g_unk_7101d47330;
extern void* g_unk_7101d47360;
extern base::file::UnkFileDevice* g_unk_7101d4d828;
extern UnkSoundHolder* g_unk_7101d47340;
extern void* g_unk_71727344a8;
__attribute__((visibility("hidden"))) extern UnkVec3f g_unk_710164bdb4;

// The object behind g_unk_7101d47330: a list of counted objects at +0x68 and a flag at +0xb14.
struct UnkRestartCounted {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};
struct UnkRestartNode {
    UnkRestartCounted* object;
    UnkRestartNode* next;
};
struct UnkRestartAllocator {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(UnkRestartNode* node);
};
struct UnkRestartList {
    u32 count;
    UnkRestartNode* head;
    UnkRestartNode* tail;
    UnkRestartAllocator* allocator;

    void Clear() {
        UnkRestartNode* node = head;
        while (node) {
            UnkRestartNode* current = node;
            node = node->next;
            if (current->object && --current->object->ref_count == 0)
                current->object->vf_0x10();
            allocator->vf_0x48(current);
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};
struct UnkRestartOwner {
    u8 pad_0x0[0x68];
    UnkRestartList field_0x68;
    u8 pad_0x88[0xa8c];
    u8 field_0xb14;
};
static_assert(__builtin_offsetof(UnkRestartOwner, field_0xb14) == 0xb14, "layout");

// The members of the game manager this function reads that the shared layout leaves as padding.
struct UnkGameManagerRestartView {
    u8 pad_0x0[0x4b8];
    const char* field_0x4b8;
    u8 pad_0x4c0[0x28];
    const char* field_0x4e8;
    u8 pad_0x4f0[0x88];
    u8 field_0x578;
    u8 field_0x579;
    u8 pad_0x57a[0x96];
    CFilePathStrId field_0x610;
    u8 pad_0x618[0x8];
    const char* field_0x620;
    u8 pad_0x628[0xa50];
    u8 field_0x1078[1];
};
static_assert(__builtin_offsetof(UnkGameManagerRestartView, field_0x610) == 0x610, "layout");
static_assert(__builtin_offsetof(UnkGameManagerRestartView, field_0x1078) == 0x1078, "layout");

template <class T>
static inline T* Instance(T*& slot, u64 size, void (*construct)(T*)) {
    T* object = slot;
    if (!object) {
        object = static_cast<T*>(fn_71000839a0(size, 0x3f));
        construct(object);
        slot = object;
    }
    return object;
}

static inline const char* Text(const char* str) {
    return str ? str : g_unk_710162e2ed;
}

static inline CRntString Concat(const CRntString& a, const char* b) {
    return fn_710007ef90(&a, b);
}

static inline void Assign(CRntString& dst, const CRntString& src) {
    fn_710007eaa0(&dst, &src);
}

static inline void Assign(CFilePathStrId& dst, const CFilePathStrId& src) {
    if (dst.data == src.data)
        return;
    UnkStringData* incoming = src.data;
    UnkStringPool* pool = g_unk_7101d47200;
    if (incoming != &pool->empty)
        ++incoming->ref_count;
    if (dst.data != &pool->empty) {
        --dst.data->ref_count;
        if (dst.data->ref_count == 0)
            fn_71000803a4(pool, dst.data);
    }
    dst.data = incoming;
}

void fn_7100defe10(UnkGameManager* self) {
    UnkGameManagerRestartView* view = reinterpret_cast<UnkGameManagerRestartView*>(self);
    fn_7100108030(reinterpret_cast<UnkSoundSystem_b65a1*>(g_unk_7101d4d820), true, false);
    fn_7100073f48(reinterpret_cast<UnkRumbleManager*>(Instance(
                      reinterpret_cast<base::input::UnkRumbleManager*&>(g_unk_7101d47348), 0x2210, fn_71000733e4)),
                  0, 0);
    fn_7101196bf4(static_cast<UnkOwner_b0188*>(g_unk_7101d47330), true);
    fn_710105383c(view->field_0x1078);
    fn_7101053968(view->field_0x1078, &g_unk_710164bdb4, &g_unk_710164bdb4, 0.0f, 0.0f, 0.0f);
    fn_7100ee34f0(static_cast<CSubAreaManager*>(Instance(g_unk_7101d47360, 0xae8, fn_7100ee1f34)));
    if (UnkGameManagerTarget* target = fn_7100decab0(self))
        if (void* object = target->field_0x248)
            fn_71010e3704(object);
    fn_7100debf3c(self);
    fn_7100de6ee8(self);
    fn_7100089894(Instance(g_unk_7101d4d828, 0x1348, fn_71000893a8));
    fn_710006cea8(Instance(g_unk_7101d47340, 0x48, fn_710006cd9c), false);
    static_cast<UnkRestartOwner*>(g_unk_7101d47330)->field_0x68.Clear();
    static_cast<UnkRestartOwner*>(g_unk_7101d47330)->field_0xb14 = 0;

    u32 flags = view->field_0x579 ? 6 : 4;
    if (view->field_0x578)
        flags |= 8;
    fn_7100def930(self, flags);
    fn_7100de6ee8(self);
    fn_71002191a4(g_unk_7101d47300, 4);

    CRntString path("maps/levels/");
    Assign(path, Concat(Concat(Concat(Concat(Concat(Concat(fn_710007f050(Text(view->field_0x620), &path),
                                                           Text(view->field_0x4b8)),
                                                    "/"),
                                             Text(view->field_0x4e8)),
                                      "/"),
                               Text(view->field_0x4e8)),
                        ".bmssd"));

    if (view->field_0x610.data != &g_unk_7101d47200->empty) {
        fn_7100e00b48(self);
        Assign(view->field_0x610, g_unk_7101d470e8);
    }

    fn_7100089894(Instance(g_unk_7101d4d828, 0x1348, fn_71000893a8));
    fn_710008989c(Instance(g_unk_7101d4d828, 0x1348, fn_71000893a8));
    fn_7100da2774(static_cast<UnkRefPtrArray*>(Instance(g_unk_71727344a8, 0x830, fn_7100da201c)));
}
