#pragma once

#ifdef _DEBUG
#pragma comment(lib, "commond.lib")
#else
#pragma comment(lib, "common.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "cored.lib")
#else
#pragma comment(lib, "core.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "shortcut_keysd.lib")
#else
#pragma comment(lib, "shortcut_keys.lib")
#endif