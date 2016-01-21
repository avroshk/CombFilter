#if !defined(__VectorFloat_hdr__)
#define __VectorFloat_hdr__

#include <cassert>
#include <cstring>
#include <limits>
#include <cmath>

class CVector
{
public:
    template<typename T>
    static void setZero (T *pSrcDest, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);

        if (iLength > 0)
            memset (pSrcDest, 0, sizeof(T)*iLength);
    }
    template<typename T>
    static void setZeroBelowThresh (T *pSrcDest, int iLength, T Thresh)
    {
        assert (iLength >= 0);
        assert (pSrcDest);

        for (int i = 0; i < iLength; i++)
            if (pSrcDest[i] < Thresh)
                pSrcDest[i] = 0;
    }
    template<typename T>
    static void copy(T *pDest, const T *pSource, int iLength)
    {
        assert(iLength >= 0);

        if (iLength > 0)
        {
            assert(pDest);
            assert(pSource);
            memcpy(pDest, pSource, sizeof(T)*iLength);
        }
    }
    template<typename T>
    static void flip_I(T *pSrcDest, int iLength)
    {
        assert(iLength >= 0);

        if (iLength > 0)
        {
            assert(pSrcDest);

            int iLoopLength = iLength / 2; // integer division!
            for (int i = 0; i < iLoopLength; i++)
            {
                T Tmp                       = pSrcDest[i];
                pSrcDest[i]                 = pSrcDest[iLength - 1 - i];
                pSrcDest[iLength - 1 - i]   = Tmp;
            }
        }
    }
    template<typename T>
    static void moveInMem (T *pSrcDest, int iDestIdx, int iSrcIdx, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);

        if (iLength > 0)
            memmove (&pSrcDest[iDestIdx], &pSrcDest[iSrcIdx], sizeof(T)*iLength);
    }
};
class CVectorFloat
{
public:

    static void setZero (float *pfSrcDest, int iLength)
    {
        CVector::setZero(pfSrcDest, iLength);
    }
    static void setZeroBelowThresh (float *pfSrcDest, int iLength, float fThresh)
    {
        CVector::setZeroBelowThresh(pfSrcDest, iLength, fThresh);
    }
    static void copy(float *pfDest, const float *pfSource, int iLength)
    {
        CVector::copy(pfDest, pfSource, iLength);
    }
    static void flip_I(float *pfSrcDest, int iLength)
    {
        CVector::flip_I(pfSrcDest, iLength);
    }
    static void moveInMem (float *pfSrcDest, int iDestIdx, int iSrcIdx, int iLength)
    {
        CVector::moveInMem(pfSrcDest, iDestIdx, iSrcIdx, iLength);
    }

    static void setValue (float *pDest, float fValue, int iLength)
    {
        assert (iLength >= 0);
        assert (pDest);

        for (int i = 0; i < iLength; i++)
            pDest[i] = fValue;
    }
    static void mulC_I (float *pSrcDest, float fScale, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);
        
