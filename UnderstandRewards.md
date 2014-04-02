REWARDS source code
===
## *run.sh*

The script take command line options and run REWARDS either in debug mode or normal mode.
in debug mode ``../../../ia32/bin/pinbin -pause_tool 10 -t ./REWARDS -- $*``, in normal mode: ``../../../pin -t ./REWARDS $mem $instr -- $*``.

This comes down to how the ``./REWARDS`` implemented.

## *Makefile* and Makefile.rules

	#Makefile
	ifdef PIN_ROOT
	CONFIG_ROOT := $(PIN_ROOT)/source/tools/Config
	else
	CONFIG_ROOT := ../Config
	endif
	include $(CONFIG_ROOT)/makefile.config
	
	include $(TOOLS_ROOT)/Config/makefile.default.rules
	include makefile.rules

### *Makefile.rules

The contents are mainly about how to config pin-tools and the command to compile REWARDS is only one line.

## *rewards.cpp*

The file is a modification of files provided by pin. The background about how pin can instrument instruction is necessary to understand the code. The following function may related to the core of REWARDS.

```
void DumpGlobalDataType();
typedef void (*InstrumentFunction)(INS ins, void *v);
extern InstrumentFunction instrument_functions[XED_ICLASS_LAST]; //instrumented functions
VOID ImageRoutineReplace(IMG img, VOID *v);
VOID ImageLoad(IMG img, VOID *v);
char * ParseImageName(char * imagename);
VOID ImageLoad_Setup_ReplaceMent(IMG img, VOID *v);
VOID ProcessMapFile(char* fname);
VOID ParseMapLine(char * line, int c);
void setup_hook();
void SetupDataflow(INS ins, void *v);
extern std::ofstream out;
void insert_dump_list(ADDRINT addr);
void InsertToSocketPool(int sockfd);
void DeleteFromSocketPool(int sockfd);
int IsSockFD(int sockfd);
VOID SyscallEntry(THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std, VOID *v);
VOID SyscallExit(THREADID threadIndex, CONTEXT *ctxt, SYSCALL_STANDARD std, VOID *v);
void ParseSyscallArgument(ADDRINT num, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3, ADDRINT arg4, ADDRINT arg5, ADDRINT ip);
void SetSysCallReturnType(ADDRINT num, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3, ADDRINT arg4, ADDRINT arg5, ADDRINT ret, ADDRINT ip);
VOID SysAfter(THREADID tid, ADDRINT ip, ADDRINT ret, ADDRINT num, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3, ADDRINT arg4, ADDRINT arg5);
VOID SysBefore(THREADID tid, ADDRINT ip, ADDRINT num, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3, ADDRINT arg4, ADDRINT arg5);
void DumpSpace(FILE *fp);
```







