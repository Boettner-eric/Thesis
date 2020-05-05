#  A simple demo application for key_crypt
from translate import *

ckey = bytearray.fromhex("2b7e151628aed2a6abf7158809cf4f3c")
key = create_keyptr(ckey)
plaintext = input()
while (plaintext != 'quit'):
    if plaintext == "key":
        ckey = input("enter key: ")
        print(ckey)
        key = create_keyptr(bytearray.fromhex(ckey))
        plaintext = input()
        continue
    ciphertext = bytearray.fromhex(plaintext)
    result = P_InvCipher(bytes(ciphertext), key)
    try:
        print(result.decode())
    except:
        print(f"can't decode {result}")
    plaintext = input()
