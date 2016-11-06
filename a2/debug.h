#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) printf("DEBUG: line %d: " M "\n", __LINE__, ##__VA_ARGS__)
#endif