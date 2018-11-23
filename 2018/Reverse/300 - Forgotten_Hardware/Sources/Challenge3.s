; GreHack2018 Jaguar interlude 3 by Phil
; compil with rmac (git clone http://shamusworld.gotdns.org/git/rmac + make on Debian9)
; ./rmac -fe Interlude.s
;
; patch compiler :
; riscasm.c 115 from:
;        { MR_UNPACK,  RI_ONE,    63 + GPUONLY | (0 << 6) },
;        { MR_PACK,    RI_ONE,    63 + GPUONLY | (1 << 6) },
; to:
;        { MR_UNPACK,  RI_ONE,    63 + GPUONLY | (1 << 6) },
;        { MR_PACK,    RI_ONE,    63 + GPUONLY | (0 << 6) },


;	include "JAGUAR.INC"
_BASE	EQU $F00000			; TOM Internal Register Base
G_RAM	EQU _BASE+$3000			; GPU Internal RAM
G_PC	EQU _BASE+$2110			; GPU Program Counter
G_CTRL	EQU _BASE+$2114			; GPU Operation Control/Status
G_FLAGS	EQU _BASE+$2100			; GPU Flags
G_END	EQU _BASE+$210C			; GPU Data Organization

	.text
	.68000
	bsr.s	setup_gpu
	bsr.s	run_gpu
dead_loop:
	jmp	dead_loop


setup_gpu:				; setup gpu: copy code & init some vars & regs etc.

	movem.l d0-a6,-(sp)		; backup registers

	clr.l G_CTRL			; stop gpu
	clr.l G_FLAGS			; clear gpu interrupts flags
	move.l #7,G_END			; set big endian

	lea gpu_code_start(pc),a0
	lea gpu_code_end(pc),a1
	lea G_RAM,a2
loop_cp:         
	move.l (a0)+,(a2)+		;copy gpu code to gpu ram
	cmp.l a0,a1
	bgt.s loop_cp

	movem.l (sp)+,d0-a6		; retore registers
	rts


run_gpu:				; launch the gpu code

	movem.l d0-a6,-(sp)		; backup registers

	move.l #G_RAM,G_PC		; set the gpu pc
	move.l #1,G_CTRL		; start the gpu

	movem.l (sp)+,d0-a6		; retore registers
	rts

	.dc.b "GreHack 2018 challenge by Phil"
	.dc.b $0A
	.dc.b "What? Radare2 does not support this target?"
	.dc.b $0A
	.dc.b "Xarkes, please help us!"


	.phrase
gpu_code_start:
	.gpu
	.org G_RAM


	movei #gpu_main,r0		; GPU start here opcode: 9800300A 00F0D000
	jump (r0)			; run main
	nop

gpu_main:       

;decode flag
	moveq #11,r0
	movei #datas,r1
	movei #flag4,r2
	movei #8,r3
main_decode:
	moveta r0,r0			; backup r0

	load (r1),r0			; read 1 compressed char (4 bytes)
	not r0				; inverse bits
	pack r0				; compress unsefull bits to position 15..08
	sh r3,r0			; shift 8 bits right
	store r0,(r2)			; store 4->1 bytes flag

	addq #4,r1			; next char
	addq #4,r2			; next chunk

	movefa r0,r0			; retore r0
	subq	#1,r0			; 
	jr	nc,main_decode		; 
	nop
end_decode:


;compress flag
	moveq #2,r0
	movei #flag4,r1
	movei #flag,r2
	movei #-8,r3
	movei #compress,r30

compress:
	load (r1),r5			; read 1 cleared char (4 bytes)
	addq #4,r1
	sh r3,r5			; shift 8 bits left
	load (r1),r4			; next char
	addq #4,r1
	or r4,r5				; mask it with previous char
	sh r3,r5
	load (r1),r4			; again
	addq #4,r1
	or r4,r5
	sh r3,r5
	load (r1),r4
	addq #4,r1
	or r4,r5
	store r5,(2)
	addq #4,r2			; next
	subq #1,r0			; next 4 chars
	jump nc,(r30)			; compress
	nop

dead:	jr dead
	nop

; comment for final release:
;	movei #flag,r2
;	load (r2),r3
;	addq #4,r2
;	load (r2),r4
;	addq #4,r2
;	load (r2),r5
;	nop

	.phrase
	.dc.b	"* Your flag is GH18{"
flag:	.dc.b "Not So Easy!"
	.dc.b	"} * "

datas:	.dc.l $02C1C02A,$02016046,$0241C04C,$0201A041,$02C0C047,$02800049,$02C0A053,$0241C048,$02C10045,$02014052,$0241C045,$0281A02A
flag4:	.dc.l 0,0,0,0,0,0,0,0,0,0,0,0

;*FLAGISHERE* = 2a 46 4c 41 47 49 53 48 45 52 45 2a
; flag = GH18{AtarI_JaGuaR}

	.68000

gpu_code_end:   
	.dc.l           0
