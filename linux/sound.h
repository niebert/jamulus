/******************************************************************************\
 * Copyright (c) 2004-2008
 *
 * Author(s):
 *  Volker Fischer, Alexander Kurpiers
 *
 * This code is based on the Open-Source sound interface implementation of
 * the Dream DRM Receiver project.
 *
\******************************************************************************/

#if !defined(_SOUND_H__9518A621345F78_3634567_8C0D_EEBF182CF549__INCLUDED_)
#define _SOUND_H__9518A621345F78_3634567_8C0D_EEBF182CF549__INCLUDED_

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include "util.h"
#include "global.h"

#if WITH_SOUND
# define ALSA_PCM_NEW_HW_PARAMS_API
# define ALSA_PCM_NEW_SW_PARAMS_API
# include <alsa/asoundlib.h>
#endif


/* Definitions ****************************************************************/
#define NUM_IN_OUT_CHANNELS         2 // always stereo

// the number of periods is critical for latency
#define NUM_PERIOD_BLOCKS_IN        2
#define NUM_PERIOD_BLOCKS_OUT       2

#define MAX_SND_BUF_IN              200
#define MAX_SND_BUF_OUT             200

/* Classes ********************************************************************/
class CSound
{
public:
    CSound ( const int iNewBufferSizeStereo )
#if WITH_SOUND
    : rhandle ( NULL ), phandle ( NULL ), iCurPeriodSizeIn ( NUM_PERIOD_BLOCKS_IN ),
    iCurPeriodSizeOut ( NUM_PERIOD_BLOCKS_OUT ), bChangParamIn ( true ),
    bChangParamOut ( true )
#endif
    {
        // set internal buffer size for read and write
        iBufferSizeIn  = iNewBufferSizeStereo / NUM_IN_OUT_CHANNELS; // mono size
        iBufferSizeOut = iNewBufferSizeStereo / NUM_IN_OUT_CHANNELS; // mono size
    }
    virtual ~CSound() { Close(); }

    // not implemented yet, always return one device and default string
    int         GetNumDev() { return 1; }
    std::string GetDeviceName ( const int iDiD ) { return "wave mapper"; }
    int         SetDev ( const int iNewDev ) {} // dummy
    int         GetDev() { return 0; }

#if WITH_SOUND
    void    SetInNumBuf ( int iNewNum );
    int     GetInNumBuf() { return iCurPeriodSizeIn; }
    void    SetOutNumBuf ( int iNewNum );
    int     GetOutNumBuf() { return iCurPeriodSizeOut; }
    void    InitRecording ( const bool bNewBlocking = true );
    void    InitPlayback ( const bool bNewBlocking = false );
    bool    Read ( CVector<short>& psData );
    bool    Write ( CVector<short>& psData );

    void    Close();
    
protected:
    snd_pcm_t* rhandle;
    snd_pcm_t* phandle;

    bool SetHWParams ( snd_pcm_t* handle, const int iBufferSizeIn,
                       const int iNumPeriodBlocks );

    int iBufferSizeOut;
    int iBufferSizeIn;
    bool bChangParamIn;
    int iCurPeriodSizeIn;
    bool bChangParamOut;
    int iCurPeriodSizeOut;
#else
    // dummy definitions
    void    SetInNumBuf ( int iNewNum ) {}
    int     GetInNumBuf() { return 1; }
    void    SetOutNumBuf ( int iNewNum ) {}
    int     GetOutNumBuf() { return 1; }
    void    InitRecording ( const bool bNewBlocking = true ) { printf ( "no sound!" ); }
    void    InitPlayback ( const bool bNewBlocking = false ) { printf ( "no sound!" ); }
    bool    Read ( CVector<short>& psData ) { printf ( "no sound!" ); return false; }
    bool    Write ( CVector<short>& psData ) { printf ( "no sound!" ); return false; }
    void    Close() {}
#endif
};


#endif // !defined(_SOUND_H__9518A621345F78_3634567_8C0D_EEBF182CF549__INCLUDED_)
