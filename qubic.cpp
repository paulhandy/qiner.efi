#include "uefi.h"

static EFI_SYSTEM_TABLE* st;
static EFI_RUNTIME_SERVICES* rs;
static EFI_BOOT_SERVICES* bs;

VOID appendText(CHAR16* dst, CONST CHAR16* src)
{
	UINT16 dstIndex = 0;
	while (dst[dstIndex] != 0)
	{
		dstIndex++;
	}
	UINT16 srcIndex = 0;
	while ((dst[dstIndex++] = src[srcIndex++]) != 0)
	{
	}
}

VOID log(CONST CHAR16* message)
{
	CHAR16 timestampedMessage[256];

	EFI_TIME time;
	rs->GetTime(&time, NULL);
	timestampedMessage[0] = time.Year / 1000 + '0';
	timestampedMessage[1] = (time.Year %= 1000) / 100 + '0';
	timestampedMessage[2] = (time.Year %= 100) / 10 + '0';
	timestampedMessage[3] = time.Year % 10 + '0';
	timestampedMessage[4] = '-';
	timestampedMessage[5] = time.Month / 10 + '0';
	timestampedMessage[6] = time.Month % 10 + '0';
	timestampedMessage[7] = '-';
	timestampedMessage[8] = time.Day / 10 + '0';
	timestampedMessage[9] = time.Day % 10 + '0';
	timestampedMessage[10] = ' ';
	timestampedMessage[11] = time.Hour / 10 + '0';
	timestampedMessage[12] = time.Hour % 10 + '0';
	timestampedMessage[13] = ':';
	timestampedMessage[14] = time.Minute / 10 + '0';
	timestampedMessage[15] = time.Minute % 10 + '0';
	timestampedMessage[16] = ':';
	timestampedMessage[17] = time.Second / 10 + '0';
	timestampedMessage[18] = time.Second % 10 + '0';
	timestampedMessage[19] = ' ';
	timestampedMessage[20] = '|';
	timestampedMessage[21] = ' ';
	timestampedMessage[22] = 0;

	appendText(timestampedMessage, message);
	appendText(timestampedMessage, L"\r\n");

	st->ConOut->OutputString(st->ConOut, timestampedMessage);
}

EFI_STATUS efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable)
{
	st = systemTable;
	rs = st->RuntimeServices;
	bs = st->BootServices;
	
	bs->SetWatchdogTimer(0, 0, 0, NULL);

	st->ConOut->ClearScreen(st->ConOut);
	log(L"Qubic 0.0.0 is launched.");

	bs->Stall(1000000);
	st->ConIn->Reset(st->ConIn, FALSE);
	UINTN eventIndex;
	bs->WaitForEvent(1, &st->ConIn->WaitForKey, &eventIndex);

	return EFI_SUCCESS;
}
