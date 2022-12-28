// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "GeomTools.h"

#include "FractureActor01.generated.h"

UCLASS()
class PEARL_ABYSS_API AFractureActor01 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFractureActor01();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//make triangle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	UProceduralMeshComponent* FractureMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	TArray<FVector> Vertices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	TArray<int> Triangles;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Defaults")
	TArray<FVector> Normals;

	TArray<FVector2D> UV0;

	TArray<FLinearColor> VertexColors;
	TArray<FColor> VertexColors2;

	TArray<FProcMeshTangent> Tangents;

	UFUNCTION(BlueprintCallable)
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

	// manipulates the vertices that are close to the impact point vector parameter
	UFUNCTION(BlueprintCallable)
	void FractureActor(FVector impactPoint, int size);
	
	UFUNCTION(BlueprintCallable)
	void ReplaceVertexAndTriangle(TArray<FVector> ui, TArray<int> tri);

	UFUNCTION(BlueprintCallable)
	void MakeNewMesh(TArray<FVector> ui, TArray<int> tri, FVector impactPoint);

	UFUNCTION(BlueprintCallable)
	void FractureSliceMesh(UProceduralMeshComponent* InProcMesh, FVector PlanePosition, FVector PlaneNormal, bool bCreateOtherHalf, UProceduralMeshComponent*& OutOtherHalfProcMesh, EProcMeshSliceCapOption CapOption, UMaterialInterface* CapMaterial);

	UFUNCTION(BlueprintCallable)
		void ClearStaticMesh();
};
