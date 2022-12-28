// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"

#include "MyTriangle.generated.h"

UCLASS()
class PEARL_ABYSS_API AMyTriangle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyTriangle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//make triangle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	UProceduralMeshComponent* TerrainMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	TArray<FVector> Vertices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	TArray<int> Triangles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	TArray<FVector> Normals;

	TArray<FVector2D> UV0;

	TArray<FLinearColor> VertexColors;

	TArray<FProcMeshTangent> Tangents;

	void GenerateMesh();

	//make other Triangle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	TArray<FVector> NewVertices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	TArray<int> NewTriangles;

	int Index_A;
	int Index_B;
	int Index_C;

	void Subdivide(int a, int b, int c);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	int Recursions;

	TArray<FVector> VertexDictionary;

	TArray<int> IndicesDictionary;

	int i_a;
	int i_b;
	int i_c;
	int i_ab;
	int i_bc;
	int i_ca;

};
