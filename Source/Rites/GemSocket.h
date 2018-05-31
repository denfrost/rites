// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GemSocket.generated.h"

class UGem;

USTRUCT(BlueprintType)
struct FGemSocket
{
	GENERATED_BODY()

public:

	//FGemSocket()
	//{
	//	Color = EGemColor::Red;
	//}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGemColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGem* Gem;

	//UPROPERTY(EditAnywhere)
	//int32 TestProp;
};
