//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"
#include "list"
#include <algorithm>

extern void emit_string_constant(ostream& str, char* s);
extern int cgen_debug;

int stackDepth = 0; // stack depth
int labelTag = 0;

//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol arg, arg2, Bool, concat, cool_abort, COPY, Int, in_int, in_string, IO,
    length, Main, main_meth, No_class, No_type, Object, out_int, out_string,
    prim_slot, self, SELF_TYPE, Str, str_field, substr, type_name, val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void) {
    arg = idtable.add_string("arg");
    arg2 = idtable.add_string("arg2");
    Bool = idtable.add_string("Bool");
    concat = idtable.add_string("concat");
    cool_abort = idtable.add_string("abort");
    COPY = idtable.add_string("copy");
    Int = idtable.add_string("Int");
    in_int = idtable.add_string("in_int");
    in_string = idtable.add_string("in_string");
    IO = idtable.add_string("IO");
    length = idtable.add_string("length");
    Main = idtable.add_string("Main");
    main_meth = idtable.add_string("main");
    //   _no_class is a symbol that can't be the name of any
    //   user-defined class.
    No_class = idtable.add_string("_no_class");
    No_type = idtable.add_string("_no_type");
    Object = idtable.add_string("Object");
    out_int = idtable.add_string("out_int");
    out_string = idtable.add_string("out_string");
    prim_slot = idtable.add_string("_prim_slot");
    self = idtable.add_string("self");
    SELF_TYPE = idtable.add_string("SELF_TYPE");
    Str = idtable.add_string("String");
    str_field = idtable.add_string("_str_field");
    substr = idtable.add_string("substr");
    type_name = idtable.add_string("type_name");
    val = idtable.add_string("_val");
}

static char* gc_init_names[] = {"_NoGC_Init", "_GenGC_Init", "_ScnGC_Init"};
static char* gc_collect_names[] = {"_NoGC_Collect", "_GenGC_Collect",
                                   "_ScnGC_Collect"};

//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************

void program_class::cgen(ostream& os) {
    // spim wants comments to start with '#'
    os << "# start of generated code\n";

    initialize_constants();
    CgenClassTable* codegen_classtable = new CgenClassTable(classes, os);

    os << "\n# end of generated code\n";
}

//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_load(char* dest_reg, int offset, char* source_reg,
                      ostream& s) {
    s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")"
      << endl;
}

static void emit_store(char* source_reg, int offset, char* dest_reg,
                       ostream& s) {
    s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
      << endl;
}

static void emit_load_imm(char* dest_reg, int val, ostream& s) {
    s << LI << dest_reg << " " << val << endl;
}

static void emit_load_address(char* dest_reg, char* address, ostream& s) {
    s << LA << dest_reg << " " << address << endl;
}

static void emit_partial_load_address(char* dest_reg, ostream& s) {
    s << LA << dest_reg << " ";
}

static void emit_load_bool(char* dest, const BoolConst& b, ostream& s) {
    emit_partial_load_address(dest, s);
    b.code_ref(s);
    s << endl;
}

static void emit_load_string(char* dest, StringEntry* str, ostream& s) {
    emit_partial_load_address(dest, s);
    str->code_ref(s);
    s << endl;
}

static void emit_load_int(char* dest, IntEntry* i, ostream& s) {
    emit_partial_load_address(dest, s);
    i->code_ref(s);
    s << endl;
}

static void emit_move(char* dest_reg, char* source_reg, ostream& s) {
    s << MOVE << dest_reg << " " << source_reg << endl;
}

static void emit_neg(char* dest, char* src1, ostream& s) {
    s << NEG << dest << " " << src1 << endl;
}

static void emit_add(char* dest, char* src1, char* src2, ostream& s) {
    s << ADD << dest << " " << src1 << " " << src2 << endl;
}

static void emit_addu(char* dest, char* src1, char* src2, ostream& s) {
    s << ADDU << dest << " " << src1 << " " << src2 << endl;
}

static void emit_addiu(char* dest, char* src1, int imm, ostream& s) {
    s << ADDIU << dest << " " << src1 << " " << imm << endl;
}

static void emit_div(char* dest, char* src1, char* src2, ostream& s) {
    s << DIV << dest << " " << src1 << " " << src2 << endl;
}

static void emit_mul(char* dest, char* src1, char* src2, ostream& s) {
    s << MUL << dest << " " << src1 << " " << src2 << endl;
}

static void emit_sub(char* dest, char* src1, char* src2, ostream& s) {
    s << SUB << dest << " " << src1 << " " << src2 << endl;
}

static void emit_sll(char* dest, char* src1, int num, ostream& s) {
    s << SLL << dest << " " << src1 << " " << num << endl;
}

static void emit_jalr(char* dest, ostream& s) {
    s << JALR << "\t" << dest << endl;
}

static void emit_jal(char* address, ostream& s) { s << JAL << address << endl; }

static void emit_return(ostream& s) { s << RET << endl; }

static void emit_gc_assign(ostream& s) { s << JAL << "_GenGC_Assign" << endl; }

static void emit_disptable_ref(Symbol sym, ostream& s) {
    s << sym << DISPTAB_SUFFIX;
}

static void emit_init_ref(Symbol sym, ostream& s) {
    s << sym << CLASSINIT_SUFFIX;
}

static void emit_label_ref(int l, ostream& s) { s << "label" << l; }

static void emit_protobj_ref(Symbol sym, ostream& s) {
    s << sym << PROTOBJ_SUFFIX;
}

static void emit_method_ref(Symbol classname, Symbol methodname, ostream& s) {
    s << classname << METHOD_SEP << methodname;
}

static void emit_jal_method(Symbol classname, Symbol methodname, ostream& s) {
    s << JAL << classname << METHOD_SEP << methodname << endl;
}

static void emit_jal_init(Symbol sym, ostream& s) {
    s << JAL << sym << CLASSINIT_SUFFIX << endl;
}

