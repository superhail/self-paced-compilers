

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <symtab.h>
#include "semant.h"
#include "utilities.h"
#include <string>

extern int semant_debug;
extern char *curr_filename;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////

static Symbol
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class,
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;

//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    arg = idtable.add_string("arg");
    arg2 = idtable.add_string("arg2");
    Bool = idtable.add_string("Bool");
    concat = idtable.add_string("concat");
    cool_abort = idtable.add_string("abort");
    copy = idtable.add_string("copy");
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

ClassTable::ClassTable(Classes classes) : semant_errors(0), error_stream(cerr)
{
    this->class_dec_table = ClassInfoTable();
    class_dec_table.enterscope();
    /*install basic classes into class_dec_table */
    install_basic_classes();
    /*install other classes and initialize their attr_env and method_env as well */
    for (size_t i = classes->first(); classes->more(i); i = classes->next(i))
    {
        Class_ class_ = classes->nth(i);
        Symbol class_name = class_->getName();
        if (class_name == IO || class_name == Str ||
            class_name == Int || class_name == Bool || class_name == Object)
        {
            semant_error(class_) << "Redefinition of basic class " 
            << class_name->get_string() << "." << endl;
            class_->is_valid = false;
        }
        else if(class_name == SELF_TYPE) {
            semant_error(class_) << "SELF_TYPE cannot be redefined." << endl;
            class_->is_valid = false;
        }
        else if (class_dec_table.lookup(class_name) != NULL)
        { /*repetitive class definition*/
            semant_error(class_) << "repetetive class definition" << endl;
        }
        else
        { /*normal routine */
            class_dec_table.addid(class_name, class_);
        }
    }
    /*check if Main class exists */
    if(class_dec_table.probe(Main) == NULL) {
        semant_error() << "Class Main is not defined." << endl;
    } else {
        /*check main method */
        Class_ mainclass = class_dec_table.probe(Main);
        Features features = mainclass->getFeatures();
        bool hasMainMethod = false;
        for (size_t i = features->first(); features->more(i); i = features->next(i))
        {
            Feature feature = features->nth(i);
            Symbol feature_name = feature->getName();
            if (strcmp(feature->getFeatureType(), "METHOD") == 0) {
                if(((method_class*) feature)->getFormals()->len() == 0) {
                    hasMainMethod = true;
                }
            }
        }
        if(!hasMainMethod) {
            semant_error() << "cannot find main method";
        } else {
            /*build inheritance graph */
            for (size_t i = classes->first(); classes->more(i); i = classes->next(i))
            {
                Class_ class_ = classes->nth(i);
                if(class_->is_valid) {
                    class_->setInheritance(this, class_dec_table);
                }
            }
            /*register features */
            for (size_t i = classes->first(); classes->more(i); i = classes->next(i))
            {
                Class_ class_ = classes->nth(i);
                if(class_->is_valid) {
                    registerFeatures(class_);
                }
            }
            /*start type checking for each class*/
            for (size_t i = classes->first(); classes->more(i); i = classes->next(i))
            {
                Class_ class_ = classes->nth(i);
                if(class_->is_valid) {
                    checkClass(class_);
                }
            }    
        }
    }
    
}

