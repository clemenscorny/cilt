#!/usr/bin/env python3

import numpy as np
import unittest

import cilt

class TestTransversalFilter(unittest.TestCase):

    def test_general(self):
        size = 5
        filt = cilt.FilterTransversal(np.zeros(size))

        for n in filt.getNumerator():
            self.assertEqual(n, 0.)

        b = np.zeros(size)

        for i in range(size):
            b[i] = i+1

        filt.setNumerator(b)
        b_filter = filt.getNumerator()

        for i in range(size):
            self.assertEqual(b[i], b_filter[i])

        b = np.zeros(size+2)
        filt.setCoeffs(b)
        self.assertEqual(filt.getOrder(), size+2)
        b_filter = filt.getNumerator()

        for n in filt.getNumerator():
            self.assertEqual(n, 0.)

if __name__ == '__main__':
    unittest.main()
