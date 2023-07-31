#include "DWDataReader.hpp"

DWDataReader::DWDataReader()
{
  DEBUG("Loading DWDataReader class (DWDataReader())");
  
  try
  {
    this->isLoaded = false;
    this->num_readers = 0;
  
    if(this->loadDWLib()==true)
    {
      DWAddReader();
      DWGetNumReaders(&this->num_readers);	
      DWSetActiveReader(this->num_readers);
      this->isLoaded = true;
    }
    else
    {
      ERR("Couldn't load DWDataReaderLib");
      this->isLoaded = false;
    }  
  }
  catch(const exception& e)
  {
    ERR("Error from constructor DWDataReader::DWDataReader")
    ERR(e.what());
    forward_exception_to_r(e);
  }
}

DWDataReader::~DWDataReader()
{
  DEBUG("Unloading DWDataReader class (~DWDataReader())");
  
  this->unloadDWLib();
}

bool DWDataReader::loadDWLib()
{
  DEBUG("Called DWDataReader::loadDWLib()");
  
  if(!this->isLoaded)
  {
    DEBUG(DLL_PATH);
    
    if(LoadDWDLL(DLL_PATH))
    {
      DWInit();
      
      this->isLoaded = true;
      return true;
    }
  }
  
  return false;
}

bool DWDataReader::unloadDWLib()
{
  DEBUG("Called DWDataReader::unloadDWLib()");
  
  if(this->isLoaded)
  {
    this->isLoaded = false;
    this->num_readers = 0;
    
    DWDeInit();
    CloseDWDLL();
    
    DEBUG("Unloading DWDataReaderLib OK");
    
    return true;
  }
  
  return false;
}

bool DWDataReader::openFile(std::string filepath)
{
  DEBUG("Called DWDataReader::openDXD(std::string filepath)");
  
  this->filepath = filepath;
  
  if(this->isLoaded) 
  {
    if (boost::filesystem::exists(filepath))
    {
      if(DWOpenDataFile((char*)filepath.c_str(), &this->fileInfo) == DWSTAT_OK)
      {
        DEBUG("Opening DXD File OK");
        
        return true;
      }
    }
    
    ERR("DXD File doesn't exist");
    DEBUG("DXD File doesn't exist");
    
  }
  
  DEBUG("Opening DXD File NOK");
  
  return false;
}

Datetime DWDataReader::getAbsoluteTimeFromSec(double start_time, double sample_time_sec)
{
  DEBUG("Called DWDataReader::getAbsoluteTimeFromSec(double start_time, double sample_time_sec)");
  
  try
  {
    // We use boost functions to format the days stored in the DXD file (since 1899-12-30) in a readable format.
    // Setting digits precision to 16 (otherwise we can't get correct value for timestamp)
    Function options("options");
    options(Named("digits")=16);
    
    double days, sec, msec, usec, tmp;
    
    boost::posix_time::ptime start(boost::gregorian::date(1899,12,30));
    
    tmp = modf(start_time,&days)*24*3600+sample_time_sec;
    modf(tmp,&sec);
    
    tmp = modf((tmp-sec)*1000,&msec);
    modf(tmp*1000,&usec);
    
    boost::posix_time::ptime timestamp = start + boost::gregorian::days(static_cast<long>(days)) + boost::posix_time::seconds(static_cast<long>(sec)) + boost::posix_time::milliseconds(static_cast<long>(msec)) + boost::posix_time::microseconds(static_cast<long>(usec));

    std::string formated_timestamp = boost::posix_time::to_iso_extended_string(timestamp);

    boost::algorithm::replace_all(formated_timestamp,"T"," ");
    
    return Datetime(formated_timestamp);
  }
  catch(const exception& e)
  {
    ERR("Error from DWDataReader::getAbsoluteTimeFromSec")
    ERR(e.what());
    forward_exception_to_r(e);
    return Datetime();
  }
}

