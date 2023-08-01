# DWDataReader
R package to import data from [DEWESoft](https://dewesoft.com/) data files (dxd, dxz extensions).

![Screenshot of the package used in RStudio](/test/screenshot.png)

## Description
This package is based on the  [DEWESoft Data Reader Library](https://dewesoft.com/download/developer-downloads).
With this package you can import reduced (Min, Max, Average, RMS) and fast data from analog or digital channels.

Note : this version can only import simple analog / digital data (no matrix like available with FFT).

## Installation
### Prerequesites
+ R (>= 4.3.1)
+ Rcpp must be downloaded and installed.
+ devtools must be downloaded and installed.
+ [DEWESoft Data Reader Library](https://dewesoft.com/download/developer-downloads) (>= 4.2.0.24) must be downloaded and DLLs should be copied in the working directory. 

### Procedure
The easiest way to install it is to use **RStudio** along with devtools and Rcpp.

`install.packages(devtools)`

`install.packages(Rcpp)`

Open the project in RStudio, then, click on **Build** » **Install package**.

Once it is built & installed, the following line shouldn't give any error :

`library(DWDataReader)`

## Usage

To use the library, just create a new object as usual :

`lib <- new(DWDataReader)`

Then open the file you want to access (an example file is available in the **test** directory) :

`# Please take care of the extra '\' in the filepath`

`lib$openFile("C:\\test.dxd")`

**/!\ Be aware that you may need to copy the DEWESoft Data Reader Library DLLs in the working directory to make it work.**

You can now get access to the data inside the file.

`lib$getChannels()`

`lib$getChannelReducedData("channelName",3)`

`lib$getChannelData("channelName")`

Please have a look at the file **[test.R](/test/test.R)** in the **test** directory to have more informations about all the available methods.

# Debug
You can have a better understanding of how the library works by defining the **`SHOW_DEBUG_INF`** FLAG in **DWDataReader.hpp** in case of a crash or freeze.

# Credits
Thanks to [Rcpp package](https://cran.r-project.org/web/packages/Rcpp/index.html).

Thanks to [Dirk Eddelbuettel](https://github.com/eddelbuettel) one of the creator / maintainer of the Rcpp package, who is very active on forums.

Thanks to DEWESoft for sharing the [DEWESoft Data Reader Library](https://dewesoft.com/download/developer-downloads)

# Example
`# For ploting data, not mandatory
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
lib$unloadDWLib()`
