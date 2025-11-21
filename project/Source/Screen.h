#pragma once

/// <summary>
/// Window‚Ì’è‹`‚ğ‚µ‚Ü‚·
/// </summary>
namespace Screen
{
	static int WIDTH = 0;
	static int HEIGHT = 0;
	static const BOOL WINDOW_MODE = FALSE;
	static const char* WINDOW_NAME = "project";
	static const float WINDOW_EXTEND = 1.0f;

	// ‰ğ‘œ“xæ“¾ŠÖ”
	inline void InitScreenSize()
	{
		WIDTH = GetSystemMetrics(SM_CXSCREEN);  // ‰¡•
		HEIGHT = GetSystemMetrics(SM_CYSCREEN); // ‚‚³
	}
};
