SECTIONS {
  .test_code 0x10000000 : { /* 256M aligned */
    test.o (.text);
  }

  .test_data 0x20000000 : {
    test.o (.data);
  }

  .test_bss  0x30000000 : {
    test.o (.bss);
  }
}
INSERT AFTER .text;
