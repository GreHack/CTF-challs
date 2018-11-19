extern flk	; flag key
global chall
chall:
	;; int chall(char *flag, char *key, int len);
	;; flag @ edi ;; user input -> flag output
	;; key  @ esi ;; const key
	;; len  @ edx ;; min(len(flag), len(key))
	;; scratch eax, ecx, edx
	mov eax, 0
	mov ecx, 0
.loop:
	test edx, edx
	jz chall.end
	;; X[i] = Input[i] ^ X[i-1]
	mov cl, byte [edi]
	xor byte [.xor1+2], cl
	xor byte [.xor2+3], cl
	;; eax += X[i] ^ K[i]
	mov ecx, 0
	mov cl, byte [esi]
.xor1:
	xor ecx, 0x5e
	movzx ecx, cl
	add eax, ecx
.xor2:
	;; flag[i] = flk[i] ^ K[i] ^ Input[i]
	xor byte [edi], 0x5e
	mov ecx, [flk]
	mov cl, byte [ecx]
	xor byte [edi], cl
	inc word [flk]
	inc esi
	inc edi
	dec edx
	jmp .loop
.end:
	ret
