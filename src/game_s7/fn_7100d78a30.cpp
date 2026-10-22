// Caches a newly added component in the slot for its class (one slot per component class, from
// CLifeComponent at +0x1b8 to CEmmyValveComponent at +0x300), when the component's owner id is
// that class's kind id. Class names come from the getters' reflection names; layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflObject;

const CClass* fn_710115ddb8();  // CLifeComponent::refl_GetClass
const CClass* fn_7101161bb4();  // CPlayerLifeComponent::refl_GetClass
const CClass* fn_7101158414();  // CCharacterLifeComponent::refl_GetClass
const CClass* fn_71011593e4();  // CEnemyLifeComponent::refl_GetClass
const CClass* fn_7100870440();  // CInputComponent::refl_GetClass
const CClass* fn_7101139a60();  // CScriptComponent::refl_GetClass
const CClass* fn_7100a5bab0();  // CTriggerComponent::refl_GetClass
const CClass* fn_71010d02f4();  // CMovementComponent::refl_GetClass
const CClass* fn_7100975b3c();  // CCharacterMovement::refl_GetClass
const CClass* fn_71010d68bc();  // CPlayerMovement::refl_GetClass
const CClass* fn_71009dda1c();  // CSamusMovement::refl_GetClass
const CClass* fn_710093b204();  // CMorphBallMovement::refl_GetClass
const CClass* fn_71010c74e0();  // CEnemyMovement::refl_GetClass
const CClass* fn_7101174bd4();  // CAnimationComponent::refl_GetClass
const CClass* fn_710116f89c();  // CModelUpdaterComponent::refl_GetClass
const CClass* fn_7100726a04();  // CBillboardComponent::refl_GetClass
const CClass* fn_710116d338();  // CInterpolationComponent::refl_GetClass
const CClass* fn_71010f6b50();  // CAIComponent::refl_GetClass
const CClass* fn_71010df7a0();  // CCameraComponent::refl_GetClass
const CClass* fn_7101149ee0();  // CPositionalSoundComponent::refl_GetClass
const CClass* fn_7101135a64();  // CAttackComponent::refl_GetClass
const CClass* fn_710113a334();  // CGrabComponent::refl_GetClass
const CClass* fn_7101138934();  // CFactionComponent::refl_GetClass
const CClass* fn_71011408c4();  // CInventoryComponent::refl_GetClass
const CClass* fn_71011336b4();  // CAIAttackComponent::refl_GetClass
const CClass* fn_710114d608();  // CFXComponent::refl_GetClass
const CClass* fn_7100925fc0();  // CMaterialFXComponent::refl_GetClass
const CClass* fn_7101181994();  // CTargetComponent::refl_GetClass
const CClass* fn_71006f3ea0();  // CAINavigationComponent::refl_GetClass
const CClass* fn_710085fc44();  // CCollisionComponent::refl_GetClass
const CClass* fn_7100838154();  // CGunComponent::refl_GetClass
const CClass* fn_710033ff68();  // CAimComponent::refl_GetClass
const CClass* fn_7100718718();  // CAudioComponent::refl_GetClass
const CClass* fn_71009d9dc4();  // CRumbleComponent::refl_GetClass
const CClass* fn_71007f8314();  // CFrozenComponent::refl_GetClass
const CClass* fn_71009f8e94();  // CShotComponent::refl_GetClass
const CClass* fn_71009f2a24();  // CSceneModelAnimationComponent::refl_GetClass
const CClass* fn_71002fbaf8();  // CAbilityComponent::refl_GetClass
const CClass* fn_7100a2a190();  // CTimelineComponent::refl_GetClass
const CClass* fn_7100951424();  // CMovablePlatformComponent::refl_GetClass
const CClass* fn_71009288b0();  // CMeleeComponent::refl_GetClass
const CClass* fn_71007b1ea8();  // CEmmyValveComponent::refl_GetClass

struct UnkComponentOwner {
    u64 field_0x0;
    CStrId id;
};

struct UnkComponent : UnkReflObject {
    u64 field_0x8;
    UnkComponentOwner* owner;
};

// The kind ids components of each class are registered under.
inline const CStrId& Kind_LIFE() {
    static const CStrId s_id("LIFE", true);
    return s_id;
}

inline const CStrId& Kind_INPUT() {
    static const CStrId s_id("INPUT", true);
    return s_id;
}

inline const CStrId& Kind_SCRIPT() {
    static const CStrId s_id("SCRIPT", true);
    return s_id;
}

inline const CStrId& Kind_TRIGGER() {
    static const CStrId s_id("TRIGGER", true);
    return s_id;
}

inline const CStrId& Kind_MOVEMENT() {
    static const CStrId s_id("MOVEMENT", true);
    return s_id;
}

inline const CStrId& Kind_ANIMATION() {
    static const CStrId s_id("ANIMATION", true);
    return s_id;
}

inline const CStrId& Kind_MODELUPDATER() {
    static const CStrId s_id("MODELUPDATER", true);
    return s_id;
}

