#include "DWLoadLib.h"

//functions
_DWInit DWInit;
_DWDeInit DWDeInit;
_DWAddReader DWAddReader;
_DWGetNumReaders DWGetNumReaders;
_DWSetActiveReader DWSetActiveReader;
_DWGetVersion DWGetVersion;
_DWOpenDataFile DWOpenDataFile;
_DWCloseDataFile DWCloseDataFile;
_DWGetMultiFileIndex DWGetMultiFileIndex;
_DWGetChannelListCount DWGetChannelListCount;
_DWGetChannelList DWGetChannelList;
_DWGetChannelFactors DWGetChannelFactors;
_DWGetChannelProps DWGetChannelProps;
_DWGetBinarySamplesCount DWGetBinarySamplesCount;
_DWGetBinarySamples DWGetBinarySamples;
_DWGetBinarySamplesEx DWGetBinarySamplesEx;
_DWGetScaledSamplesCount DWGetScaledSamplesCount;
_DWGetScaledSamples DWGetScaledSamples;
_DWGetRawSamplesCount DWGetRawSamplesCount;
_DWGetRawSamples DWGetRawSamples;
_DWGetComplexChannelListCount DWGetComplexChannelListCount;
_DWGetComplexChannelList DWGetComplexChannelList;
_DWGetComplexScaledSamplesCount DWGetComplexScaledSamplesCount;
_DWGetComplexScaledSamples DWGetComplexScaledSamples;
_DWGetComplexRawSamplesCount DWGetComplexRawSamplesCount;
_DWGetComplexRawSamples DWGetComplexRawSamples;
_DWGetEventListCount DWGetEventListCount;
_DWGetEventList DWGetEventList;
_DWGetStream DWGetStream;
_DWExportHeader DWExportHeader;
_DWGetTextChannelListCount DWGetTextChannelListCount;
_DWGetTextChannelList DWGetTextChannelList;
_DWGetTextValuesCount DWGetTextValuesCount;
_DWGetTextValues DWGetTextValues;
_DWGetReducedValuesCount DWGetReducedValuesCount;
_DWGetReducedValues DWGetReducedValues;
_DWGetReducedValuesBlock DWGetReducedValuesBlock;
_DWGetHeaderEntryCount DWGetHeaderEntryCount;
_DWGetHeaderEntryList DWGetHeaderEntryList;
_DWGetHeaderEntryText DWGetHeaderEntryText;
_DWGetStoringType DWGetStoringType;
_DWGetArrayInfoCount DWGetArrayInfoCount;
_DWGetArrayInfoList DWGetArrayInfoList;
_DWGetArrayIndexValue DWGetArrayIndexValue;
_DWGetArrayIndexValueF DWGetArrayIndexValueF;
_DWGetChannelListItem DWGetChannelListItem;
_DWGetHeaderEntryListItem DWGetHeaderEntryListItem;
_DWGetEventListItem DWGetEventListItem;
_DWGetReducedAveValues DWGetReducedAveValues;
_DWGetReducedMinValues DWGetReducedMinValues;
_DWGetReducedMaxValues DWGetReducedMaxValues;
_DWGetReducedRMSValues DWGetReducedRMSValues;
_DWGetHeaderEntryTextF DWGetHeaderEntryTextF;
_DWGetHeaderEntryNameF DWGetHeaderEntryNameF;
_DWGetHeaderEntryIDF DWGetHeaderEntryIDF;
_DWGetEventTimeF DWGetEventTimeF;
_DWGetEventTextF DWGetEventTextF;
_DWGetEventTypeF DWGetEventTypeF;
_DWGetReducedDataChannelCountF DWGetReducedDataChannelCountF;
_DWGetReducedDataChannelNameF DWGetReducedDataChannelNameF;
_DWGetReducedDataChannelIndexF DWGetReducedDataChannelIndexF;
_DWGetRecudedDataChannelInfoF DWGetRecudedDataChannelInfoF;
_DWGetRecudedDataF DWGetRecudedDataF;
_DWGetRecudedYDataF DWGetRecudedYDataF;
_DWGetRecudedDataAllF DWGetRecudedDataAllF;
_DWGetTriggerDataTriggerCountF DWGetTriggerDataTriggerCountF;
_DWGetTriggerDataTriggerTimeF DWGetTriggerDataTriggerTimeF;
_DWGetTriggerDataChannelNameF DWGetTriggerDataChannelNameF;
_DWGetTriggerDataChannelIndexF DWGetTriggerDataChannelIndexF;
_DWGetTriggerDataChannelInfoF DWGetTriggerDataChannelInfoF;
_DWGetTriggerDataF DWGetTriggerDataF;

