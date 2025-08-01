// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::Init()
{
	Super::Init();
}

void UMyGameInstance::SetMusicVolume(float Volume)
{
	MusicVolume = Volume;
	if (CurrentAudioComponent != nullptr)
	{
		CurrentAudioComponent->AdjustVolume(1, MusicVolume);
	}
}

void UMyGameInstance::PlayMainMenuMusic()
{
	PlayMusicTrack(MainMenuMusicTrack);
}

void UMyGameInstance::PlayMusic()
{
	if (CurrentAudioComponent == nullptr)
	{
		GoToNextMusicTrack();
	}
	else
	{
		CurrentAudioComponent->SetPaused(false);
	}
}

void UMyGameInstance::PlayMusicTrack(USoundBase* Track)
{
	if (Track == nullptr)
	{
		return;
	}
	
	if (CurrentAudioComponent != nullptr)
	{
		CurrentAudioComponent->Stop();
		CurrentAudioComponent->OnAudioFinished.Remove(TrackFinished);
	}
	float volume = MusicVolume * Track->GetVolumeMultiplier();
	CurrentAudioComponent = UGameplayStatics::CreateSound2D(GetWorld(), Track,
		volume, 1, 0.0, nullptr, true);
	TrackFinished.BindUFunction(this, "GoToNextMusicTrack");
	if (CurrentAudioComponent == nullptr)
	{
		return;
	}
	CurrentAudioComponent->OnAudioFinished.Add(TrackFinished);

	CurrentAudioComponent->Activate();
	CurrentAudioComponent->Play(0.0);
}

void UMyGameInstance::PauseMusic()
{
	if (CurrentAudioComponent == nullptr)
	{
		return;
	}
	CurrentAudioComponent->SetPaused(true);
}

void UMyGameInstance::GoToNextMusicTrack()
{
	PlayMusicTrack(GetNextMusicTrack());
}

USoundBase* UMyGameInstance::GetNextMusicTrack()
{
	int newTrackIndex = FMath::RandRange(0, InGameMusicTracks.Num() - 1);
	if (newTrackIndex < 0)
	{
		return nullptr;
	}
	return InGameMusicTracks[newTrackIndex];
}
