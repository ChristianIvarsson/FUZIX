
	.export __ldbyte1

	.setcpu 8080
	.code
__ldbyte1:
	lxi h,1

	mov l,m
	ret