static void emit_label_def(int l, ostream& s) {
    emit_label_ref(l, s);
    s << ":" << endl;
}

static void emit_beqz(char* source, int label, ostream& s) {
    s << BEQZ << source << " ";
    emit_label_ref(label, s);
    s << endl;
}

static void emit_beq(char* src1, char* src2, int label, ostream& s) {
    s << BEQ << src1 << " " << src2 << " ";
    emit_label_ref(label, s);
    s << endl;
}

static void emit_bne(char* src1, char* src2, int label, ostream& s) {
    s << BNE << src1 << " " << src2 << " ";
    emit_label_ref(label, s);
    s << endl;
}

static void emit_bleq(char* src1, char* src2, int label, ostream& s) {
    s << BLEQ << src1 << " " << src2 << " ";
    emit_label_ref(label, s);
    s << endl;
}

static void emit_blt(char* src1, char* src2, int label, ostream& s) {
    s << BLT << src1 << " " << src2 << " ";
    emit_label_ref(label, s);
    s << endl;
}

static void emit_blti(char* src1, int imm, int label, ostream& s) {
    s << BLT << src1 << " " << imm << " ";
    emit_label_ref(label, s);
    s << endl;
}

static void emit_bgti(char* src1, int imm, int label, ostream& s) {
    s << BGT << src1 << " " << imm << " ";
    emit_label_ref(label, s);
    s << endl;
}

static void emit_branch(int l, ostream& s) {
    s << BRANCH;
    emit_label_ref(l, s);
    s << endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
static void emit_push(char* reg, ostream& str) {
    emit_store(reg, 0, SP, str);
    emit_addiu(SP, SP, -4, str);
}

//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source into the register dest
//
static void emit_fetch_int(char* dest, char* source, ostream& s) {
    emit_load(dest, DEFAULT_OBJFIELDS, source, s);
}

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
static void emit_store_int(char* source, char* dest, ostream& s) {
    emit_store(source, DEFAULT_OBJFIELDS, dest, s);
}

static void emit_test_collector(ostream& s) {
    emit_push(ACC, s);
    emit_move(ACC, SP, s);  // stack end
    emit_move(A1, ZERO, s); // allocate nothing
    s << JAL << gc_collect_names[cgen_Memmgr] << endl;
    emit_addiu(SP, SP, 4, s);
    emit_load(ACC, 0, SP, s);
}

static void emit_gc_check(char* source, ostream& s) {
    if (source != (char*)A1)
        emit_move(A1, source, s);
    s << JAL << "_gc_check" << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s) { s << STRCONST_PREFIX << index; }

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream& s, int stringclasstag) {
    IntEntryP lensym = inttable.add_int(len);

    // Add -1 eye catcher
    s << WORD << "-1" << endl;

    code_ref(s);
    s << LABEL                          // label
      << WORD << stringclasstag << endl // tag
      << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len + 4) / 4)
      << endl // size
      << WORD;

    /***** Add dispatch information for class String ******/

    s << Str << DISPTAB_SUFFIX << endl; // dispatch table
    s << WORD;
    lensym->code_ref(s);
    s << endl;                    // string length
    emit_string_constant(s, str); // ascii string
    s << ALIGN;                   // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the
// stringtable.
//
void StrTable::code_string_table(ostream& s, int stringclasstag) {
    for (List<StringEntry>* l = tbl; l; l = l->tl()) {
        l->hd()->code_def(s, stringclasstag);
    }
}

//
// Ints
//
void IntEntry::code_ref(ostream& s) { s << INTCONST_PREFIX << index; }

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream& s, int intclasstag) {
    // Add -1 eye catcher
    s << WORD << "-1" << endl;

    code_ref(s);
    s << LABEL                                           // label
      << WORD << intclasstag << endl                     // class tag
      << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl // object size
      << WORD;

    /***** Add dispatch information for class Int ******/

    s << Int << DISPTAB_SUFFIX << endl; // dispatch table
    s << WORD << str << endl;           // integer value
}

//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream& s, int intclasstag) {
    for (List<IntEntry>* l = tbl; l; l = l->tl())
        l->hd()->code_def(s, intclasstag);
}

//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream& s) const { s << BOOLCONST_PREFIX << val; }

//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream& s, int boolclasstag) {
    // Add -1 eye catcher
    s << WORD << "-1" << endl;

    code_ref(s);
    s << LABEL                                            // label
      << WORD << boolclasstag << endl                     // class tag
      << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl // object size
      << WORD;

    /***** Add dispatch information for class Bool ******/

    s << Bool << DISPTAB_SUFFIX << endl; // dispatch table
    s << WORD << val << endl;            // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//***************************************************
//
//  Emit code to start the .data segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_data() {
    Symbol main = idtable.lookup_string(MAINNAME);
    Symbol string = idtable.lookup_string(STRINGNAME);
    Symbol integer = idtable.lookup_string(INTNAME);
    Symbol boolc = idtable.lookup_string(BOOLNAME);

    str << "\t.data\n" << ALIGN;
    //
    // The following global names must be defined first.
    //
    str << GLOBAL << CLASSNAMETAB << endl;
    str << GLOBAL;
    emit_protobj_ref(main, str);
    str << endl;
    str << GLOBAL;
    emit_protobj_ref(integer, str);
    str << endl;
    str << GLOBAL;
    emit_protobj_ref(string, str);
    str << endl;
    str << GLOBAL;
    falsebool.code_ref(str);
    str << endl;
    str << GLOBAL;
    truebool.code_ref(str);
    str << endl;
    str << GLOBAL << INTTAG << endl;
    str << GLOBAL << BOOLTAG << endl;
    str << GLOBAL << STRINGTAG << endl;

    //
    // We also need to know the tag of the Int, String, and Bool classes
    // during code generation.
    //
    str << INTTAG << LABEL << WORD << intclasstag << endl;
    str << BOOLTAG << LABEL << WORD << boolclasstag << endl;
    str << STRINGTAG << LABEL << WORD << stringclasstag << endl;
}

