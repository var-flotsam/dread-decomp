// The engine's start-up (tools/tu_map.py unit 23, 0x7100014d94-0x7100016890; no reflected class):
// fn_7100015c70, which the game's main function (fn_7100000980, src/engine/HostArguments.cpp)
// calls with its settings record and the root path, creates the engine's singletons in order and
// calls the settings' file-system callback, fn_7100015fc0, which mounts the devices ("rom:/",
// "data:/", the packs and the binaries cache) and loads the file table and the system packs.
// Names and layouts are placeholders inferred from the code.

#include "base/UnkRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::input::UnkRumbleManager;
using base::reflection::UnkReflObject;

// Creates the object on first use: storage from the default allocator (flags 0x3f unless given),
// built by its constructor function.
template <class T, u64 Size, void (*Ctor)(T*), u32 Flags = 0x3f>
static inline T* Singleton(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, Flags));
        Ctor(p);
        instance = p;
    }
    return instance;
}

// A callback and the object it is for (src/engine/HostArguments.cpp).
struct UnkStartupCallback {
    void* object;
    void (*function)();
};

// The settings record fn_7100000980 builds (src/engine/HostArguments.cpp has the same layout).
struct UnkStartupSettings {
    u64 field_0x0;
    void (*field_0x8)();
    void (*field_0x10)(const CFilePathStrId& root);
    UnkStartupCallback field_0x18;
    UnkStartupCallback field_0x28;
    bool field_0x38;
    bool field_0x39;
    u8 field_0x3a;
    bool field_0x3b;
};
static_assert(sizeof(UnkStartupSettings) == 0x40, "UnkStartupSettings layout");

// base::reflection's registry (0x1a8 bytes; unit 83): the settings' first word goes to +0x178
// before it is set up.
struct UnkVersionRegistry {
    u8 unk_0x0[0x178];
    u64 field_0x178;
    u8 unk_0x180[0x28];
};
extern UnkVersionRegistry* g_unk_7101d4c8d0;
void fn_71000a4598(UnkVersionRegistry* self);  // constructor
void fn_71000a497c(UnkVersionRegistry* self);

// The frame timer (0x50 bytes; src/game_s1/fn_7100080c40.cpp resets it).
struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;
struct UnkTimer_7100080c40;
void fn_7100080b3c(UnkTimer_7100080c40* self);  // constructor
void fn_7100080c40(UnkTimer_7100080c40* self);  // reset

static inline void TimerCtor(UnkGfxFrameSource* self) {
    fn_7100080b3c(reinterpret_cast<UnkTimer_7100080c40*>(self));
}

// The device registry (base/UnkRegistry.h); the settings' first callback goes to +0x60.
extern UnkRegistry_0x70* g_unk_7101d4b5a0;
void fn_7100087380(UnkRegistry_0x70* self);
void* fn_7100087740(UnkRegistry_0x70* self, const CStrId* name);  // the device registered under `name`

// The pack manager device ("packs", base::file::CPackManagerDevice): the callback at +0x58.
struct UnkPackManagerDevice : UnkReflObject {
    u8 unk_0x8[0x50];
    UnkStartupCallback field_0x58;
};

void fn_710001fb94(bool flag);
void fn_71000ea018();
void fn_710007baa0();

// An object of unit 110 (0x58 bytes), only created here.
struct UnkEngineObject_0x58;
extern UnkEngineObject_0x58* g_unk_7101d4d818;
void fn_7100131e48(UnkEngineObject_0x58* self);  // constructor

// The sound holder (0x48 bytes; src/game/CGameManager.cpp).
struct UnkSoundHolder;
extern UnkSoundHolder* g_unk_7101d47340;
void fn_710006cd9c(UnkSoundHolder* holder);  // constructor
void fn_710006cdb8(UnkSoundHolder* holder);

// base::snd::CSoundSystem (0x400940 bytes, allocated with flags 0xb).
struct UnkSoundMgr {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
};
extern UnkSoundMgr* g_unk_7101d4d820;
void fn_710010790c(UnkSoundMgr* self);  // constructor

