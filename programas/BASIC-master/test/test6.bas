LET C=1
LET N=10

loop:
	PRINT C
	GOTO inc
b:
	IF C>N THEN GOTO end
	GOTO loop
end:
	PRINT "FINAL"
	END
inc:
	LET C=C+1
	GOTO b