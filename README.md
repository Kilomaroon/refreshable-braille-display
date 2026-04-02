# refreshable-braille-display

## Background
In 2016, [Statistics Canada](https://www150.statcan.gc.ca/n1/en/catalogue/89-654-X) found that nearly 3% of Canadians, aged 15 years and older, identify as having a vision impairment which limits their daily activities. Within this group, 5.8% are legally blind. The same survey found that the same demographic had overall lower levels of educational attainment compared with those who did not have any disability. More recently, the [2019 Canadian Income Survey](https://www150.statcan.gc.ca/n1/daily-quotidien/230627/dq230627b-eng.htm) reported that Canadians aged 16 and older with disabilities earned an average of $11,500 less per year than those without disabilities ($43,400 versus $55,200). These statistics highlight significant barriers to literacy and employment for individuals with vision impairment.

Everyone deserves the opportunity to read and access information, regardless of visual ability or financial circumstance, however, commercial braille readers remain prohibitively expensive for many individuals. Braille readers allow blind users to read and write text directly through touch, promoting true literacy and independence that audio devices alone cannot provide. Currently available [commercial braille readers](https://specialneedscomputers.ca/products/orbit-reader-20-plus) typically range from $2800 to [over $4000 CAD](https://store.humanware.com/hca/mantis-q40.html). 

**The purpose of this project is to develop a Refreshable Braille display that is more affordable. All files are available to anyone and we encourage people to modify and improve upon it.**

## Functionality
The user creates a text file called `text.txt` on a micro SD card and plugs it into the external memory card reader. The user then uses the forward and backward input buttons can read the resulting output on the braille cells. The device is battery operated and it is easy to change the number of cells via editing the code in `firepins.c`.

## Design
The program consists of a basic implementation for reading a FAT32 cluster from an SD card connected to the system. It then stores this cluster in a buffer and translate it to a binary encoding for the braille pins. The program currentl supports ascii characters a-z,A-Z, 0-9, and a handful of special characters. Future improvement for this project would include a more memory-effective encoding for the braille cells and implementation of [elm-chan's FATFS](https://elm-chan.org/fsw/ff/).

The PCB, braille cell models, and casing files can also be found here, and can be modified if desired. The braille cell design is based on [this project](https://hackaday.com/tag/refresh/). Several modifications had to be made to the original design based on cost, feasibility, and project availability.
