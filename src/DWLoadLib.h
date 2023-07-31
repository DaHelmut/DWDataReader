#ifndef DWLOADLIB__H
#define DWLOADLIB__H

#ifdef _WIN32
    #include <windows.h>
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <dlfcn.h>

    #define __int64 int64_t
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum DWStatus					// status returned from library function calls
{
    DWSTAT_OK = 0,									// status OK
    DWSTAT_ERROR = 1, 								// error occurred in the dll
    DWSTAT_ERROR_FILE_CANNOT_OPEN = 2,				// unable to open file
    DWSTAT_ERROR_FILE_ALREADY_IN_USE = 3,			// file already in use
    DWSTAT_ERROR_FILE_CORRUPT = 4, 					// file corrupted
    DWSTAT_ERROR_NO_MEMORY_ALLOC = 5,				// memory not allocated
    DWSTAT_ERROR_CREATE_DEST_FILE = 6,				// error creating destination file (only for d7z files)
    DWSTAT_ERROR_EXTRACTING_FILE = 7,				// error extracting data (only for d7z files)
    DWSTAT_ERROR_CANNOT_OPEN_EXTRACTED_FILE = 8,		// error opening extracted file (only for d7z files)
    DWSTAT_ERROR_INVALID_IB_LEVEL = 9

};

// channel properties
enum DWChannelProps				// used for DWGetChannelProps() calls
{
    DW_DATA_TYPE = 0,	     	// get data type
    DW_DATA_TYPE_LEN_BYTES = 1,	// get length of data type in bytes
    DW_CH_INDEX = 2,			// get channel index
    DW_CH_INDEX_LEN = 3,		// get length of channel index
    DW_CH_TYPE = 4,				// get channel type
    DW_CH_SCALE = 5,			// get channel scale
    DW_CH_OFFSET = 6,			// get channel offset
    DW_CH_XML = 7,				// get channel XML
    DW_CH_XML_LEN = 8,			// get length of channel XML
    DW_CH_XMLPROPS = 9,			// get channel XML properties
    DW_CH_XMLPROPS_LEN = 10,	// get length of channel XML properties
    DW_CH_CUSTOMPROPS = 11,		// get channel XML custom properties
    DW_CH_CUSTOMPROPS_COUNT = 12,	// get length of channel XML custom properties
    DW_CH_LONGNAME = 13,        // get channel long name
    DW_CH_LONGNAME_LEN = 14     // get length of channel long name
};

enum DWChannelType				// channel type
{
    DW_CH_TYPE_SYNC = 0,		// sync channel
    DW_CH_TYPE_ASYNC = 1,		// async channel
    DW_CH_TYPE_SV = 2			// single value channel
};

enum DWCustomPropValueType
{
    DW_CUSTOM_PROP_VAL_TYPE_EMPTY = 0,
    DW_CUSTOM_PROP_VAL_TYPE_INT64 = 1,
    DW_CUSTOM_PROP_VAL_TYPE_DOUBLE = 2,
    DW_CUSTOM_PROP_VAL_TYPE_STRING = 3,
};

