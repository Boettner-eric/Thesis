import unittest
from translate import *
from random import getrandbits

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
    # Test encryption input and output
    def test_encrypt(self):
        c, py = encrypt(self.txt, self.key)
        assert c == py, f"Failed C\"{c}\" != Py\"{py}\""

    # Test encrpyion and decryption inline for python (should always work perfectly)
    def test_python_circ(self):
        state = P_Cipher(self.txt, self.key)
        invCipher = P_InvCipher(state, self.key)
        assert invCipher.decode() == self.txt, f"Failed circularity {invCipher} != {self.txt}"
        assert invCipher == str.encode(self.txt), f"Failed circularity {invCipher} != {self.txt}"

    # Test circularity (plaintext -> cipher -> plaintext -> cipher -> plaintext) to test for drift errors
    def test_circularity(self):
        pass

class Standard(Test):
    __test__ = False
    def setUp(self):
        self.txt = "3243f6a8885a308d313198a2e0370734"
        self.key = "2b7e151628aed2a6abf7158809cf4f3c"

    # AES Publication Appendix B Cipher Example
    # in   3243f6a8885a308d313198a2e0370734
    # key  2b7e151628aed2a6abf7158809cf4f3c
    def test_sample(self):
        known = '3925841d02dc09fbdc118597196a0b32'
        c, py = encrypt(self.txt, self.key)
        assert c == py == known, "Comparison failed for sample"

    # AES Publication Appendix A.1 EXPANSION OF A 128-BIT CIPHER KEY
    def test_expansion(self):
        wrd = key_exp(self.key)
        assert bytearray([wrd[i] for i in range(176)]).hex() == overflow, "Key Expansion Failed for sample"

class Random(Test):
    __test__ = False
    def setUp(self):
        self.txt = "".join([f"{getrandbits(8):02X}" for _ in range(16)])
        self.key = "".join([f"{getrandbits(8):02X}" for _ in range(16)])

    # Try 10000 randomly generated 16 bit strings : fail if any fail
    def test_randbits(self):
        for i in range(10000):
            self.txt = "".join([f"{getrandbits(8):02X}" for _ in range(16)])
            self.key = "".join([f"{getrandbits(8):02X}" for _ in range(16)])
            a, b = encrypt(self.txt, self.key)
            assert a == b, f"Test {i} of random failed"

class DeadBeef(Test):
    __test__ = False
    def setUp(self):
        self.txt = "deadbeefdeadbeefdeadbeefdeadbeef"
        self.key = "deadbeefdeadbeefdeadbeefdeadbeef"

class ZeroOne(Test):
    __test__ = False
    def setUp(self):
        self.txt = "00011100110110101001110011101011"
        self.key = "11010100100100111010010010010010"

class Sentence(Test):
    __test__ = False
    def setUp(self):
        self.txt = "this is the text".encode('utf-8').hex()
        self.key = "very complex key".encode('utf-8').hex()

del(Test)

if __name__ == "__main__":
    unittest.main()
