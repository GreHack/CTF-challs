; GreHack2018 Jaguar interlude
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

	.dc.b "GreHack 2018 interlude"
	.dc.b $0A
	.dc.b "What? Radare2 doesn't support this target?"
	.dc.b $0A
	.dc.b "Xarkes, please help us!"


	.phrase
gpu_code_start:
	.gpu
	.org G_RAM

; 9800300A
	movei #gpu_main,r0		;point to start
	jump (r0)			;go start
	nop

gpu_main:       

	moveq #11,r0
	movei #flag,r1
	movei #datas,r2
	movei #-8,r3
main_decode:
	moveta r0,r0			; backup r0

	load (r1),r0			; read 1 char
	not r0				; inverse bits
	sh r3,r0			; shift 8 bits left
	unpack r0			; split left bits
	store r0,(r2)			; store 1->4 bytes flag

	addq #4,r1			; next char
	addq #4,r2			; next chunk

	movefa r0,r0			; retore r0
	subq	#1,r0			; 
	jr	nc,main_decode		; 
	nop
end_decode:
	movei #datas,r2
	load (r2),r3
	addq #4,r2
	load (r2),r4
	addq #4,r2
	load (r2),r5
	addq #4,r2
	load (r2),r6
	addq #4,r2
	load (r2),r7
	addq #4,r2
	load (r2),r8
	addq #4,r2
	load (r2),r9
	addq #4,r2
	load (r2),r10
	addq #4,r2
	load (r2),r11
	addq #4,r2
	load (r2),r12
	addq #4,r2
	load (r2),r13
	addq #4,r2
	load (r2),r14
	nop

	.phrase
	.dc.b	"* Your flag is GH18{"
	.dc.b	"} *"

	.phrase
flag:	;AtarI_JaGuaR"
	.dc.l "A"
	.dc.l "t"
	.dc.l "a"
	.dc.l "r"
	.dc.l "I"
	.dc.l "_"
	.dc.l "J"
	.dc.l "a"
	.dc.l "G"
	.dc.l "u"
	.dc.l "a"
	.dc.l "R"
datas:	.dc.l 0,0,0,0,0,0,0,0,0,0,0,0

; .dc.l $02C1C046,$0201606C,$0241C061,$0201A067,$02C0C05F,$02800069,$02C0A073,$0241C05F,$02C10068,$02014065,$0241C072,$0281A065

;Flag_is_here = 46 6c 61 67 5f 69 73 5f 68 65 72 65
	.68000

gpu_code_end:   
.dc.l           0
