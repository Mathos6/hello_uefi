

#include <efi.h>
#include <efilib.h>

EFI_STATUS EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);

    Print(L"================================\n");
    Print(L"       Hello from UEFI!\n");
    Print(L"================================\n");

    Print(L"Le firmware UEFI fonctionne.\n");
    Print(L"Appuyez sur une touche pour quitter...\n");

    uefi_call_wrapper(
        SystemTable->BootServices->WaitForEvent,
        3,
        1,
        &SystemTable->ConIn->WaitForKey,
        &(UINTN){0}
    );

    return EFI_SUCCESS;
}
