// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CalculateComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PEARL_ABYSS_API UCalculateComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCalculateComp();

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetVectorArrayFromString(FString str);
protected:

};
