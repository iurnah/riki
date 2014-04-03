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

The function ``ParseImageName()`` calling from ``main`` is defined in this file. This file also have lots of other functions that read memory maps like what ``init_env`` did in the s2e tools set. Functions in this file can locate the stack, heap, etc. (``ParsemapLine()``) in the memory region of the particular process. For example, it decleared the following variables. 
	
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

This file is contain the ``init_shadow_memory(void)``, which creates the shadow memory with the ``PAGE_SIZE``, which the author define ``#define PAGE_SIZE 65536``, so the page size is 64KB. 

This file also definded the following functions:

	alloc_secondary_map()
	get_mem_ins_addr()
	set_mem_ins_addr()
	get_mem_time_stamp()
	set_men_time_stamp()
	set_mem_shadow_tag()
	get_mem_shadow_tag()

Try to relate those functio to the discussion of how REWARDS backtrack algorithm maintain different set 
of typed and untyped memory region.

## *rewards_helper.cpp*

This file define some of the functions that have extensively called by ``xxx_hook.c``. Those function doing the taging for the memory. For example, function ``void SetRegTag(REG reg, reg_tag_t * reg_src)`` set the register tags. The structure defined as 

```
typedef struct _reg_reg_tag_t {
	uint32_t pc;
	uint32_t time_stamp;
	uint32_t imm;
} reg_tag_t;
```

Other functions defined:

```
void resolve_reg_tag_type(reg_tag_t * t, T_TYPE tt);
void resolve_mem_tag_type(mem_tag_t * t, T_TYPE tt);
void resolve_arg_tag_type(ADDRINT addr, mem_tag_t * t, T_TYPE tt);
void UpdateTimeStamp();
void GetRegTag(REG reg, reg_tag_t * reg_src, ADDRINT pc);
void SetRegTag(REG reg, reg_tag_t * reg_src);
void MovRegToReg(REG reg, reg_tag_t * reg_src);
void MovMemToReg(REG reg, reg_tag_t * reg_src);
void MovImmToReg(REG reg, ADDRINT imm, ADDRINT pc);
void GetMemTag(ADDRINT addr, ADDRINT size, reg_tag_t * reg_src, ADDRINT pc);
void SetPushRegTag(ADDRINT addr, REG reg, reg_tag_t * reg_src, ADDRINT pc);
void SetPushImmTag(ADDRINT addr, ADDRINT imm, ADDRINT pc);
void SetPushMemTag(ADDRINT addr, reg_tag_t * reg_src, ADDRINT pc);
void SetMemTag(ADDRINT addr, ADDRINT size, mem_tag_t * reg_src, ADDRINT pc);
void MovRegToMem(ADDRINT addr, ADDRINT size, reg_tag_t * reg_src, ADDRINT pc);
void MovMemToMem(ADDRINT addr, ADDRINT size, reg_tag_t * reg_src, ADDRINT pc);
void MovImmToMem(ADDRINT addr, ADDRINT size, ADDRINT imm, ADDRINT pc);
void SetLeaAddr(ADDRINT ip, ADDRINT val2, REG reg);
void UnionTwoTags(reg_tag_t * d, reg_tag_t * s);
void SetEaxTagType(T_TYPE t, ADDRINT pc);
void SetFunPointerType(ADDRINT value, reg_tag_t * src);
void SetInputBufferTagTypes(ADDRINT addr, int size, T_TYPE tag);
void SetArg0TagType(T_TYPE t);	
void SetArg1TagType(T_TYPE t);
void SetArg2TagType(T_TYPE t);
void SetArg3TagType(T_TYPE t);
void SetArg4TagType(T_TYPE t);
void SetArg5TagType(T_TYPE t);
void CheckPushEBP(VOID * ip, REG reg, ADDRINT val, ADDRINT esp);
void SetMOVSBTag(VOID * ip, ADDRINT addrr, ADDRINT addrw);
void SetMOVSWTag(VOID * ip, ADDRINT addrr, ADDRINT addrw);
void SetMOVSDTag(VOID * ip, ADDRINT addrr, ADDRINT addrw);
```
The functions with the format `SetArgXTagType()` above is actually try to match the register value with the system call arguments. The types are hard coded in the function `void ParseSyscallArgument(ADDRINT num, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3, ADDRINT arg4, ADDRINT arg5, ADDRINT, ip)`, and thusly, type the register and so on.

This moment we have much knowledge about how rewards propagate type messages. See the explanation in the **sys_hook.c** section.


## *rewards_helper.h*

Other than the above mentioned functions decleared for ``rewards_helper.cpp``, it also define functions for ``api_hook.c``. They are 