/* register all features of a class */
void ClassTable::registerFeatures(Class_ class_)
{
    if(!class_->is_registered) {
        /*if parent class is not registered, register it first */
        if(class_->getName() != Object) {
            Class_ parent_class = class_dec_table.lookup(class_->getParent());
            if(parent_class != NULL && !parent_class->is_registered) {
                registerFeatures(parent_class);
            }
        }
        Features features = class_->getFeatures();
        for (size_t i = features->first(); features->more(i); i = features->next(i))
        {
            Feature feature = features->nth(i);
            Symbol feature_name = feature->getName();
            if (strcmp(feature->getFeatureType(), "METHOD") == 0) {
                if (class_->method_env.probe(feature_name) != NULL)
                {
                    semant_error(class_->get_filename(), feature) 
                    << "repititive method definition for " << feature_name 
                    << " in class " << class_->getName()->get_string() << endl;
                    continue;
                }
                Formals formals = ((method_class *)feature)->getFormals();
                bool formal_correct = true;
                for (size_t i = formals->first(); formals->more(i); i = formals->next(i))
                {
                    Formal formal = formals->nth(i);
                    Symbol formal_type = formal->getTypeDecl();
                    if (formal_type == SELF_TYPE)
                    {
                        semant_error(class_->get_filename(), feature) << "SELF_TYPE appear in formal type";
                        formal_correct = false;
                        continue;
                    }
                    if (!hasClass(formal_type))
                    {
                        semant_error(class_->get_filename(), feature) 
                        << "formal type " << class_->getName()->get_string()
                        << " does not exist" << endl;
                        formal_correct = false;
                        continue;
                    }
                }
                if (!formal_correct)
                {
                    continue;
                }
                /*method redefinition checking */
                if(class_->getName() != Object) {
                    Class_ parent_class = class_dec_table.lookup(class_->getParent());
                    method_class* ori_method = (method_class*) parent_class->method_env.lookup(feature_name);
                    if(ori_method != NULL) {
                        /*check formal and actual length */
                        Formals formals = ((method_class *)feature)->getFormals();
                        Formals ori_formals = ori_method->getFormals();
                        if (formals->len() != ori_formals->len())
                        {
                            semant_error(class_->get_filename(), feature)
                            << "In redefinition of method " << feature->getName()->get_string() 
                            << "parameter length " << formals->len() << " is different from "
                            << "original parameter length" << ori_formals->len() << endl;
                            continue;
                        }
                        /*check formal and actual match */
                        bool is_param_matched = true;
                        for (size_t i = 0; formals->more(i); i = formals->next(i))
                        {
                            Formal formal = formals->nth(i);
                            Formal ori_formal = ori_formals->nth(i);
                            Symbol formal_type = formal->getTypeDecl();
                            Symbol ori_formal_type = ori_formal->getTypeDecl();
                            if (ori_formal_type != formal_type)
                            {
                                is_param_matched = false;
                                semant_error(class_->get_filename(), feature)
                                << "In redefinition of method " << feature->getName()->get_string() 
                                << ", parameter type " << formal_type->get_string() 
                                << " different from original type " 
                                << ori_formal_type->get_string() << endl;
                                break;
                            }
                        }
                        if(!is_param_matched) {
                            continue;
                        }
                        /*check return type */
                        Symbol ori_ret_type = ori_method->getRetType();
                        if(ori_ret_type == SELF_TYPE) ori_ret_type = class_->getName();
                        Symbol ret_type = ((method_class *)feature)->getRetType();
                        if(ret_type == SELF_TYPE) ret_type = class_->getName();
                        if(ori_ret_type != ret_type) {
                            semant_error(class_->get_filename(), feature)
                            << "In redefinition of method " << feature->getName()->get_string() 
                            << " return type " << ((method_class *)feature)->getRetType()->get_string()
                            << " different from original type " << ori_method->getRetType()->get_string();
                            continue;
                        }
                    }
                }
                class_->method_env.addid(feature_name, (method_class *)feature);
            }
            else if (strcmp(feature->getFeatureType(), "ATTR") == 0)
            { /*attribute feature */
                if (class_->attr_env.lookup(feature_name) != NULL)
                {
                    semant_error(class_->get_filename(), feature) 
                    << "repititive attribute definition for " << feature_name 
                    << " in class " << class_->getName()->get_string() << endl;
                    continue;
                }
                Symbol attr_type = ((attr_class *)feature)->getTypeDecl();
                if (!hasClass(attr_type))
                {
                    semant_error(class_->get_filename(), feature) << "Attribute type " 
                    << attr_type->get_string() << " does not exist" << endl;
                    continue;
                }
                class_->attr_env.addid(feature_name,
                                ((attr_class *)feature)->getTypeDecl());
            }
        }
        class_->is_registered = true;
    }
}

