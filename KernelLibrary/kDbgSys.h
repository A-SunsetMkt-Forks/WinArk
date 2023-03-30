#pragma once
#include "SysMon.h"
#include "kDbgCore.h"

extern "C"
NTSTATUS ObCreateObject(
	_In_opt_ KPROCESSOR_MODE ObjectAttributesAccessMode,
	_In_ POBJECT_TYPE Type,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_ KPROCESSOR_MODE AccessMode,
	_Inout_opt_ PVOID ParseContext,
	_In_ ULONG ObjectSize,
	_In_opt_ ULONG PagedPoolCharge,
	_In_opt_ ULONG NonPagedPoolCharge,
	_Out_ PVOID * Object
);

extern "C"
PIMAGE_NT_HEADERS RtlImageNtHeader(
	_In_ PVOID BaseAddress
);

extern "C"
NTSTATUS
PsReferenceProcessFilePointer(
	_In_ PEPROCESS Process,
	_Out_ PFILE_OBJECT* FileObject
);

extern "C"{
	NTSYSAPI NTSTATUS NTAPI ZwFlushInstructionCache(_In_ HANDLE 	ProcessHandle,
		_In_ PVOID 	BaseAddress,
		_In_ ULONG 	NumberOfBytesToFlush
	);
}


extern "C"
NTSTATUS ObDuplicateObject(
	_In_ PEPROCESS SourceProcess,
	_In_ HANDLE SourceHandle,
	_In_opt_ PEPROCESS TargetProcess,
	_Out_opt_ PHANDLE TargetHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ ULONG HandleAttributes,
	_In_ ULONG Options,
	_In_ KPROCESSOR_MODE PreviousMode
);

PEPROCESS PsGetCurrentProcessByThread(PETHREAD Thread);

using PPsGetNextProcessThread = PETHREAD(NTAPI*) (
	_In_ PEPROCESS Process,
	_In_ PETHREAD Thread
);

using PPsSuspendThread = NTSTATUS(NTAPI*) (
	_In_ PETHREAD Thread,
	_Out_opt_ PULONG PreviousSuspendCount
);

using PDbgkpSectionToFileHandle = HANDLE(NTAPI*) (
	_In_ PVOID SectionObject
);

using PPsResumeThread = NTSTATUS(NTAPI*) (
	_In_ PETHREAD Thread,
	_Out_opt_ PULONG PreviousSuspendCount
);

using PMmGetFileNameForAddress = NTSTATUS(NTAPI*) (
	_In_ PVOID ProcessVa,
	_Out_ PUNICODE_STRING FileName
);

using PDbgkpSuspendProcess = BOOLEAN(NTAPI*)();

using PKeThawAllThreads = VOID(NTAPI*)();

using PDbgkpResumeProcess = VOID(NTAPI*) (
	_In_ PEPROCESS Process
);

using PPsQuerySystemDllInfo = PPS_SYSTEM_DLL_INFO(NTAPI*)(_In_ ULONG Type);

using PPsCallImageNotifyRoutines = VOID(NTAPI*)(
	_In_ PUNICODE_STRING ImageName,
	_In_ HANDLE ProcessId,
	_In_ PVOID FileObject,
	_Out_ PIMAGE_INFO_EX ImageInfoEx);

using PObFastReferenceObject = PVOID(NTAPI*) (
	_In_ PEX_FAST_REF FastRef
);

using PExfAcquirePushLockShared = PVOID(NTAPI*)(
	_Inout_ PEX_PUSH_LOCK PushLock
);

using PExfReleasePushLockShared = PVOID(NTAPI*)(
	_Inout_ PEX_PUSH_LOCK PushLock
	);

using PObFastReferenceObjectLocked = PVOID(NTAPI*)(
	_In_ PEX_FAST_REF FastRef
	);

using PPsGetNextProcess = PEPROCESS (NTAPI*)(
	_In_ PEPROCESS Process
);

// �������Զ���
NTSTATUS 
NTAPI
NtCreateDebugObject(
	_Out_ PHANDLE DebugObjectHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_ ULONG Flags
);

// ��һ���Ѿ����еĽ��̽������ԻỰ
NTSTATUS
NtDebugActiveProcess(
	_In_ HANDLE ProcessHandle,
	_In_ HANDLE DebugObjectHandle
);