#ifdef _WIN32
    static HINSTANCE hInstLibrary;//Dll handle
    static int m_InitDLL;
    #define OPEN_LIBRARY(lib_name) LoadLibrary(lib_name)
    #define GET_PROC_ADDRESS(libraryInstance, func_name) GetProcAddress(libraryInstance, func_name)
#else
    static void* hInstLibrary;
    static int m_InitDLL;
    #define OPEN_LIBRARY(lib_name) dlopen(lib_name, RTLD_LAZY)
    #define GET_PROC_ADDRESS(libraryInstance, func_name) dlsym(libraryInstance, func_name)
#endif

int LoadDWDLL(const char* lib_name)
{
    hInstLibrary = OPEN_LIBRARY(lib_name);
	
	if (!hInstLibrary)
		return 0;
	DWInit = (_DWInit)GET_PROC_ADDRESS(hInstLibrary, "DWInit");
	if (!DWInit)
		return 0;
	DWDeInit = (_DWDeInit)GET_PROC_ADDRESS(hInstLibrary, "DWDeInit");
	if (!DWDeInit)
		return 0;
	DWAddReader = (_DWAddReader)GET_PROC_ADDRESS(hInstLibrary, "DWAddReader");
	if (!DWAddReader)
		return 0;
	DWGetNumReaders = (_DWGetNumReaders)GET_PROC_ADDRESS(hInstLibrary, "DWGetNumReaders");
	if (!DWGetNumReaders)
		return 0;
	DWSetActiveReader = (_DWSetActiveReader)GET_PROC_ADDRESS(hInstLibrary, "DWSetActiveReader");
	if (!DWSetActiveReader)
		return 0;
	DWGetVersion = (_DWGetVersion)GET_PROC_ADDRESS(hInstLibrary, "DWGetVersion");
	if (!DWGetVersion)
		return 0;
	DWOpenDataFile = (_DWOpenDataFile)GET_PROC_ADDRESS(hInstLibrary, "DWOpenDataFile");
	if (!DWOpenDataFile)
		return 0;
	DWCloseDataFile = (_DWCloseDataFile)GET_PROC_ADDRESS(hInstLibrary, "DWCloseDataFile");
	if (!DWCloseDataFile)
		return 0;
	DWGetMultiFileIndex = (_DWGetMultiFileIndex)GET_PROC_ADDRESS(hInstLibrary, "DWGetMultiFileIndex");
	if (!DWGetMultiFileIndex)
		return 0;
	DWGetChannelListCount = (_DWGetChannelListCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetChannelListCount");
	if (!DWGetChannelListCount)
		return 0;
	DWGetChannelList = (_DWGetChannelList)GET_PROC_ADDRESS(hInstLibrary, "DWGetChannelList");
	if (!DWGetChannelList)
		return 0;
	DWGetChannelFactors = (_DWGetChannelFactors)GET_PROC_ADDRESS(hInstLibrary, "DWGetChannelFactors");
	if (!DWGetChannelFactors)
		return 0;
	DWGetChannelProps = (_DWGetChannelProps)GET_PROC_ADDRESS(hInstLibrary, "DWGetChannelProps");
	if (!DWGetChannelProps)
		return 0;
	DWGetBinarySamplesCount = (_DWGetBinarySamplesCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetBinarySamplesCount");
	if (!DWGetBinarySamplesCount)
		return 0;
	DWGetBinarySamples = (_DWGetBinarySamples)GET_PROC_ADDRESS(hInstLibrary, "DWGetBinarySamples");
	if (!DWGetBinarySamples)
		return 0;
	DWGetBinarySamplesEx = (_DWGetBinarySamplesEx)GET_PROC_ADDRESS(hInstLibrary, "DWGetBinarySamplesEx");
	if (!DWGetBinarySamplesEx)
		return 0;
	DWGetScaledSamplesCount = (_DWGetScaledSamplesCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetScaledSamplesCount");
	if (!DWGetScaledSamplesCount)
		return 0;
	DWGetScaledSamples = (_DWGetScaledSamples)GET_PROC_ADDRESS(hInstLibrary, "DWGetScaledSamples");
	if (!DWGetScaledSamples)
		return 0;
	DWGetRawSamplesCount = (_DWGetRawSamplesCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetRawSamplesCount");
	if (!DWGetRawSamplesCount)
		return 0;
	DWGetRawSamples = (_DWGetRawSamples)GET_PROC_ADDRESS(hInstLibrary, "DWGetRawSamples");
	if (!DWGetRawSamples)
		return 0;
	DWGetComplexChannelListCount = (_DWGetComplexChannelListCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetComplexChannelListCount");
	if (!DWGetComplexChannelListCount)
		return 0;
	DWGetComplexChannelList = (_DWGetComplexChannelList)GET_PROC_ADDRESS(hInstLibrary, "DWGetComplexChannelList");
	if (!DWGetComplexChannelList)
		return 0;
	DWGetComplexScaledSamplesCount = (_DWGetComplexScaledSamplesCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetComplexScaledSamplesCount");
	if (!DWGetComplexScaledSamplesCount)
		return 0;
	DWGetComplexScaledSamples = (_DWGetComplexScaledSamples)GET_PROC_ADDRESS(hInstLibrary, "DWGetComplexScaledSamples");
	if (!DWGetComplexScaledSamples)
		return 0;
	DWGetComplexRawSamplesCount = (_DWGetComplexRawSamplesCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetComplexRawSamplesCount");
	if (!DWGetComplexRawSamplesCount)
		return 0;
	DWGetComplexRawSamples = (_DWGetComplexRawSamples)GET_PROC_ADDRESS(hInstLibrary, "DWGetComplexRawSamples");
	if (!DWGetComplexRawSamples)
		return 0;
	DWGetEventListCount = (_DWGetEventListCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetEventListCount");
	if (!DWGetEventListCount)
		return 0;
	DWGetEventList = (_DWGetEventList)GET_PROC_ADDRESS(hInstLibrary, "DWGetEventList");
	if (!DWGetEventList)
		return 0;
	DWGetStream = (_DWGetStream)GET_PROC_ADDRESS(hInstLibrary, "DWGetStream");
	if (!DWGetStream)
		return 0;
	DWExportHeader = (_DWExportHeader)GET_PROC_ADDRESS(hInstLibrary, "DWExportHeader");
	if (!DWExportHeader)
		return 0;
	DWGetTextChannelListCount = (_DWGetTextChannelListCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetTextChannelListCount");
	if (!DWGetTextChannelListCount)
		return 0;
	DWGetTextChannelList = (_DWGetTextChannelList)GET_PROC_ADDRESS(hInstLibrary, "DWGetTextChannelList");
	if (!DWGetTextChannelList)
		return 0;
	DWGetTextValuesCount = (_DWGetTextValuesCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetTextValuesCount");
	if (!DWGetTextValuesCount)
		return 0;
	DWGetTextValues = (_DWGetTextValues)GET_PROC_ADDRESS(hInstLibrary, "DWGetTextValues");
	if (!DWGetTextValues)
		return 0;
	DWGetReducedValuesCount = (_DWGetReducedValuesCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetReducedValuesCount");
	if (!DWGetReducedValuesCount)
		return 0;
	DWGetReducedValues = (_DWGetReducedValues)GET_PROC_ADDRESS(hInstLibrary, "DWGetReducedValues");
	if (!DWGetReducedValues)
		return 0;
	DWGetReducedValuesBlock = (_DWGetReducedValuesBlock)GET_PROC_ADDRESS(hInstLibrary, "DWGetReducedValuesBlock");
	if (!DWGetReducedValuesBlock)
		return 0;
	DWGetHeaderEntryCount = (_DWGetHeaderEntryCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetHeaderEntryCount");
	if (!DWGetHeaderEntryCount)
		return 0;
	DWGetHeaderEntryList = (_DWGetHeaderEntryList)GET_PROC_ADDRESS(hInstLibrary, "DWGetHeaderEntryList");
	if (!DWGetHeaderEntryList)
		return 0;
	DWGetHeaderEntryText = (_DWGetHeaderEntryText)GET_PROC_ADDRESS(hInstLibrary, "DWGetHeaderEntryText");
	if (!DWGetHeaderEntryText)
		return 0;
	DWGetStoringType = (_DWGetStoringType)GET_PROC_ADDRESS(hInstLibrary, "DWGetStoringType");
	if (!DWGetStoringType)
		return 0;
	DWGetArrayInfoCount = (_DWGetArrayInfoCount)GET_PROC_ADDRESS(hInstLibrary, "DWGetArrayInfoCount");
	if (!DWGetArrayInfoCount)
		return 0;
	DWGetArrayInfoList = (_DWGetArrayInfoList)GET_PROC_ADDRESS(hInstLibrary, "DWGetArrayInfoList");
	if (!DWGetArrayInfoList)
		return 0;
	DWGetArrayIndexValue = (_DWGetArrayIndexValue)GET_PROC_ADDRESS(hInstLibrary, "DWGetArrayIndexValue");
	if (!DWGetArrayIndexValue)
		return 0;
	DWGetArrayIndexValueF = (_DWGetArrayIndexValueF)GET_PROC_ADDRESS(hInstLibrary, "DWGetArrayIndexValueF");
	if (!DWGetArrayIndexValueF)
		return 0;

	//spec
	DWGetChannelListItem = (_DWGetChannelListItem)GET_PROC_ADDRESS(hInstLibrary, "DWGetChannelListItem");
	if (!DWGetChannelListItem)
		return 0;
	DWGetHeaderEntryListItem = (_DWGetHeaderEntryListItem)GET_PROC_ADDRESS(hInstLibrary, "DWGetHeaderEntryListItem");
	if (!DWGetHeaderEntryListItem)
		return 0;
	DWGetEventListItem = (_DWGetEventListItem)GET_PROC_ADDRESS(hInstLibrary, "DWGetEventListItem");
	if (!DWGetEventListItem)
		return 0;
	DWGetReducedAveValues = (_DWGetReducedAveValues)GET_PROC_ADDRESS(hInstLibrary, "DWGetReducedAveValues");
	if (!DWGetReducedAveValues)
		return 0;
	DWGetReducedMinValues = (_DWGetReducedMinValues)GET_PROC_ADDRESS(hInstLibrary, "DWGetReducedMinValues");
	if (!DWGetReducedMinValues)
		return 0;
	DWGetReducedMaxValues = (_DWGetReducedMaxValues)GET_PROC_ADDRESS(hInstLibrary, "DWGetReducedMaxValues");
	if (!DWGetReducedMaxValues)
		return 0;
	DWGetReducedRMSValues = (_DWGetReducedRMSValues)GET_PROC_ADDRESS(hInstLibrary, "DWGetReducedRMSValues");
	if (!DWGetReducedRMSValues)
		return 0;
	
	//spec
	DWGetHeaderEntryTextF = (_DWGetHeaderEntryTextF)GET_PROC_ADDRESS(hInstLibrary, "DWGetHeaderEntryTextF");
	if (!DWGetHeaderEntryTextF)
		return 0;
	DWGetHeaderEntryNameF = (_DWGetHeaderEntryNameF)GET_PROC_ADDRESS(hInstLibrary, "DWGetHeaderEntryNameF");
	if (!DWGetHeaderEntryNameF)
		return 0;
	DWGetHeaderEntryIDF = (_DWGetHeaderEntryIDF)GET_PROC_ADDRESS(hInstLibrary, "DWGetHeaderEntryIDF");
	if (!DWGetHeaderEntryIDF)
		return 0;
	DWGetEventTimeF = (_DWGetEventTimeF)GET_PROC_ADDRESS(hInstLibrary, "DWGetEventTimeF");
	if (!DWGetEventTimeF)
		return 0;
	DWGetEventTextF = (_DWGetEventTextF)GET_PROC_ADDRESS(hInstLibrary, "DWGetEventTextF");
	if (!DWGetEventTextF)
		return 0;
	DWGetEventTypeF = (_DWGetEventTypeF)GET_PROC_ADDRESS(hInstLibrary, "DWGetEventTypeF");
	if (!DWGetEventTypeF)
		return 0;
	DWGetReducedDataChannelCountF = (_DWGetReducedDataChannelCountF)GET_PROC_ADDRESS(hInstLibrary, "DWGetReducedDataChannelCountF");
	if (!DWGetReducedDataChannelCountF)
		return 0;
	DWGetReducedDataChannelNameF = (_DWGetReducedDataChannelNameF)GET_PROC_ADDRESS(hInstLibrary, "DWGetReducedDataChannelNameF");
	if (!DWGetReducedDataChannelNameF)
		return 0;
	DWGetReducedDataChannelIndexF = (_DWGetReducedDataChannelIndexF)GET_PROC_ADDRESS(hInstLibrary, "DWGetReducedDataChannelIndexF");
	if (!DWGetReducedDataChannelIndexF)
		return 0;
	DWGetRecudedDataChannelInfoF = (_DWGetRecudedDataChannelInfoF)GET_PROC_ADDRESS(hInstLibrary, "DWGetRecudedDataChannelInfoF");
	if (!DWGetRecudedDataChannelInfoF)
		return 0;
	DWGetRecudedDataF = (_DWGetRecudedDataF)GET_PROC_ADDRESS(hInstLibrary, "DWGetRecudedDataF");
	if (!DWGetRecudedDataF)
		return 0;
	DWGetRecudedYDataF = (_DWGetRecudedYDataF)GET_PROC_ADDRESS(hInstLibrary, "DWGetRecudedYDataF");
	if (!DWGetRecudedYDataF)
		return 0;
	DWGetRecudedDataAllF = (_DWGetRecudedDataAllF)GET_PROC_ADDRESS(hInstLibrary, "DWGetRecudedDataAllF");
	if (!DWGetRecudedDataAllF)
		return 0;
	DWGetTriggerDataTriggerCountF = (_DWGetTriggerDataTriggerCountF)GET_PROC_ADDRESS(hInstLibrary, "DWGetTriggerDataTriggerCountF");
	if (!DWGetTriggerDataTriggerCountF)
		return 0;
	DWGetTriggerDataTriggerTimeF = (_DWGetTriggerDataTriggerTimeF)GET_PROC_ADDRESS(hInstLibrary, "DWGetTriggerDataTriggerTimeF");
	if (!DWGetTriggerDataTriggerTimeF)
		return 0;
	DWGetTriggerDataChannelNameF = (_DWGetTriggerDataChannelNameF)GET_PROC_ADDRESS(hInstLibrary, "DWGetTriggerDataChannelNameF");
	if (!DWGetTriggerDataChannelNameF)
		return 0;
	DWGetTriggerDataChannelIndexF = (_DWGetTriggerDataChannelIndexF)GET_PROC_ADDRESS(hInstLibrary, "DWGetTriggerDataChannelIndexF");
	if (!DWGetTriggerDataChannelIndexF)
		return 0;
	DWGetTriggerDataChannelInfoF = (_DWGetTriggerDataChannelInfoF)GET_PROC_ADDRESS(hInstLibrary, "DWGetTriggerDataChannelInfoF");
	if (!DWGetTriggerDataChannelInfoF)
		return 0;
	DWGetTriggerDataF = (_DWGetTriggerDataF)GET_PROC_ADDRESS(hInstLibrary, "DWGetTriggerDataF");
	if (!DWGetTriggerDataF)
		return 0;

	return 1;
}

int CloseDWDLL()
{
#ifdef _WIN32
    if (!FreeLibrary(hInstLibrary))
        return GetLastError();
    return 0;
#else
    return dlclose(hInstLibrary);
#endif
}