#define DW_CUSTOM_PROP_FUNCTION_TYPE_GENERAL 0
#define DW_CUSTOM_PROP_FUNCTION_TYPE_TIME_RESPONSE 1
#define DW_CUSTOM_PROP_FUNCTION_TYPE_AUTO_SPECTRUM 2
#define DW_CUSTOM_PROP_FUNCTION_TYPE_CROSS_SPECTRUM 3
#define DW_CUSTOM_PROP_FUNCTION_TYPE_FREQUENCY_RESPONSE_FUNCTION 4
#define DW_CUSTOM_PROP_FUNCTION_TYPE_TRANSMISSIBILITY 5
#define DW_CUSTOM_PROP_FUNCTION_TYPE_COHERENCE 6
#define DW_CUSTOM_PROP_FUNCTION_TYPE_AUTOCORRELATION 7
#define DW_CUSTOM_PROP_FUNCTION_TYPE_CROSSCORRELATION 8
#define DW_CUSTOM_PROP_FUNCTION_TYPE_POWER_SPECTRAL_DENSITY_(PSD) 9
#define DW_CUSTOM_PROP_FUNCTION_TYPE_ENERGY_SPECTRAL_DENSITY_(ESD) 10
#define DW_CUSTOM_PROP_FUNCTION_TYPE_PROBABILITY_DENSITY_FUNCTION 11
#define DW_CUSTOM_PROP_FUNCTION_TYPE_SPECTRUM 12
#define DW_CUSTOM_PROP_FUNCTION_TYPE_CUMULATIVE_FREQUENCY_DISTRIBUTION 13
#define DW_CUSTOM_PROP_FUNCTION_TYPE_PEAKS_VALLEY 14
#define DW_CUSTOM_PROP_FUNCTION_TYPE_STRESS_CYCLES 15
#define DW_CUSTOM_PROP_FUNCTION_TYPE_STRAIN_CYCLES 16
#define DW_CUSTOM_PROP_FUNCTION_TYPE_ORBIT 17
#define DW_CUSTOM_PROP_FUNCTION_TYPE_MODE_INDICATOR_FUNCTION 18
#define DW_CUSTOM_PROP_FUNCTION_TYPE_FORCE_PATTERN 19
#define DW_CUSTOM_PROP_FUNCTION_TYPE_PARTIAL_POWER 20
#define DW_CUSTOM_PROP_FUNCTION_TYPE_PARTIAL_COHERENCE 21
#define DW_CUSTOM_PROP_FUNCTION_TYPE_EIGENVALUE 22
#define DW_CUSTOM_PROP_FUNCTION_TYPE_EIGENVECTOR 23
#define DW_CUSTOM_PROP_FUNCTION_TYPE_SHOCK_RESPONSE_SPECTRUM 24
#define DW_CUSTOM_PROP_FUNCTION_TYPE_FINITE_IMPULSE_RESPONSE_FILTER 25
#define DW_CUSTOM_PROP_FUNCTION_TYPE_MULTIPLE_COHERENCE 26
#define DW_CUSTOM_PROP_FUNCTION_TYPE_ORDER_FUNCTION 27
#define DW_CUSTOM_PROP_FUNCTION_TYPE_PHASE_COMPENSATION 28

//Response/Reference direction
#define DW_CUSTOM_PROP_DIRECTION_SCALAR 0
#define DW_CUSTOM_PROP_DIRECTION_PX_TRANS 1
#define DW_CUSTOM_PROP_DIRECTION_NX_TRANS -1
#define DW_CUSTOM_PROP_DIRECTION_PY_TRANS 2
#define DW_CUSTOM_PROP_DIRECTION_NY_TRANS -2
#define DW_CUSTOM_PROP_DIRECTION_PZ_TRANS 3
#define DW_CUSTOM_PROP_DIRECTION_NZ_TRANS -3
#define DW_CUSTOM_PROP_DIRECTION_PX_ROT 4
#define DW_CUSTOM_PROP_DIRECTION_NX_ROT -4
#define DW_CUSTOM_PROP_DIRECTION_PY_ROT 5
#define DW_CUSTOM_PROP_DIRECTION_NY_ROT -5
#define DW_CUSTOM_PROP_DIRECTION_PZ_ROT 6
#define DW_CUSTOM_PROP_DIRECTION_NZ_ROT -6

#pragma pack(push)

#pragma pack(1)
struct DWCANPortData
{
    unsigned long arb_id;	    // arbitration id
    char data[8];				// CAN data
};

#pragma pack(1)
struct DWCANFDPortData
{
    unsigned long arb_id;	    // arbitration id
    unsigned char extended;
    unsigned char reserved1;
    unsigned char reserved2;
    unsigned char dataSize;
    unsigned char data[64];				// CAN FD data
};

