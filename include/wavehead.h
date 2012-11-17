#ifndef __WAVEHEAD_H__
#define __WAVEHEAD_H__

typedef int SR_DWORD;
typedef short int SR_WORD ;

typedef struct wave_pcm_hdr
        {
                char            riff[4];                        // = "RIFF"
                SR_DWORD        size_8;                         // = FileSize - 8
                char            wave[4];                        // = "WAVE"
                char            fmt[4];                         // = "fmt "
                SR_DWORD        dwFmtSize;                      // = ��һ���ṹ��Ĵ�С : 16

                SR_WORD         format_tag;              // = PCM : 1
                SR_WORD         channels;                       // = ͨ���� : 1
                SR_DWORD        samples_per_sec;        // = ������ : 8000 | 6000 | 11025 | 16000
                SR_DWORD        avg_bytes_per_sec;      // = ÿ���ֽ��� : dwSamplesPerSec * wBitsPerSample / 8
                SR_WORD         block_align;            // = ÿ�������ֽ��� : wBitsPerSample / 8
                SR_WORD         bits_per_sample;         // = ����������: 8 | 16

                char            data[4];                        // = "data";
                SR_DWORD        data_size;                // = �����ݳ��� : FileSize - 44 
        } ;

#endif
