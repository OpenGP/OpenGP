#pragma once

/// HEADERONLY_INLINE is either nothing or the keyword "inline"
#ifdef DISABLE_HEADERONLY
    #undef HEADERONLY
    #undef HEADERONLY_INLINE
    #define HEADERONLY_INLINE
#else
    #undef HEADERONLY
    #define HEADERONLY
    #undef HEADERONLY_INLINE
    #define HEADERONLY_INLINE inline
#endif
