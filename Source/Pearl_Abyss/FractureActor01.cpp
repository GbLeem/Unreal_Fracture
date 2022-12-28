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

void AFractureActor01::FractureSliceMesh(UProceduralMeshComponent* InProcMesh, FVector PlanePosition, FVector PlaneNormal, bool bCreateOtherHalf, UProceduralMeshComponent*& OutOtherHalfProcMesh, EProcMeshSliceCapOption CapOption, UMaterialInterface* CapMaterial)
{
	//if (InProcMesh != nullptr)
	//{
	//	// Transform plane from world to local space
	//	FTransform ProcCompToWorld = InProcMesh->GetComponentToWorld();
	//	FVector LocalPlanePos = ProcCompToWorld.InverseTransformPosition(PlanePosition);
	//	FVector LocalPlaneNormal = ProcCompToWorld.InverseTransformVectorNoScale(PlaneNormal);
	//	LocalPlaneNormal = LocalPlaneNormal.GetSafeNormal(); // Ensure normalized

	//	FPlane SlicePlane(LocalPlanePos, LocalPlaneNormal);

	//	// Set of sections to add to the 'other half' component
	//	TArray<FProcMeshSection> OtherSections;
	//	// Material for each section of other half
	//	TArray<UMaterialInterface*> OtherMaterials;

	//	// Set of new edges created by clipping polys by plane
	//	TArray<FUtilEdge3D> ClipEdges;

	//	for (int32 SectionIndex = 0; SectionIndex < InProcMesh->GetNumSections(); SectionIndex++)
	//	{
	//		FProcMeshSection* BaseSection = InProcMesh->GetProcMeshSection(SectionIndex);
	//		// If we have a section, and it has some valid geom
	//		if (BaseSection != nullptr && BaseSection->ProcIndexBuffer.Num() > 0 && BaseSection->ProcVertexBuffer.Num() > 0)
	//		{
	//			// Compare bounding box of section with slicing plane
	//			int32 BoxCompare = BoxPlaneCompare(BaseSection->SectionLocalBox, SlicePlane);

	//			// Box totally clipped, clear section
	//			if (BoxCompare == -1)
	//			{
	//				// Add entire section to other half
	//				if (bCreateOtherHalf)
	//				{
	//					OtherSections.Add(*BaseSection);
	//					OtherMaterials.Add(InProcMesh->GetMaterial(SectionIndex));
	//				}

	//				InProcMesh->ClearMeshSection(SectionIndex);
	//			}
	//			// Box totally on one side of plane, leave it alone, do nothing
	//			else if (BoxCompare == 1)
	//			{
	//				// ...
	//			}
	//			// Box intersects plane, need to clip some polys!
	//			else
	//			{
	//				// New section for geometry
	//				FProcMeshSection NewSection;

	//				// New section for 'other half' geometry (if desired)
	//				FProcMeshSection* NewOtherSection = nullptr;
	//				if (bCreateOtherHalf)
	//				{
	//					int32 OtherSectionIndex = OtherSections.Add(FProcMeshSection());
	//					NewOtherSection = &OtherSections[OtherSectionIndex];

	//					OtherMaterials.Add(InProcMesh->GetMaterial(SectionIndex)); // Remember material for this section
	//				}

	//				// Map of base vert index to sliced vert index
	//				TMap<int32, int32> BaseToSlicedVertIndex;
	//				TMap<int32, int32> BaseToOtherSlicedVertIndex;

	//				const int32 NumBaseVerts = BaseSection->ProcVertexBuffer.Num();

	//				// Distance of each base vert from slice plane
	//				TArray<float> VertDistance;
	//				VertDistance.AddUninitialized(NumBaseVerts);

	//				// Build vertex buffer 
	//				for (int32 BaseVertIndex = 0; BaseVertIndex < NumBaseVerts; BaseVertIndex++)
	//				{
	//					FProcMeshVertex& BaseVert = BaseSection->ProcVertexBuffer[BaseVertIndex];

	//					// Calc distance from plane
	//					VertDistance[BaseVertIndex] = SlicePlane.PlaneDot(BaseVert.Position);

	//					// See if vert is being kept in this section
	//					if (VertDistance[BaseVertIndex] > 0.f)
	//					{
	//						// Copy to sliced v buffer
	//						int32 SlicedVertIndex = NewSection.ProcVertexBuffer.Add(BaseVert);
	//						// Update section bounds
	//						NewSection.SectionLocalBox += BaseVert.Position;
	//						// Add to map
	//						BaseToSlicedVertIndex.Add(BaseVertIndex, SlicedVertIndex);
	//					}
	//					// Or add to other half if desired
	//					else if (NewOtherSection != nullptr)
	//					{
	//						int32 SlicedVertIndex = NewOtherSection->ProcVertexBuffer.Add(BaseVert);
	//						NewOtherSection->SectionLocalBox += BaseVert.Position;
	//						BaseToOtherSlicedVertIndex.Add(BaseVertIndex, SlicedVertIndex);
	//					}
	//				}


	//				// Iterate over base triangles (ie 3 indices at a time)
	//				for (int32 BaseIndex = 0; BaseIndex < BaseSection->ProcIndexBuffer.Num(); BaseIndex += 3)
	//				{
	//					int32 BaseV[3]; // Triangle vert indices in original mesh
	//					int32* SlicedV[3]; // Pointers to vert indices in new v buffer
	//					int32* SlicedOtherV[3]; // Pointers to vert indices in new 'other half' v buffer

	//					// For each vertex..
	//					for (int32 i = 0; i < 3; i++)
	//					{
	//						// Get triangle vert index
	//						BaseV[i] = BaseSection->ProcIndexBuffer[BaseIndex + i];
	//						// Look up in sliced v buffer
	//						SlicedV[i] = BaseToSlicedVertIndex.Find(BaseV[i]);
	//						// Look up in 'other half' v buffer (if desired)
	//						if (bCreateOtherHalf)
	//						{
	//							SlicedOtherV[i] = BaseToOtherSlicedVertIndex.Find(BaseV[i]);
	//							// Each base vert _must_ exist in either BaseToSlicedVertIndex or BaseToOtherSlicedVertIndex 
	//							check((SlicedV[i] != nullptr) != (SlicedOtherV[i] != nullptr));
	//						}
	//					}

	//					// If all verts survived plane cull, keep the triangle
	//					if (SlicedV[0] != nullptr && SlicedV[1] != nullptr && SlicedV[2] != nullptr)
	//					{
	//						NewSection.ProcIndexBuffer.Add(*SlicedV[0]);
	//						NewSection.ProcIndexBuffer.Add(*SlicedV[1]);
	//						NewSection.ProcIndexBuffer.Add(*SlicedV[2]);
	//					}
	//					// If all verts were removed by plane cull
	//					else if (SlicedV[0] == nullptr && SlicedV[1] == nullptr && SlicedV[2] == nullptr)
	//					{
	//						// If creating other half, add all verts to that
	//						if (NewOtherSection != nullptr)
	//						{
	//							NewOtherSection->ProcIndexBuffer.Add(*SlicedOtherV[0]);
	//							NewOtherSection->ProcIndexBuffer.Add(*SlicedOtherV[1]);
	//							NewOtherSection->ProcIndexBuffer.Add(*SlicedOtherV[2]);
	//						}
	//					}
	//					// If partially culled, clip to create 1 or 2 new triangles
	//					else
	//					{
	//						int32 FinalVerts[4];
	//						int32 NumFinalVerts = 0;

	//						int32 OtherFinalVerts[4];
	//						int32 NumOtherFinalVerts = 0;

	//						FUtilEdge3D NewClipEdge;
	//						int32 ClippedEdges = 0;

	//						float PlaneDist[3];
	//						PlaneDist[0] = VertDistance[BaseV[0]];
	//						PlaneDist[1] = VertDistance[BaseV[1]];
	//						PlaneDist[2] = VertDistance[BaseV[2]];

	//						for (int32 EdgeIdx = 0; EdgeIdx < 3; EdgeIdx++)
	//						{
	//							int32 ThisVert = EdgeIdx;

	//							// If start vert is inside, add it.
	//							if (SlicedV[ThisVert] != nullptr)
	//							{
	//								check(NumFinalVerts < 4);
	//								FinalVerts[NumFinalVerts++] = *SlicedV[ThisVert];
	//							}
	//							// If not, add to other side
	//							else if (bCreateOtherHalf)
	//							{
	//								check(NumOtherFinalVerts < 4);
	//								OtherFinalVerts[NumOtherFinalVerts++] = *SlicedOtherV[ThisVert];
	//							}

	//							// If start and next vert are on opposite sides, add intersection
	//							int32 NextVert = (EdgeIdx + 1) % 3;

	//							if ((SlicedV[EdgeIdx] == nullptr) != (SlicedV[NextVert] == nullptr))
	//							{
	//								// Find distance along edge that plane is
	//								float Alpha = -PlaneDist[ThisVert] / (PlaneDist[NextVert] - PlaneDist[ThisVert]);
	//								// Interpolate vertex params to that point
	//								FProcMeshVertex InterpVert = InterpolateVert(BaseSection->ProcVertexBuffer[BaseV[ThisVert]], BaseSection->ProcVertexBuffer[BaseV[NextVert]], FMath::Clamp(Alpha, 0.0f, 1.0f));

	//								// Add to vertex buffer
	//								int32 InterpVertIndex = NewSection.ProcVertexBuffer.Add(InterpVert);
	//								// Update bounds
	//								NewSection.SectionLocalBox += InterpVert.Position;

	//								// Save vert index for this poly
	//								check(NumFinalVerts < 4);
	//								FinalVerts[NumFinalVerts++] = InterpVertIndex;

	//								// If desired, add to the poly for the other half as well
	//								if (NewOtherSection != nullptr)
	//								{
	//									int32 OtherInterpVertIndex = NewOtherSection->ProcVertexBuffer.Add(InterpVert);
	//									NewOtherSection->SectionLocalBox += InterpVert.Position;
	//									check(NumOtherFinalVerts < 4);
	//									OtherFinalVerts[NumOtherFinalVerts++] = OtherInterpVertIndex;
	//								}

	//								// When we make a new edge on the surface of the clip plane, save it off.
	//								check(ClippedEdges < 2);
	//								if (ClippedEdges == 0)
	//								{
	//									NewClipEdge.V0 = (FVector3f)InterpVert.Position;
	//								}
	//								else
	//								{
	//									NewClipEdge.V1 = (FVector3f)InterpVert.Position;
	//								}

	//								ClippedEdges++;
	//							}
	//						}

	//						// Triangulate the clipped polygon.
	//						for (int32 VertexIndex = 2; VertexIndex < NumFinalVerts; VertexIndex++)
	//						{
	//							NewSection.ProcIndexBuffer.Add(FinalVerts[0]);
	//							NewSection.ProcIndexBuffer.Add(FinalVerts[VertexIndex - 1]);
	//							NewSection.ProcIndexBuffer.Add(FinalVerts[VertexIndex]);
	//						}

	//						// If we are making the other half, triangulate that as well
	//						if (NewOtherSection != nullptr)
	//						{
	//							for (int32 VertexIndex = 2; VertexIndex < NumOtherFinalVerts; VertexIndex++)
	//							{
	//								NewOtherSection->ProcIndexBuffer.Add(OtherFinalVerts[0]);
	//								NewOtherSection->ProcIndexBuffer.Add(OtherFinalVerts[VertexIndex - 1]);
	//								NewOtherSection->ProcIndexBuffer.Add(OtherFinalVerts[VertexIndex]);
	//							}
	//						}

	//						check(ClippedEdges != 1); // Should never clip just one edge of the triangle

	//						// If we created a new edge, save that off here as well
	//						if (ClippedEdges == 2)
	//						{
	//							ClipEdges.Add(NewClipEdge);
	//						}
	//					}
	//				}

	//				// Remove 'other' section from array if no valid geometry for it
	//				if (NewOtherSection != nullptr && (NewOtherSection->ProcIndexBuffer.Num() == 0 || NewOtherSection->ProcVertexBuffer.Num() == 0))
	//				{
	//					OtherSections.RemoveAt(OtherSections.Num() - 1);
	//				}

	//				// If we have some valid geometry, update section
	//				if (NewSection.ProcIndexBuffer.Num() > 0 && NewSection.ProcVertexBuffer.Num() > 0)
	//				{
	//					// Assign new geom to this section
	//					InProcMesh->SetProcMeshSection(SectionIndex, NewSection);
	//				}
	//				// If we don't, remove this section
	//				else
	//				{
	//					InProcMesh->ClearMeshSection(SectionIndex);
	//				}
	//			}
	//		}
	//	}

	//	// Create cap geometry (if some edges to create it from)
	//	if (CapOption != EProcMeshSliceCapOption::NoCap && ClipEdges.Num() > 0)
	//	{
	//		FProcMeshSection CapSection;
	//		int32 CapSectionIndex = INDEX_NONE;

	//		// If using an existing section, copy that info first
	//		if (CapOption == EProcMeshSliceCapOption::UseLastSectionForCap)
	//		{
	//			CapSectionIndex = InProcMesh->GetNumSections() - 1;
	//			CapSection = *InProcMesh->GetProcMeshSection(CapSectionIndex);
	//		}
	//		// Adding new section for cap
	//		else
	//		{
	//			CapSectionIndex = InProcMesh->GetNumSections();
	//		}

	//		// Project 3D edges onto slice plane to form 2D edges
	//		TArray<FUtilEdge2D> Edges2D;
	//		FUtilPoly2DSet PolySet;
	//		FGeomTools::ProjectEdges(Edges2D, PolySet.PolyToWorld, ClipEdges, SlicePlane);

	//		// Find 2D closed polygons from this edge soup
	//		FGeomTools::Buid2DPolysFromEdges(PolySet.Polys, Edges2D, FColor(255, 255, 255, 255));

	//		// Remember start point for vert and index buffer before adding and cap geom
	//		int32 CapVertBase = CapSection.ProcVertexBuffer.Num();
	//		int32 CapIndexBase = CapSection.ProcIndexBuffer.Num();

	//		// Triangulate each poly
	//		for (int32 PolyIdx = 0; PolyIdx < PolySet.Polys.Num(); PolyIdx++)
	//		{
	//			// Generate UVs for the 2D polygon.
	//			FGeomTools::GeneratePlanarTilingPolyUVs(PolySet.Polys[PolyIdx], 64.f);

	//			// Remember start of vert buffer before adding triangles for this poly
	//			int32 PolyVertBase = CapSection.ProcVertexBuffer.Num();

	//			// Transform from 2D poly verts to 3D
	//			Transform2DPolygonTo3D(PolySet.Polys[PolyIdx], PolySet.PolyToWorld, CapSection.ProcVertexBuffer, CapSection.SectionLocalBox);

	//			// Triangulate this polygon
	//			TriangulatePoly(CapSection.ProcIndexBuffer, CapSection.ProcVertexBuffer, PolyVertBase, (FVector3f)LocalPlaneNormal);
	//		}

	//		// Set geom for cap section
	//		InProcMesh->SetProcMeshSection(CapSectionIndex, CapSection);

	//		// If creating new section for cap, assign cap material to it
	//		if (CapOption == EProcMeshSliceCapOption::CreateNewSectionForCap)
	//		{
	//			InProcMesh->SetMaterial(CapSectionIndex, CapMaterial);
	//		}

	//		// If creating the other half, copy cap geom into other half sections
	//		if (bCreateOtherHalf)
	//		{
	//			// Find section we want to use for the cap on the 'other half'
	//			FProcMeshSection* OtherCapSection;
	//			if (CapOption == EProcMeshSliceCapOption::CreateNewSectionForCap)
	//			{
	//				OtherSections.Add(FProcMeshSection());
	//				OtherMaterials.Add(CapMaterial);
	//			}
	//			OtherCapSection = &OtherSections.Last();

	//			// Remember current base index for verts in 'other cap section'
	//			int32 OtherCapVertBase = OtherCapSection->ProcVertexBuffer.Num();

	//			// Copy verts from cap section into other cap section
	//			for (int32 VertIdx = CapVertBase; VertIdx < CapSection.ProcVertexBuffer.Num(); VertIdx++)
	//			{
	//				FProcMeshVertex OtherCapVert = CapSection.ProcVertexBuffer[VertIdx];

	//				// Flip normal and tangent TODO: FlipY?
	//				OtherCapVert.Normal *= -1.f;
	//				OtherCapVert.Tangent.TangentX *= -1.f;

	//				// Add to other cap v buffer
	//				OtherCapSection->ProcVertexBuffer.Add(OtherCapVert);
	//				// And update bounding box
	//				OtherCapSection->SectionLocalBox += OtherCapVert.Position;
	//			}

	//			// Find offset between main cap verts and other cap verts
	//			int32 VertOffset = OtherCapVertBase - CapVertBase;

	//			// Copy indices over as well
	//			for (int32 IndexIdx = CapIndexBase; IndexIdx < CapSection.ProcIndexBuffer.Num(); IndexIdx += 3)
	//			{
	//				// Need to offset and change winding
	//				OtherCapSection->ProcIndexBuffer.Add(CapSection.ProcIndexBuffer[IndexIdx + 0] + VertOffset);
	//				OtherCapSection->ProcIndexBuffer.Add(CapSection.ProcIndexBuffer[IndexIdx + 2] + VertOffset);
	//				OtherCapSection->ProcIndexBuffer.Add(CapSection.ProcIndexBuffer[IndexIdx + 1] + VertOffset);
	//			}
	//		}
	//	}

	//	// Array of sliced collision shapes
	//	TArray< TArray<FVector> > SlicedCollision;
	//	TArray< TArray<FVector> > OtherSlicedCollision;

	//	UBodySetup* ProcMeshBodySetup = InProcMesh->GetBodySetup();

	//	for (int32 ConvexIndex = 0; ConvexIndex < ProcMeshBodySetup->AggGeom.ConvexElems.Num(); ConvexIndex++)
	//	{
	//		FKConvexElem& BaseConvex = ProcMeshBodySetup->AggGeom.ConvexElems[ConvexIndex];

	//		int32 BoxCompare = BoxPlaneCompare(BaseConvex.ElemBox, SlicePlane);

	//		// If box totally clipped, add to other half (if desired)
	//		if (BoxCompare == -1)
	//		{
	//			if (bCreateOtherHalf)
	//			{
	//				OtherSlicedCollision.Add(BaseConvex.VertexData);
	//			}
	//		}
	//		// If box totally valid, just keep mesh as is
	//		else if (BoxCompare == 1)
	//		{
	//			SlicedCollision.Add(BaseConvex.VertexData);				// LWC_TODO: Perf pessimization
	//		}
	//		// Need to actually slice the convex shape
	//		else
	//		{
	//			TArray<FVector> SlicedConvexVerts;
	//			SliceConvexElem(BaseConvex, SlicePlane, SlicedConvexVerts);
	//			// If we got something valid, add it
	//			if (SlicedConvexVerts.Num() >= 4)
	//			{
	//				SlicedCollision.Add(SlicedConvexVerts);
	//			}

	//			// Slice again to get the other half of the collision, if desired
	//			if (bCreateOtherHalf)
	//			{
	//				TArray<FVector> OtherSlicedConvexVerts;
	//				SliceConvexElem(BaseConvex, SlicePlane.Flip(), OtherSlicedConvexVerts);
	//				if (OtherSlicedConvexVerts.Num() >= 4)
	//				{
	//					OtherSlicedCollision.Add(OtherSlicedConvexVerts);
	//				}
	//			}
	//		}
	//	}

	//	// Update collision of proc mesh
	//	InProcMesh->SetCollisionConvexMeshes(SlicedCollision);

	//	// If creating other half, create component now
	//	if (bCreateOtherHalf)
	//	{
	//		// Create new component with the same outer as the proc mesh passed in
	//		OutOtherHalfProcMesh = NewObject<UProceduralMeshComponent>(InProcMesh->GetOuter());

	//		// Set transform to match source component
	//		OutOtherHalfProcMesh->SetWorldTransform(InProcMesh->GetComponentTransform());

	//		// Add each section of geometry
	//		for (int32 SectionIndex = 0; SectionIndex < OtherSections.Num(); SectionIndex++)
	//		{
	//			OutOtherHalfProcMesh->SetProcMeshSection(SectionIndex, OtherSections[SectionIndex]);
	//			OutOtherHalfProcMesh->SetMaterial(SectionIndex, OtherMaterials[SectionIndex]);
	//		}

	//		// Copy collision settings from input mesh
	//		OutOtherHalfProcMesh->SetCollisionProfileName(InProcMesh->GetCollisionProfileName());
	//		OutOtherHalfProcMesh->SetCollisionEnabled(InProcMesh->GetCollisionEnabled());
	//		OutOtherHalfProcMesh->bUseComplexAsSimpleCollision = InProcMesh->bUseComplexAsSimpleCollision;

	//		// Assign sliced collision
	//		OutOtherHalfProcMesh->SetCollisionConvexMeshes(OtherSlicedCollision);

	//		// Finally register
	//		OutOtherHalfProcMesh->RegisterComponent();
	//	}
	//}
}

void AFractureActor01::ClearStaticMesh()
{

}