#pragma pack(1)
struct DWFileInfo				// structure used for data file
{ 
    double sample_rate;			// the sampling rate
    double start_store_time;	// absolute time of the start of storing (days)
    double duration;			// duration of data file (seconds)
};

#pragma pack(1)
struct DWChannel				// structure used for DeweSoft channel
{
    int index;					// unique channel identifier  
    char name[100];				// the name of a channel
    char unit[20];				// the unit of a channel
    char description[200];		// the description of a channel
    unsigned int color;			// specifies the color of a channel
    int array_size;				// length of the array channel (normal array_size = 1)
    int data_type;			    // channel data type - see data types in buffer 
};

#pragma pack(1)
struct DWEvent
{
    int event_type;				// 1..start event; 2..stop event
    double time_stamp;			// relative position in seconds
    char event_text[200];
};

#pragma pack(1)
struct DWReducedValue
{
    double time_stamp;			// relative time in seconds
    double ave;
    double min;
    double max;
    double rms;
};

#pragma pack(1)
struct DWArrayInfo
{
    int index;				// unique axis identifier  
    char name[100];			// axis name
    char unit[20];			// axis unit
    int size;				// length of the axis size
};

#pragma pack(1)
struct DWComplex
{
    double re;
    double im;
};

#pragma pack(1)
struct DWCustomProp
{
    char key[100];
    enum DWCustomPropValueType valueType;
    union
    {
        __int64 int64Val;
        double doubleVal;
        char strVal[100];
    } value;
};
#pragma pack(pop)

//event list
#define etStart 1
#define etStop 2
#define etTrigger 3
#define etVStart 11
#define etVStop 12
#define etKeyboard 20
#define etNotice 21
#define etVoice 22
#define etPicture 23
#define etModule 24
#define etAlarm 25
#define etCursorInfo 26
#define etAlarmLevel 27

//Storing type
#define ST_ALWAYS_FAST 0
#define ST_ALWAYS_SLOW 1
#define ST_FAST_ON_TRIGGER 2
#define ST_FAST_ON_TRIGGER_SLOW_OTH 3 

// data types in buffer
#define MaxDataTypes 15
#define dtByte 0
#define dtShortInt 1
#define dtSmallInt 2
#define dtWord 3
#define dtInteger 4
#define dtSingle 5
#define dtInt64 6
#define dtDouble 7
#define dtLongword 8
#define dtComplexSingle 9
#define dtComplexDouble 10
#define dtText 11
#define dtBinary 12
#define dtCANPortData 13
#define dtCANFDPortData 14
#define dtBytes8 15
#define dtBytes16 16
#define dtBytes32 17
#define dtBytes64 18

