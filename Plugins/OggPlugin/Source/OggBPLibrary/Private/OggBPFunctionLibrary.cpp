// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.
#include "OggBPLibraryPrivatePCH.h"

//////////////////////////////////////////////////////////////////////////
// UOggBPFunctionLibrary

UOggBPFunctionLibrary::UOggBPFunctionLibrary(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	 
}

class UAudioComponent* UOggBPFunctionLibrary::PlaySoundAttachedFromFile(const FString& FilePath, class USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings)
{	
	USoundWave* sw = GetSoundWaveFromFile(FilePath);

	if (!sw)
		return NULL;

	return UGameplayStatics::PlaySoundAttached(sw, AttachToComponent, AttachPointName, Location, LocationType, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings);
}

void UOggBPFunctionLibrary::PlaySoundAtLocationFromFile(UObject* WorldContextObject, const FString& FilePath, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings)
{
	USoundWave* sw = GetSoundWaveFromFile(FilePath);

	if (!sw)
		return;

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sw, Location, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings);
}

class USoundWave* UOggBPFunctionLibrary::GetSoundWaveFromFile(const FString& FilePath)
{
	USoundWave* sw = (USoundWave*)StaticConstructObject(USoundWave::StaticClass());

	if (!sw)
		return NULL;

	//* If true the song was successfully loaded
	bool loaded = false;

	//* loaded song file (binary, encoded)
	TArray < uint8 > rawFile;

	loaded = FFileHelper::LoadFileToArray(rawFile, FilePath.GetCharArray().GetTypedData());

	if (loaded)
	{
		FByteBulkData* bulkData = &sw->CompressedFormatData.GetFormat(TEXT("OGG"));

		bulkData->Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(bulkData->Realloc(rawFile.Num()), rawFile.GetTypedData(), rawFile.Num());
		bulkData->Unlock();

		loaded = fillSoundWaveInfo(sw, &rawFile) == 0 ? true : false;
	}

	if (!loaded)
		return NULL;

	return sw;
}

int UOggBPFunctionLibrary::fillSoundWaveInfo(class USoundWave* sw, TArray<uint8>* rawFile)
{
    FSoundQualityInfo info;
    FVorbisAudioInfo vorbis_obj = FVorbisAudioInfo();
    if (!vorbis_obj.ReadCompressedInfo(rawFile->GetTypedData(), rawFile->Num(), &info))
    {
        //Debug("Can't load header");
        return 1;
    }

	sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
    sw->NumChannels = info.NumChannels;
    sw->Duration = info.Duration;
    sw->RawPCMDataSize = info.SampleDataSize;
    sw->SampleRate = info.SampleRate;

    return 0;
}

      
int UOggBPFunctionLibrary::findSource(class USoundWave* sw, class FSoundSource* out_audioSource)
{
	FAudioDevice* device = GEngine ? GEngine->GetAudioDevice() : NULL; //gently ask for the audio device

	FActiveSound* activeSound;
	FSoundSource* audioSource;
	FWaveInstance* sw_instance;
	if (!device)
	{
		activeSound = NULL;
		audioSource = NULL;
		out_audioSource = audioSource;
		return -1;
	}

	TArray<FActiveSound*> tmpActualSounds = device->GetActiveSounds();
	if (tmpActualSounds.Num())
	{
		for (auto activeSoundIt(tmpActualSounds.CreateIterator()); activeSoundIt; ++activeSoundIt)
		{
			activeSound = *activeSoundIt;
			for (auto WaveInstanceIt(activeSound->WaveInstances.CreateIterator()); WaveInstanceIt; ++WaveInstanceIt)
			{
				sw_instance = WaveInstanceIt.Value();
				if (sw_instance->WaveData->CompressedDataGuid == sw->CompressedDataGuid)
				{
					audioSource = device->WaveInstanceSourceMap.FindRef(sw_instance);
					out_audioSource = audioSource;
					return 0;
				}
			}
		}
	}

	audioSource = NULL;
	activeSound = NULL;
	out_audioSource = audioSource;
	return -2;
}