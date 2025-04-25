// CopyRight HenryChen

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Templates/SubclassOf.h"
#include "VM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

class UVM_LoadSlot;
/**
 *
 */
UCLASS()
class AURA_API UVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<int32, UVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UVM_LoadSlot> LoadSlot_2;

	UPROPERTY()
	TObjectPtr<UVM_LoadSlot> SelectedSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"));
	int32 NumLoadSlots;
public:
	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVM_LoadSlot> LoadSlotViewModelClass;

	void InitializeLoadSlots();

	UFUNCTION(BlueprintPure)
	UVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);

	void LoadData();

	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();

	void SetNumLoadSlots(int32 InNumLoadSlots);

	int32 GetNumLoadSlots() const {
		return NumLoadSlots;
	}
};
