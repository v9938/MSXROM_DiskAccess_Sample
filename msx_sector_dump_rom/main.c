/*
	DISK Access test program
	Copyright 2023 @V9938
	
	V1.0		1st version
*/

#include <stdio.h>
#include <string.h>
#include <msx.h>

#define BUF_SIZE 512

//Global
unsigned char TransferBuffer[BUF_SIZE];		// Page3 Temporary Buffer


void set_color(char forcolor, char bakcolor, char bdrcolor)
{
#asm
    ld   ix,0
    add  ix,sp
    ld   a,(ix+2)       ; BDRCLR
    ld   (0f3ebh),a
    ld   a,(ix+4)       ; BAKCLR
    ld   (0f3eah),a
    ld   a,(ix+6)       ; FORCLR
    ld   (0f3e9h),a
	call 0062h			; CHGCLR
;    ld   ix,0111h       ; CHGCLR
;    call 015fh          ; EXTROM
#endasm
}
void set_text() __z88dk_fastcall __naked {
#asm
    call 00D2h			; TOTEXT
	jp 00C3h			; CLS
#endasm
}
void set_screen(unsigned char *p) __z88dk_fastcall __naked {
#asm
    ld a,(hl)
    jp 005fh			;CHGMOD 
#endasm
}

//Assembler SubRoutine
void hitSpaceKey()  __z88dk_fastcall __naked {
#asm
	xor 	a					; Space key
	call 	$00d8				; GTRIG
	jr		z,	_hitSpaceKey	; Check  input or not
	ret
#endasm
}
int chget(){
#asm
	call 009fh					; CHGET
    ld l,a						; Return CHR
    ld h,0			
#endasm
}


void diskReadSector(int sector) __z88dk_fastcall __naked {
#asm
	push	hl
	ld 		de,hl
	ld		hl,_TransferBuffer	; RAM Address
	xor 	a					; Drive A & Read Mode
	ld		bc,$01f9			; Transfer size(1Sector) & Media ID (2DD)
	call	$0ffa7				; H.PHYDSK
	pop		hl
	ret		nc					; No Error
	ld		c,a					; Disk Error

	push	hl
	xor		a
	call	_diskError
	pop		hl
	ld		a,1
	cp		c
	jr		_diskReadSector
#endasm
}

void setHookDiskError()__z88dk_fastcall __naked {
#asm
	di
	ld		hl,_diskError		;Disk Error Function address
	ld		($0F323),hl			;Set Jump Address to DISKVE
	ret
#endasm
}

void diskError() __z88dk_fastcall __naked {
#asm
	ld		h,a					; a= Drive number
	ld		l,c					; c = error code
	push 	hl
	call	_diskErrorMain
	ld		c,l
	ret
#endasm
}


unsigned char diskErrorMain(int errorCode ){
	printf("\nDisk Error code=%04x\n",errorCode);
	printf("Drive %c : ",'A'+(errorCode>>8));

	if ((errorCode & 0xfe) == 0x80) printf("Bad FAT\n");
	if ((errorCode & 0xfe) == 0x00) printf("Write Protect\n");
	if ((errorCode & 0xfe) == 0x02) printf("Not ready\n");
	if ((errorCode & 0xfe) == 0x04) printf("CRC error\n");
	if ((errorCode & 0xfe) == 0x06) printf("Seek error\n");
	if ((errorCode & 0xfe) == 0x08) printf("Record not found\n");
	if ((errorCode & 0xff) == 0x0A) printf("Unsupport media type\n");
	if ((errorCode & 0xff) == 0x0B) printf("Write fault\n");
	if ((errorCode & 0xfe) == 0x0C) printf("Not ready\n");

	while (1){
		printf("[i]gore/[r]etry/[a]bort?\n");
		if (chget()=='i') return 0;
		if (chget()=='r') return 1;
		if (chget()=='a') return 2;
	}
}

int main()
{
	int	x,y;
	int sector;
	unsigned char *pTransferBuffer;					// Page3 Temporary Buffer
	
	set_text();
	set_color(15,5,7);
	sector = 0;
	setHookDiskError();
	
	printf("Please insert the disk\n");
	
	while (1){
		hitSpaceKey();								//Key Input Wait

		if (sector >= 1440) sector = 0;
		diskReadSector(sector);

		printf("\nSector = %d\n",sector);

		pTransferBuffer = TransferBuffer;			// Dump List
		for (y = 0; y < 16; y++){
			for (x = 0; x < 8; x++){
				printf("%02x ",*pTransferBuffer);
				pTransferBuffer++;
			}
			printf("\n");
		}
		sector++;
	}

}