void ClassTable::install_basic_classes()
{

    // The tree package uses these globals to annotate the classes built below.
    // curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");
    Class_ prim_slot_class = class_(prim_slot, prim_slot, nil_Features(), filename);
    class_dec_table.addid(prim_slot, prim_slot_class);


    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.

    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    //
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.

    Class_ Object_class =
        class_(Object,
               No_class,
               append_Features(
                   append_Features(
                       single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
                       single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
                   single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
               filename);

    class_dec_table.addid(Object, Object_class);
    Object_class->method_env = MethodInfoTable();
    Object_class->method_env.enterscope();
    Object_class->attr_env = AttrInfoTable();
    Object_class->attr_env.enterscope();
    Object_class->class_inh = ClassInfoTable();
    Object_class->class_inh.enterscope();
    Object_class->class_inh.addid(Object, Object_class);
    Object_class->is_visited = true;

    //
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    Class_ IO_class =
        class_(IO,
               Object,
               append_Features(
                   append_Features(
                       append_Features(
                           single_Features(method(out_string, single_Formals(formal(arg, Str)),
                                                  SELF_TYPE, no_expr())),
                           single_Features(method(out_int, single_Formals(formal(arg, Int)),
                                                  SELF_TYPE, no_expr()))),
                       single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
                   single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
               filename);

    class_dec_table.addid(IO, IO_class);
    Object_class->method_env.enterscope();
    IO_class->method_env = Object_class->method_env;
    Object_class->method_env.exitscope();
    Object_class->attr_env.enterscope();
    IO_class->attr_env = Object_class->attr_env;
    Object_class->attr_env.exitscope();
    Object_class->class_inh.enterscope();
    IO_class->class_inh = Object_class->class_inh;
    IO_class->class_inh.addid(IO, IO_class);
    Object_class->class_inh.exitscope();
    IO_class->is_visited = true;

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer.
    //
    Class_ Int_class =
        class_(Int,
               Object,
               single_Features(attr(val, prim_slot, no_expr())),
               filename);

    class_dec_table.addid(Int, Int_class);
    Object_class->method_env.enterscope();
    Int_class->method_env = Object_class->method_env;
    Object_class->method_env.exitscope();
    Object_class->attr_env.enterscope();
    Int_class->attr_env = Object_class->attr_env;
    Object_class->attr_env.exitscope();
    Object_class->class_inh.enterscope();
    Int_class->class_inh = Object_class->class_inh;
    Int_class->class_inh.addid(Int, Int_class);
    Object_class->class_inh.exitscope();
    Int_class->is_visited = true;

    //
    // Bool also has only the "val" slot.
    //
    Class_ Bool_class =
        class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())), filename);

    class_dec_table.addid(Bool, Bool_class);
    Object_class->method_env.enterscope();
    Bool_class->method_env = Object_class->method_env;
    Object_class->method_env.exitscope();
    Object_class->attr_env.enterscope();
    Bool_class->attr_env = Object_class->attr_env;
    Object_class->attr_env.exitscope();
    Object_class->class_inh.enterscope();
    Bool_class->class_inh = Object_class->class_inh;
    Bool_class->class_inh.addid(Bool, Bool_class);
    Object_class->class_inh.exitscope();
    Bool_class->is_visited = true;

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //
    Class_ Str_class =
        class_(Str,
               Object,
               append_Features(
                   append_Features(
                       append_Features(
                           append_Features(
                               single_Features(attr(val, Int, no_expr())),
                               single_Features(attr(str_field, prim_slot, no_expr()))),
                           single_Features(method(length, nil_Formals(), Int, no_expr()))),
                       single_Features(method(concat,
                                              single_Formals(formal(arg, Str)),
                                              Str,
                                              no_expr()))),
                   single_Features(method(substr,
                                          append_Formals(single_Formals(formal(arg, Int)),
                                                         single_Formals(formal(arg2, Int))),
                                          Str,
                                          no_expr()))),
               filename);

    class_dec_table.addid(Str, Str_class);
    Object_class->method_env.enterscope();
    Str_class->method_env = Object_class->method_env;
    Object_class->method_env.exitscope();
    Object_class->attr_env.enterscope();
    Str_class->attr_env = Object_class->attr_env;
    Object_class->attr_env.exitscope();
    Object_class->class_inh.enterscope();
    Str_class->class_inh = Object_class->class_inh;
    Str_class->class_inh.addid(Str, Str_class);
    Object_class->class_inh.exitscope();
    Str_class->is_visited = true;

    registerFeatures(Object_class);
    registerFeatures(IO_class);
    registerFeatures(Int_class);
    registerFeatures(Bool_class);
    registerFeatures(Str_class);
}

/* type check a class*/
void ClassTable::checkClass(Class_ class_)
{
    Features features = class_->getFeatures();
    for (size_t i = features->first(); features->more(i); i = features->next(i))
    {
        Feature feature = features->nth(i);
        Symbol feature_name = feature->getName();
        feature->check(this, class_);
    }
}

/**check if ltype is the ascent of rtype 
* both rtype and ltype must be valid types
*/
bool ClassTable::checkAscent(Symbol ltype, Symbol rtype)
{
    Class_ r_class = class_dec_table.lookup(rtype);
    Class_ found = r_class->class_inh.lookup(ltype);
    return found != NULL;
}

/*check if a class exists */
bool ClassTable::hasClass(Symbol tocheck)
{
    Class_ tocheck_class = class_dec_table.lookup(tocheck);
    return tocheck_class != NULL;
}

Symbol ClassTable::unionType(Symbol ltype, Symbol rtype)
{
    if (ltype == Object || rtype == Object)
    {
        return Object;
    }
    else
    {
        Class_ l_class = class_dec_table.lookup(ltype);
        Class_ r_class = class_dec_table.lookup(rtype);
        if (l_class->class_inh.lookup(rtype) != NULL)
        {
            return rtype;
        }
        else if (r_class->class_inh.lookup(ltype) != NULL)
        {
            return ltype;
        }
        while (l_class->class_inh.lookup(rtype) == NULL)
        {
            rtype = r_class->getParent();
            r_class = class_dec_table.lookup(rtype);
        }
        return rtype;
    }
}

void class__class::setInheritance(ClassTable *class_table,
                                  ClassInfoTable class_dec_table)
{
    /*exclude classes already set its inheritance*/
    if (!is_visited)
    {
        is_visited = true;
        if(parent == SELF_TYPE) {
            class_table->semant_error(this) << "Cant inherit from SELF_TYPE." << endl;
            this->is_valid = false;
        } else {
            Class_ parent_class = class_dec_table.lookup(parent);
            if(parent_class != NULL) {
                if (parent == Str ||
                    parent == Bool || parent == Int)
                {
                    class_table->semant_error(this) << "Cant inherit from class "
                    << parent->get_string() << endl;
                    this->is_valid = false;
                } else {
                    parent_class->setInheritance(class_table, class_dec_table);
                    if(parent_class->is_valid) {
                        parent_class->method_env.enterscope();
                        this->method_env = parent_class->method_env;
                        parent_class->method_env.exitscope();

                        parent_class->attr_env.enterscope();
                        this->attr_env = parent_class->attr_env;
                        parent_class->attr_env.exitscope();
                        
                        parent_class->class_inh.enterscope();
                        this->class_inh = parent_class->class_inh;
                        parent_class->class_inh.exitscope();
                        /*add class to its own inh */
                        class_inh.addid(name, this);
                    } else {
                        this->is_valid= false;
                    }
                }
            } else {
                class_table->semant_error(this) << "Parent class " 
                << parent->get_string() << "does not exist." << endl;
                this->is_valid = false;
            }
        }
    }
}

Symbol method_class::matchParam(Expressions actual,
                                ClassTable *class_table,
                                AttrInfoTable &env,
                                Class_ class_)
{
    /*check formal and actual length */
    if (formals->len() != actual->len())
    {
        class_table->semant_error(class_->get_filename(), this)
            << "expected " << formals->len() << " parameters but "
            << actual->len() << " were given" << endl;
        return Object;
    }
    /*check formal and actual match */
    bool is_param_matched = true;
    for (size_t i = 0; formals->more(i); i = formals->next(i))
    {
        Formal formal = formals->nth(i);
        Symbol formal_type = formal->getTypeDecl();
        Expression actual_expr = actual->nth(i);
        actual_expr->check(class_table, env, class_);
        Symbol actual_type = actual_expr->type;
        if(actual_type == SELF_TYPE) {
            actual_type = class_->getName();
        }
        if (!class_table->checkAscent(formal_type, actual_type))
        {
            is_param_matched = false;
            class_table->semant_error(class_->get_filename(), this) << "expected " 
            << formal_type->get_string() << " but were given " 
            << actual_type->get_string() << endl;
        }
    }
    if (is_param_matched)
    {
        return return_type;
    }
    else
    {
        return Object;
    }
}

/*method feature */
void method_class::check(ClassTable *class_table,
                         Class_ class_)
{
    class_->attr_env.enterscope();
    AttrInfoTable curr_env = class_->attr_env;
    /*register formals */
    for (size_t i = formals->first(); formals->more(i); i = formals->next(i))
    {
        Formal formal = formals->nth(i);
        Symbol formal_type = formal->getTypeDecl();
        if(formal->getName() == self) {
            class_table->semant_error(class_->get_filename(), formal) 
            << "Formal parameter cannot be named as 'self'." << endl;
        }
        else if(formal_type == SELF_TYPE) {
            class_table->semant_error(class_->get_filename(), formal) 
            << "Formal parameter" << formal->getName()->get_string()
            <<"cannot have type SELF_TYPE." << endl;
        }
        else if(curr_env.probe(formal->getName()) != NULL) {
            class_table->semant_error(class_->get_filename(), formal) 
            << "Already defined formal parameter " 
            << formal->getName()->get_string() << endl;
        }
        else if (!class_table->hasClass(formal_type))
        {
            class_table->semant_error(class_->get_filename(), formal) 
            << "class does not exist" << endl;
            curr_env.addid(formal->getName(), Object);
        }
        else
        {
            curr_env.addid(formal->getName(), formal->getTypeDecl());
        }
    }
    /*check return type exists */
    Symbol ret_type = return_type;
    if(ret_type != SELF_TYPE && !class_table->hasClass(ret_type)) {
        class_table->semant_error(class_->get_filename(), this)
        << "return type "<< return_type->get_string() 
        << " does not exist" << endl;
    } else {
        /*compare return type and expression type */
        expr->check(class_table, curr_env, class_);
        Symbol expr_type = expr->type;
        if(expr_type == SELF_TYPE) {
            if(expr_type != ret_type && !class_table->checkAscent(ret_type, class_->getName())) {
                class_table->semant_error(class_->get_filename(), this)
                << "return type "<< return_type->get_string() 
                << " does not match with expr type " 
                << expr->get_type()->get_string() << endl;
            }
        } else {
            if(ret_type == SELF_TYPE || !class_table->checkAscent(ret_type, expr_type)) {
                class_table->semant_error(class_->get_filename(), this)
                << "return type "<< return_type->get_string() 
                << " does not match with expr type " 
                << expr->get_type()->get_string() << endl;
            }
        }
    }  
    class_->attr_env.exitscope();
}

/*attribute feature */
void attr_class::check(ClassTable *class_table,
                       Class_ class_)
{
    init->check(class_table, class_->attr_env, class_);
    Symbol init_type = init->type;
    if(init_type == SELF_TYPE) {
        init_type = class_->getName();
    }
    if(init_type == No_type) {
        if(name == self) {
            class_table->semant_error(class_->get_filename(), this)
            << "'self' cannot be a name of an attribute." << endl;
        } else {
            class_->attr_env.addid(name, type_decl);
        }
    } else {
        if(type_decl == SELF_TYPE) {
            if(name == self) {
                class_table->semant_error(class_->get_filename(), this) 
                << "'self' cannot be a name of an attribute." << endl;
            } else {
                if (!class_table->checkAscent(class_->getName(), init_type))
                {
                    class_table->semant_error(class_->get_filename(), this)
                    << name->get_string() << "'s declared type " 
                    << class_->getName()->get_string()
                    << " and expression type " << init_type
                    << " does not match" << endl;
                } else {
                    class_->attr_env.addid(name, init_type);
                }
            }       
        } else {
            if(name == self) {
                class_table->semant_error(class_->get_filename(), this) 
                << "'self' cannot be a name of an attribute." << endl;
            } else {
                if (!class_table->checkAscent(type_decl, init_type))
                {
                    class_table->semant_error(class_->get_filename(), this)
                        << name->get_string() << "'s declared type " << type_decl
                        << " and expression type " << init_type
                        << " does not match" << endl;
                } else {
                    class_->attr_env.addid(name, init_type);
                }
            }
        } 
    }
}

/*branch */
void branch_class::check(ClassTable *class_table,
                         AttrInfoTable &env, Class_ class_)
{
    /*check if name has been defined already */
    // if (env.probe(name) != NULL)
    // {
    //     class_table->semant_error(class_->get_filename(), this)
    //         << "name " << name->get_string()
    //         << " has been defined before" << endl;
    //     this->type = Object;
    //     return;
    // }
    /*type_decl exists or is SELF_TYPE */
    if (!class_table->hasClass(type_decl))
    {
        class_table->semant_error(class_->get_filename(), this)
            << "branch type decl class " << type_decl->get_string()
            << " does not exist" << endl;
        this->type = Object;
        return;
    }
    else if (type_decl == SELF_TYPE)
    {
        class_table->semant_error(class_->get_filename(), this)
            << "SELF_TYPE should not appear in case" << endl;
        this->type = Object;
        return;
    }
    env.addid(name, type_decl);
    /*check if expr and type decl's type match */
    expr->check(class_table, env, class_);
    Symbol expr_type = expr->type;
    if(expr_type == SELF_TYPE) {
        expr_type = class_->getName();
    }
    if (!class_table->checkAscent(type_decl, expr_type))
    {
        class_table->semant_error(class_->get_filename(), this)
            << "case " << name->get_string() << "expr's type "
            << expr_type->get_string() << " and its declaration "
            << type_decl->get_string() << " does not match" << endl;
        this->type = Object;
        return;
    }
    this->type = expr->type;
}

/*expressions */
void assign_class::check(ClassTable *class_table,
                         AttrInfoTable &env, Class_ class_)
{
    /*check if self is assigned */
    if (name == self)
    {
        class_table->semant_error(class_->get_filename(), this)
            << "self should not appear in assignment" << endl;
        this->type = Object;
        return;
    }
    /*check if name exists */
    Symbol type_decl = env.lookup(name);
    if (type_decl == NULL)
    {
        class_table->semant_error(class_->get_filename(), this) << "name " 
            << name->get_string() << " is not defined" << endl;
        this->type = Object;
        return;
    }
    /*check declared type and assignment type match */
    expr->check(class_table, env, class_);
    Symbol expr_type = expr->type;
    if(expr_type == SELF_TYPE) {
        expr_type = class_->getName();
    }
    if (!class_table->checkAscent(type_decl, expr_type))
    {
        class_table->semant_error(class_->get_filename(), this) << "declared type " 
            << type_decl << " and assignment expression type" << expr_type 
            << " does not match" << endl;
        this->type = Object;
        return;
    }
    this->type = expr->type;
}

void static_dispatch_class::check(ClassTable *class_table,
                                  AttrInfoTable &env, Class_ class_)
{
    expr->check(class_table, env, class_);
    Symbol expr_type = expr->type;
    if(expr_type == SELF_TYPE) {
        expr_type = class_->getName();
    }
    Class_ expr_class = class_table->class_dec_table.lookup(expr_type);
    Class_ parent_class = expr_class->class_inh.lookup(type_name);
    if (parent_class == NULL)
    {
        class_table->semant_error(class_->get_filename(), this) << "class " 
        << expr_type->get_string() << " has no parent named " 
        << type_name->get_string() << endl;
        this->type = Object;
        return;
    }

    method_class *method = (method_class *)parent_class->method_env.lookup(name);
    if (method == NULL)
    {
        class_table->semant_error(class_->get_filename(), this)
        << "Dispatch to undefined method " 
        << name->get_string() << endl;
    }
    this->type = method->matchParam(actual, class_table, env, class_);
    if(this->type == SELF_TYPE) {
        if(expr_class->getName() == class_->getName()) {
            this->type = SELF_TYPE;
        } else {
            this->type = expr_class->getName();
        }
    }
}

void dispatch_class::check(ClassTable *class_table,
                           AttrInfoTable &env, Class_ class_)
{
    expr->check(class_table, env, class_);
    Symbol expr_type = expr->type;
    if(expr_type == SELF_TYPE) {
        expr_type = class_->getName();
    }
    Class_ expr_class = class_table->class_dec_table.lookup(expr_type);
    /*check if method exists */
    method_class *method = (method_class *)expr_class->method_env.lookup(name);
    if (method == NULL)
    {
        class_table->semant_error(class_->get_filename(), this) << "method " 
        << name->get_string() << " is not defined in " 
        << "class " << expr_class->getName()->get_string() << endl;
        this->type = Object;
        return;
    }
    this->type = method->matchParam(actual, class_table, env, class_);
    if(this->type == SELF_TYPE) {
        if(expr_class->getName() == class_->getName()) {
            this->type = SELF_TYPE;
        } else {
            this->type = expr_class->getName();
        }
    }
}

void cond_class::check(ClassTable *class_table,
                       AttrInfoTable &env, Class_ class_)
{
    pred->check(class_table, env, class_);
    Symbol pred_type = pred->type;
    if(pred_type == SELF_TYPE) {
        pred_type = class_->getName();
    }
    if (Bool != pred_type)
    {
        class_table->semant_error(class_->get_filename(), this) 
        << "pred type should be bool other than " 
        << pred_type->get_string() << endl;
    }
    then_exp->check(class_table, env, class_);
    else_exp->check(class_table, env, class_);
    Symbol then_exp_type = then_exp->type, else_exp_type = else_exp->type;
    if(then_exp_type == SELF_TYPE) {
        then_exp_type = class_->getName();
    }
    if(else_exp_type == SELF_TYPE) {
        else_exp_type = class_->getName();
    }
    this->type = class_table->unionType(then_exp_type, else_exp_type);
}

void loop_class::check(ClassTable *class_table,
                       AttrInfoTable &env, Class_ class_)
{
    pred->check(class_table, env, class_);
    Symbol pred_type = pred->type;
    if(pred_type == SELF_TYPE) {
        pred_type = class_->getName();
    }
    if (Bool != pred_type)
    {
        class_table->semant_error(class_->get_filename(), this) 
        << "pred type should be bool other than " 
        << pred_type->get_string() << endl;
    }
    body->check(class_table, env, class_);
    this->type = Object;
}

void typcase_class::check(ClassTable *class_table,
                          AttrInfoTable &env, Class_ class_)
{
    expr->check(class_table, env, class_);
    env.enterscope();
    Symbol curr_type = No_type;
    AttrInfoTable* case_table = new AttrInfoTable();
    case_table->enterscope();
    for (size_t i = cases->first(); cases->more(i); i = cases->next(i))
    {
        Case case_ = cases->nth(i);
        case_->check(class_table, env, class_);
        if (curr_type == No_type)
        {
            curr_type = case_->type;
            if(case_table->probe(case_->getTypeDecl()) != NULL) {
                class_table->semant_error(class_->get_filename(), this)
                << "Duplicate Branch " << case_->getTypeDecl() 
                << " in case statement." << endl;
            } else {
                case_table->addid(case_->getTypeDecl(), case_->getTypeDecl());
            }
        }
        else if(case_->type == SELF_TYPE) {
            if(case_table->probe(case_->getTypeDecl()) != NULL) {
                class_table->semant_error(class_->get_filename(), this)
                << "Duplicate Branch " << case_->getTypeDecl() 
                << " in case statement." << endl;
            } else {
                case_table->addid(case_->getTypeDecl(), case_->getTypeDecl());
            }
            curr_type = class_table->unionType(class_->getName(), curr_type);
        } else {
            if(case_table->probe(case_->getTypeDecl()) != NULL) {
                class_table->semant_error(class_->get_filename(), this)
                << "Duplicate Branch " << case_->getTypeDecl() 
                << " in case statement."<< endl;
            } else {
                case_table->addid(case_->getTypeDecl(), case_->getTypeDecl());
            }
            curr_type = class_table->unionType(case_->type, curr_type);
        }
    }
    this->type = curr_type;
    env.exitscope();
}

void block_class::check(ClassTable *class_table,
                        AttrInfoTable &env, Class_ class_)
{
    Expression expr;
    for (size_t i = body->first(); body->more(i); i = body->next(i))
    {
        expr = body->nth(i);
        expr->check(class_table, env, class_);
    }
    this->type = expr->type;
}

void let_class::check(ClassTable *class_table,
                      AttrInfoTable &env, Class_ class_)
{
    env.enterscope();
    if(identifier == self) {
        class_table->semant_error(class_->get_filename(), this)
        << "'self' cannot be bound in a 'let' expression." << endl;
        this->type = Object;
    }
    else if (type_decl == SELF_TYPE) {
        init->check(class_table, env, class_);
        Symbol init_type = init->type;
        if(init_type == No_type) {
            init_type = type_decl;
        }
        if(init_type == SELF_TYPE) {
            init_type = class_->getName();
        }
        if(class_table->checkAscent(class_->getName(), init_type)) {
            env.addid(identifier, SELF_TYPE);
            body->check(class_table, env, class_);
            this->type = body->type;
        } else {
            class_table->semant_error(class_->get_filename(), this) 
                << "declared type " << class_->getName()->get_string() 
                << " does not match with " << init_type->get_string() << endl;
            body->check(class_table, env, class_);
            this->type = body->type;
        }
    }
    else if (!class_table->hasClass(type_decl))
    {
        class_table->semant_error(class_->get_filename(), this) << "class " 
        << type_decl->get_string() << " has not been defined" << endl;
        this->type = Object;
    } 
    else
    {
        init->check(class_table, env, class_);
        Symbol init_type = init->type;
        if(init_type == No_type) {
            init_type = type_decl;
        }
        if(init_type == SELF_TYPE) {
            init_type = class_->getName();
        }
        if(class_table->checkAscent(type_decl, init_type)) {
            env.addid(identifier, type_decl);
            body->check(class_table, env, class_);
            this->type = body->type;
        } else {
            class_table->semant_error(class_->get_filename(), this) 
                << "declared type " << class_->getName()->get_string() 
                << " does not match with " << init_type->get_string() << endl;
            body->check(class_table, env, class_);
            this->type = body->type;
        }
    }

    env.exitscope();
}

void plus_class::check(ClassTable *class_table,
                       AttrInfoTable &env, Class_ class_)
{
    e1->check(class_table, env, class_);
    if (e1->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "plus expression has left type " << e1->type->get_string() 
        << " instead of Int" << endl;
        return;
    }
    e2->check(class_table, env, class_);
    if (e2->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "plus expression has right type " << e2->type->get_string() 
        << " instead of Int" << endl;
        return;
    }
    this->type = Int;
}

void sub_class::check(ClassTable *class_table,
                      AttrInfoTable &env, Class_ class_)
{
    e1->check(class_table, env, class_);
    if (e1->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "sub expression has left type " << e1->type->get_string() 
        << " instead of Int" << endl;
        return;
    }
    e2->check(class_table, env, class_);
    if (e2->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "plus expression has right type " << e2->type->get_string() 
        << " instead of Int" << endl;
        return;
    }
    this->type = Int;
}

void mul_class::check(ClassTable *class_table,
                      AttrInfoTable &env, Class_ class_)
{
    e1->check(class_table, env, class_);
    if (e1->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "mul expression has left type " << e1->type->get_string() 
        << " instead of Int" << endl;
        return;
    }
    e2->check(class_table, env, class_);
    if (e2->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "mul expression has right type " << e2->type->get_string() 
        << " instead of Int" << endl;
        return;
    }
    this->type = Int;
}

void divide_class::check(ClassTable *class_table,
                         AttrInfoTable &env, Class_ class_)
{
    e1->check(class_table, env, class_);
    if (e1->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "divide expression has left type "
        << e1->type->get_string() << " instead of Int" << endl;
        return;
    }
    e2->check(class_table, env, class_);
    if (e2->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "divide expression has right type "
        << e2->type->get_string() << " instead of Int" << endl;
        return;
    }
    this->type = Int;
}

void neg_class::check(ClassTable *class_table,
                      AttrInfoTable &env, Class_ class_)
{
    e1->check(class_table, env, class_);
    if (e1->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "neg expression has type " << e1->type->get_string() 
        << " instead of Int" << endl;
        return;
    }
    this->type = Int;
}

void lt_class::check(ClassTable *class_table,
                     AttrInfoTable &env, Class_ class_)
{
    e1->check(class_table, env, class_);
    if (e1->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "lt expression has left type " << e1->type->get_string()
        << " instead of Int" << endl;
        return;
    }
    e2->check(class_table, env, class_);
    if (e2->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "lt expression has right type " << e2->type->get_string() 
        << " instead of Int" << endl;
        return;
    }
    this->type = Bool;
}

void eq_class::check(ClassTable *class_table,
                     AttrInfoTable &env, Class_ class_)
{
    e1->check(class_table, env, class_);
    e2->check(class_table, env, class_);
    if (e1->type == Int || e1->type == Bool || e1->type == Str
        || e2->type == Int || e2->type == Bool || e2->type ==  Str)
    {
        if(e2->type != e1->type) {
            class_table->semant_error(class_->get_filename(), this) 
            << "eq expression between " << e1->type->get_string()
            << " and " << e2->type->get_string() << " invalid" << endl;
        }
    } 
    
    this->type = Bool;
}

void leq_class::check(ClassTable *class_table,
                      AttrInfoTable &env, Class_ class_)
{
    e1->check(class_table, env, class_);
    if (e1->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "leq expression has left type " << e1->type->get_string() 
        << " instead of Int" << endl;
        return;
    }
    e2->check(class_table, env, class_);
    if (e2->type != Int)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "leq expression has right type " << e2->type->get_string() 
        << " instead of Int" << endl;
        return;
    }
    this->type = Bool;
}

