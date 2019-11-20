import unittest
import argparse
from os import stat
from translate import *
import sys
from random import getrandbits

display = False # swap if needed

def compare(state, key): # a wrapper function for test diff
    return test_diff(bytearray.fromhex(state), bytearray.fromhex(key), display)

class Test(unittest.TestCase):
    __test__ = False
    @classmethod
    def setUpClass(cls):
        cls.txt = ""
        cls.key = ""

    def setUp(self):
        pass

    def tearDown(self):
        pass

    # Indivigual tests
    def test_compare(self):
        c, py = compare(self.txt, self.key)
        assert c == py, f"Failed C\"{c}\" != Py\"{py}\""

class Standard(Test):
    __test__ = False
    def setUp(self):
        self.txt = "3243f6a8885a308d313198a2e0370734"
        self.key = "2b7e151628aed2a6abf7158809cf4f3c"
    # AES Publication Appendix B Cipher Example
    # in   3243f6a8885a308d313198a2e0370734
    # key  2b7e151628aed2a6abf7158809cf4f3c
    def test_sample(self):
        known = bytearray(b'9%\x84\x1d\x02\xdc\t\xfb\xdc\x11\x85\x97\x19j\x0b2')
        c, py = compare(self.txt, self.key)
        assert c == py == known, "Comparison failed for sample"
    # AES Publication Appendix A.1 EXPANSION OF A 128-BIT CIPHER KEY
    def test_expansion(self):
        wrd = key_exp(self.key)
        assert bytearray([wrd[i] for i in range(176)]) == overflow, "Key Expansion Failed for sample"


class Random(Test):
    __test__ = False
    def setUp(self):
        self.txt = "".join([f"{getrandbits(8):02X}" for _ in range(16)])
        self.key = "".join([f"{getrandbits(8):02X}" for _ in range(16)])

del(Test)

if __name__ == "__main__":
    unittest.main()