//FUNCTIONS:
typedef enum DWStatus (*_DWInit)(void);
typedef enum DWStatus (*_DWDeInit)(void);
typedef enum DWStatus (*_DWAddReader)(void);
typedef enum DWStatus (*_DWGetNumReaders)(int* num_readers);
typedef enum DWStatus (*_DWSetActiveReader)(int index);
typedef int (*_DWGetVersion)(void);
typedef enum DWStatus (*_DWOpenDataFile)(char* file_name, struct DWFileInfo* file_info);
typedef enum DWStatus (*_DWCloseDataFile)(void);
typedef int (*_DWGetMultiFileIndex)(void);
typedef int (*_DWGetChannelListCount)(void);
typedef enum DWStatus (*_DWGetChannelList)(struct DWChannel* channel_list);
typedef enum DWStatus (*_DWGetChannelFactors)(int ch_index, double* scale, double* offset);
typedef enum DWStatus (*_DWGetChannelProps)(int ch_index, enum DWChannelProps ch_prop, void* buffer, int* max_len);
typedef __int64 (*_DWGetBinarySamplesCount)(int ch_index);
typedef  enum DWStatus (*_DWGetBinarySamples)(int ch_index, __int64 sampleIndex, char* data, double* time_stamp, int* datalen);		
typedef  enum DWStatus(*_DWGetBinarySamplesEx)(int ch_index, __int64 position, int count, char* data, double* time_stamp, int* datalen);
typedef __int64 (*_DWGetScaledSamplesCount)(int ch_index);
typedef  enum DWStatus (*_DWGetScaledSamples)(int ch_index, __int64 position, int count, double* data, double* time_stamp);
typedef __int64 (*_DWGetRawSamplesCount)(int ch_index);
typedef  enum DWStatus (*_DWGetRawSamples)(int ch_index, __int64 position, int count, void* data, double* time_stamp);
typedef int (*_DWGetComplexChannelListCount)(void);
typedef enum DWStatus (*_DWGetComplexChannelList)(struct DWChannel* channel_list);
typedef __int64 (*_DWGetComplexScaledSamplesCount)(int ch_index);
typedef  enum DWStatus (*_DWGetComplexScaledSamples)(int ch_index, __int64 position, int count, struct DWComplex* data, double* time_stamp);
typedef __int64 (*_DWGetComplexRawSamplesCount)(int ch_index);
typedef  enum DWStatus (*_DWGetComplexRawSamples)(int ch_index, __int64 position, int count, struct DWComplex* data, double* time_stamp);
typedef int (*_DWGetEventListCount)(void);
typedef enum DWStatus (*_DWGetEventList)(struct DWEvent* event_list);
typedef enum DWStatus (*_DWGetStream)(char* stream_name, char* buffer, int* max_len);
typedef enum DWStatus (*_DWExportHeader)(char* file_name);
typedef int (*_DWGetTextChannelListCount)(void);
typedef enum DWStatus (*_DWGetTextChannelList)(struct DWChannel* channel_list);
typedef __int64 (*_DWGetTextValuesCount)(int ch_index);
typedef enum DWStatus (*_DWGetTextValues)(int ch_index, int position, int count, char* text_values, double* time_stamp);
typedef enum DWStatus (*_DWGetReducedValuesCount)(int ch_index, int* count, double* block_size);
typedef enum DWStatus (*_DWGetReducedValues)(int ch_index, int position, int count, struct DWReducedValue* data);
typedef enum DWStatus(*_DWGetReducedValuesBlock)(int* ch_ids, int ch_count, int position, int count, int ib_level, struct DWReducedValue* data);
typedef int (*_DWGetHeaderEntryCount)(void);
typedef enum DWStatus (*_DWGetHeaderEntryList)(struct DWChannel* channel_list);
typedef enum DWStatus (*_DWGetHeaderEntryText)(int ch_index, char* text_value, int text_value_size);
typedef int (*_DWGetStoringType)(void);
typedef int (*_DWGetArrayInfoCount)(int ch_index);
typedef enum DWStatus (*_DWGetArrayInfoList)(int ch_index, struct DWArrayInfo* array_inf_list);
typedef enum DWStatus (*_DWGetArrayIndexValue)(int ch_index, int array_info_index, int array_value_index, char* value, int value_size);
typedef enum DWStatus (*_DWGetArrayIndexValueF)(int ch_index, int array_info_index, int array_value_index, double* value);

//spec. m
typedef enum DWStatus (*_DWGetChannelListItem)(int array_index, int* index, char* name, char* unit, char* description, int* color, int* array_size, int max_char_size);
typedef enum DWStatus (*_DWGetHeaderEntryListItem)(int array_index, int* index, char* name, char* unit, char* description, int* color, int* array_size, int max_char_size);
typedef enum DWStatus (*_DWGetEventListItem)(int event_Index, int* event_type, double* time_stamp, char* event_text, int max_char_size);
typedef enum DWStatus (*_DWGetReducedAveValues)(int ch_index, int position, int count, double* data, double* time_stamp);
typedef enum DWStatus (*_DWGetReducedMinValues)(int ch_index, int position, int count, double* data, double* time_stamp);
typedef enum DWStatus (*_DWGetReducedMaxValues)(int ch_index, int position, int count, double* data, double* time_stamp);
typedef enum DWStatus (*_DWGetReducedRMSValues)(int ch_index, int position, int count, double* data, double* time_stamp);