extern UnkRumbleManager* g_unk_7101d47348;
void fn_710007388c(UnkRumbleManager* self);

// An object of unit 52 (0xf0 bytes).
struct UnkEngineObject_0xf0;
extern UnkEngineObject_0xf0* g_unk_7101d47338;
void fn_7100075db8(UnkEngineObject_0xf0* self);  // constructor
void fn_7100075e4c(UnkEngineObject_0xf0* self);

static inline UnkRegistry_0x70* Devices() {
    return Singleton<UnkRegistry_0x70, 0x70, fn_7100087388>(g_unk_7101d4b5a0);
}

void fn_7100015c70(const UnkStartupSettings* settings, CFilePathStrId root) {
    fn_7100083870(nullptr, 0);
    UnkVersionRegistry* registry = Singleton<UnkVersionRegistry, 0x1a8, fn_71000a4598>(g_unk_7101d4c8d0);
    registry->field_0x178 = settings->field_0x0;
    fn_71000a497c(registry);
    fn_7100080c40(reinterpret_cast<UnkTimer_7100080c40*>(
        Singleton<UnkGfxFrameSource, 0x50, TimerCtor>(g_unk_7101d47328)));
    Devices();
    fn_7100087380(Devices());
    if (settings->field_0x18.function)
        *reinterpret_cast<UnkStartupCallback*>(&Devices()->field_0x60) = settings->field_0x18;
    if (settings->field_0x10)
        settings->field_0x10(root);
    UnkRegistry_0x70* devices = Devices();
    UnkReflObject* packs;
    {
        CStrId name("packs", true);
        packs = static_cast<UnkReflObject*>(fn_7100087740(devices, &name));
    }
    if (packs && fn_710009d50c(packs->vf_0x0(), fn_710008a028()))
        static_cast<UnkPackManagerDevice*>(packs)->field_0x58 = settings->field_0x28;
    fn_710001fb94(settings->field_0x3b);
    fn_71000ea018();
    Singleton<UnkEngineObject_0x58, 0x58, fn_7100131e48>(g_unk_7101d4d818);
    fn_710007baa0();
    Singleton<UnkSoundHolder, 0x48, fn_710006cd9c>(g_unk_7101d47340);
    fn_710006cdb8(Singleton<UnkSoundHolder, 0x48, fn_710006cd9c>(g_unk_7101d47340));
    Singleton<UnkSoundMgr, 0x400940, fn_710010790c, 0xb>(g_unk_7101d4d820)->vf_0x18();
    Singleton<UnkRumbleManager, 0x2210, fn_71000733e4>(g_unk_7101d47348);
    fn_710007388c(Singleton<UnkRumbleManager, 0x2210, fn_71000733e4>(g_unk_7101d47348));
    Singleton<UnkEngineObject_0xf0, 0xf0, fn_7100075db8>(g_unk_7101d47338);
    fn_7100075e4c(Singleton<UnkEngineObject_0xf0, 0xf0, fn_7100075db8>(g_unk_7101d47338));
}

// ---------------------------------------------------------------------------------------------
// The settings' file-system callback (fn_7100015fc0): the devices, their registration and the
// system packs. The root path is not used.

#include "base/CDeviceQueue.h"
#include "base/file/UnkFileDevice.h"

using base::file::CDeviceQueue;
using base::file::UnkDevice;
using base::file::UnkFileDevice;

// Adds a device to the registry under the name it reports (src/game_s1/fn_71000875d0.cpp).
struct UnkNamedObjectMap;
struct UnkNamedObject;
void fn_71000875d0(UnkNamedObjectMap* self, UnkNamedObject* object);

static inline void AddDevice(UnkRegistry_0x70* devices, void* device) {
    fn_71000875d0(reinterpret_cast<UnkNamedObjectMap*>(devices), static_cast<UnkNamedObject*>(device));
}

