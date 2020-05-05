#  A simple demo application for key_crypt
from translate import *
import sys

ckey = bytearray.fromhex("2b7e151628aed2a6abf7158809cf4f3c")
key = create_keyptr(ckey)
while True:
    plaintext = input()
    if plaintext == "key":
        ckey = input("enter key: ")
        key = create_keyptr(bytearray.fromhex(ckey))
        continue
    if len(plaintext) == 32:
        try:
            ciphertext = bytearray.fromhex(plaintext)
            result = P_InvCipher(bytes(ciphertext), key)
            print(result.decode())
        except:
            print(f"can't decode")