// ��һ�����Զ��󸽼ӵ������Խ�����
NTSTATUS
DbgkpSetProcessDebugObject(
	_In_ PEPROCESS Process,
	_In_ PDEBUG_OBJECT DebugObject,
	_In_ NTSTATUS MsgStatus,
	_In_ PETHREAD LastThread
);

/**
* Emulation System
* 
*/
// �������ϵͳ������ٵĽ��̴�����Ϣ
NTSTATUS DbgkpPostFakeProcessCreateMessages(
	_In_ PEPROCESS Process,
	_In_ PDEBUG_OBJECT DebugObject,
	_In_ PETHREAD* pLastThread
);

// �������ϵͳ������ٵ��̴߳�����Ϣ
NTSTATUS DbgkpPostFakeThreadMessages(
	PEPROCESS	Process,
	PDEBUG_OBJECT	DebugObject,
	PETHREAD	StartThread,
	PETHREAD* pFirstThread,
	PETHREAD* pLastThread
);

// �������ϵͳ������ٵ�ģ�������Ϣ
NTSTATUS DbgkpPostModuleMessages(
	_In_ PEPROCESS Process,
	_In_ PETHREAD Thread,
	_In_ PDEBUG_OBJECT DebugObject
);

//PVOID ObFastReferenceObjectLocked(
//	_In_ PEX_FAST_REF FastRef
//);
//
//PVOID ObFastDereferenceObject(
//	_In_ PEX_FAST_REF FastRef,
//	_In_ PVOID Object
//);

/**
* Event Collection Routine
*
*/
// �ɼ��̴߳����¼�
VOID DbgkCreateThread(
	PETHREAD Thread
);

// �߳��˳���Ϣ
VOID DbgkExitThread(
	NTSTATUS ExitStatus
);

// �����˳���Ϣ
VOID DbgkExitProcess(
	NTSTATUS ExitStatus
);

// ģ�����
VOID DbgkMapViewOfSection(
	_In_ PVOID SectionObject,
	_In_ PVOID BaseAddress,
	_In_ PEPROCESS Process
);

// ģ��ж��
VOID DbgkUnMapViewOfSection(
	_In_ PEPROCESS Process,
	_In_ PVOID BaseAddress
);

// ��һ�����Զ������Ϣ������׷�ӵ����¼�
NTSTATUS DbgkpQueueMessage(
	_In_ PEPROCESS Process,
	_In_ PETHREAD Thread,
	_Inout_ PDBGKM_APIMSG ApiMsg,
	_In_ ULONG Flags,
	_In_ PDEBUG_OBJECT TargetDebugObject
);


// �ȴ������¼�
NTSTATUS NtWaitForDebugEvent(
	_In_ HANDLE DebugObjectHandle,
	_In_ BOOLEAN Alertable,
	_In_opt_ PLARGE_INTEGER Timeout,
	_Out_ PDBGUI_WAIT_STATE_CHANGE WaitStateChange
);


// �ظ������¼����ָ������Խ���
NTSTATUS NtDebugContinue(
	_In_ HANDLE DebugObjectHandle,
	_In_ PCLIENT_ID AppClientId,
	_In_ NTSTATUS ContinueStatus
);

// ������ԻỰ
NTSTATUS NtRemoveProcessDebug(
	_In_ HANDLE ProcessHandle,
	_In_ HANDLE DebugObjectHandle
);

// ���͵����¼�
NTSTATUS DbgkpSendApiMessage(
	UCHAR	Flags,
	PDBGKM_APIMSG ApiMsg
);

//PSYSTEM_DLL_INFO PsQuerySystemDllInfo(
//	ULONG index
//);

VOID ExAcquirePushLockShared(_In_ PEX_PUSH_LOCK_S PushLock);

VOID ExReleasePushLockShared(_In_ PEX_PUSH_LOCK_S PushLock);

LOGICAL ExFastRefDereference(
	_Inout_ PEX_FAST_REF FastRef,
	_In_ PVOID Object
);

EX_FAST_REF
ExFastReference(
	_Inout_ PEX_FAST_REF FastRef
);

LOGICAL
ExFastRefAddAdditionalReferenceCounts(
	_Inout_ PEX_FAST_REF FastRef,
	_In_ PVOID Object,
	_In_ ULONG RefsToAdd
);

