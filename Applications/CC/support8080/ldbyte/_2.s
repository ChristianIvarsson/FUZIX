
	.export __ldbyte2

	.setcpu 8080
	.code
__ldbyte2:
	lxi h,2

	mov l,m
	ret