DataFrame DWDataReader::getFormatedFileInfo()
{
  DEBUG("Called DWDataReader::getFormatedFileInfo()");
  
  if(this->isLoaded)
  {
    std::stringstream tmpStr;
    
    // Setting digits precision to 16 (otherwise we can't get correct value for timestamp)
    Function options("options");
    options(Named("digits")=16);
    
    double days = this->fileInfo.start_store_time;
    
    Datetime timestamp = getAbsoluteTimeFromSec(days, 0);
    DatetimeVector timeVector = DatetimeVector::create(timestamp);

    DataFrame df = DataFrame::create(Named("duration")=NumericVector({this->fileInfo.duration}),
                                     Named("sample_rate")=NumericVector({this->fileInfo.sample_rate}),
                                     Named("date")=timeVector,
                                     Named("days")=NumericVector({this->fileInfo.start_store_time})
                                    );

    return df;
  }
  
  ERR("No DXD file loaded");
  
  return DataFrame();
}

List DWDataReader::getChannels()
{
  DEBUG("Called DWDataReader::getChannels()");
  
  if(this->isLoaded)
  {
    try
    {
      DWGetNumReaders(&this->num_readers);
      
      if(this->num_readers>0)
      {
        DWSetActiveReader(this->num_readers);
        
        this->channelCount = DWGetChannelListCount();
        
        if(this->channelCount>0)
        {
          this->channelList = (struct DWChannel*)malloc(sizeof(struct DWChannel) * this->channelCount);
          DWGetChannelList(channelList);
          
          if(DWGetChannelList(this->channelList)!=DWSTAT_OK)
          {
            free(this->channelList);
            this->channelList = NULL;
            
            return List();
          }
          else
          {
            List listeVoie;
            
            for(int i=0;i<this->channelCount;i++)
            {
              listeVoie.push_back((std::string(this->channelList[i].name)));
            }
            
            free(this->channelList);
            this->channelList = NULL;
            
            return listeVoie;
          }
        }
      }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getChannels");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return List();
    }
  }
  
  return List();
}

bool DWDataReader::closeFile()
{
  DEBUG("Called DWDataReader::closeFile()");
  
  if(DWCloseDataFile() != DWSTAT_OK)
  {
    return false;  
  }
  
  return true;
}

double DWDataReader::getFileStoreTime()
{
  DEBUG("Called DWDataReader::getFileStoreTime()");
  
  if(this->isLoaded)
  {
    return this->fileInfo.start_store_time;
  }
  
  return 0;
}

std::vector<std::pair<std::string, int> > DWDataReader::getChannelsNameAndIndex()
{
  DEBUG("Called DWDataReader::getChannelsNameAndIndex()");
  
  if(this->isLoaded)
  {
    try
    {
      DWGetNumReaders(&this->num_readers);
      
      if(this->num_readers>0)
      {
        DWSetActiveReader(this->num_readers-1);
        
        this->channelCount = DWGetChannelListCount();
        
        if(this->channelCount>0)
        {
          this->channelList = (struct DWChannel*)malloc(sizeof(struct DWChannel) * this->channelCount);
          DWGetChannelList(channelList);
          
          if(DWGetChannelList(this->channelList)!=DWSTAT_OK)
          {
            free(this->channelList);
            this->channelList = NULL;
            
            return std::vector<std::pair<std::string, int>>();
          }
          else
          {
            std::vector<std::pair<std::string, int> > listeVoieAvecIndex;
            
            for(int i=0;i<this->channelCount;i++)
            {
              listeVoieAvecIndex.push_back(std::make_pair(std::string(this->channelList[i].name),this->channelList[i].index));
            }
            
            free(this->channelList);
            this->channelList = NULL;
            
            return listeVoieAvecIndex;
          }
        }
      }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getChannelsNameAndIndex");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return std::vector<std::pair<std::string, int>>();
    }
  }
  
  return std::vector<std::pair<std::string, int>>();
}

int DWDataReader::findChannelIndexByName(const std::string& channelName)
{
  DEBUG("Called DWDataReader::findChannelIndexByName(const std::string& channelName)");
  
  if(this->isLoaded)
  {
    try
    {
      std::vector<std::pair<std::string,int>> listeVoiesAvecIndex;
      
      listeVoiesAvecIndex = this->getChannelsNameAndIndex();
      
      if(!listeVoiesAvecIndex.empty())
      {
        for(auto& voie:listeVoiesAvecIndex)
        {
          if(voie.first.compare(channelName)==0)
          {
            return voie.second;
          }
        }
      }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::findChannelIndexByName");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return -1;
    }
  }
  
  return -1;
}

