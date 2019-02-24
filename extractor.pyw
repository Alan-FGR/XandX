import random, tkinter

def convert(txt):
    
    txt = txt.replace("\r\n", ' ');
    txt = txt.replace("\n", ' ');
    for i in range(16): # :trollface:
        txt = txt.replace("  ", ' ');

    sigs = [x.strip() for x in txt.split(";")]

    out = []

    for sig in sigs:
        if not sig:
            continue
        try:
            spcsplit = sig.split(" ");

            ftype = spcsplit[0]

            try:
                if spcsplit[1] == "*" or spcsplit[2][0] == "*":
                    ftype += "*"
            except:pass

            #sux
            api = ''
            for kw in sig.split("(")[0].split(" "):
                if ('API' in kw):
                    api = kw
                    break;

            fname = sig.split("(")[0].split(" ")[-1]

            fargs = sig.split("(")[1].split(")")[0].split(",")

            fptw = ftype+" ("+api+" *orig"+fname+")("+",".join(fargs)+") = "+fname+";"

            fisg = ' '.join(sig.split("(")[0].split(" ")[:-1])+" itcp"+fname+"("+",".join(fargs)+") {\n"+\
                '    log("hooked itcp'+fname+' called");\n'+\
                '    auto r = orig'+fname+"("+', '.join([p.split(' ')[-1] for p in fargs])+");\n    return r;//TODO\n}"

            fatt = "void hook"+fname+"(){DetourAttach(&(PVOID&)orig"+fname+", itcp"+fname+");}\n"
            fdet = "void detach"+fname+"(){DetourDetach(&(PVOID&)orig"+fname+", itcp"+fname+");}\n"

            out.append(fptw+"\n"+fisg+"\n"+fatt+fdet+"\n")

        except:
            out.append("ERROR")

    return '\n'.join(out)+'\n'

root = tkinter.Tk()
root.geometry("674x512")
root.resizable(False,False)

intext = tkinter.Text(root, height=14)
outtext = tkinter.Text(root, height=14, wrap=tkinter.NONE)

def gui_convert():
    outtext.delete('1.0', 'end') # wtf
    outtext.insert('1.0',convert(intext.get('1.0','end'))) #what the actual hell?? :(

tkinter.Label(root,text="input decls").pack()
intext.pack()
button = tkinter.Button(root, text="extract hooks", command=gui_convert).pack()
outtext.pack()

root.mainloop()