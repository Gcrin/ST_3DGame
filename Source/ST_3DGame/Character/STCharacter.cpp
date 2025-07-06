// Fill out your copyright notice in the Description page of Project Settings.


#include "STCharacter.h"
#include "ST_3DGame/Character/STPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTCharacter::ASTCharacter() : MaxHealth(100.0f),
                               Health(MaxHealth),
                               NormalSpeed(600.0f),
                               SprintSpeedMultiplier(1.5f)
{
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

int32 ASTCharacter::GetHealth() const
{
	return static_cast<int32>(Health);
}

void ASTCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Log, TEXT("체력 회복: "), Health);
}

void ASTCharacter::OnDeath()
{
	UE_LOG(LogTemp, Error, TEXT("캐릭터 사망"));
}

float ASTCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
                               AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("체력: %f"), Health);

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
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
}

void ASTCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
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
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * SprintSpeedMultiplier;
	}
}

void ASTCharacter::StopSprint(const FInputActionValue& Value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}
