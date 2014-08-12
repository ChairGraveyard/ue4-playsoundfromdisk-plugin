// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Components/AudioComponent.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "ActiveSound.h"
#include "Audio.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"

#include "OggBPFunctionLibrary.generated.h"

// BP Library for playing sound files from disk at runtime.
//
// Written by n00854180t

UCLASS()
class OGGBPLIBRARY_API UOggBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/** Plays a sound from file, attached to and following the specified component. This is a fire and forget sound. Replication is also not handled at this point.
	* @param FilePath - Path to sound file to play
	* @param AttachComponent - Component to attach to.
	* @param AttachPointName - Optional named point within the AttachComponent to play the sound at
	* @param Location - Depending on the value of Location Type this is either a relative offset from the attach component/point or an absolute world position that will be translated to a relative offset
	* @param LocationType - Specifies whether Location is a relative offset or an absolute world position
	* @param bStopWhenAttachedToDestroyed - Specifies whether the sound should stop playing when the owner of the attach to component is destroyed.
	* @param VolumeMultiplier - Volume multiplier
	* @param PitchMultiplier - PitchMultiplier
	* @param AttenuationSettings - Override attenuation settings package to play sound with
	*/ 
	UFUNCTION(BlueprintCallable, Category = "OggBPLibrary", meta = (VolumeMultiplier = "1.0", PitchMultiplier = "1.0", AdvancedDisplay = "2", UnsafeDuringActorConstruction = "true"))
	static class UAudioComponent* PlaySoundAttachedFromFile(const FString& FilePath, class USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector(ForceInit), EAttachLocation::Type LocationType = EAttachLocation::SnapToTarget, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, class USoundAttenuation* AttenuationSettings = NULL);
	
	/** Plays a sound at the given location. This is a fire and forget sound and does not travel with any actor. Replication is also not handled at this point.
	* @param FilePath - Path to sound file to play
	* @param Location - World position to play sound at
	* @param World - The World in which the sound is to be played
	* @param VolumeMultiplier - Volume multiplier
	* @param PitchMultiplier - PitchMultiplier
	* @param AttenuationSettings - Override attenuation settings package to play sound with
	*/
	UFUNCTION(BlueprintCallable, Category = "OggBPLibrary", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", VolumeMultiplier = "1.0", PitchMultiplier = "1.0", AdvancedDisplay = "3", UnsafeDuringActorConstruction = "true"))
	static void PlaySoundAtLocationFromFile(UObject* WorldContextObject, const FString& FilePath, FVector Location, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, class USoundAttenuation* AttenuationSettings = NULL);
	
	/** Creates a USoundWave* from file path.
	* Read .ogg header file and refresh USoundWave metadata.
	* @param FilePath		path to file to create sound wave from
	*/
	UFUNCTION(BlueprintCallable, Category = "OggBPLibrary")
	static class USoundWave* GetSoundWaveFromFile(const FString& FilePath);

private:
	// Thanks to @keru for the base code for loading an Ogg into a USoundWave: 
	// https://forums.unrealengine.com/showthread.php?7936-Custom-Music-Player&p=97659&viewfull=1#post97659

	     /**
        * Read .ogg header file and refresh USoundWave metadata.
        * @param sw             wave to put metadata
        * @param rawFile        pointer to src file in memory
        * @return 0     if everything is ok
        *                 1 if couldn't read metadata.
        */
        static int fillSoundWaveInfo(USoundWave* sw, TArray<uint8>* rawFile);



        /**
        * Tries to find out FSoundSource object associated to the USoundWave.
        * @param sw     wave, search key
        * @return 0 if wave found and correctly set
        *        -1 if error: sound device not set
        *        -2 if error: sound wave not found
        */
		static int findSource(class USoundWave* sw, class FSoundSource* out_audioSource);
};

