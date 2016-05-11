SECTIONS {
  .test_meta : {
    *.test.o (.meta)
  }
  .test_xchg_code 0x10000000 : {
    xchg.test.o (.text)
  }
  .test_xchg_data 0x20000000 : {
    xchg.test.o (.data)
  }
  .test_xchg_bss 0x30000000 : {
    xchg.test.o (.bss)
  }
  .test_xor3_code 0x40000000 : {
    xor3.test.o (.text)
  }
  .test_xor3_data 0x50000000 : {
    xor3.test.o (.data)
  }
  .test_xor3_bss 0x60000000 : {
    xor3.test.o (.bss)
  }
}
INSERT AFTER .text;
