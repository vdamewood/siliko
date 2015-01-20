#if !defined XAVI_W32_DLL

#if defined _WIN32 && defined _MSC_VER
#  if defined Xavi_EXPORTS
#    define XAVI_API __declspec(dllexport)
#    define XAVI_EXTERN
#  else // Xavi_EXPORTS
#    define XAVI_API __declspec(dllimport)
#    define XAVI_EXTERN extern
#  endif // Xavi_EXPORTS
#else // _WIN32 && __MSC_VER
#  define XAVI_API
#  define XAVI_EXTERN
#endif // _WIN32 && __MSC_VER

#endif // XAVI_W32_DLL