inline const CStrId& Kind_BILLBOARD() {
    static const CStrId s_id("BILLBOARD", true);
    return s_id;
}

inline const CStrId& Kind_INTERPOLATION() {
    static const CStrId s_id("INTERPOLATION", true);
    return s_id;
}

inline const CStrId& Kind_AI() {
    static const CStrId s_id("AI", true);
    return s_id;
}

inline const CStrId& Kind_CAMERA() {
    static const CStrId s_id("CAMERA", true);
    return s_id;
}

inline const CStrId& Kind_POSITIONALSOUND() {
    static const CStrId s_id("POSITIONALSOUND", true);
    return s_id;
}

inline const CStrId& Kind_ATTACK() {
    static const CStrId s_id("ATTACK", true);
    return s_id;
}

inline const CStrId& Kind_GRAB() {
    static const CStrId s_id("GRAB", true);
    return s_id;
}

inline const CStrId& Kind_FACTION() {
    static const CStrId s_id("FACTION", true);
    return s_id;
}

inline const CStrId& Kind_INVENTORY() {
    static const CStrId s_id("INVENTORY", true);
    return s_id;
}

inline const CStrId& Kind_FX() {
    static const CStrId s_id("FX", true);
    return s_id;
}

inline const CStrId& Kind_MATERIALFX() {
    static const CStrId s_id("MATERIALFX", true);
    return s_id;
}

inline const CStrId& Kind_TARGETCOMP() {
    static const CStrId s_id("TARGETCOMP", true);
    return s_id;
}

inline const CStrId& Kind_AINAVIGATION() {
    static const CStrId s_id("AINAVIGATION", true);
    return s_id;
}

inline const CStrId& Kind_COLLISION() {
    static const CStrId s_id("COLLISION", true);
    return s_id;
}

inline const CStrId& Kind_GUN() {
    static const CStrId s_id("GUN", true);
    return s_id;
}

inline const CStrId& Kind_AIM() {
    static const CStrId s_id("AIM", true);
    return s_id;
}

inline const CStrId& Kind_AUDIO() {
    static const CStrId s_id("AUDIO", true);
    return s_id;
}

inline const CStrId& Kind_RUMBLE() {
    static const CStrId s_id("RUMBLE", true);
    return s_id;
}

inline const CStrId& Kind_FROZEN() {
    static const CStrId s_id("FROZEN", true);
    return s_id;
}

inline const CStrId& Kind_SHOT() {
    static const CStrId s_id("SHOT", true);
    return s_id;
}

inline const CStrId& Kind_SCENEANIM() {
    static const CStrId s_id("SCENEANIM", true);
    return s_id;
}

inline const CStrId& Kind_ABILITY() {
    static const CStrId s_id("ABILITY", true);
    return s_id;
}

inline const CStrId& Kind_TIMELINECOMPONENT() {
    static const CStrId s_id("TIMELINECOMPONENT", true);
    return s_id;
}

inline const CStrId& Kind_MELEE() {
    static const CStrId s_id("MELEE", true);
    return s_id;
}

inline const CStrId& Kind_EMMYVALVE() {
    static const CStrId s_id("EMMYVALVE", true);
    return s_id;
}

// Compares by data pointer (both ids are read after both are found).
inline bool SameId(const CStrId& a, const CStrId& b) {
    return a.data == b.data;
}

template <const CStrId& (*Kind)(), const CClass* (*GetClass)()>
inline void CacheAs(UnkComponent*& slot, UnkComponent* comp) {
    if (SameId(Kind(), comp->owner->id))
        slot = fn_710009d50c(comp->vf_0x0(), GetClass()) ? comp : nullptr;
}

struct UnkComponentCache {
    u8 unk_0x0[0x1b8];
    UnkComponent* lifeComponent;
    UnkComponent* playerLifeComponent;
    UnkComponent* characterLifeComponent;
    UnkComponent* enemyLifeComponent;
    UnkComponent* inputComponent;
    UnkComponent* scriptComponent;
    UnkComponent* triggerComponent;
    UnkComponent* movementComponent;
    UnkComponent* characterMovement;
    UnkComponent* playerMovement;
    UnkComponent* samusMovement;
    UnkComponent* morphBallMovement;
    UnkComponent* enemyMovement;
    UnkComponent* animationComponent;
    UnkComponent* modelUpdaterComponent;
    UnkComponent* billboardComponent;
    UnkComponent* interpolationComponent;
    UnkComponent* aIComponent;
    UnkComponent* cameraComponent;
    UnkComponent* positionalSoundComponent;
    UnkComponent* attackComponent;
    UnkComponent* grabComponent;
    UnkComponent* factionComponent;
    UnkComponent* inventoryComponent;
    UnkComponent* aIAttackComponent;
    UnkComponent* fXComponent;
    UnkComponent* materialFXComponent;
    UnkComponent* targetComponent;
    UnkComponent* aINavigationComponent;
    UnkComponent* collisionComponent;
    UnkComponent* gunComponent;
    UnkComponent* aimComponent;
    UnkComponent* audioComponent;
    UnkComponent* rumbleComponent;
    UnkComponent* frozenComponent;
    UnkComponent* shotComponent;
    UnkComponent* sceneModelAnimationComponent;
    UnkComponent* abilityComponent;
    UnkComponent* timelineComponent;
    UnkComponent* movablePlatformComponent;
    UnkComponent* meleeComponent;
    UnkComponent* emmyValveComponent;
};

