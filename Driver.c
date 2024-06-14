/*++

Module Name :    driver.c
Abstract    :    This file contains the driver entry points and callbacks.
Environment :    Kernel-mode Driver Framework
--*/
/*++

Routine Description for DriverEntry:
    DriverEntry initializes the driver and is the first routine called by the
    system after the driver is loaded. DriverEntry specifies the other entry
    points in the function driver, such as EvtDevice and DriverUnload.
Parameters Description:
    DriverObject - represents the instance of the function driver that is loaded
    into memory. DriverEntry must initialize members of DriverObject before it
    returns to the caller. DriverObject is allocated by the system before the
    driver is loaded, and it is released by the system after the system unloads
    the function driver from memory.
    RegistryPath - represents the driver specific path in the Registry.
    The function driver can use the path to store driver related data between
    reboots. The path does not store hardware instance specific data.
Return Value:
    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise.
--*/

/*++
Routine Description for EvtDeviceAdd:
    EvtDeviceAdd is called by the framework in response to AddDevice
    call from the PnP manager. We create and initialize a device object to
    represent a new instance of the device.
Arguments:
    Driver - Handle to a framework driver object created in DriverEntry
    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.
Return Value:    NTSTATUS

--*/

#include "driver.h"
#include "driver.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, BMSUPPERFILTEREvtDeviceAdd)
#pragma alloc_text (PAGE, BMSUPPERFILTEREvtDriverContextCleanup)
#endif

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
{
    WDF_DRIVER_CONFIG config;
    NTSTATUS status;
    WDF_OBJECT_ATTRIBUTES attributes;
    KdPrint(("Generic Upper Filter Driver Sample DriverEntry.\n"));
    // Initialize WPP Tracing
    //WPP_INIT_TRACING(“WPPDemo”)
    //The application name “WPPDemo” will be prefixed to every trace message.
    WPP_INIT_TRACING(DriverObject, RegistryPath);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Entry");
    // Register a cleanup callback so that we can call WPP_CLEANUP when
    // the framework driver object is deleted during driver unload.
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.EvtCleanupCallback = BMSUPPERFILTEREvtDriverContextCleanup;
    WDF_DRIVER_CONFIG_INIT(&config,BMSUPPERFILTEREvtDeviceAdd);
    // used to create a WDFDRIVER object
    status = WdfDriverCreate(DriverObject,RegistryPath,&attributes,&config,WDF_NO_HANDLE);
    // NT_SUCCESS is a macro that checks if the given NTSTATUS value is zero,
    //  which signifies success in the Windows kernel environment.
    // If status is not zero(indicating failure), the code inside
    //  the curly braces{ ... } is executed.
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "WdfDriverCreate failed %!STATUS!", status);
        WPP_CLEANUP(DriverObject);
        KdPrint(("WdfDriverCreate failed with status 0x%x\n", status));
        return status;
    }
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Exit");
    return status;
}

NTSTATUS
BMSUPPERFILTEREvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
{
    NTSTATUS status;
    UNREFERENCED_PARAMETER(Driver); //A macro used to suppress compiler warnings about unused parameters
    PAGED_CODE();//Ensures that the function is only called from pageable code
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Entry");//Logs an informational message indicating the start of the function
    status = BMSUPPERFILTERCreateDevice(DeviceInit);

    if (!NT_SUCCESS(status)){
        KdPrint(("WdfDeviceCreate failed with status code 0x%x\n", status));
        return status;
    }
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Exit");
    return status;
}

/*++
Routine Description:    Free all the resources allocated in DriverEntry.
Arguments          :    DriverObject - handle to a WDF Driver object.
Return Value       :    VOID.
--*/
VOID BMSUPPERFILTEREvtDriverContextCleanup(_In_ WDFOBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);
    PAGED_CODE();
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Entry");
    // Stop WPP Tracing
    WPP_CLEANUP(WdfDriverWdmGetDriverObject((WDFDRIVER)DriverObject));
}
