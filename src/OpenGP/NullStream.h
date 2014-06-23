#pragma once
#include <ostream>

/// @brief This class allows to discard stream output, similarly to sending output
/// to "/dev/null" while remaining portable. See apps/isoremesh for example usage
class NullStream : public std::ostream { 
private:
    class NullBuffer : public std::streambuf{
    public:
        int overflow(int c) { return c; }
    } m_sb;    
public: 
    NullStream() : std::ostream(&m_sb) {} 
};
