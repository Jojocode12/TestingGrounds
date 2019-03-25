// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "Casts.h"
#include "TP_ThirdPerson/TP_ThirdPerson.h"
#include "NPC/PatrolRoute.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{

	/// Get the patrol points
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();

	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Guard is missing patrol points"));
		return EBTNodeResult::Failed;
	}


	/// Set Next Waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	auto PatrolPoint = PatrolPoints[Index];
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoint);

	//Cycle index
	auto ArrayLength = PatrolPoints.Num();
	Index = (Index + 1)%ArrayLength;
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index);

	return EBTNodeResult::Succeeded;
}


