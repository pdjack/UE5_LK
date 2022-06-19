// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(ProjectLK, Log, All);
#define LKLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LKLOG_S(Verbosity) UE_LOG(ProjectLK, Verbosity, TEXT("%s"), *LKLOG_CALLINFO)
#define LKLOG(Verbosity, Format, ...) UE_LOG(ProjectLK, Verbosity, TEXT("%s%s"), *LKLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define LKCHECK(Expr, ...) { if(!(Expr)){ LKLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}


