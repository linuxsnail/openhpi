#!/usr/bin/env python

"""
 (C) Copyright IBM Corp. 2008
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 file and program are licensed under a BSD style license.  See
 the Copying file included with the OpenHPI distribution for
 full licensing terms.
 
 Authors:
    Jayashree Padmanabhan <jayshree@in.ibm.com>
"""

import unittest
from openhpi import *


class TestSequence(unittest.TestCase):
       
    """
    runTest : EL test
 *
 * This test verifies failure of oh_el_get when prev == NULL
 *
 * Return value: 0 on success, 1 on failure
    """
    
    def runTest(self):
        
        el = oh_el()
        entry = oh_el_entry()
        next = None
        retc = None
    
        # set prev == None
        el = oh_el_create(20)
        retc = oh_el_map_from_file(el, "./elTest.data")
        self.assertEqual (retc != SA_OK,False)
            
        #entry = (oh_el_entry *)(g_list_first(el->list)->data)
        retc,prev,next,entry = oh_el_get(el, entry.event.EntryId)
        self.assertEqual (retc == SA_OK,True)
        	
        # close el 
        retc = oh_el_close(el)
        self.assertEqual (retc != SA_OK,False)
        
if __name__=='__main__':
        unittest.main()  
