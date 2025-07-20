// Fill out your copyright notice in the Description page of Project Settings.


#include "STCharacter.h"
#include "ST_3DGame/Character/STPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ST_3DGame/GameModes/STGameState.h"
#include "ST_3DGame/Interaction/STInteractionInterface.h"

ASTCharacter::ASTCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	Health = MaxHealth;
}

int32 ASTCharacter::GetHealth() const
{
	return static_cast<int32>(Health);
}

int32 ASTCharacter::GetMaxHealth() const
{
	return static_cast<int32>(MaxHealth);
}

void ASTCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadWidget();

	OnHealthChanged.Broadcast(Health, MaxHealth);
}

void ASTCharacter::OnDeath()
{
	if (ASTGameState* STGameState = GetWorld() ? GetWorld()->GetGameState<ASTGameState>() : nullptr)
	{
		STGameState->OnGameOver();
	}
}

float ASTCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
                               AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("체력: %f"), Health);
	UpdateOverheadWidget();

	OnHealthChanged.Broadcast(Health, MaxHealth);

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ASTCharacter::UpdateOverheadWidget()
{
	if (!OverheadWidget) return;

	const UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance)
	{
		return;
	}

	if (UProgressBar* HealthBar = Cast<UProgressBar>(OverheadWidgetInstance->GetWidgetFromName(TEXT("HealthBar"))))
	{
		HealthBar->SetPercent(Health / MaxHealth);
	}
}

void ASTCharacter::ApplySlowingEffect(float Duration, float SpeedMultiplier)
{
	bIsSlowed = true;
	SlowingMultiplier = SpeedMultiplier;
	UpdateCharacterSpeed();

	GetWorldTimerManager().ClearTimer(SlowingTimerHandle);
	GetWorldTimerManager().SetTimer(SlowingTimerHandle, this, &ASTCharacter::ClearSlowingEffect, Duration, false);

	UE_LOG(LogTemp, Warning, TEXT("슬로우 디버프 효과 %f초 적용"), Duration);
}

void ASTCharacter::ApplyBlindEffect(float Duration)
{
	bIsBlinded = true;
	OnBlindStateChanged.Broadcast(bIsBlinded);

	GetWorldTimerManager().ClearTimer(BlindTimerHandle);
	GetWorldTimerManager().SetTimer(BlindTimerHandle, this, &ASTCharacter::ClearBlindEffect, Duration, false);

	UE_LOG(LogTemp, Warning, TEXT("실명 디버프 효과 %f초 적용"), Duration);
}

void ASTCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckInteraction();
}

void ASTCharacter::CheckInteraction()
{
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + CameraComponent->GetForwardVector() * InteractDistance; // 5미터 앞까지 체크
	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		TScriptInterface<ISTInteractionInterface> NewInteractable = HitResult.GetActor();

		if (NewInteractable) // 인터페이스를 가진 액터를 바라봤다면
		{
			if (NewInteractable != FocusedInteractable)
			{
				// 이전에 보던 게 있다면 숨김 처리
				if (FocusedInteractable)
				{
					FocusedInteractable->Execute_HideInteractionPrompt(FocusedInteractable.GetObject());
				}
				// 새로 보는 것에게 UI를 보여달라고 요청
				NewInteractable->Execute_ShowInteractionPrompt(NewInteractable.GetObject());
				FocusedInteractable = NewInteractable;
			}
		}
		else // 인터페이스가 없는 액터를 바라봤다면
		{
			if (FocusedInteractable)
			{
				FocusedInteractable->Execute_HideInteractionPrompt(FocusedInteractable.GetObject());
				FocusedInteractable = nullptr;
			}
		}
	}
	else // 아무것도 안 맞았다면
	{
		if (FocusedInteractable)
		{
			FocusedInteractable->Execute_HideInteractionPrompt(FocusedInteractable.GetObject());
			FocusedInteractable = nullptr;
		}
	}
}

void ASTCharacter::OnInteract()
{
	if (FocusedInteractable)
	{
		FocusedInteractable->Execute_Interact(FocusedInteractable.GetObject(), this);
	}
}

void ASTCharacter::ClearSlowingEffect()
{
	bIsSlowed = false;
	SlowingMultiplier = 1.0f;
	UpdateCharacterSpeed();
	UE_LOG(LogTemp, Warning, TEXT("슬로우 디버프 효과 제거"));
}

void ASTCharacter::ClearBlindEffect()
{
	bIsBlinded = false;
	OnBlindStateChanged.Broadcast(bIsBlinded);

	UE_LOG(LogTemp, Warning, TEXT("실명 디버프 효과 제거"));
}

void ASTCharacter::UpdateCharacterSpeed()
{
	float TargetSpeed = NormalSpeed;

	if (bIsSlowed)
	{
		TargetSpeed *= SlowingMultiplier;
	}

	if (bIsSprinting)
	{
		TargetSpeed *= SprintSpeedMultiplier;
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = TargetSpeed;
	}
}

void ASTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ASTPlayerController* PlayerController = Cast<ASTPlayerController>(GetController());

	if (!EnhancedInputComponent || !PlayerController)
	{
		return;
	}

	auto BindAction = [&](UInputAction* Action, ETriggerEvent TriggerEvent, auto Handler)
	{
		if (Action)
		{
			EnhancedInputComponent->BindAction(Action, TriggerEvent, this, Handler);
		}
	};

	BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, &ASTCharacter::Move);
	BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, &ASTCharacter::Look);

	BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, &ASTCharacter::StartJump);
	BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, &ASTCharacter::StopJump);

	BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, &ASTCharacter::StartSprint);
	BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, &ASTCharacter::StopSprint);

	BindAction(PlayerController->InteractAction, ETriggerEvent::Started, &ASTCharacter::OnInteract);
}

void ASTCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	UpdateOverheadWidget();
}

void ASTCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D MoveInput = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASTCharacter::StartJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void ASTCharacter::StopJump(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		StopJumping();
	}
}

void ASTCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ASTCharacter::StartSprint(const FInputActionValue& Value)
{
	bIsSprinting = true;
	UpdateCharacterSpeed();
}

void ASTCharacter::StopSprint(const FInputActionValue& Value)
{
	bIsSprinting = false;
	UpdateCharacterSpeed();
}
