// CopyRight HenryChen


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	Pt_1 = CreateDefaultSubobject<USceneComponent>("Pt_1");
	Pt_2 = CreateDefaultSubobject<USceneComponent>("Pt_2");
	Pt_3 = CreateDefaultSubobject<USceneComponent>("Pt_3");
	Pt_4 = CreateDefaultSubobject<USceneComponent>("Pt_4");
	Pt_5 = CreateDefaultSubobject<USceneComponent>("Pt_5");
	Pt_6 = CreateDefaultSubobject<USceneComponent>("Pt_6");
	Pt_7 = CreateDefaultSubobject<USceneComponent>("Pt_7");
	Pt_8 = CreateDefaultSubobject<USceneComponent>("Pt_8");
	Pt_9 = CreateDefaultSubobject<USceneComponent>("Pt_9");
	Pt_10 = CreateDefaultSubobject<USceneComponent>("Pt_10");

	ImmutablePts = {
		Pt_0,
		Pt_1,
		Pt_2,
		Pt_3,
		Pt_4,
		Pt_5,
		Pt_6,
		Pt_7,
		Pt_8,
		Pt_9,
		Pt_10
	};

	SetRootComponent(Pt_0);
	for(int i = 1; i < ImmutablePts.Num(); ++i)
	{
		ImmutablePts[i]->SetupAttachment(RootComponent);
	}
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("Attempted to access ImmutablePts out of bounds."));

	TArray<USceneComponent*> ArrayCopy;

	for(USceneComponent* Pt : ImmutablePts)
	{
		if(ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if(Pt != Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);

		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();

}