#if !defined(__Util_hdr__)
#define __Util_hdr__

#include <cassert>
#include <cstring>
#include <limits>

class CUtil
{
public:
    template<typename T>
    static T float2int (float fInput)
    {
        if (fInput >= 0.F)
            return static_cast<T>(fInput + .5F);
        else
            return static_cast<T>(fInput - .5F);
    }
    template<typename T>
    static T double2int (double fInput)
    {
        if (fInput >= 0)
            return static_cast<T>(fInput + .5);
        else
            return static_cast<T>(fInput - .5);
    }

    static bool isPowOf2 (int n) 
    {
        return !(n & (n-1));
    }

    static int nextPowOf2(int n)
    {
        int iOrder = 0;

        if (n == 0)
            return 0;

        while (n>>iOrder)
            iOrder++;

        if (!(n%(1<<(iOrder-1))))
            iOrder--;

        return (1<<(iOrder));
    }
};
#endif // __Util_hdr__