        for (int i = 0; i < iLength; i++)
            pSrcDest[i] *= fScale;
    }

    static void mul_I (float *pSrcDest, const float *pSrc, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);
        assert (pSrc);

        for (int i = 0; i < iLength; i++)
            pSrcDest[i] *= pSrc[i];
    }

    static float mulScalar (const float *pSrc1, const float *pSrc2, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrc1);
        assert (pSrc2);
        float  fResult = 0;

        for (int i = 0; i < iLength; i++)
            fResult += pSrc1[i] * pSrc2[i];

        return fResult;
    }

    static void div_I (float *pSrcDest, const float *pSrc, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);
        assert (pSrc);

        for (int i = 0; i < iLength; i++)
        {
            assert(pSrc[i] != 0);
            pSrcDest[i] /= pSrc[i];
        }
    }

    static void add_I (float *pSrcDest, const float *pSrc, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);
        assert (pSrc);

        for (int i = 0; i < iLength; i++)
            pSrcDest[i] += pSrc[i];
    }
    static void addC_I (float *pSrcDest, float fScale, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);

        for (int i = 0; i < iLength; i++)
            pSrcDest[i] += fScale;
    }

    static void sub_I (float *pSrcDest, const float *pSrc, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);
        assert (pSrc);

        for (int i = 0; i < iLength; i++)
            pSrcDest[i] -= pSrc[i];
    }

    static float sum (const float *pSrc, int iLength, bool bAbs = false)
    {
        assert (iLength >= 0);
        assert (pSrc);

        float fResult = 0;
        if (bAbs)
        {
            for (int i = 0; i < iLength; i++)
                fResult += std::abs(pSrc[i]);
        }
        else
        {
            for (int i = 0; i < iLength; i++)
                fResult += pSrc[i];
        }
        return fResult;
    }

    static bool isEqual (const float *pSrc1, const float *pSrc2, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrc1);
        assert (pSrc2);

        return (memcmp (pSrc1, pSrc2, iLength * sizeof(float)) == 0);
    }
    static float getMean (float *pfSrc, long long int iLength)
    {
        assert (iLength >= 0);

        float fMean = 0;

        for (int i=0; i < iLength; i++)
        {
            fMean  += pfSrc[i];
        }

        if (iLength > 0)
        {
            fMean  /= iLength;
        }

        return fMean;
    }
    static float getStd (float *pfSrc, long long int iLength, float fMean = std::numeric_limits<float>::max())
    {
        assert (iLength >= 0);

        float  fStd = 0;

        if (fMean == std::numeric_limits<float>::max())
        {
            fMean   = getMean(pfSrc, iLength);
        }

        for (int i=0; i < iLength; i++)
        {
            fStd   += (pfSrc[i] - fMean) * (pfSrc[i] - fMean);
        }

        if (iLength > 1)
        {
            //dStd   /= (iLength - 1);
            fStd   /= iLength;
        }

        return std::sqrt(fStd);
    }
    static float getRms (float *pfSrc, long long int iLength)
    {
        assert (iLength >= 0);

        float fRms = 0;


        for (int i=0; i < iLength; i++)
        {
            fRms   += pfSrc[i] * pfSrc[i];
        }

        if (iLength > 0)
        {
            fRms   /= iLength;
        }

        return std::sqrt(fRms);
    }
    static float getMax (float *pfSrc, long long int iLength, bool bAbs = false)
    {
        float fMax;
        long long iMax;

        findMax(pfSrc, fMax, iMax, iLength, bAbs);

        return fMax;
    }
    static float getMin (const float *pfSrc, long long int iLength, bool bAbs = false)
    {
        float fMin;
        long long iMin;

        findMin(pfSrc, fMin, iMin, iLength, bAbs);

        return fMin;
    }
    static void findMax (float *pfSrc, float &fMax, long long &iMax, long long int iLength, bool bAbs = false)
    {
        assert (iLength >= 0);
        assert (pfSrc);

        fMax    = -std::numeric_limits<float>::max();
        iMax    = -1;

        for (int i = 0; i < iLength; i++)
        {
            float fCurr   = (bAbs)? std::abs(pfSrc[i]) : pfSrc[i];

            if (fCurr > fMax)
            {
                fMax = fCurr;
                iMax = i;
            }
        }
    }
    static void findMin (const float *pfSrc, float &fMin, long long &iMin, long long int iLength, bool bAbs = false)
    {
        assert (iLength >= 0);
        assert (pfSrc);

        fMin    = std::numeric_limits<float>::max();
        iMin    = -1;

        for (int i = 0; i < iLength; i++)
        {
            float fCurr   = (bAbs)? std::abs(pfSrc[i]) : pfSrc[i];

            if (fCurr < fMin)
            {
                fMin    = fCurr;
                iMin    = i;
            }
        }
    }
};
#endif // __VectorFloat_hdr__