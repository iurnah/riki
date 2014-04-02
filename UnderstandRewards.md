REWARDS source code
===
## *run.sh*

The script take command line options and run REWARDS either in debug mode or normal mode.
in debug mode ``../../../ia32/bin/pinbin -pause_tool 10 -t ./REWARDS -- $*``, in normal mode: ``../../../pin -t ./REWARDS $mem $instr -- $*``.

This comes down to how the ``./REWARDS`` implemented.

## *Makefile* and _Makefile.rules_

	#Makefile
	ifdef PIN_ROOT
	CONFIG_ROOT := $(PIN_ROOT)/source/tools/Config
	else
	CONFIG_ROOT := ../Config
	endif
	include $(CONFIG_ROOT)/makefile.config
	
	include $(TOOLS_ROOT)/Config/makefile.default.rules
	include makefile.rules

### *Makefile.rules*

The contents are mainly about how to config pin-tools and the command to compile REWARDS is only one line.

## *rewards.h*

The file is a modification of files provided by pin. The background about how pin can instrument instruction is necessary to understand the code. The following function may related to the core of REWARDS.


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
	void ParseSyscallArgument(ADDRINT num, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3, 	ADDRINT arg4, ADDRINT arg5, ADDRINT ip);
	void SetSysCallReturnType(ADDRINT num, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3, ADDRINT arg4, ADDRINT arg5, ADDRINT ret, ADDRINT ip);
	VOID SysAfter(THREADID tid, ADDRINT ip, ADDRINT ret, ADDRINT num, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3, ADDRINT arg4, ADDRINT arg5);
	VOID SysBefore(THREADID tid, ADDRINT ip, ADDRINT num, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3, ADDRINT arg4, ADDRINT arg5);
	void DumpSpace(FILE *fp);

## *rewards.cpp*

This file is a tool that generate instruction trace with values, it also including some debugging functionality. The main function is as following:

	PIN_InitSymbols();

	if (PIN_Init(argc, argv)) {
		return Usage();
	}

	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "--")) {
			strcpy(ImageName, ParseImageName(argv[i + 1]));
			break;
		}
	}

	init_shadow_memory();

	msglevel = 10;

	PID = PIN_GetTid();

	sprintf(filename_prefix, "%s-%d", ImageName, PID);

	sprintf(temp_file_name, "mkdir %s", filename_prefix);
	system(temp_file_name);

	sprintf(temp_file_name, "%s/semantic.out", filename_prefix);
	semantic_file = fopen(temp_file_name, "w");

	sprintf(temp_file_name, "%s/syscall.out", filename_prefix);
	syscall_file = fopen(temp_file_name, "w");

	sprintf(temp_file_name, "%s/alltrace.out", filename_prefix);
	fptrace = fopen(temp_file_name, "w");
	control.RegisterHandler(Handler, 0, FALSE);
	control.Activate();

	TRACE_AddInstrumentFunction(Trace, 0);
	PIN_AddContextChangeFunction(OnSig, 0);

	IMG_AddInstrumentFunction(ImageLoad, 0);

	INS_AddInstrumentFunction(SetupDataflow, 0);
	setup_hook();

	PIN_AddThreadStartFunction(ThreadStart, 0);
	PIN_AddThreadFiniFunction(ThreadFini, 0);

	PIN_AddSyscallEntryFunction(SyscallEntry, 0);
	PIN_AddSyscallExitFunction(SyscallExit, 0);

	filter.Activate();
	//icount.Activate();

	PIN_AddFiniFunction(Fini, 0);
	// Never returns

	sprintf(map_fname, "/proc/%d/maps", PID);
	ProcessMapFile(map_fname);

	PIN_StartProgram();

	return 0;

## *image_lodad.c*

The function ``ParseImageName()`` calling from ``main`` is defined in this file. This file also have lots of other functions that readout memory maps like what ``init_env`` did in the s2e tools set. Functions in this file can locate the stack, heap, etc. (``ParsemapLine``) in the memory region of the particular process. For example, it decleared the following variables. 
	
	uint32_t bin_heap_start_addr;
	uint32_t bin_heap_end_addr;
	uint32_t bin_stack_start_addr = 0xffffffff;
	uint32_t bin_stack_end_addr;
	uint32_t bin_vdso_start_addr;
	uint32_t bin_vdso_end_addr;

Other important functions defined in this file including:

	DumpDataSpace(FILE * fp);
	DumpVDSOSpace(FILE * fp);
	DumpCodeSpace(FILE * fp);
	DumpSpace(FILE * fp);

## *shadow_mem.c*

This file is contain the ``init_shadow_memory(void)``, which creates the shadow memory with the ``PAGE_SIZE``. ``#define PAGE_SIZE 65536``, we see the page size is 64KB.






