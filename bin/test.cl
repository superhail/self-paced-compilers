(*hello everyone\ *)
class B inherits A {};
class C inherits B {};
class A {
  io: IO <- new IO;
  c_: Int;
  str_field: String <- get_str();
  str_field2: String <- "222";
  get_str(): String {{
    "str_field";
  }};
  f(): Int {{
    c_ <- 00000; 
    let c: String <- "111\n" in {
      io.out_string(c);
      c <- "222\n";
      let d: String <- "333\n" in {
        io.out_string(c);
        io.out_string(d);
        let c: String <- "444\n" in {
          let e: String <- "555\n" in {
            io.out_string(e);
            let f: String <- "666\n" in {
              let g: String <- "777\n" in {
                io.out_string(f);
                let h: String <- "888\n" in {
                  io.out_string(h);
                };
              };
            };
          };
        };
      };
    };
    case 1 of
      c: String => { io.out_string("string\n"); };
      d: Int => { io.out_string("int\n"); };
    esac;
    1;
  }};
};
class M {
  t(a:Int, b:String): Int {{
    1;
  }};
};
class N inherits M{
  t(a:Int, b:String): Int {{
    1;
  }};
};
class O inherits N {
  test(a:Int, b:Int, c:Int): Int {{
    a + b + c;
  }};
};
class Base {
  name(): String {{
    "Base";
  }};
};
class Inh1 inherits Base {
  name(): String {{
    "Inh1";
  }};
};
class Inh2 inherits Base {
  name(): String {{
    if 1 < 0 then "a" else "b" fi;
    "Inh2";
  }};
};

class Main {
  io: IO <- new IO;
  s: String;
  bool_val: Bool;
  a: A <- new A;
  a1: A <- a;
  m: Int <- 1;
  o: O <- new O;
  inh: Inh2 <- new Inh2;
  let_test(): Object {{
    let n: A in n.f();
  }};
  get(m: Int): Base {{
    if m = 1 then new Inh1 else new Inh2 fi;
  }};
  main(): Object {{
    get(1);
    s <- "Hello World\n";
    io.out_string(s);
    a.f();
    io.out_string( get((1 + 2) * 2 - 5).name() );
    io.out_string("\n");
    o.test(1, 2, 3);
    io.out_string(inh@Base.name());
  }};
};