//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text() {
    str << GLOBAL << HEAP_START << endl
        << HEAP_START << LABEL << WORD << 0 << endl
        << "\t.text" << endl
        << GLOBAL;
    emit_init_ref(idtable.add_string("Main"), str);
    str << endl << GLOBAL;
    emit_init_ref(idtable.add_string("Int"), str);
    str << endl << GLOBAL;
    emit_init_ref(idtable.add_string("String"), str);
    str << endl << GLOBAL;
    emit_init_ref(idtable.add_string("Bool"), str);
    str << endl << GLOBAL;
    emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"),
                    str);
    str << endl;
}

void CgenClassTable::code_bools(int boolclasstag) {
    falsebool.code_def(str, boolclasstag);
    truebool.code_def(str, boolclasstag);
}

void CgenClassTable::code_select_gc() {
    //
    // Generate GC choice constants (pointers to GC functions)
    //
    str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
    str << "_MemMgr_INITIALIZER:" << endl;
    str << WORD << gc_init_names[cgen_Memmgr] << endl;
    str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
    str << "_MemMgr_COLLECTOR:" << endl;
    str << WORD << gc_collect_names[cgen_Memmgr] << endl;
    str << GLOBAL << "_MemMgr_TEST" << endl;
    str << "_MemMgr_TEST:" << endl;
    str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
}

//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************

void CgenClassTable::code_constants() {
    //
    // Add constants that are required by the code generator.
    //
    stringtable.add_string("");
    inttable.add_string("0");

    stringtable.code_string_table(str, stringclasstag);
    inttable.code_string_table(str, intclasstag);
    code_bools(boolclasstag);
}

CgenClassTable::CgenClassTable(Classes classes, ostream& s)
    : nds(NULL), str(s) {

    enterscope();
    if (cgen_debug)
        cout << "Building CgenClassTable" << endl;
    install_basic_classes();
    install_classes(classes);
    build_inheritance_tree();

    if (cgen_debug)
        cout << "set class infos" << endl;
    set_class_infos();

    if (cgen_debug)
        cout << "code" << endl;
    code();
    exitscope();
}

/**
 * @brief build attribute table and method table as inheritance requires
 */
void CgenClassTable::set_class_infos() {
    /*start from object class*/
    CgenNodeP root = lookup(Object);
    root->set_info(0, tagList, this);
}

void CgenClassTable::install_basic_classes() {

    // The tree package uses these globals to annotate the classes built below.
    // curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");

    //
    // A few special class names are installed in the lookup table but not
    // the class list.  Thus, these classes exist, but are not part of the
    // inheritance hierarchy.
    // No_class serves as the parent of Object and the other special classes.
    // SELF_TYPE is the self class; it cannot be redefined or inherited.
    // prim_slot is a class known to the code generator.
    //
    addid(No_class,
          new CgenNode(class_(No_class, No_class, nil_Features(), filename),
                       Basic, this));
    addid(SELF_TYPE,
          new CgenNode(class_(SELF_TYPE, No_class, nil_Features(), filename),
                       Basic, this));
    addid(prim_slot,
          new CgenNode(class_(prim_slot, No_class, nil_Features(), filename),
                       Basic, this));

    //
    // The Object class has no parent class. Its methods are
    //        cool_abort() : Object    aborts the program
    //        type_name() : Str        returns a string representation of class
    //        name
    //        copy() : SELF_TYPE       returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.
    //
    install_class(new CgenNode(
        class_(Object, No_class,
               append_Features(
                   append_Features(
                       single_Features(method(cool_abort, nil_Formals(), Object,
                                              no_expr())),
                       single_Features(
                           method(type_name, nil_Formals(), Str, no_expr()))),
                   single_Features(
                       method(COPY, nil_Formals(), SELF_TYPE, no_expr()))),
               filename),
        Basic, this));

    //
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE          writes a string to the output
    //        out_int(Int) : SELF_TYPE               "    an int    "  "     "
    //        in_string() : Str                    reads a string from the input
    //        in_int() : Int                         "   an int     "  "     "
    //
    install_class(new CgenNode(
        class_(
            IO, Object,
            append_Features(
                append_Features(
                    append_Features(
                        single_Features(method(out_string,
                                               single_Formals(formal(arg, Str)),
                                               SELF_TYPE, no_expr())),
                        single_Features(method(out_int,
                                               single_Formals(formal(arg, Int)),
                                               SELF_TYPE, no_expr()))),
                    single_Features(
                        method(in_string, nil_Formals(), Str, no_expr()))),
                single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
            filename),
        Basic, this));

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer.
    //
    install_class(new CgenNode(
        class_(Int, Object, single_Features(attr(val, prim_slot, no_expr())),
               filename),
        Basic, this));

    //
    // Bool also has only the "val" slot.
    //
    install_class(new CgenNode(
        class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),
               filename),
        Basic, this));

    //
    // The class Str has a number of slots and operations:
    //       val                                  ???
    //       str_field                            the string itself
    //       length() : Int                       length of the string
    //       concat(arg: Str) : Str               string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring
    //
    install_class(new CgenNode(
        class_(Str, Object,
               append_Features(
                   append_Features(
                       append_Features(
                           append_Features(
                               single_Features(attr(val, Int, no_expr())),
                               single_Features(
                                   attr(str_field, prim_slot, no_expr()))),
                           single_Features(
                               method(length, nil_Formals(), Int, no_expr()))),
                       single_Features(method(concat,
                                              single_Formals(formal(arg, Str)),
                                              Str, no_expr()))),
                   single_Features(
                       method(substr,
                              append_Formals(single_Formals(formal(arg, Int)),
                                             single_Formals(formal(arg2, Int))),
                              Str, no_expr()))),
               filename),
        Basic, this));
}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_class(CgenNodeP nd) {
    Symbol name = nd->get_name();

    if (probe(name)) {
        return;
    }

    // The class name is legal, so add it to the list of classes
    // and the symbol table.
    nds = new List<CgenNode>(nd, nds);
    addid(name, nd);
}

