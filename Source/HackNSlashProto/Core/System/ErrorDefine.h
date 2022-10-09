#pragma once

#define CUR_LINE  (FString::FromInt(__LINE__))

#define CUR_FUNCTION_AND_LINE (FString(__FUNCTION__) + "(" + CUR_LINE + ")")

#define CORE_LOG(LogCat, Param1)   UE_LOG(LogCat,Warning,TEXT("%s: %s"), *CUR_FUNCTION_AND_LINE, *FString(Param1))

#define CORE_LOGM(LogCat, FormatString , ...) UE_LOG(LogCat,Warning,TEXT("%s: %s"), *CUR_FUNCTION_AND_LINE, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__ ) )