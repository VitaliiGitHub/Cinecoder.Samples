#pragma once

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

#include "Cinecoder_h.h"

struct TEST_PARAMS
{
	LPCSTR						CompanyName;
	LPCSTR						LicenseKey;

	LPCTSTR						InputFileName;
	BOOL						SetOfFiles;
	int							FileSize;
	int							DataOffset;		

	LPCTSTR						OutputFileName;

	int							Width, Height;
	int							FrameRateN, FrameRateD;
	CC_COLOR_FMT				InputColorFormat;
	int							InputPitch;
	CC_PICTURE_ORIENTATION		PictureOrientation;
	int							StartFrameNum, StopFrameNum;
	bool						Looped;

	int							NumReadThreads;
	int							QueueSize;
	bool						UseCache;

	CC_DANIEL2_CODING_METHOD	CodingMethod;
	CC_CHROMA_FORMAT			ChromaFormat;
	int							BitDepth;
	CC_BITRATE_MODE				BitrateMode;
	CC_BITRATE					Bitrate;
	FLOAT						QuantScale;
	int							NumSingleEncoders;
	int							DeviceId;
};

struct	ENCODER_STATS
{
	LONG						NumFramesRead;
	LONG						NumFramesWritten;
	LONGLONG					NumBytesRead;
	LONGLONG					NumBytesWritten;
};

//---------------------------------------------------------------
class CEncoderTest
//---------------------------------------------------------------
{
public:
	CEncoderTest();
	~CEncoderTest();

	int		AssignParameters(const TEST_PARAMS&);
	int		Close();

	int		Run();
	int		Cancel();
	bool	IsActive() const;
	HRESULT	GetResult() const;

	int		GetCurrentEncodingStats(ENCODER_STATS*);

private:
	static int		CheckParameters(const TEST_PARAMS &par);

	int		CreateEncoder(const TEST_PARAMS&);

	TEST_PARAMS	m_EncPar{};
	com_ptr<ICC_VideoEncoder> m_pEncoder;

	com_ptr<ICC_ClassFactory> m_pFactory;
	com_ptr<ICC_Multiplexer> m_pMuxer;
	
	DWORD m_FrameSizeInBytes{};

	BOOL m_bRunning;
	HRESULT	m_hrResult{};

	std::atomic<LONG> m_NumActiveThreads{};
	std::atomic<LONG> m_ReadFrameCounter{};

	std::vector<std::thread> m_ReadingThreads;
	DWORD	ReadingThreadProc(int n);
	static	DWORD	reading_thread_proc(void *p, int n);

	std::thread	m_EncodingThread;
	DWORD	EncodingThreadProc();
	static	DWORD	encoding_thread_proc(void *p);

	HANDLE	m_evCancel{};

	struct BufferDescr
	{
		LPBYTE	pBuffer;
		HANDLE	evVacant;
		HANDLE	evFilled;
		HRESULT	hrReadStatus;
	};
	std::vector<BufferDescr> m_Queue;

	std::mutex m_StatsLock;
	ENCODER_STATS	m_Stats{};
};
