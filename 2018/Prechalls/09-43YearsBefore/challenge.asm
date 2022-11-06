// reset at $0000 & full ram zeroed

	org $0000
	JMP start

	org  $0666
:start
	LDA first
	BNE run
:alreadyrunned
	JMP alreadyrunned

:run
	INC first

:main
	DEC fctsel
	LDA fctsel
	CMP #1
	BEQ callfct1
	CMP #2
	BEQ callfct2
	CMP #3
	BEQ callfct3
	CMP #4
	BEQ callfct4
	CMP #5
	BEQ callfct5
	JMP alreadyrunned
	
:callfct5
	JSR jmpfct5
	JMP main

:callfct4	
	JSR jmpfct4
	JMP main
	
:callfct3
	JSR jmpfct3
	JMP main
	
:callfct2
	JSR jmpfct2
	JMP main
	
:callfct1
	JSR jmpfct1
	JMP main


:jmpfct5
	JMP(fcttbl+2*0)

:jmpfct4
	JMP(fcttbl+2*1)

:jmpfct3
	JMP(fcttbl+2*2)

:jmpfct2
	JMP(fcttbl+2*3)

:jmpfct1
	JMP(fcttbl+2*4)


	org  $08f4
:first	DB $ff
:tmp	DB 0
:fctsel DB 6
:fcttbl	DW fct5
	DW fct4
	DW fct3
	DW fct2
	DW fct1 
	

	org  $1500
:fct1
	LDA $9008
	TAY
	LDA $9007
	TAX
	LDA $9005
	STA $9007
	TYA
	STA $9005
	LDA $9006
	STA $9008
	TXA
	STA $9006
	LDA #$34
	EOR $9006
	STA $9006
	RTS

:fct2	
	LDA $9007
	STA tmp
:loop2	CMP $9005
	BMI end2
	INC $9006
	DEC tmp
	LDA tmp
	JMP loop2
:end2	RTS

:fct3
	LDX $9008
:loop3	DEC $9005
	DEX
	CPX #'0'
	BNE loop3
	RTS

:fct4
	LDA $9008
	TAX
	LDA $9007
	TAY
	LDA $9005
	STA $9008
	TXA
	STA $9005
	LDA $9006
	STA $9007
	TYA
	STA $9006
	RTS

:fct5
	LDA #$1A
	EOR $9005
	STA $9005
	LDA #$79
	EOR $9006
	STA $9006
	LDA #$46
	EOR $9007
	STA $9007
	LDA #$7C
	EOR $9008
	STA $9008
	RTS


	org  $9000
	data 'GH18{....}'
