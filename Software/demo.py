#  A simple demo application for key_crypt
from translate import *

ckey = ("2b7e151628aed2a6abf7158809cf4f3c")
key = create_keyptr(ckey.encode())
plaintext = input()
while (plaintext != 'quit'):
    ciphertext = bytearray.fromhex(plaintext)
    result = P_InvCipher(bytes(ciphertext), key).decode()
    print(result)
    plaintext = input()

debug = False
if debug:
    # test code for bytes to string and string to bytes
    key = create_keyptr("2b7e151628aed2a6abf7158809cf4f3c".encode())
    x = create_stateptr("                ".encode()) # empty state pointer
    p = (c_char*16)()
    p = create_string_buffer(b"this is a test !", 16) # ctypes function
    _aes.String_to_bytes(x, p) # encode string to state array
    _aes.Cipher(x, key) # encrypt with c
    z = (c_char*32)()
    _aes.Bytes_to_String(x, z) # get cipher state array as string buffer
    c_ver = bytearray.fromhex("".join(["".join([f"{(x)[i][j]:02x}" for i in range(4)]) for j in range(4)])) # decode c version
    #print(f"c version : {c_ver}")
    checking_str = "".join([f"{z[i]}" for i in range(32)]).replace('\'b\'', '') # translate
    #print(f"c printed : {checking_str}")
    p_ver = P_InvCipher(bytes(c_ver), key)
    assert(p_ver.decode() == "this is a test !")
    assert("this is a test !".encode() == p_ver)
