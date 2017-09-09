import subprocess as s

datapath = "C:/Users/HOME/Source/Repos/Jakobi/data"
exepath = "C:/Users/HOME/Source/Repos/Jakobi/x64/Release"
sizes = [1024, 2048, 4096, 8192]
tns = [4, 8]
#tns = [1, 2, 4, 8]
#tns = [1, 16, 32, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240] #phi

for x in sizes:
	print("[Jakoby " + str(x) + " x " + str(x) + "]")
	s.call([exepath + "/Jakobi.exe", str(x), datapath])
	print("")

	print("[JakobiVec " + str(x) + " x " + str(x) + "]")
	s.call([exepath + "/JakobiVec.exe", str(x), datapath])
	print("")

	print("[ConjGrad " + str(x) + " x " + str(x) + "]")
	s.call([exepath + "/ConjGrad.exe", str(x), datapath])
	print("")

	print("[ConjGradVec " + str(x) + " x " + str(x) + "]")
	s.call([exepath + "/ConjGradVec.exe", str(x), datapath])
	print("")
        
	for tn in tns:                
		print("[JacobiOmp " + str(x) + " x " + str(x) + "]")		
		s.call([exepath + "/JacobiOmp.exe", str(x), datapath, str(tn)])
		print("")
			
		print("[JacobiOmpVec " + str(x) + " x " + str(x) + "]")
		s.call([exepath + "/JacobiOmpVec.exe", str(x), datapath, str(tn)])
		print("")

		print("[ConjGradOmp " + str(x) + " x " + str(x) + "]")
		s.call([exepath + "/ConjGradOmp.exe", str(x), datapath, str(tn)])
		print("")

		print("[ConjGradOmpVec " + str(x) + " x " + str(x) + "]")
		s.call([exepath + "/ConjGradOmpVec.exe", str(x), datapath, str(tn)])
		print("")