void CgenClassTable::install_classes(Classes cs) {
    for (int i = cs->first(); cs->more(i); i = cs->next(i))
        install_class(new CgenNode(cs->nth(i), NotBasic, this));
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree() {
    for (List<CgenNode>* l = nds; l; l = l->tl())
        set_relations(l->hd());
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNodeP nd) {
    CgenNode* parent_node = probe(nd->get_parent());
    nd->set_parentnd(parent_node);
    parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n) {
    children = new List<CgenNode>(n, children);
}

void CgenNode::set_parentnd(CgenNodeP p) {
    assert(parentnd == NULL);
    assert(p != NULL);
    parentnd = p;
}

void CgenClassTable::code() {
    if (cgen_debug)
        cout << "coding global data" << endl;
    code_global_data();

    if (cgen_debug)
        cout << "choosing gc" << endl;
    code_select_gc();

    if (cgen_debug)
        cout << "coding constants" << endl;
    code_constants();

    /*class name tab */
    if (cgen_debug)
        cout << "coding class nametab" << endl;
    code_class_nametab();

    if (cgen_debug)
        cout << "coding class objtab" << endl;
    code_class_objtab();

    /*dispatch tab */
    if (cgen_debug)
        cout << "coding dispatch table" << endl;
    code_disptables();

    if (cgen_debug)
        cout << "coding protoObj" << endl;
    code_prot_objs();

    if (cgen_debug)
        cout << "coding global text" << endl;
    code_global_text();

    if (cgen_debug)
        cout << "coding object initializer" << endl;
    code_init();

    if (cgen_debug)
        cout << "coding class methods" << endl;

    code_classes();
}

/**
 * @brief generate class name tab code
 *
 */
void CgenClassTable::code_class_nametab() {
    str << CLASSNAMETAB;
    str << LABEL;
    std::list<Symbol>::iterator it;
    for (it = tagList.begin(); it != tagList.end(); ++it) {
        Symbol className = *it;
        str << WORD;
        int class_str_index =
            stringtable.lookup_string(className->get_string())->get_index();
        str << STRCONST_PREFIX << class_str_index << endl;
    }
}

void CgenClassTable::code_class_objtab() {
    str << CLASSOBJTAB << LABEL;
    for (auto it = tagList.begin(); it != tagList.end(); ++it) {
        Symbol className = (*it);
        str << WORD << className << PROTOBJ_SUFFIX << endl;
        str << WORD << className << CLASSINIT_SUFFIX << endl;
    }
}

/**
 * @brief generate method dispatch tables
 *
 */
void CgenClassTable::code_disptables() {
    for (List<CgenNode>* l = nds; l; l = l->tl()) {
        CgenNodeP node = l->hd();
        str << node->name << DISPTAB_SUFFIX << LABEL;
        unsigned int method_num = node->methodTable->size();
        method_class* methodDispTable[method_num];
        node->get_method_disp_table(methodDispTable);
        for (size_t i = 0; i < method_num; i++) {
            method_class* method = methodDispTable[i];
            str << WORD << method->className << METHOD_SEP << method->name
                << endl;
        }
    }
}

/**
 * @brief get classtag through class name
 *
 * @param className Symbol
 */
int CgenClassTable::get_classtag(Symbol className) {
    std::list<Symbol>::iterator pos =
        std::find(tagList.begin(), tagList.end(), className);
    int index = std::distance(tagList.begin(), pos);
    return index;
}

/**
 * @brief generate protobjs
 *
 */
void CgenClassTable::code_prot_objs() {
    int nullstr_const_index = stringtable.lookup_string("")->get_index();
    int zeroint_const_index = inttable.lookup_string("0")->get_index();
    for (List<CgenNode>* l = nds; l; l = l->tail) {
        CgenNodeP node = l->head;
        str << WORD << "-1" << endl;
        str << node->name << PROTOBJ_SUFFIX << LABEL;
        int classtag = get_classtag(node->name);
        int objSize = DEFAULT_OBJFIELDS + node->attrTable->size();
        str << WORD << classtag << endl;
        str << WORD << objSize << endl;
        str << WORD << node->name << DISPTAB_SUFFIX << endl;
        std::list<attr_class*>::iterator it = (*node->attrTable).begin();
        std::list<attr_class*>::iterator end = (*node->attrTable).end();
        while (it != end) {
            attr_class* attr = *it;
            Symbol type_decl = attr->type_decl;
            Expression init_expr = attr->init;
            if (type_decl == Str) {
                str << WORD << STRCONST_PREFIX << nullstr_const_index << endl;
            } else if (type_decl == Int) {
                str << WORD << INTCONST_PREFIX << zeroint_const_index << endl;
            } else if (type_decl == Bool) {
                str << WORD << BOOLCONST_PREFIX << FALSE << endl;
            } else {
                str << WORD << 0 << endl;
            }
            it++;
        }
    }
}

/**
 * @brief generate codes when enter a function
 *
 * @param s output stream
 */
static void code_func_prefix(ostream& s) {
    emit_addiu(SP, SP, -12, s);
    emit_store(FP, 3, SP, s);
    emit_store(SELF, 2, SP, s);
    emit_store(RA, 1, SP, s);
    emit_addiu(FP, SP, 4, s);
    emit_move(SELF, ACC, s);
}

/**
 * @brief generate codes when exit a function
 *
 * @param s output stream
 */
static void code_func_suffix(ostream& s) {
    emit_load(FP, 3, SP, s);
    emit_load(SELF, 2, SP, s);
    emit_load(RA, 1, SP, s);
    emit_addiu(SP, SP, 12, s);
}

template <typename W, typename T> std::list<T> get_values(std::map<W, T> map_) {
    std::list<T> ret;
    for (auto it = map_.begin(); it != map_.end(); ++it) {
        ret.push_back(it->second);
    }
    return ret;
}

static void generate_new_class(ostream& s, Symbol type_name) {
    emit_partial_load_address(ACC, s);
    emit_protobj_ref(type_name, s);
    s << endl;
    emit_jal_method(Object, COPY, s);
    s << JAL << "\t";
    emit_init_ref(type_name, s);
    s << endl;
}

/**
 * @brief generate object init method
 *
 */
void CgenClassTable::code_init() {
    for (auto i = nds; i; i = i->tail) {
        CgenNodeP node = i->hd();
        str << node->name << CLASSINIT_SUFFIX << LABEL;
        code_func_prefix(str);

        if (node->parent != No_class) { // parent init
            char* parentName = strdup(node->parent->get_string());
            parentName = strcat(parentName, CLASSINIT_SUFFIX);
            emit_jal(parentName, str);
        }

        /*attribute initialization */
        if (node->name != Str && node->name != Int && node->name != Bool) {
            auto attrTable = node->attrTable;
            node->env_init();
            for (auto it = attrTable->begin(); it != attrTable->end(); ++it) {
                attr_class* attr = *it;
                if (attr->className == node->name) {
                    Expression init = attr->init;
                    init->localEnv = node->localEnv;
                    init->classTable = (SymbolTable<Symbol, Class__class>*)this;
                    str << "# init attr " << attr->name << endl;
                    init->code(str);
                    if (init->type != No_type) {
                        emit_store(ACC,
                                   node->get_attrtag(attr->name) +
                                       DEFAULT_OBJFIELDS,
                                   SELF, str);
                    }
                }
            }
        }

        emit_move(ACC, SELF, str);
        code_func_suffix(str);
        emit_return(str);
    }
}

void CgenClassTable::code_classes() {
    for (auto l = nds; l; l = l->tail) {
        CgenNodeP node = l->hd();
        if (!node->basic()) {
            node->code_methods(str, this);
        }
    }
}

CgenNodeP CgenClassTable::root() { return probe(Object); }

///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct)
    : class__class((const class__class&)*nd), parentnd(NULL), children(NULL),
      basic_status(bstatus) {
    stringtable.add_string(
        name->get_string()); // Add class name to string table
}

void CgenNode::get_method_disp_table(method_class* methods[]) {
    for (auto it = methodTable->begin(); it != methodTable->end(); ++it) {
        method_class* method = it->second;
        methods[method->methodTag] = method;
    }
}

/**
 * @brief set classtag, attribute table, method table as required by inheritance
 */
int CgenNode::set_info(int classtag, std::list<Symbol>& tagList,
                       CgenClassTableP classTable) {
    this->classtag = classtag;
    tagList.push_back(name);
    if (name == Str) {
        classTable->stringclasstag = classtag;
    }
    if (name == Bool) {
        classTable->boolclasstag = classtag;
    }
    if (name == Int) {
        classTable->intclasstag = classtag;
    }
    classtag++; /*increment classtag by inheritance order as it makes case expr
                   much easier */
    /*set method and attribute table */
    if (methodTable == NULL) {
        methodTable = new std::map<Symbol, method_class*>();
    }
    if (attrTable == NULL) {
        attrTable = new std::list<attr_class*>();
    }
    unsigned int methodTag = methodTable->size();
    for (size_t i = features->first(); features->more(i);
         i = features->next(i)) {
        Feature feature = features->nth(i);
        if (feature->feature_type == methodFeature) {
            ((method_class*)feature)->className = name;
            Symbol methodName = ((method_class*)feature)->name;
            if (methodTable->find(methodName) != methodTable->end()) {
                ((method_class*)feature)->methodTag =
                    ((methodTable->find(methodName))->second)->methodTag;
            } else {
                ((method_class*)feature)->methodTag = methodTag++;
            }
            (*methodTable)[methodName] = (method_class*)feature;
        } else if (feature->feature_type == attrFeature) {
            ((attr_class*)feature)->className = name;
            attrTable->push_back((attr_class*)feature);
        }
    }

    if (children != NULL) {
        for (List<CgenNode>* l = children; l; l = l->tl()) {
            CgenNodeP child = l->hd();
            child->methodTable =
                new std::map<Symbol, method_class*>(*methodTable);
            child->attrTable = new std::list<attr_class*>(*attrTable);
            classtag = child->set_info(classtag, tagList, classTable);
        }
    }
    return classtag;
}

/**
 * @brief return an attribute's attrtag by its attrName
 *
 * @param attrName[Symbol]
 * @return int
 */
int CgenNode::get_attrtag(Symbol attrName) {
    for (auto it = attrTable->begin(); it != attrTable->end(); ++it) {
        if ((*it)->name == attrName) {
            return std::distance(attrTable->begin(), it);
        }
    }
    return -1;
}

/**
 * @brief get local variable environment
 *
 * @return LocalEnv*
 */
void CgenNode::env_init() {
    localEnv = new LocalEnv();
    localEnv->enterscope();
    localEnv->className = name;
    for (auto it = attrTable->begin(); it != attrTable->end(); ++it) {
        attr_class* attr = *it;
        VarInfo* info = new VarInfo;
        info->storePos = ATTRIBUTE;
        info->pos = get_attrtag(attr->name);
        localEnv->addid(attr->name, info);
    }
}

/**
 * @brief check if a method is a basic method
 *
 * @param methodName
 * @return true
 * @return false
 */
static bool isBasicMethod(Symbol methodName) {
    return methodName == Object || methodName == IO || methodName == Str ||
           methodName == Int;
}

/**
 * @brief generate code for every methods
 *
 */
void CgenNode::code_methods(ostream& str, CgenClassTableP classTable) {
    unsigned int method_num = methodTable->size();
    method_class* methodDispTable[method_num];
    get_method_disp_table(methodDispTable);
    for (size_t i = 0; i < method_num; i++) {
        method_class* method = methodDispTable[i];
        if (!isBasicMethod(method->className) &&
            method->className == name) { // ignore basic methods
            method->classTable = (SymbolTable<Symbol, Class__class>*)classTable;
            method->localEnv = localEnv;
            method->code(str);
        }
    }
}

//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

/**
 * @brief generate void dispatch check
 *
 * @param filename
 * @param line_number
 * @param s
 */
static void void_ref_check(int line_number, ostream& s) {
    emit_bne(ACC, ZERO, labelTag, s);
    emit_load_string(ACC, stringtable.lookup(0), s);
    emit_load_imm(T1, line_number, s);
    emit_jal("_dispatch_abort", s);
    emit_label_def(labelTag++, s);
}

void method_class::code(ostream& str) {
    localEnv->enterscope();

    str << className << METHOD_SEP << name << LABEL;
    code_func_prefix(str);
    stackDepth = 0;

    for (int i = formals->first(); formals->more(i); i = formals->next(i)) {
        formal_class* formal = (formal_class*)formals->nth(i);
        VarInfo* info = new VarInfo;
        info->storePos = STACK;
        info->pos = formals->len() - i + 2;
        localEnv->addid(formal->name, info);
    }

    expr->classTable = classTable;
    expr->localEnv = localEnv;
    expr->code(str); // generate expr code

    code_func_suffix(str);
    if (formals->len() > 0) {
        emit_addiu(SP, SP, WORD_SIZE * formals->len(), str); // pop argument
    }
    emit_return(str);

    localEnv->exitscope();
}

void assign_class::code(ostream& s) {
    expr->localEnv = localEnv;
    expr->classTable = classTable;
    expr->code(s);
    VarInfo* info = localEnv->lookup(name);
    if (info->storePos == ATTRIBUTE) {
        emit_store(ACC, DEFAULT_OBJFIELDS + info->pos, SELF, s);
    } else if (info->storePos == STACK) {
        emit_store(ACC, info->pos, FP, s);
    }
}

void static_dispatch_class::code(ostream& s) {
    s << "# static dispatch " << name << endl;
    for (size_t i = actual->first(); actual->more(i); i = actual->next(i)) {
        Expression actualExpr = actual->nth(i);
        actualExpr->localEnv = localEnv;
        actualExpr->classTable = classTable;
        actualExpr->code(s);
        emit_push(ACC, s);
        stackDepth++;
    }
    expr->localEnv = localEnv;
    expr->classTable = classTable;
    expr->code(s);
    void_ref_check(line_number, s);
    emit_partial_load_address(T1, s);
    emit_disptable_ref(type_name, s);
    s << endl;
    CgenNodeP node = (CgenNodeP)classTable->lookup(type_name);
    int methodTag = (*(node->methodTable))[name]->methodTag;
    s << "# methodTag for " << name << endl;
    emit_load(T1, methodTag, T1, s);
    emit_jalr(T1, s);
    stackDepth = stackDepth - actual->len();
}

void dispatch_class::code(ostream& s) {
    s << "# dispatch " << name << endl;
    for (size_t i = actual->first(); actual->more(i); i = actual->next(i)) {
        Expression actualExpr = actual->nth(i);
        actualExpr->localEnv = localEnv;
        actualExpr->classTable = classTable;
        actualExpr->code(s);
        emit_push(ACC, s);
        stackDepth++;
    }
    CgenNodeP node;
    if (expr->type == SELF_TYPE) {
        node = (CgenNodeP)classTable->lookup(localEnv->className);
    } else {
        node = (CgenNodeP)classTable->lookup(expr->type);
    }
    expr->localEnv = localEnv;
    expr->classTable = classTable;
    expr->code(s);
    void_ref_check(line_number, s);
    int methodTag = (*(node->methodTable))[name]->methodTag;
    s << "# methodTag for " << node->name << METHOD_SEP << name << endl;
    emit_load(T1, 2, ACC, s);
    emit_load(T1, methodTag, T1, s);
    emit_jalr(T1, s);
    stackDepth = stackDepth - actual->len();
}

void cond_class::code(ostream& s) {
    s << "# cond class" << endl;
    pred->localEnv = localEnv;
    pred->classTable = classTable;
    pred->code(s);
    emit_load(T1, 3, ACC, s);
    int falseTag = labelTag++;
    emit_beqz(T1, falseTag, s);
    int nextTag = labelTag++;
    then_exp->localEnv = localEnv;
    then_exp->classTable = classTable;
    then_exp->code(s);
    emit_branch(nextTag, s);
    emit_label_def(falseTag, s);
    else_exp->localEnv = localEnv;
    else_exp->classTable = classTable;
    else_exp->code(s);
    emit_label_def(nextTag, s);
}

void loop_class::code(ostream& s) {
    s << "# loop class" << endl;
    pred->localEnv = localEnv;
    pred->classTable = classTable;
    int predTag = labelTag++;
    emit_label_def(predTag, s);
    s << "#pred " << endl;
    pred->code(s);
    emit_load(T1, 3, ACC, s);
    int exitTag = labelTag++;
    emit_beqz(T1, exitTag, s);
    body->localEnv = localEnv;
    body->classTable = classTable;
    s << "# body" << endl;
    body->code(s);
    emit_branch(predTag, s);
    emit_label_def(exitTag, s);
    emit_move(ACC, ZERO, s);
}

static void void_case_check(int line_number, ostream& s) {
    emit_bne(ACC, ZERO, labelTag, s);
    emit_load_string(ACC, stringtable.lookup(0), s);
    emit_load_imm(T1, line_number, s);
    emit_jal("_case_abort2", s);
}

void typcase_class::code(ostream& s) {
    s << "#typcase class " << endl;
    expr->localEnv = localEnv;
    expr->classTable = classTable;
    expr->code(s);
    void_case_check(line_number, s);
    emit_label_def(labelTag++, s);
    emit_push(ACC, s);
    stackDepth++;
    emit_load(ACC, 0, ACC, s); // load class tag
    int endTag = labelTag++;
    std::list<int> case_classtags;
    std::map<int, branch_class*> case_map;
    for (size_t i = 0; cases->more(i); i = cases->next(i)) {
        branch_class* branch = (branch_class*) cases->nth(i);
        int classTag = ((CgenClassTableP)classTable)->get_classtag(branch->type_decl);
        case_classtags.push_front(classTag);
        case_map[classTag] = branch;
    }
    case_classtags.sort();
    case_classtags.reverse();
    for (auto it = case_classtags.begin(); it != case_classtags.end(); it++) {
        int caseClassTag = *it;
        branch_class* branch = case_map[caseClassTag];
        branch->expr->localEnv = localEnv;
        branch->expr->classTable = classTable;
        branch->expr->localEnv->enterscope();
        VarInfo* info = new VarInfo;
        info->storePos = STACK;
        info->pos = -stackDepth;
        branch->expr->localEnv->addid(branch->name, info);
        CgenNodeP caseClass = ((CgenClassTableP)classTable)->lookup(expr->type);
        if (caseClass->get_children() == NULL) {
            emit_load_imm(T1, caseClassTag, s);
            int nextTag = labelTag++;
            emit_bne(ACC, T1, nextTag, s);
            branch->expr->code(s);
            emit_branch(endTag, s);
            emit_label_def(nextTag, s);
        } else {
            List<CgenNode>* children = caseClass->get_children();
            int firstClassTag = caseClassTag;
            CgenNodeP node;
            while (children != NULL) {
                for (auto l = children; l; l = l->tl()) {
                    node = l->hd();
                }
                children = node->get_children();
            }
            int lastClassTag =
                ((CgenClassTableP)classTable)->get_classtag(node->name);
            int nextTag = labelTag++;
            emit_blti(ACC, firstClassTag, nextTag, s);
            emit_bgti(ACC, lastClassTag, nextTag, s);
            branch->expr->code(s);
            emit_branch(endTag, s);
            emit_label_def(nextTag, s);
        }
        branch->expr->localEnv->exitscope();
    }
    emit_load(ACC, 1, SP, s);
    emit_jal("_case_abort", s);

    emit_label_def(endTag, s);
    emit_addiu(SP, SP, 4, s);
    stackDepth--;
}

void block_class::code(ostream& s) {
    for (size_t i = 0; body->more(i); i = body->next(i)) {
        Expression expr = body->nth(i);
        expr->localEnv = localEnv;
        expr->classTable = classTable;
        expr->code(s);
    }
}

void let_class::code(ostream& s) {
    s << "# letexpr" << endl;
    init->localEnv = localEnv;
    init->classTable = classTable;
    init->code(s);
    if (init->type == No_type) {
        if (type_decl == Str) {
            emit_load_string(ACC, stringtable.lookup_string(""), s);
        } else if (type_decl == Int) {
            emit_load_int(ACC, inttable.lookup_string("0"), s);
        } else if (type_decl == Bool) {
            emit_load_bool(ACC, falsebool, s);
        } else {
            emit_move(ACC, ZERO, s);
            // if (type_decl == SELF_TYPE) {
            //     emit_move(ACC, SELF, s);
            //     emit_jal_method(Object, COPY, s);
            //     emit_load_address(T1, CLASSOBJTAB, s);
            //     emit_load(T2, 0, SELF, s);
            //     emit_sll(T2, T2, 3, s);
            //     emit_add(T1, T1, T2, s);
            //     emit_load(T1, 1, T1, s);
            //     emit_jalr(T1, s);
            // } else {
            //     generate_new_class(s, type_decl);
            // }
        }
    }
    emit_push(ACC, s);
    stackDepth++;
    // cout << stackDepth << endl;
    localEnv->enterscope();
    VarInfo* info = new VarInfo;
    info->storePos = STACK;
    info->pos = -stackDepth;
    localEnv->addid(identifier, info);
    body->localEnv = localEnv;
    body->classTable = classTable;
    body->code(s);
    localEnv->exitscope();
    emit_addiu(SP, SP, 4, s);
    stackDepth--;
}

void plus_class::code(ostream& s) {
    e1->localEnv = localEnv;
    e1->classTable = classTable;
    e1->code(s);
    emit_push(ACC, s);
    stackDepth++;
    e2->localEnv = localEnv;
    e2->classTable = classTable;
    e2->code(s);
    emit_load(T1, 1, SP, s);
    emit_fetch_int(T1, T1, s);
    emit_fetch_int(T2, ACC, s);
    emit_add(T1, T1, T2, s);
    emit_push(T1, s);
    stackDepth++;
    emit_jal_method(Object, COPY, s);
    emit_load(T1, 1, SP, s);
    emit_store_int(T1, ACC, s);
    emit_addiu(SP, SP, 8, s);
    stackDepth -= 2;
}

void sub_class::code(ostream& s) {
    e1->localEnv = localEnv;
    e1->classTable = classTable;
    e1->code(s);
    emit_push(ACC, s);
    stackDepth++;
    e2->localEnv = localEnv;
    e2->classTable = classTable;
    e2->code(s);
    emit_load(T1, 1, SP, s);
    emit_fetch_int(T1, T1, s);
    emit_fetch_int(T2, ACC, s);
    emit_sub(T1, T1, T2, s);
    emit_push(T1, s);
    stackDepth++;
    emit_jal_method(Object, COPY, s);
    emit_load(T1, 1, SP, s);
    emit_store_int(T1, ACC, s);
    emit_addiu(SP, SP, 8, s);
    stackDepth -= 2;
}

void mul_class::code(ostream& s) {
    e1->localEnv = localEnv;
    e1->classTable = classTable;
    e1->code(s);
    emit_push(ACC, s);
    stackDepth++;
    e2->localEnv = localEnv;
    e2->classTable = classTable;
    e2->code(s);
    emit_load(T1, 1, SP, s);
    emit_fetch_int(T1, T1, s);
    emit_fetch_int(T2, ACC, s);
    emit_mul(T1, T1, T2, s);
    emit_push(T1, s);
    stackDepth++;
    emit_jal_method(Object, COPY, s);
    emit_load(T1, 1, SP, s);
    emit_store_int(T1, ACC, s);
    emit_addiu(SP, SP, 8, s);
    stackDepth -= 2;
}

void divide_class::code(ostream& s) {
    e1->localEnv = localEnv;
    e1->classTable = classTable;
    e1->code(s);
    emit_push(ACC, s);
    stackDepth++;
    e2->localEnv = localEnv;
    e2->classTable = classTable;
    e2->code(s);
    emit_load(T1, 1, SP, s);
    emit_fetch_int(T1, T1, s);
    emit_fetch_int(T2, ACC, s);
    emit_div(T1, T1, T2, s);
    emit_push(T1, s);
    stackDepth++;
    emit_jal_method(Object, COPY, s);
    emit_load(T1, 1, SP, s);
    emit_store_int(T1, ACC, s);
    emit_addiu(SP, SP, 8, s);
    stackDepth -= 2;
}

void neg_class::code(ostream& s) {
    e1->localEnv = localEnv;
    e1->classTable = classTable;
    e1->code(s);
    emit_fetch_int(T1, ACC, s);
    emit_sub(T1, ZERO, T1, s);
    emit_push(T1, s);
    stackDepth++;
    emit_jal_method(Object, COPY, s);
    emit_load(T1, 1, SP, s);
    emit_store_int(T1, ACC, s);
    emit_addiu(SP, SP, 4, s);
    stackDepth--;
}

void lt_class::code(ostream& s) {
    e1->localEnv = localEnv;
    e1->classTable = classTable;
    e1->code(s);
    emit_push(ACC, s);
    stackDepth++;
    e2->localEnv = localEnv;
    e2->classTable = classTable;
    e2->code(s);
    emit_load(T1, 1, SP, s);
    emit_load(T1, 3, T1, s);
    emit_load(T2, 3, ACC, s);
    emit_load_bool(ACC, truebool, s);
    emit_blt(T1, T2, labelTag, s);
    emit_load_bool(ACC, falsebool, s);
    emit_label_def(labelTag++, s);
    emit_addiu(SP, SP, 4, s);
    stackDepth--;
}

void eq_class::code(ostream& s) {
    e1->localEnv = localEnv;
    e1->classTable = classTable;
    e1->code(s);
    emit_push(ACC, s);
    stackDepth++;
    e2->localEnv = localEnv;
    e2->classTable = classTable;
    e2->code(s);
    emit_move(T2, ACC, s);
    emit_load(T1, 1, SP, s);
    emit_load_bool(ACC, truebool, s);
    emit_load_bool(A1, falsebool, s);
    emit_beq(T1, T2, labelTag, s);
    emit_jal("equality_test", s);
    emit_label_def(labelTag++, s);
    emit_addiu(SP, SP, 4, s);
    stackDepth--;
}

void leq_class::code(ostream& s) {
    e1->localEnv = localEnv;
    e1->classTable = classTable;
    e1->code(s);
    emit_push(ACC, s);
    stackDepth++;
    e2->localEnv = localEnv;
    e2->classTable = classTable;
    e2->code(s);
    emit_load(T1, 1, SP, s);
    emit_fetch_int(T1, T1, s);
    emit_fetch_int(T2, ACC, s);
    emit_load_bool(ACC, truebool, s);
    emit_blt(T1, T2, labelTag, s);
    emit_beq(T1, T2, labelTag, s);
    emit_load_bool(ACC, falsebool, s);
    emit_label_def(labelTag++, s);
    emit_addiu(SP, SP, 4, s);
    stackDepth--;
}

void comp_class::code(ostream& s) {
    s << "# comp class" << endl;
    e1->localEnv = localEnv;
    e1->classTable = classTable;
    e1->code(s);
    emit_load(T1, 3, ACC, s);
    emit_load_bool(ACC, truebool, s);
    emit_beqz(T1, labelTag, s);
    emit_load_bool(ACC, falsebool, s);
    emit_label_def(labelTag++, s);
}

void int_const_class::code(ostream& s) {
    //
    // Need to be sure we have an IntEntry *, not an arbitrary Symbol
    //
    emit_load_int(ACC, inttable.lookup_string(token->get_string()), s);
}

void string_const_class::code(ostream& s) {
    emit_load_string(ACC, stringtable.lookup_string(token->get_string()), s);
}

void bool_const_class::code(ostream& s) {
    emit_load_bool(ACC, BoolConst(val), s);
}

void new__class::code(ostream& s) {
    if (type_name == SELF_TYPE) {
        emit_load(ACC, 0, SELF, s);
        emit_sll(ACC, ACC, 3, s);
        emit_load_address(T2, CLASSOBJTAB, s);
        emit_add(ACC, ACC, T2, s);
        emit_load(ACC, 0, ACC, s);
        emit_jal_method(Object, COPY, s);

        emit_load(T2, 0, SELF, s);
        emit_sll(T2, T2, 3, s);
        emit_load_address(T1, CLASSOBJTAB, s);
        emit_add(T1, T1, T2, s);
        emit_load(T1, 1, T1, s);
        emit_jalr(T1, s);
    } else {
        generate_new_class(s, type_name);
    }
}

void isvoid_class::code(ostream& s) {
    e1->localEnv = localEnv;
    e1->classTable = classTable;
    e1->code(s);
    emit_move(T1, ACC, s);
    emit_load_bool(ACC, truebool, s);
    emit_beqz(T1, labelTag, s);
    emit_load_bool(ACC, falsebool, s);
    emit_label_def(labelTag, s);
    labelTag++;
}

void no_expr_class::code(ostream& s) { type = No_type; }

void object_class::code(ostream& s) {
    if (name == self) {
        emit_move(ACC, SELF, s);
    } else {
        VarInfo* info = localEnv->lookup(name);
        if (info->storePos == ATTRIBUTE) {
            emit_load(ACC, info->pos + DEFAULT_OBJFIELDS, SELF, s);
        } else if (info->storePos == STACK) {
            emit_load(ACC, info->pos, FP, s);
        }
    }
}
