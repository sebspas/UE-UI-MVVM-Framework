#include "View.h"

bool UView::Initialize()
{
	// 
	
	
	return UUserWidget::Initialize();
}

void UView::RemoveFromParent()
{
	UUserWidget::RemoveFromParent();
}

auto
	UView::
	GetViewModelsRegistered() const
	-> const TArray<UViewModel*>&
{
	return ViewModels;
}