void comp_class::check(ClassTable *class_table,
                       AttrInfoTable &env, Class_ class_)
{
    e1->check(class_table, env, class_);
    if (e1->type != Bool)
    {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) 
        << "comp expression has type " << e1->type->get_string() 
        << " instead of Bool" << endl;
        return;
    }
    this->type = Bool;
}

void int_const_class::check(ClassTable *class_table,
                            AttrInfoTable &env, Class_ class_)
{
    this->type = Int;
}

void bool_const_class::check(ClassTable *class_table,
                             AttrInfoTable &env, Class_ class_)
{
    this->type = Bool;
}

void string_const_class::check(ClassTable *class_table,
                               AttrInfoTable &env, Class_ class_)
{
    this->type = Str;
}

void new__class::check(ClassTable *class_table,
                       AttrInfoTable &env, Class_ class_)
{
    if (type_name == SELF_TYPE)
    {
        this->type = SELF_TYPE;
    } else if (!class_table->hasClass(type_name)) {
        this->type = Object;
        class_table->semant_error(class_->get_filename(), this) << "class " 
        << type_name->get_string() << " has not been defined" << endl;
    } else {
        this->type = type_name;
    }
}

void isvoid_class::check(ClassTable *class_table,
                         AttrInfoTable &env, Class_ class_)
{
    e1->check(class_table, env, class_);
    this->type = Bool;
}

