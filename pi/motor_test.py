import mshield

mshield.enable()
mshield.backward()
cmd = raw_input("press quit to quit: ")
if cmd == 'q':
	mshield.disable()
	print("Exiting...")