// The device constructors (plain functions; callers pass the storage): a host file-system device
// (0x48 bytes, base::file::nx::CDeviceNX's unit) with its name and mount path, the pack manager
// (0x88 bytes) over a device with its own allocator, the queue (0x40 bytes) and the cache
// (0x148 bytes) over a queue.
void fn_710002dd30();
void fn_710002dd34(UnkDevice* self, const CStrId& name, const CFilePathStrId& path);
base::UnkAllocator* fn_7100083724(base::UnkAllocator* parent, u32 flags);
void fn_710008a288(UnkDevice* self, const CStrId& name, UnkDevice* device, base::UnkAllocator* allocator,
                   u32 flags);
void fn_71000846f8(CDeviceQueue* self, const CStrId& name);
void fn_7100084e34(CDeviceQueue* self, UnkDevice* device);
void fn_7100086d08(UnkDevice* self, const CStrId& name, CDeviceQueue* queue);
// The registry's default and save devices, by name, and its set-up.
void fn_710008874c(UnkRegistry_0x70* self, const CStrId& name);
void fn_71000887a8(UnkRegistry_0x70* self, const CStrId& name);
void fn_7100087800(UnkRegistry_0x70* self);
// The pack sets object (0x1348 bytes, base/file/UnkFileDevice.h): its pack manager and the pack
// set table it loads.
extern base::file::UnkFileDevice* g_unk_7101d4d828;
void fn_71000895c8(UnkFileDevice* self, UnkDevice* packs);
void fn_7100089c1c(UnkFileDevice* self, const CFilePathStrId& path);
// Loads the cache's file table; mounts a pack.
void fn_7100087124(UnkDevice* cache, const CFilePathStrId& path);
void fn_710008aa90(UnkDevice* packs, const CFilePathStrId& path);

static inline UnkDevice* NewHostDevice(const char* name, const char* path) {
    UnkDevice* device = static_cast<UnkDevice*>(fn_71000839a0(0x48, 3));
    fn_710002dd34(device, CStrId(name, true), CFilePathStrId(path, true));
    return device;
}

static inline UnkFileDevice* PackSets() {
    return Singleton<UnkFileDevice, 0x1348, fn_71000893a8>(g_unk_7101d4d828);
}

void fn_7100015fc0(const CFilePathStrId& root) {
    UnkRegistry_0x70* devices = Devices();
    fn_710002dd30();
    UnkDevice* rom = NewHostDevice("rom", "rom:/");
    AddDevice(devices, rom);
    UnkDevice* save = NewHostDevice("savedata", "data:/");
    AddDevice(devices, save);
    UnkDevice* textures = NewHostDevice("textures", "rom:/textures/");
    AddDevice(devices, textures);
    base::UnkAllocator* allocator = fn_7100083724(fn_7100083990(), 3);
    UnkDevice* packs = static_cast<UnkDevice*>(fn_71000839a0(0x88, 3));
    fn_710008a288(packs, CStrId("packs", true), rom, allocator, 0x1b);
    CDeviceQueue* queue = static_cast<CDeviceQueue*>(fn_71000839a0(0x40, 3));
    fn_71000846f8(queue, CStrId("binaries_queue", true));
    fn_7100084e34(queue, packs);
    fn_7100084e34(queue, rom);
    fn_7100084e34(queue, textures);
    UnkDevice* binaries = static_cast<UnkDevice*>(fn_71000839a0(0x148, 3));
    fn_7100086d08(binaries, CStrId("binaries", true), queue);
    AddDevice(devices, binaries);
    AddDevice(devices, queue);
    AddDevice(devices, packs);
    fn_710008874c(devices, CStrId("binaries", true));
    fn_71000887a8(devices, CStrId("savedata", true));
    fn_7100087800(devices);
    fn_71000895c8(PackSets(), packs);
    fn_7100087124(binaries, CFilePathStrId("system/files.toc", true));
    fn_710008aa90(packs, CFilePathStrId("packs/system/system.pkg", true));
    fn_710008aa90(packs, CFilePathStrId("packs/system/vibrations.pkg", true));
    fn_7100089c1c(PackSets(), CFilePathStrId("system/pkg/pack_sets.bpsi", true));
}