__int64 DWDataReader::getChannelDataCount(const std::string& channelName)
{
  DEBUG("Called DWDataReader::getChannelDataCount(const std::string& channelName)");
  
  if(this->isLoaded)
  {
    try
    {
      int index = this->findChannelIndexByName(channelName);
      if(index>=0)return DWGetScaledSamplesCount(index);
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getChannelDataCount");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return 0;
    }
}
  
  return 0;
}

std::string DWDataReader::getChannelInfosEx(const std::string& channelName, const std::string& RegExp)
{
  DEBUG("Called DWDataReader::getChannelInfosEx(const std::string& channelName, const std::string& RegExp)");
  
  if(this->isLoaded)
  {
    try
    {
      int max_len = sizeof(int);
      void *buffer = malloc(max_len);
      
      if(buffer==NULL)
      {
        return std::string();
      }
      else
      {
        int index = this->findChannelIndexByName(channelName);
        
        if(DWGetChannelProps(index,DW_CH_XML_LEN,buffer,&max_len)!=DWSTAT_OK)
        {
          free(buffer);
          return std::string();
        }
        else
        {
          int xml_len = 0;
          xml_len = *(int*)buffer;
          free(buffer);
          
          buffer = malloc((xml_len+1)*sizeof(char));
          memset(buffer, 0, xml_len);
          
          if(buffer == NULL)
          {
            return std::string();
          }
          else
          {
            if(DWGetChannelProps(index,DW_CH_XML,buffer,&xml_len)!=DWSTAT_OK)
            {
              free(buffer);
              return std::string();
            }
            else
            {
              boost::regex re{RegExp};
              boost::smatch re_valeur;
              std::string re_str = std::string((char*)buffer);
              
                if(!boost::regex_search(re_str,re_valeur,re))
              {
                free(buffer);
                return std::string();
              }
              else
              {
                free(buffer);
                return re_valeur.str(1);
              }
            }
          }
        }
      }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getChannelInfosEx");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return NULL;
    }
  }
  
  return std::string();
}

struct DWChannel * DWDataReader::getChannelInfos(const std::string& channelName)
{
  DEBUG("Called DDWDataReader::getChannelInfos(const std::string& channelName)");
  
  if(isLoaded)
  {
    try
    {
        DWGetNumReaders(&this->num_readers);
        
        if(this->num_readers>0)
        {
          DWSetActiveReader(this->num_readers);
          
          this->channelCount = DWGetChannelListCount();
          
          if(this->channelCount>0)
          {
            this->channelList = (struct DWChannel*)malloc(sizeof(struct DWChannel) * this->channelCount);
            DWGetChannelList(channelList);
            
            if(DWGetChannelList(this->channelList)!=DWSTAT_OK)
            {
              free(this->channelList);
              this->channelList = NULL;
              
              return NULL;
            }
            else
            {
              struct DWChannel * tmpChannel = NULL;
              
              for(int i=0;i<this->channelCount;i++)
              {
                if(channelName.compare(std::string(this->channelList[i].name))==0)
                {
                  tmpChannel = (struct DWChannel*)malloc(sizeof(struct DWChannel));
                  *tmpChannel = this->channelList[i];
                  break;
                }
              }
              
              free(this->channelList);
              this->channelList = NULL;
              
              return tmpChannel;
            }
          }
        }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getArraySize");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return NULL;
    }
  }
  return NULL;
}

int DWDataReader::getArraySize(const std::string& channelName)
{
  DEBUG("Called DWDataReader::getArraySize(const std::string& channelName)");
  
  if(isLoaded)
  {
    try
    {
      struct DWChannel *tmpVoie = NULL;
      tmpVoie = this->getChannelInfos(channelName);
      
      if(tmpVoie != NULL)
      {
        int tailleArray = tmpVoie->array_size;
        free(tmpVoie);
        return tailleArray;
      }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getChannelInfos");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return 0;
    }
  }
  
  return 0;
}


bool DWDataReader::_getChannelData(const std::string& channelName, double * data, double * time_stamp)
{
  DEBUG("Called DWDataReader::_getChannelData(const std::string& channelName, double * data, double * time_stamp)");
  
  if(this->isLoaded)
  {
    try
    {
      int channelCount = this->getChannelDataCount(channelName);
        int index = this->findChannelIndexByName(channelName);
        
        if((channelCount>0) && (index>=0))
        {
          if(data != NULL && time_stamp != NULL)
          {
            if(DWGetScaledSamples(index, 0, channelCount, data, time_stamp)== DWSTAT_OK)
            {
              return true;
            }
          }
          else
          {
            return false;
          }
        }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::_getChannelData");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return false;
    }
  }
  
  return true;
}