void fn_7100d78a30(UnkComponentCache* self, UnkComponent* comp) {
    if (!comp)
        return;
    CacheAs<Kind_LIFE, fn_710115ddb8>(self->lifeComponent, comp);
    CacheAs<Kind_LIFE, fn_7101161bb4>(self->playerLifeComponent, comp);
    CacheAs<Kind_LIFE, fn_7101158414>(self->characterLifeComponent, comp);
    CacheAs<Kind_LIFE, fn_71011593e4>(self->enemyLifeComponent, comp);
    CacheAs<Kind_INPUT, fn_7100870440>(self->inputComponent, comp);
    CacheAs<Kind_SCRIPT, fn_7101139a60>(self->scriptComponent, comp);
    CacheAs<Kind_TRIGGER, fn_7100a5bab0>(self->triggerComponent, comp);
    CacheAs<Kind_MOVEMENT, fn_71010d02f4>(self->movementComponent, comp);
    CacheAs<Kind_MOVEMENT, fn_7100975b3c>(self->characterMovement, comp);
    CacheAs<Kind_MOVEMENT, fn_71010d68bc>(self->playerMovement, comp);
    CacheAs<Kind_MOVEMENT, fn_71009dda1c>(self->samusMovement, comp);
    CacheAs<Kind_MOVEMENT, fn_710093b204>(self->morphBallMovement, comp);
    CacheAs<Kind_MOVEMENT, fn_71010c74e0>(self->enemyMovement, comp);
    CacheAs<Kind_ANIMATION, fn_7101174bd4>(self->animationComponent, comp);
    CacheAs<Kind_MODELUPDATER, fn_710116f89c>(self->modelUpdaterComponent, comp);
    CacheAs<Kind_BILLBOARD, fn_7100726a04>(self->billboardComponent, comp);
    CacheAs<Kind_INTERPOLATION, fn_710116d338>(self->interpolationComponent, comp);
    CacheAs<Kind_AI, fn_71010f6b50>(self->aIComponent, comp);
    CacheAs<Kind_CAMERA, fn_71010df7a0>(self->cameraComponent, comp);
    CacheAs<Kind_POSITIONALSOUND, fn_7101149ee0>(self->positionalSoundComponent, comp);
    CacheAs<Kind_ATTACK, fn_7101135a64>(self->attackComponent, comp);
    CacheAs<Kind_GRAB, fn_710113a334>(self->grabComponent, comp);
    CacheAs<Kind_FACTION, fn_7101138934>(self->factionComponent, comp);
    CacheAs<Kind_INVENTORY, fn_71011408c4>(self->inventoryComponent, comp);
    CacheAs<Kind_ATTACK, fn_71011336b4>(self->aIAttackComponent, comp);
    CacheAs<Kind_FX, fn_710114d608>(self->fXComponent, comp);
    CacheAs<Kind_MATERIALFX, fn_7100925fc0>(self->materialFXComponent, comp);
    CacheAs<Kind_TARGETCOMP, fn_7101181994>(self->targetComponent, comp);
    CacheAs<Kind_AINAVIGATION, fn_71006f3ea0>(self->aINavigationComponent, comp);
    CacheAs<Kind_COLLISION, fn_710085fc44>(self->collisionComponent, comp);
    CacheAs<Kind_GUN, fn_7100838154>(self->gunComponent, comp);
    CacheAs<Kind_AIM, fn_710033ff68>(self->aimComponent, comp);
    CacheAs<Kind_AUDIO, fn_7100718718>(self->audioComponent, comp);
    CacheAs<Kind_RUMBLE, fn_71009d9dc4>(self->rumbleComponent, comp);
    CacheAs<Kind_FROZEN, fn_71007f8314>(self->frozenComponent, comp);
    CacheAs<Kind_SHOT, fn_71009f8e94>(self->shotComponent, comp);
    CacheAs<Kind_SCENEANIM, fn_71009f2a24>(self->sceneModelAnimationComponent, comp);
    CacheAs<Kind_ABILITY, fn_71002fbaf8>(self->abilityComponent, comp);
    CacheAs<Kind_TIMELINECOMPONENT, fn_7100a2a190>(self->timelineComponent, comp);
    CacheAs<Kind_MOVEMENT, fn_7100951424>(self->movablePlatformComponent, comp);
    CacheAs<Kind_MELEE, fn_71009288b0>(self->meleeComponent, comp);
    CacheAs<Kind_EMMYVALVE, fn_71007b1ea8>(self->emmyValveComponent, comp);
}
