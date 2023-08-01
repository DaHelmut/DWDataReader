# For ploting data, not mandatory
library(plotly)

library(DWDataReader)
# new object instantiating (automatically load the "DWDataReaderLib)
lib <- new(DWDataReader)

# We open the data file (don't forget the extra(s) '\' in the path due to std::string type)
lib$openFile("C:\\test.dxd")

# We get the start date / time of acquisition in days
days <- lib$getFileStoreTime()

# We convert it to readable format by setting the digits visible to 16 and formating it with POSIXlt
options(digits=16)
format(as.POSIXlt(days*86400,origin="1899-12-30 00:00:00.000000",tz="UTC"),"%Y-%m-%d %H:%M:%OS6")

# We get all the channels
channels <- lib$getChannels();

# We get the channel "ACC1" unit with getChannelInfosEx using a RegExp
unit_ACC1 <- lib$getChannelInfosEx("ACC1","<Unit>(.*)</Unit")

# We get the channel "ACC2" description with getChannelInfosEx using a RegExp
desc_ACC2 <- lib$getChannelInfosEx("ACC2","<Description>(.*)</Description>")

# We get the frequency of the channel "ACC2"
lib$getChannelFrequency("ACC2")

# Let's get the reduced RMS data of ACC1 over time (10 Hz), 2nd parameter = 4 means "RMS" (1 = Min, 2 = Max, 3 = average, 4 = RMS)
ACC1_slow <- lib$getChannelReducedData("ACC1",4)

# Let's get the reduced average data of ACC1 over time (10 Hz),  2nd parameter = 4 means "RMS" (1 = Min, 2 = Max, 3 = average, 4 = RMS)
ACC2_slow <- lib$getChannelReducedData("ACC2",3)

fig_slow <- plot_ly(ACC1_slow, type = 'scatter', mode = 'lines')%>%add_trace(x = ACC1_slow$timestamp, y = ACC1_slow$data, name = 'ACC1')%>%add_trace(x = ACC2_slow$timestamp, y = ACC2_slow$data, name = 'ACC2')
fig_slow

# Let's get the fast data of ACC1 over time (20 000 Hz)
ACC1_fast <- lib$getChannelData("ACC1")

# Let's get the fast data of ACC1 over time (2500 Hz) 
ACC2_fast <- lib$getChannelData("ACC2")

# We plot fast data
fig_fast <- plot_ly(ACC1_fast, type = 'scatter', mode = 'lines')%>%add_trace(x = ACC1_fast$timestamp, y = ACC1_fast$data, name = 'ACC1')%>%add_trace(x = ACC2_fast$timestamp, y = ACC2_fast$data, name = 'ACC2')
fig_fast

# Let's close the file
lib$closeFile()

# Let's unload the library
lib$unloadDWLib()

