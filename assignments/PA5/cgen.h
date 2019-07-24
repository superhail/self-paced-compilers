#include "cool-tree.h"
#include "emit.h"
#include "list"
#include "map"
#include "symtab.h"
#include <assert.h>
#include <stdio.h>

enum Basicness { Basic, NotBasic };
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable* CgenClassTableP;

class CgenNode;
typedef CgenNode* CgenNodeP;

class CgenClassTable : public SymbolTable<Symbol, CgenNode> {
  private:
    List<CgenNode>* nds;
    ostream& str;

    // The following methods emit code for
    // constants and global declarations.

    void code_global_data();
    void code_global_text();
    void code_bools(int);
    void code_select_gc();
    void code_constants();
    void code_class_nametab(void);
    void code_class_objtab(void);
    void code_disptables(void);
    void code_prot_objs(void);
    void code_init(void);
    void code_classes(void);

    // The following creates an inheritance graph from
    // a list of classes.  The graph is implemented as
    // a tree of `CgenNode', and class names are placed
    // in the base class symbol table.

    void install_basic_classes(void);
    void install_class(CgenNodeP nd);
    void install_classes(Classes cs);
    void build_inheritance_tree(void);
    void set_relations(CgenNodeP nd);
    void set_class_infos(void);

  public:
    std::list<Symbol> tagList;
    int stringclasstag;
    int intclasstag;
    int boolclasstag;

  public:
    CgenClassTable(Classes, ostream& str);
    void code();
    CgenNodeP root();
    int get_classtag(Symbol);
};

class CgenNode : public class__class {
  private:
    CgenNodeP parentnd;       // Parent of class
    List<CgenNode>* children; // Children of class
    Basicness basic_status;   // `Basic' if class is basic
                              // `NotBasic' otherwise

  public:
    int classtag;
    std::map<Symbol, method_class*>* methodTable; // methodname=>methodclass
    std::list<attr_class*>* attrTable;
    LocalEnv* localEnv;

  public:
    CgenNode(Class_ c, Basicness bstatus, CgenClassTableP class_table);

    void add_child(CgenNodeP child);
    List<CgenNode>* get_children() { return children; }
    void set_parentnd(CgenNodeP p);
    CgenNodeP get_parentnd() { return parentnd; }
    int basic() { return (basic_status == Basic); }
    int set_info(int classtag, std::list<Symbol>& tagList, CgenClassTableP);
    int get_attrtag(Symbol attrName);
    void env_init(void);
    void code_methods(ostream& str, CgenClassTableP classTable);
    void get_method_disp_table(method_class* methods[]);
};

class BoolConst {
  private:
    int val;

  public:
    BoolConst(int);
    void code_def(ostream&, int boolclasstag);
    void code_ref(ostream&) const;
};
