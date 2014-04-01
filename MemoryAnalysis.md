Author: Rui Han Email: iurnah@gmail.com
================================

This repo is for my reseach paper reading notes. I found it is better to understand the
paper by writing a summary. So I kept them all here for future reference and
showcase. Yes, showcase. 

DFRWS 2005 Forensics Challenge
===
---------------------------------------------
Report & Answers by Chris Betz
---
It recovered strings and process names across the two dump file. the results is a list of PID, process name, and process path on disk. It found several suspectable processes [dfrws2005](#), [UMGR32(BO2K)](#), and [nc.exe](#). Hacker Defender used to hook selected Windows API calls.

Chris use [MAC times](#) to identify whether the important file has been stealed. This could be a very good hint in memory foriencis. It also make use of [Back Orifice 2000](#https://sourceforge.net/projects/bo2k/) in figuring out the memory contents (parsed file names and network information(IP, ports, etc.) hiden in the malicious process. 

the parsed format looking like this:


```
Process Page-in       String
Dump Address
000DF054 000DF054         memory examination challenge
000DF080 000DF080         dfrws2005.ini
000DF09C 000DF09C         DRIVERNAME=DFRWSDRV2$
000DF0C0 000DF0C0         [Hidden Tabl@
000DF0DC 000DF0DC         [Hidden Tabl\
000DF0F8 000DF0F8         [HIDDEN TABLE]
000DF114 000DF114         SERVICEDISPLAYNAME=DFRWS2005 CHALLEN4
000DF148 000DF148         [Root Processes]T
000DF168 000DF168         [Root Processes]t
000DF188 000DF188         [ROOT PROCESSES]
000DF1A8 000DF1A8         d<r>f<w>:s<*
000DF1C4 000DF1C4         drfw0
000DF1D8 000DF1D8         drfwD
000DF1EC 000DF1EC         DRFWS*
000DF200 000DF200         <\r\c:\m\d.\e\x\x
000DF220 000DF220         rcmd.exe
000DF238 000DF238         RCMD.EXE
000DF250 000DF250         <n|c.ex\
000DF290 000DF290         NC.EXE
000DF2A4 000DF2A4         [Hidden Services 
000DF2C4 000DF2C4         [Hidden Services@
000DF2E4 000DF2E4         [HIDDEN SERVICES]
000DF304 000DF304         DriverName=DFRWSDRV2005
000DF32C 000DF32C         DFRW<
000DF340 000DF340
000DF358 000DF358         [Hidden RegKeys]
000DF378 000DF378         [Hidden RegKeys]@
000DF398 000DF398         [HIDDEN REGKEYS]
000DF3CC 000DF3CC         D:"FR<WS2\00
000DF3E8 000DF3E8         DFRWS2004
000DF400 000DF400         DFRWS200L
000DF410 000DF410         DFRWS2005
000DF51C 000DF51C         DFRWS200h
000DF548 000DF548         LEGACY_DFRWS2005 
000DF568 000DF568         LEGACY_DFRWS2005@
000DF588 000DF588         LEGACY_DFRWS2005
000DF5A0 000DF5A0         LEGACY_DFRWS2005
000DF6AC 000DF6AC         DFRWSDRV2005
000DF6C8 000DF6C8         DFRWSDRV2005
000DF6E4 000DF6E4         DFRWSDRV2005T
000DF6F8 000DF6F8         DFRWSDRV2005
000DF804 000DF804         LEGACY_DFRWSDRV2 
000DF824 000DF824         LEGACY_DFRWSDRV2
000DF844 000DF844         LEGACY_DFRWSDRV2005
000DF864 000DF864         FRWSDRV2005
000DF968 000DF968         DFRWSDRV2005
000DF984 000DF984
.....

```

Reference
---
[1. Wiki: Computer Forensics](#http://en.wikipedia.org/wiki/Computer_forensics)

[2. MAC times](#http://en.wikipedia.org/wiki/MAC_times)
