﻿// Copyright Epic Games, Inc. All Rights Reserved.

#include "Importers/CurveFloatImporter.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Dom/JsonObject.h"
#include "Factories/CurveFactory.h"
#include "Utilities/AssetUtilities.h"

bool UCurveFloatImporter::ImportData() {
	try {
		// Quick way to access the curve keys
		TArray<TSharedPtr<FJsonValue>> Keys = JsonObject->GetObjectField("Properties")->GetObjectField("FloatCurve")->GetArrayField("Keys");

		UCurveFloatFactory* CurveFactory = NewObject<UCurveFloatFactory>();
		UCurveFloat* CurveAsset = Cast<UCurveFloat>(CurveFactory->FactoryCreateNew(UCurveFloat::StaticClass(), OutermostPkg, *FileName, RF_Standalone | RF_Public, nullptr, GWarn));

		// Add Keys
		for (int32 i = 0; i < Keys.Num(); i++)
			CurveAsset->FloatCurve.Keys.Add(FAssetUtilities::ObjectToRichCurveKey(Keys[i]->AsObject()));

		// Handle edit changes, and add it to the content browser
		if (!HandleAssetCreation(CurveAsset)) return false;
	} catch (const char* Exception) {
		UE_LOG(LogJson, Error, TEXT("%s"), *FString(Exception));
		return false;
	}

	return true;
}