```
VOID RetOneMalloc(ADDRINT eip, ADDRINT eax);
VOID RetOneChar(ADDRINT eip, ADDRINT eax);
VOID RetOneStr(ADDRINT eip, ADDRINT eax);
VOID RetOneWchar(ADDRINT eip, ADDRINT eax);
VOID RetOneFD(ADDRINT eip, ADDRINT eax);
VOID RetOneFile(ADDRINT eip, ADDRINT eax);
VOID RetOneInt(ADDRINT eip, ADDRINT eax);
VOID RetOnePID(ADDRINT eip, ADDRINT eax);
VOID RetOnePointerChar(ADDRINT eip, ADDRINT eax);
VOID RetOnePointerInt(ADDRINT eip, ADDRINT eax);
VOID RetOnePointerVoid(ADDRINT eip, ADDRINT eax);
VOID RetOneAddrinfo(ADDRINT eip, ADDRINT eax);
VOID RetOneUtsname(ADDRINT eip, ADDRINT eax);
VOID RetOneTime(ADDRINT eip, ADDRINT eax);
VOID RetOneTimeb(ADDRINT eip, ADDRINT eax);
VOID RetOneTm(ADDRINT eip, ADDRINT eax);
VOID RetOneSize(ADDRINT eip, ADDRINT eax);
VOID RetOneFpos(ADDRINT eip, ADDRINT eax);
VOID RetOneOff(ADDRINT eip, ADDRINT eax);
VOID RetOneVoid(ADDRINT eip, ADDRINT eax);
```

## *api_hook.c*

TODO:read the ``VOID ImageRoutineReplace(IMG img, VOID * v)``

Library implementation of the functions that can hook and mark the memory region, it is a explicit implementation of the various functions corresponds to library functions. It also implement the instrumentation routines function `` VOID ImageRoutineReplace(IMG img, VOID * v)``. It is a very big function, read it through. It is important!!!

## *inst_hook.cpp*

There is a comment talking about how instructions are modeled and taint is propagated. Should focus on this file also. Pay attention to the Comment in the line of 149.

## *malloc_hook.c*

Some of the function have not been called by Rewards, such as ``FreeBefore()``, some of them are called by ``api_hook.c`` 

```
RTN_InsertCall(mallocRtn, IPOINT_AFTER, (AFUNPTR) MallocAfter,
	       IARG_FUNCRET_EXITPOINT_VALUE,
	       IARG_REG_VALUE, REG_ESP, IARG_END);
```

## *sys_hook.c*

Declared a syscall table contain 228 syscalls, with syscall number as the key, syscall name as the value. 
It also define the function `void ParseSyscallArgument(ADDRINT num, ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3, ADDRINT arg4, ADDRINT arg5, ADDRINT, ip)`, which is the key to tag the register type. For example, when the system hook a system call and find it is read, it goto the following case

```
...
case 3:			//read
	SetArg0TagType(T_FILE_FD);
	SetArg1TagType((T_TYPE)(T_CHAR|T_PTR));
	SetArg2TagType(T_SIZE_T);
	break;
...
```

Each argument to read is hard coded to the type of the read system call. The generic linux read system call is ``ssize_t read(int fd, void *buf, size_t count)``. With the ``SetArgXTagType`` function, which is implemented as following:

```
void SetArg0TagType(T_TYPE t)
{
	reg_tag_t tag;
	GetRegTag(LEVEL_BASE::REG_EBX, &tag, -1);
	resolve_reg_tag_type(&tag, t);
}
```

in which it called ``GetRegTag`` to tag the register EBX of the instrumented register. Here is the ``GetRegTag`` implementation:

```
void GetRegTag(REG reg, reg_tag_t * reg_src, ADDRINT pc)
{
	if (regTagMap[(uint32_t) reg].pc == 0) {
		fprintf(fptrace, "D 0x%08x 0x%08x\n", pc, time_stamp_g);
		regTagMap[(uint32_t) reg].pc = pc;
		regTagMap[(uint32_t) reg].time_stamp = time_stamp_g;
		regTagMap[(uint32_t) reg].imm = 0;
	}
	(*reg_src).pc = regTagMap[(uint32_t) reg].pc;
	(*reg_src).time_stamp = regTagMap[(uint32_t) reg].time_stamp;
	(*reg_src).imm = regTagMap[(uint32_t) reg].imm;

#ifdef DEBUG_REWARDS
	fprintf(fptrace, "Get REG %s [ %x %x ]\n", REG_StringShort(reg).c_str(),
		reg_src->pc, reg_src->time_stamp);
#endif
}
```

it also called the ``resolve_reg_tag_type`` function, which is a single line of function that print the corresponds address and type. This how reward tag registers. 

**TODO: where is the backtrack algorithm implemented, i.e. how the recursive type resolve implemented is still not clear.**

## *sys_helper.c*

## *types.c*

Function that resolve the data type, see `types.h`

## *types.h*

Declear the enum `c_program_type_t` and the following functions:

	char GetTypeNameByType(T_TYPE t);
	void ResolveStructType(T_TYPE t, unsigned int virt_addr);
	void ResolveStructTypeArg(T_TYPE t, unsigned int esp);


**After reading source for the first day, I guess that the implementation of typing is based on instruction taint and typing function to finally resolve the data type in memory. Of course the instrumentation is facilitated by Pin tool. which is the focus of the second day.**





