#ifndef _DWDATAREADERLIB_
#define _DWDATAREADERLIB_
#include <Rcpp.h>

#include <vector>
#include <algorithm>
#include <regex>
#include <sstream>

// #include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/algorithm/string.hpp>

extern"C"{
#include "DWLoadLib.h" 
}

using namespace Rcpp;

/*#define SHOW_DEBUG_INF 1 */

#ifdef SHOW_DEBUG_INF
  #define DEBUG(x) Rcpp::Rcout << "DEBUG : " << x << std::endl;
#else
  #define DEBUG(x) 
#endif

#define PRINT(x) Rcpp::Rcout << x << std::endl;
#define ERR(x) Rcpp::Rcerr << x << std::endl;

#ifdef _WIN32
#ifdef _WIN64
const char* DLL_PATH = TEXT("DWDataReaderLib64.dll");
#else
const char* DLL_PATH = TEXT("DWDataReaderLib.dll");
#endif
#else
const char DLL_PATH[] = "DWDataReaderLib.so";
#endif

enum REDUCED_DATA_TYPE
{
  MIN_DATA = 1,
  MAX_DATA = 2,
  AVERAGE_DATA = 3,
  RMS_DATA = 4
};


//' @name DWDataReader
//' @title DWDataReader library class
//' @description Library to import data from DEWESoft data files (*.dxd, *.dxz)
class DWDataReader
{
  public:
    DWDataReader();
    
    ~DWDataReader();
    
    bool loadDWLib();
    bool unloadDWLib();
    
    bool openFile(std::string filepath);
    bool closeFile();
    
    Datetime getAbsoluteTimeFromSec(double start_time, double sample_time_sec=0);
    DataFrame getFormatedFileInfo();
    double getFileStoreTime();
    
    __int64 getChannelDataCount(const std::string& channelName);
    int getChannelReducedDataCount(const std::string& channelName);
    
    DataFrame getChannelData(std::string channelName);
    DataFrame getChannelReducedData(std::string channelName, int reduced_type);
    
    List getChannels();
    
    struct DWChannel * getChannelInfos(const std::string& channelName);
    std::string getChannelInfosEx(const std::string& channelName, const std::string& RegExp);
    
    int getChannelSRDiv(const std::string& channelName);
    int getArraySize(const std::string& channelName);
    double getChannelFrequency(const std::string& channelName);
    
  private:
    std::vector<std::pair<std::string, int>> getChannelsNameAndIndex();
    int findChannelIndexByName(const std::string& channelName);
    bool _getChannelData(const std::string& channelName, double * data, double * time_stamp);
    bool _getChannelReducedData(const std::string& channelName, double * data, double * time_stamp, REDUCED_DATA_TYPE reduced_type);
    NumericVector fromPointerToVector(double *pointer, __int64 size);
    std::string iso_8859_1_to_utf8(std::string str);
    
    bool isLoaded=false;
    std::string filepath="";
    struct DWFileInfo fileInfo;
    int num_readers;
    
    int channelCount=0;
    struct DWChannel * channelList = NULL;
};

RCPP_EXPOSED_CLASS(DWDataReader)
RCPP_MODULE(DWDataReaderLib) {
  using namespace Rcpp;
  
  class_<DWDataReader>("DWDataReader")
    .constructor("DWDataReader")
    .method("loadDWLib",&DWDataReader::loadDWLib)
    .method("unloadDWLib",&DWDataReader::unloadDWLib)
    .method("openFile",&DWDataReader::openFile)
    .method("closeFile",&DWDataReader::closeFile)
    .method("getFormatedFileInfo",&DWDataReader::getFormatedFileInfo)
    .method("getAbsoluteTimeFromSec", &DWDataReader::getAbsoluteTimeFromSec)
    .method("getChannels",&DWDataReader::getChannels)
    .method("getFileStoreTime",&DWDataReader::getFileStoreTime)
    .method("getChannelDataCount",&DWDataReader::getChannelDataCount)
    .method("getChannelInfosEx",&DWDataReader::getChannelInfosEx)
    .method("getChannelInfos",&DWDataReader::getChannelInfos)
    .method("getArraySize",&DWDataReader::getArraySize)
    .method("getChannelData",&DWDataReader::getChannelData)
    .method("getChannelSRDiv",&DWDataReader::getChannelSRDiv)
    .method("getChannelFrequency",&DWDataReader::getChannelFrequency)
    .method("getChannelReducedData",&DWDataReader::getChannelReducedData)
    .method("getChannelReducedDataCount",&DWDataReader::getChannelReducedDataCount)
  ;
}

#endif