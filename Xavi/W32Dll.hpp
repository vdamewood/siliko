#if !defined XAVI_W32_DLL

#if defined _WIN32 && defined _MSC_VER
#  if defined Xavi_EXPORTS
#    define API __declspec(dllexport)
#    define EXT
#  else Xavi_EXPORTS
#    define API __declspec(dllimport)
#    define EXT extern
#  endif // Xavi_EXPORTS
#else // _WIN32 && __MSC_VER
#  define API
#  define EXT
#endif // _WIN32 && __MSC_VER

#endif // XAVI_W32_DLL