DataFrame DWDataReader::getChannelData(std::string channelName)
{
  DEBUG("Called DWDataReader::getChannelData(std::string channelName)");
  
  if(this->isLoaded)
  {
    try
    {
        __int64 channelCount = this->getChannelDataCount(channelName);
        int arraySize = this->getArraySize(channelName);
        
        if(channelCount > 0 && arraySize > 0)
        {
          double * timestamp = (double*)malloc(sizeof(double)*channelCount);
          double * data = (double*)malloc(sizeof(double)*channelCount*arraySize);
          
          if(this->_getChannelData(channelName,data,timestamp))
          {
            NumericVector time;
            time = this->fromPointerToVector(timestamp, channelCount);
            
            NumericVector values;
            values =  this->fromPointerToVector(data, channelCount);
            
            DataFrame df = DataFrame::create(Named("timestamp")=clone(time),
                                             Named("data")=clone(values));
            
            free(timestamp);
            free(data);
            
            return df;
          }
          
          free(timestamp);
          free(data);
          
          DEBUG("No data found...")
          
        }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getChannelData");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return DataFrame();
    }
  }
  
  return DataFrame();
}

NumericVector DWDataReader::fromPointerToVector(double *pointer, __int64 size)
{
  DEBUG("Called DWDataReader::fromPointerToVector(double *pointer, __int64 size)");
  
  if(pointer != NULL && size >0)
  {
    try
    {
      std::vector<double> temp(pointer, pointer+size);
      
      return NumericVector(wrap(temp));
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::fromPointerToVector");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return NumericVector();
    }
      
  }
  
  return NumericVector();
}

int DWDataReader::getChannelSRDiv(const std::string& channelName)
{
  DEBUG("Called DWDataReader::getChannelSRDiv(const std::string& channelName)");
  
  if(this->isLoaded)
  {
    try
    {
      int max_len = sizeof(int);
      void *buffer = malloc(max_len);
      
      if(buffer==NULL)
      {
        return 0;
      }
      else
      {
        int index = this->findChannelIndexByName(channelName);
  
        if(DWGetChannelProps(index,DW_CH_XML_LEN,buffer,&max_len)!=DWSTAT_OK)
        {
          free(buffer);
          return 0;
        }
        else
        {
          int xml_len = 0;
          xml_len = *(int*)buffer;
          free(buffer);
          
          buffer = malloc((xml_len+1)*sizeof(char));
          memset(buffer, 0, xml_len);
          
          if(buffer == NULL)
          {
            return 0;
          }
          else
          {
            if(DWGetChannelProps(index,DW_CH_XML,buffer,&xml_len)!=DWSTAT_OK)
            {
              free(buffer);
              return 0;
            }
            else
            {
              boost::smatch SRDiv_valeur;
              std::string SRDiv_str = std::string((char*)buffer);
              if(!boost::regex_search(SRDiv_str,SRDiv_valeur,boost::regex("<SRDiv>([0-9]+)</SRDiv>")))
              {
                free(buffer);
                return 0;
              }
              else
              {
                free(buffer);
                return std::stoi(SRDiv_valeur.str(1));
              }
            }
          }
        }
      }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getChannelSRDiv");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return 0;
    }
  }
  
  return 0;
}

double DWDataReader::getChannelFrequency(const std::string& channelName)
{
  DEBUG("Called DWDataReader::getChannelFrequency(const std::string& channelName)");
  
  if(isLoaded)
  {
    try
    {
      struct DWChannel *tmpVoie = NULL;
      tmpVoie = this->getChannelInfos(channelName);
      
      if(tmpVoie != NULL)
      {
        int SRDiv = this->getChannelSRDiv(channelName);
        double frequence = (SRDiv>0?((this->fileInfo.sample_rate)/((double)SRDiv)):0);
        
        return frequence;
      }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getChannelFrequency");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return 0;
    }
  }
  
  return 0;
}

