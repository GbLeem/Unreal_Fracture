// Fill out your copyright notice in the Description page of Project Settings.


#include "FractureActor01.h"

// Sets default values
AFractureActor01::AFractureActor01()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	FractureMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("FractureMesh"));
	FractureMesh->K2_AttachTo(RootComponent);

	Index_A = 0;
	Index_B = 1;
	Index_C = 2;
}

void AFractureActor01::BeginPlay()
{
	Super::BeginPlay();

	//GenerateMesh();
}

void AFractureActor01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFractureActor01::GenerateMesh()
{
	//create mesh
	FractureMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);

	NewVertices = Vertices;

	if (Recursions > 0)
	{
		for (int i = 0; i < Recursions; ++i)
		{
			for (int j = 0; j < Triangles.Num() / 3; ++j)
			{
				Subdivide(Triangles[Index_A], Triangles[Index_B], Triangles[Index_C]);
			}

			Vertices.Empty();

			Vertices = NewVertices;

			Triangles.Empty();

			Triangles = NewTriangles;
			NewTriangles.Empty();

			Index_A = 0;
			Index_B = 1;
			Index_C = 2;

			VertexDictionary.Empty();
			IndicesDictionary.Empty();
		}
		FractureMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	}
}

void AFractureActor01::Subdivide(int a, int b, int c)
{
	//get endpoints
	FVector va = Vertices[a];
	FVector vb = Vertices[b];
	FVector vc = Vertices[c];

	//get midpoints
	FVector vab = FMath::Lerp(va, vb, 0.5);
	FVector vbc = FMath::Lerp(vb, vc, 0.5);
	FVector vca = FMath::Lerp(vc, va, 0.5);

	//set endpoint indices
	i_a = a;
	i_b = b;
	i_c = c;

	//midpoint duplicate bools
	bool vab_duplicates = false;
	bool vbc_duplicates = false;
	bool vca_duplicates = false;

	//check for midpoint duplicates
	for (int i = 0; i < VertexDictionary.Num(); ++i)
	{
		if (vab == VertexDictionary[i])
		{
			vab_duplicates = true;
			i_ab = IndicesDictionary[i];
		}
		if (vbc == VertexDictionary[i])
		{
			vbc_duplicates = true;
			i_bc = IndicesDictionary[i];
		}
		if (vca == VertexDictionary[i])
		{
			vca_duplicates = true;
			i_ca = IndicesDictionary[i];
		}
	}

	if (!vab_duplicates)
	{
		NewVertices.Add(vab);
		VertexDictionary.Add(vab);
		IndicesDictionary.Add(NewVertices.Num() - 1);
		i_ab = NewVertices.Num() - 1;
	}
	if (!vbc_duplicates)
	{
		NewVertices.Add(vbc);
		VertexDictionary.Add(vbc);
		IndicesDictionary.Add(NewVertices.Num() - 1);
		i_bc = NewVertices.Num() - 1;
	}
	if (!vca_duplicates)
	{
		NewVertices.Add(vca);
		VertexDictionary.Add(vca);
		IndicesDictionary.Add(NewVertices.Num() - 1);
		i_ca = NewVertices.Num() - 1;
	}

	//first
	NewTriangles.Add(i_a);
	NewTriangles.Add(i_ab);
	NewTriangles.Add(i_ca);

	//second
	NewTriangles.Add(i_ca);
	NewTriangles.Add(i_bc);
	NewTriangles.Add(i_c);

	//third
	NewTriangles.Add(i_ab);
	NewTriangles.Add(i_b);
	NewTriangles.Add(i_bc);

	//fourth
	NewTriangles.Add(i_ab);
	NewTriangles.Add(i_bc);
	NewTriangles.Add(i_ca);

	Index_A = Index_A + 3;
	Index_B = Index_B + 3;
	Index_C = Index_C + 3;
}

void AFractureActor01::FractureActor(FVector impactPoint, int size)
{
	for (int i = 0; i < Vertices.Num(); ++i)
	{
		if (FVector(Vertices[i] - impactPoint).Size() < size)
		{
			Vertices[i] = Vertices[i] - FVector(0, 0, 10);
			FractureMesh->UpdateMeshSection(0, Vertices, Normals, UV0, VertexColors2, Tangents);
		}
	}
}

void AFractureActor01::ReplaceVertexAndTriangle(TArray<FVector> ui, TArray<int> tri)
{
	for (int i = 0; i < Vertices.Num(); ++i)
	{
		Vertices[i] = ui[i];
		Triangles[i] = tri[i];
	}
	/*Vertices.Empty();
	Vertices = ui;

	Triangles.Empty();
	Triangles = tri;*/

	//FractureMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	FractureMesh->UpdateMeshSection(0, Vertices, Normals, UV0, VertexColors2, Tangents);
}

void AFractureActor01::MakeNewMesh(TArray<FVector> ui, TArray<int> tri, FVector impactPoint)
{
	for (int i = 0; i < Vertices.Num(); ++i)
	{
		if (FVector(Vertices[i] - impactPoint).Size() < 800)
		{
			Vertices.Empty();
			Vertices = ui;

			Triangles.Empty();
			Triangles = tri;

			FractureMesh->UpdateMeshSection(0, Vertices, Normals, UV0, VertexColors2, Tangents);
		}
	}
}
