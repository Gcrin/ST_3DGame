// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "STGameState.generated.h"

class USTGameInstance;

USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()

	// 해당 웨이브에서 스폰할 아이템의 총 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 TotalItemSpawnCount;

	// 해당 웨이브의 제한 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveDuration;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveChanged, int32, NewWave);

UCLASS()
class ST_3DGAME_API ASTGameState : public AGameState
{
	GENERATED_BODY()

public:
	ASTGameState();

	UFUNCTION(BlueprintPure, Category="Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Wave")
	int32 GetCurrentWaveIndex() const;
	
	// 게임이 완전히 끝났을 때 (모든 레벨 종료) 실행되는 함수
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void OnGameOver();

	// 코인을 주웠을 때 호출
	void OnCoinCollected();
	// 레벨을 시작할 때, 아이템 스폰 및 타이머 설정
	void StartWave();
	// 레벨을 강제 종료하고 다음 레벨로 이동
	void EndWave(bool bIsTimeUp);

	UFUNCTION(BlueprintPure, Category = "Wave")
	float GetWaveRemainingTime() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWaveChanged OnWaveChanged;

protected:
	virtual void BeginPlay() override;

	// 레벨 제한 시간이 만료되었을 때 호출
	void OnWaveTimeUp();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave", meta = (AllowPrivateAccess = "true"))
	TArray<FWaveData> WaveSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave", meta = (AllowPrivateAccess = "true"))
	int32 CurrentWaveIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Score", meta = (AllowPrivateAccess = "true"))
	int32 Score = 0;
	// 현재 레벨에서 스폰된 코인 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin", meta = (AllowPrivateAccess = "true"))
	int32 SpawnedCoinCount = 0;
	// 플레이어가 수집한 코인 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin", meta = (AllowPrivateAccess = "true"))
	int32 CollectedCoinCount = 0;

private:
	UPROPERTY(Transient)
	TObjectPtr<USTGameInstance> GameInstance;
	// 매 레벨이 끝나기 전까지 시간이 흐르도록 관리하는 타이머
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
};
