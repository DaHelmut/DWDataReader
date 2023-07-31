# DWDataReader
R package to import data from [DEWESoft](https://dewesoft.com/) data files (DXD, DXZ extensions).

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

**/!\ Be aware that you may need  to copy the DEWESoft Data Reader Library DLLs in the working directory to make it work.**

You can now get access to the data inside the file.

`lib$getChannels()`

`lib$getChannelReducedData("channelName",3)`

`lib$getChannelData("channelName")`

Please have a look at the file **test.R** in the **test** directory to have more informations about all the available methods.

# Debug
You can have a better understanding of how the library works by defining the **`SHOW_DEBUG_INF`** FLAG in **DWDataReader.hpp** in case of a crash or freeze.

# Credits
Thanks to [Rcpp package](https://cran.r-project.org/web/packages/Rcpp/index.html).

Thanks to [Dirk Eddelbuettel](https://github.com/eddelbuettel) one of the creator / maintainer of the Rcpp package, who is very active on forums.

Thanks to DEWESoft for sharing the [DEWESoft Data Reader Library](https://dewesoft.com/download/developer-downloads)