int DWDataReader::getChannelReducedDataCount(const std::string& channelName)
{
  DEBUG("Called DWDataReader::getChannelReducedDataCount(const std::string& channelName)");
  
  if(this->isLoaded)
  {
    int index = this->findChannelIndexByName(channelName);
    
    try
    {
      if(index>=0)
      {
        double block_size=0;
        
        int reduced_data_cnt;
        if(DWGetReducedValuesCount(index, &reduced_data_cnt, &block_size)==DWSTAT_OK)return reduced_data_cnt;
      }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getChannelReducedDataCount");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return -1;
    }
  }
  
  return -1;
}

std::string DWDataReader::iso_8859_1_to_utf8(std::string str)
{
  DEBUG("Called DWDataReader::iso_8859_1_to_utf8(const std::string& channelName)");
  
  std::string strOut;
  for (std::string::iterator it = str.begin(); it != str.end(); ++it)
  {
    uint8_t ch = *it;
    if (ch < 0x80) {
      strOut.push_back(ch);
    }
    else {
      strOut.push_back(0xc0 | ch >> 6);
      strOut.push_back(0x80 | (ch & 0x3f));
    }
  }
  return strOut;
}

bool DWDataReader::_getChannelReducedData(const std::string& channelName, double * data, double * time_stamp, REDUCED_DATA_TYPE reduced_type)
{
  DEBUG("Called DWDataReader::_getChannelReducedData(const std::string& channelName, double * data, double * time_stamp, REDUCED_DATA_TYPE reduced_type)");
  
  if(this->isLoaded)
  {
    try
    {
      int channelCount = this->getChannelReducedDataCount(channelName);
      int index = this->findChannelIndexByName(channelName);
      
      if((channelCount>0) && (index>=0))
      {
        if(data != NULL && time_stamp != NULL)
        {
          if(reduced_type==MIN_DATA)
          {
            if(DWGetReducedMinValues(index, 0, channelCount, data, time_stamp) == DWSTAT_OK)
            {
              return true;
            }
            else
            {
              return false;
            }
          }
          else if(reduced_type==MAX_DATA)
          {
            if(DWGetReducedMaxValues(index, 0, channelCount, data, time_stamp) == DWSTAT_OK)
            {
              return true;
            }
            else
            {
              return false;
            }
          }
          else if(reduced_type==AVERAGE_DATA)
          {
            if(DWGetReducedAveValues(index, 0, channelCount, data, time_stamp) == DWSTAT_OK)
            {
              return true;
            }
            else
            {
              return false;
            }
          }
          else if(reduced_type==RMS_DATA)
          {
            if(DWGetReducedRMSValues(index, 0, channelCount, data, time_stamp) == DWSTAT_OK)
            {
              return true;
            }
            else
            {
              return false;
            }
          }
          else
          {
            return false;
          }
        }
        else
        {
          return false;
        }
      }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::_getChannelReducedData");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return false;
    }
  }
  
  return true;
}

DataFrame DWDataReader::getChannelReducedData(std::string channelName, int reduced_type)
{
  DEBUG("Called DWDataReader::getChannelReducedData(std::string channelName, int reduced_type)");
  
  if(this->isLoaded)
  {
    try
    {
      int channelCount = this->getChannelReducedDataCount(channelName);
      int arraySize = this->getArraySize(channelName);
      
      if(channelCount > 0 && arraySize > 0)
      {
        double * timestamp = (double*)malloc(sizeof(double)*channelCount);
        double * data = (double*)malloc(sizeof(double)*channelCount*arraySize);
        
        if(this->_getChannelReducedData(channelName,data,timestamp,(REDUCED_DATA_TYPE)reduced_type))
        {
          NumericVector time;
          time = this->fromPointerToVector(timestamp, channelCount);
          
          NumericVector values;
          values =  this->fromPointerToVector(data, channelCount);
          
          DataFrame df = DataFrame::create(Named("timestamp")=clone(time),
                                           Named("data")=clone(values));
          
          free(timestamp);
          free(data);
          
          return df;
        }
        
        free(timestamp);
        free(data);
        
        DEBUG("No data found...")
      }
    }
    catch(const std::exception& e)
    {
      ERR("Error from DWDataReader::getChannelReducedData");
      ERR(e.what());
      forward_exception_to_r(e);
      
      return DataFrame();
    }
  }
  
  return DataFrame();
}

