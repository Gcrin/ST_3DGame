// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ST_3DGame/Debuffs/DebuffType.h"
#include "STCharacter.generated.h"

class ISTInteractionInterface;
class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

USTRUCT(BlueprintType)
struct FActiveDebuff
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	EDebuffType Type;
	
	FTimerHandle TimerHandle;
	
	UPROPERTY()
	TObjectPtr<USTDebuffEffectBase> EffectInstance;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveDebuffsChanged, const TArray<FActiveDebuff>&, ActiveDebuffs);

UCLASS()
class ST_3DGAME_API ASTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASTCharacter();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnActiveDebuffsChanged OnActiveDebuffsChanged;

	void ApplyDebuff(EDebuffType Type, float Duration);

	void SetIsSlowed(bool bNewState, float Multiplier);
	void SetIsBlinded(bool bNewState);
	void UpdateCharacterSpeed();

	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetHealth() const;
	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetMaxHealth() const;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> OverheadWidget;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;
	void UpdateOverheadWidget();

protected:
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void RemoveDebuff(EDebuffType Type);
	
	void CheckInteraction();
	void OnInteract();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier = 1.5f;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

private:
	
	UPROPERTY()
	TArray<FActiveDebuff> ActiveDebuffs;
	
	bool bIsSprinting = false;
	bool bIsSlowed = false;
	bool bIsBlinded = false;
	float SlowingMultiplier = 1.0f;

	float InteractDistance = 500.0f;
	UPROPERTY()
	TScriptInterface<ISTInteractionInterface> FocusedInteractable; // 현재 바라보고 있는 상호작용 객체
};