void no_expr_class::check(ClassTable *class_table,
                          AttrInfoTable &env, Class_ class_)
{
    this->type = No_type;
}

void object_class::check(ClassTable *class_table,
                         AttrInfoTable &env, Class_ class_)
{
    if (name == self)
    {
        this->type = SELF_TYPE;
    }
    else
    {
        Symbol type = env.lookup(name);
        if (type == NULL)
        {
            class_table->semant_error(class_->get_filename(), this)
            << name->get_string() << " is not defined" << endl;
            this->type = Object;
        }
        else
        {
            this->type = type;
        }
    }
}

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
//       print a line number and filename
//
///////////////////////////////////////////////////////////////////

ostream &ClassTable::semant_error(Class_ c)
{
    return semant_error(c->get_filename(), c);
}

ostream &ClassTable::semant_error(Symbol filename, tree_node *t)
{
    error_stream << filename << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream &ClassTable::semant_error()
{
    semant_errors++;
    return error_stream;
}

/*   This is the entry point to the semantic checker.

     Your checker should do the following two things:

     1) Check that the program is semantically correct
     2) Decorate the abstract syntax tree with type information
     by setting the `type' field in each Expression node.
     (see `tree.h')

     You are free to first do 1), make sure you catch all semantic
     errors. Part 2) can be done in a second stage, when you want
     to build mycoolc.
     */
void program_class::semant()
{
    initialize_constants();

    /* ClassTable constructor may do some semantic analysis */
    ClassTable *classtable = new ClassTable(classes);

    /* some semantic analysis code may go here */

    if (classtable->errors())
    {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}
