import subprocess as s

sizes = [1024, 2048, 4096, 8192]
for x in sizes:
    print("[Jakoby " + str(x) + " x " + str(x) + "]")
    s.call(["C:/Users/HOME/Source/Repos/Jakobi/x64/Release/Jakobi.exe", str(x)])
    print("")

    print("[JakobiVec " + str(x) + " x " + str(x) + "]")
    s.call(["C:/Users/HOME/Source/Repos/Jakobi/x64/Release/JakobiVec.exe", str(x)])
    print("")

    print("[JacobiOmp " + str(x) + " x " + str(x) + "]")
    s.call(["C:/Users/HOME/Source/Repos/Jakobi/x64/Release/JacobiOmp.exe", str(x)])
    print("")
    
    print("[JacobiOmpVec " + str(x) + " x " + str(x) + "]")
    s.call(["C:/Users/HOME/Source/Repos/Jakobi/x64/Release/JacobiOmpVec.exe", str(x)])
    print("")

    print("[ConjGrad " + str(x) + " x " + str(x) + "]")
    s.call(["C:/Users/HOME/Source/Repos/Jakobi/x64/Release/ConjGrad.exe", str(x)])
    print("")

    print("[ConjGradVec " + str(x) + " x " + str(x) + "]")
    s.call(["C:/Users/HOME/Source/Repos/Jakobi/x64/Release/ConjGradVec.exe", str(x)])
    print("")

    print("[ConjGradOmp " + str(x) + " x " + str(x) + "]")
    s.call(["C:/Users/HOME/Source/Repos/Jakobi/x64/Release/ConjGradOmp.exe", str(x)])
    print("")

    print("[ConjGradOmpVec " + str(x) + " x " + str(x) + "]")
    s.call(["C:/Users/HOME/Source/Repos/Jakobi/x64/Release/ConjGradOmpVec.exe", str(x)])
    print("")
