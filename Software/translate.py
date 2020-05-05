from ctypes import *
"""Python AES"""
from Crypto.Cipher import AES

def P_Cipher(state, key):
    cipher = AES.new(key, AES.MODE_ECB)
    return cipher.encrypt(state)

def P_InvCipher(state, key):
    plain = AES.new(key, AES.MODE_ECB)
    return plain.decrypt(state)

"""Custom C AES"""
# Acsess shared c lib
_aes = CDLL('./aes.so')
# void Cipher(state_t state, uint8_t* key);
_aes.Cipher.argtypes = (POINTER((c_uint8)*4*4), (c_uint8)*16)
_aes.Cipher.restype = None
# void Key_expansion(uint8_t* key, word w);
_aes.Key_expansion.argtypes = ((c_uint8)*16, (c_uint8)*192)
_aes.Key_expansion.res = None

_aes.Bytes_to_String.argtypes = (POINTER((c_uint8)*4*4), (POINTER(c_char * 32)))
_aes.Bytes_to_String.res = None

_aes.String_to_bytes.argtypes = (POINTER((c_uint8)*4*4), (POINTER(c_char * 16)))
_aes.String_to_bytes.res = None
# creates a c style pointer to a state_t of uint8_t[4][4]
def create_stateptr(arr):
    ptr = ((c_uint8)*4*4)()
    for i in range(4):
        for j in range(4):
            ptr[j][i] = c_uint8(arr[i*4+j])
    return ptr

# creates a c style pointer to a key of uint8_t[16]
def create_keyptr(arr):
    key = (c_uint8*16)()
    for i in range(16):
        key[i] = (c_uint8(arr[i]))
    return key

def key_exp(key):
    key_ptr = (c_uint8*16)()
    key_ptr = create_keyptr(bytearray.fromhex(key))
    wrd = (c_uint8*192)()
    _aes.Key_expansion(key_ptr, wrd)
    return wrd

def encrypt(state, key):
    state, key = bytearray.fromhex(state), bytearray.fromhex(key)
    state_ptr = create_stateptr(state)
    key_ptr = create_keyptr(key)
    _aes.Cipher(state_ptr, key_ptr)
    c_ver = "".join(["".join([f"{(state_ptr)[i][j]:02x}" for i in range(4)]) for j in range(4)])
    p_ver = P_Cipher(bytes(state), bytes(key)).hex()
    return (c_ver, p_ver)


overflow = '2b7e151628aed2a6abf7158809cf4f3ca0fafe1788542cb123a339392a6c7605f2c295f27a96b9435935807a7359f67f3d80477d4716fe3e1e237e446d7a883bef44a541a8525b7fb671253bdb0bad00d4d1c6f87c839d87caf2b8bc11f915bc6d88a37a110b3efddbf98641ca0093fd4e54f70e5f5fc9f384a64fb24ea6dc4fead27321b58dbad2312bf5607f8d292fac7766f319fadc2128d12941575c006ed014f9a8c9ee2589e13f0cc8b6630ca6'

# test code for bytes to string and string to bytes
key = create_keyptr(bytearray.fromhex("2b7e151628aed2a6abf7158809cf4f3c"))
x = create_stateptr("                ".encode()) # empty state pointer
p = (c_char*16)()
p = create_string_buffer(b"this is a test !", 16) # ctypes function
_aes.String_to_bytes(x, p) # encode string to state array
z = (c_char*32)()
_aes.Bytes_to_String(x, z) # get cipher state array as string buffer
_aes.Cipher(x, key) # encrypt with C AES impl
z = (c_char*32)()
_aes.Bytes_to_String(x, z) # get cipher state array as string buffer
c_ver = bytearray.fromhex("".join(["".join([f"{(x)[i][j]:02x}" for i in range(4)]) for j in range(4)])) # decode c version
#print(f"c version : {c_ver}")
checking_str = "".join([f"{z[i]}" for i in range(32)]).replace('\'b\'', '') # translate
#print(f"c printed : {checking_str}")
p_ver = P_InvCipher(bytes(c_ver), key)
assert(p_ver.decode() == "this is a test !")
assert("this is a test !".encode() == p_ver)
#print(p_ver)
