#pragma once

namespace ImGui
{
	inline const char* ToConstCharPtr(const FString& String)
	{
		return TCHAR_TO_ANSI(*String);
	}

	inline const char* ToConstCharPtr(const FName& Name)
	{
		return ToConstCharPtr(Name.ToString());
	}
}
