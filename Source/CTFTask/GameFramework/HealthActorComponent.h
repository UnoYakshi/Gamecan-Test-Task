// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthActorComponent.generated.h"


/** Adding an ability to assign delegates that will be called when the component's owner is dead... */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathSignature, AActor*, DeadActor);


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CTFTASK_API UHealthActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthActorComponent();

	/** Gets replication properties, i.e., Health in our case... */
	virtual void GetLifetimeReplicatedProps(TArray < class FLifetimeProperty > & OutLifetimeProps) const override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
protected:
    /** Current Health value... */
    UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Health, Category = "Game|Health")
    float Health;

	/** Maximum value of Health... */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Health")
    float MaxHealth;

	/** States has Death happened or has not... */
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_IsDead, Category = "Game|Health")
    bool bIsDead;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Checks if Death happened, i.e., 0 HP reached... */
    virtual void CheckDeath();
	

public:
    UPROPERTY(BlueprintAssignable, Category = "Game|Health")
    FDeathSignature OnDeath;

	/** Sets new current value of Health... */
	UFUNCTION(BlueprintCallable, Category = "Game|Health")
    void SetHealthValue(const float NewHealthValue);

	UFUNCTION(Server, Reliable, WithValidation)
    void Server_SetHealthValue(const float NewHealthValue);

	/** Decreases current Health's value... */
	UFUNCTION(BlueprintCallable, Category = "Game|Health")
    void DecreaseHealthValue(const float DecreaseValue);

	/** Increases current Health's value... */
	UFUNCTION(BlueprintCallable, Category = "Game|Health")
    void IncreaseHealthValue(const float IncreaseValue);

    /** Called when Health is <= 0.. */
	UFUNCTION(BlueprintCallable)
    void Die();

	/** Sets values to "alive" condition and sets Health to given level... */
	UFUNCTION(BlueprintCallable, Category = "Game|Health")
    void BringToLife(const float NewHealthValue);

public:
    /** Gets current Health's value... */
    UFUNCTION(BlueprintGetter, Category = "Game|Health")
    float GetHealth() const { return Health; }

	/** Gets maximum health... */
	UFUNCTION(BlueprintGetter, Category = "Game|Health")
    float GetMaxHealth() const { return MaxHealth; }
	
	/** Tells if dead... */
	UFUNCTION(BlueprintGetter, Category = "Game|Health")
    bool IsDead() const { return bIsDead; }

	/** Tells if alive... */
	UFUNCTION(BlueprintGetter, Category = "Game|Health")
    bool IsAlive() const { return !bIsDead; }

	
private:
    UFUNCTION()
    void OnRep_Health();

	UFUNCTION()
    void OnRep_IsDead();
};