//spec. f
typedef enum DWStatus (*_DWGetHeaderEntryTextF)(int entry_number, char* text_value, int text_value_size);
typedef enum DWStatus (*_DWGetHeaderEntryNameF)(int entry_number, char* name, int name_size);
typedef enum DWStatus (*_DWGetHeaderEntryIDF)(int entry_number, char* ID, int name_size);
typedef double (*_DWGetEventTimeF)(int event_number);
typedef enum DWStatus (*_DWGetEventTextF)(int event_number, char* text, int text_size);
typedef int (*_DWGetEventTypeF)(int event_number);
typedef int (*_DWGetReducedDataChannelCountF)(void);
typedef enum DWStatus (*_DWGetReducedDataChannelNameF)(int Channel_Number, char* name, int name_size);
typedef int (*_DWGetReducedDataChannelIndexF)(char* name);
typedef enum DWStatus (*_DWGetRecudedDataChannelInfoF)(int Channel_Number, char* X_Axis_Units, int X_Axis_Units_size, char* Y_Axis_Units, int Y_Axis_Units_size, double* Chn_Offset, int* Channel_Length, double* ch_rate);
typedef enum DWStatus (*_DWGetRecudedDataF)(int Channel_Number, double* X_Axis, double* Y_Axis, int position, int count);
typedef enum DWStatus (*_DWGetRecudedYDataF)(int Channel_Number, double* Y_Axis, int position, int count);
typedef enum DWStatus (*_DWGetRecudedDataAllF)(int Channel_Number, double* Y_MIN_Axis, double* Y_AVE_Axis, double* Y_MAX_Axis, double* Y_RMS_Axis, int position, int count);
typedef int (*_DWGetTriggerDataTriggerCountF)(void);
typedef double (*_DWGetTriggerDataTriggerTimeF)(int Trigger_Number);
typedef enum DWStatus (*_DWGetTriggerDataChannelNameF)(int Channel_Number, char* name, int name_size);
typedef int (*_DWGetTriggerDataChannelIndexF)(char* name);
typedef enum DWStatus (*_DWGetTriggerDataChannelInfoF)(int Trigger_Number, int Channel_Number, char* X_Axis_Units, int X_Axis_Units_size, char* Y_Axis_Units, int Y_Axis_Units_size, double* Chn_Offset, double* Channel_Length, double* ch_rate, int* ch_type);
typedef enum DWStatus (*_DWGetTriggerDataF)(int Trigger_Number , int Channel_Number, double* Y_Axis, double* X_Axis, double position, int count);

