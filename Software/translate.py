# TODO: Migrate to makefile before commit
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

# prints state matrix
def print_state(state):
    res = ["%02x" % b for b in state]
    for j in range(4):
        for i in range(4):
            print(res[i+j*4], end=" ")
        print("")

def test_diff(state, key, display=False):
    state_ptr = create_stateptr(state)
    key_ptr = create_keyptr(key)
    _aes.Cipher(state_ptr, key_ptr)
    x = ""
    #print(''.join("%02x" % ','.join(map(str, x)) for x in state))
    for i in range(4): # flip x/y t follow convention
        for j in range(4):
            x += f"{(state_ptr)[j][i]:02x}"
            #print(f"{(state_ptr)[j][i]:02x} ", end="")
    c_ver = bytearray.fromhex(x)
    p_ver = P_Cipher(bytes(state), bytes(key))
    if display:
        print_state(state)
        print_state(c_ver)
        print_print(p_ver)
    return (c_ver, p_ver)

overflow = bytearray(b'+~\x15\x16(\xae\xd2\xa6\xab\xf7\x15\x88\t\xcfO<\xa0\xfa\xfe\x17\x88T,\xb1#\xa399*lv\x05\xf2\xc2\x95\xf2z\x96\xb9CY5\x80zsY\xf6\x7f=\x80G}G\x16\xfe>\x1e#~Dmz\x88;\xefD\xa5A\xa8R[\x7f\xb6q%;\xdb\x0b\xad\x00\xd4\xd1\xc6\xf8|\x83\x9d\x87\xca\xf2\xb8\xbc\x11\xf9\x15\xbcm\x88\xa3z\x11\x0b>\xfd\xdb\xf9\x86A\xca\x00\x93\xfdNT\xf7\x0e__\xc9\xf3\x84\xa6O\xb2N\xa6\xdcO\xea\xd2s!\xb5\x8d\xba\xd21+\xf5`\x7f\x8d)/\xacwf\xf3\x19\xfa\xdc!(\xd1)AW\\\x00n\xd0\x14\xf9\xa8\xc9\xee%\x89\xe1?\x0c\xc8\xb6c\x0c\xa6')
