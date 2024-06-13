/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_BMSUPPERFILTER,
    0x8ce796a4,0x06a6,0x41dc,0xb3,0x78,0xcf,0xb3,0x9a,0x38,0xa1,0x2b);
// {8ce796a4-06a6-41dc-b378-cfb39a38a12b}
