SECTIONS {
  . = 0x00400000;
  .text : {
    amh.o (.text);
    test.o (.text);
  }

  .data : {
    amh.o (.data);
    test.o (.data);
  }
}