//PETHREAD PsGetNextProcessThread(
//	_In_ PEPROCESS Process,
//	_In_ PETHREAD Thread
//);

// ȡ��Section��Ӧ���ļ����
//HANDLE DbgkpSectionToFileHandle(
//	_In_ PVOID SectionObject
//);

//NTSTATUS PsSuspendThread(
//	_In_ PETHREAD Thread,
//	_Out_opt_ PULONG PreviousSuspendCount
//);

//NTSTATUS PsResumeThread(
//	_In_ PETHREAD Thread,
//	_Out_opt_ PULONG PreviousSuspendCount
//);

VOID DbgkSendSystemDllMessages(
	PETHREAD Thread,
	PDEBUG_OBJECT DebugObject,
	PDBGKM_APIMSG ApiMsg
);



//BOOLEAN DbgkpSuppressDbgMsg(
//	_In_ PTEB Teb
//);


// ��ǰ���̵��쳣�˿ڷ����쳣�ĵڶ��ִ������




// �ָ������Խ��̵�ִ��
VOID DbgkpResumeProcess(
	_In_opt_ PEPROCESS Process
);

// �������ϵͳ�����쳣��Ϣ
BOOLEAN DbgkForwardException(
	_In_ PEXCEPTION_RECORD ExceptionRecord,
	_In_ BOOLEAN DebugException,
	_In_ BOOLEAN SecondChance
);

// ���ѵȴ��������ظ����߳�


// �ͷŵ����¼�
VOID DbgkpFreeDebugEvent(
	_In_ PDEBUG_EVENT DebugEvent
);

// ���ý��̻������BeingDebugged
VOID DbgkpMarkProcessPeb(
	_In_ PEPROCESS Process
);

// ���ں�ģʽ�Ľṹ��ת��Ϊ�û�ģʽ�Ľṹ��


//NTSTATUS MmGetFileNameForAddress(
//	_In_ PVOID ProcessVa,
//	_Out_ PUNICODE_STRING FileName
//);

NTSTATUS DbgkClearProcessDebugObject(
	_In_ PEPROCESS Process,
	_In_ PDEBUG_OBJECT SourceDebugObject
);


// ���õ��Զ��������
NTSTATUS NtSetInformationDebugObject(
	_In_ HANDLE DebugObjectHandle,
	_In_ DEBUG_OBJECT_INFORMATION_CLASS DebugObjectInformationClass,
	_In_ PVOID DebugInformation,
	_In_ ULONG DebugInformationLength,
	_Out_opt_ PULONG ReturnLength
);

// ��ѯ������Ϣ����Ĺ��˼���
NTSTATUS NtQueryDebugFilterState(
	_In_ ULONG ComponentId,
	_In_ ULONG Level
);

// ���õ�����Ϣ����Ĺ��˼���
NTSTATUS NtSetDebugFilterState(
	_In_ ULONG ComponentId,
	_In_ ULONG Level,
	_In_ BOOLEAN State
);

// ����ָ�������еĵ��Զ���
NTSTATUS DbgkOpenProcessObject(
	_In_ PEPROCESS Process,
	_In_ PACCESS_STATE AccessState,
	_In_ ACCESS_MASK DesiredAccess
);

// �򿪽��̣��̶߳����������ü���
VOID DbgkpOpenHandles(
	_In_ PDBGUI_WAIT_STATE_CHANGE WaitStateChange,
	_In_ PEPROCESS Process,
	_In_ PETHREAD Thread
);

// �رյ��Զ���
VOID DbgkpCloseObject(
	_In_ PEPROCESS Process,
	_In_ PVOID Object,
	_In_ ACCESS_MASK GrantedAccess,
	_In_ ULONG_PTR ProcessHandleCount,
	_In_ ULONG_PTR SystemHandleCount
);

// �������Զ���
NTSTATUS DbgkCopyProcessDebugPort(
	_In_ PEPROCESS TargetProcess,
	_In_ PEPROCESS SourceProcess,
	_In_ PDEBUG_OBJECT DebugObject,
	_Out_ PBOOLEAN bFlag
);

NTSTATUS PsTerminateProcess(
	PEPROCESS Process,
	NTSTATUS Status
);


VOID DbgkpDeleteObject(_In_ PDEBUG_OBJECT DebugObject);