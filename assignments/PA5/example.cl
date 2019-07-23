
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

class Main {
  io: IO <- new IO;
  f(): Int {{ io.out_int(1); 1; }};
  main():Int { f() };
};

