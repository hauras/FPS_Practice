

#include "Controller/FPSController.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/PlayerCharacter.h"
#include "InputActionValue.h"

AFPSController::AFPSController()
{
    bReplicates = true;

}

void AFPSController::BeginPlay()
{
	Super::BeginPlay();
	check(InputContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(InputContext, 0);
}

void AFPSController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSController::Move);
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSController::Look);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFPSController::Jump);

}

void AFPSController::Move(const FInputActionValue& Value)
{
    const FVector2D InputAxisVector = Value.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}

void AFPSController::Look(const FInputActionValue& Value)
{
    APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetPawn());
    if (PlayerChar)
    {
        PlayerChar->Look(Value);
    }
}

void AFPSController::Jump()
{
    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        ACharacter* MyCharacter = Cast<ACharacter>(ControlledPawn);
        if (MyCharacter)
        {
            MyCharacter->Jump();
        }
    }
}
