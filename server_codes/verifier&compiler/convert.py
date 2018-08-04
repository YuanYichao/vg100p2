from ctypes import cdll, c_int, pointer

lib = cdll.LoadLibrary("test.dll")

code = (c_int*10000)()
code_pointer = pointer(code)
string = 'var a; a = -3; var b; b = 3; start; a = sin(time() + a); back;'
python_code = []
sz = 0


def copyInt():
	global python_code
	python_code = []
	for i in range(0, sz):
		python_code.append(code[i])


def interpreter(string_input, code_pointer):
    global sz
    sz = lib.connectPython_sz(bytes(string_input, 'utf-8'), code_pointer) // 8


def convert(stri = string):
    interpreter(stri, code_pointer)
    if sz == -1:
        raise RuntimeError("compiling error")
    copyInt()
    return python_code

if __name__ == '__main__':
    print(convert())