//functions
extern _DWInit DWInit;
extern _DWDeInit DWDeInit;
extern _DWAddReader DWAddReader;
extern _DWGetNumReaders DWGetNumReaders;
extern _DWSetActiveReader DWSetActiveReader;
extern _DWGetVersion DWGetVersion;
extern _DWOpenDataFile DWOpenDataFile;
extern _DWCloseDataFile DWCloseDataFile;
extern _DWGetMultiFileIndex DWGetMultiFileIndex;
extern _DWGetChannelListCount DWGetChannelListCount;
extern _DWGetChannelList DWGetChannelList;
extern _DWGetChannelFactors DWGetChannelFactors;
extern _DWGetChannelProps DWGetChannelProps;
extern _DWGetBinarySamplesCount DWGetBinarySamplesCount;
extern _DWGetBinarySamples DWGetBinarySamples;
extern _DWGetBinarySamplesEx DWGetBinarySamplesEx;
extern _DWGetScaledSamplesCount DWGetScaledSamplesCount;
extern _DWGetScaledSamples DWGetScaledSamples;
extern _DWGetRawSamplesCount DWGetRawSamplesCount;
extern _DWGetRawSamples DWGetRawSamples;
extern _DWGetComplexChannelListCount DWGetComplexChannelListCount;
extern _DWGetComplexChannelList DWGetComplexChannelList;
extern _DWGetComplexScaledSamplesCount DWGetComplexScaledSamplesCount;
extern _DWGetComplexScaledSamples DWGetComplexScaledSamples;
extern _DWGetComplexRawSamplesCount DWGetComplexRawSamplesCount;
extern _DWGetComplexRawSamples DWGetComplexRawSamples;
extern _DWGetEventListCount DWGetEventListCount;
extern _DWGetEventList DWGetEventList;
extern _DWGetStream DWGetStream;
extern _DWExportHeader DWExportHeader;
extern _DWGetTextChannelListCount DWGetTextChannelListCount;
extern _DWGetTextChannelList DWGetTextChannelList;
extern _DWGetTextValuesCount DWGetTextValuesCount;
extern _DWGetTextValues DWGetTextValues;
extern _DWGetReducedValuesCount DWGetReducedValuesCount;
extern _DWGetReducedValues DWGetReducedValues;
extern _DWGetReducedValuesBlock DWGetReducedValuesBlock;
extern _DWGetHeaderEntryCount DWGetHeaderEntryCount;
extern _DWGetHeaderEntryList DWGetHeaderEntryList;
extern _DWGetHeaderEntryText DWGetHeaderEntryText;
extern _DWGetStoringType DWGetStoringType;
extern _DWGetArrayInfoCount DWGetArrayInfoCount;
extern _DWGetArrayInfoList DWGetArrayInfoList;
extern _DWGetArrayIndexValue DWGetArrayIndexValue;
extern _DWGetArrayIndexValueF DWGetArrayIndexValueF;
//
extern _DWGetChannelListItem DWGetChannelListItem;
extern _DWGetHeaderEntryListItem DWGetHeaderEntryListItem;
extern _DWGetEventListItem DWGetEventListItem;
extern _DWGetReducedAveValues DWGetReducedAveValues;
extern _DWGetReducedMinValues DWGetReducedMinValues;
extern _DWGetReducedMaxValues DWGetReducedMaxValues;
extern _DWGetReducedRMSValues DWGetReducedRMSValues;
//
extern _DWGetHeaderEntryTextF DWGetHeaderEntryTextF;
extern _DWGetHeaderEntryNameF DWGetHeaderEntryNameF;
extern _DWGetHeaderEntryIDF DWGetHeaderEntryIDF;
extern _DWGetEventTimeF DWGetEventTimeF;
extern _DWGetEventTextF DWGetEventTextF;
extern _DWGetEventTypeF DWGetEventTypeF;
extern _DWGetReducedDataChannelCountF DWGetReducedDataChannelCountF;
extern _DWGetReducedDataChannelNameF DWGetReducedDataChannelNameF;
extern _DWGetReducedDataChannelIndexF DWGetReducedDataChannelIndexF;
extern _DWGetRecudedDataChannelInfoF DWGetRecudedDataChannelInfoF;
extern _DWGetRecudedDataF DWGetRecudedDataF;
extern _DWGetRecudedYDataF DWGetRecudedYDataF;
extern _DWGetRecudedDataAllF DWGetRecudedDataAllF;
extern _DWGetTriggerDataTriggerCountF DWGetTriggerDataTriggerCountF;
extern _DWGetTriggerDataTriggerTimeF DWGetTriggerDataTriggerTimeF;
extern _DWGetTriggerDataChannelNameF DWGetTriggerDataChannelNameF;
extern _DWGetTriggerDataChannelIndexF DWGetTriggerDataChannelIndexF;
extern _DWGetTriggerDataChannelInfoF DWGetTriggerDataChannelInfoF;
extern _DWGetTriggerDataF DWGetTriggerDataF;

int LoadDWDLL(const char* lib_name);
int CloseDWDLL();

#ifdef __cplusplus
}
#endif

#endif
