
//=============================================================================
/**
 *  @file    sequence_cs.cpp
 *
 *  Visitor generating code for Sequences in the client stubs file
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "sequence.h"

// ************************************************************
// Root visitor for client stub class
// ************************************************************

be_visitor_sequence_cs::be_visitor_sequence_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cs::~be_visitor_sequence_cs ()
{
}

int be_visitor_sequence_cs::visit_sequence (be_sequence *node)
{
  // First create a name for ourselves.
  if (node->create_name (this->ctx_->tdef ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_sequence_cs::")
                         ACE_TEXT ("visit_sequence - ")
                         ACE_TEXT ("failed creating name\n")),
                        -1);
    }

  // We don't check cli_stub_gen() here. If we are generated more
  // than once as an anonymous sequence, the name guard will cause
  // the C++ preprocessor to catch it. If we are generated more than
  // once as a typedef (caused by a comma separated list of
  // typedefs), our name will be changed by the call above and the
  // name guard will not catch it, but that's ok - we want to
  // be generated for each typedef.
  if (node->imported ())
    {
      return 0;
    }

  if (idl_global->dcps_sequence_type_defined (node->full_name ()))
    {
      return 0;
    }

  be_type *bt = dynamic_cast<be_type*> (node->base_type ());

  if (bt == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_sequence_cs::")
                         ACE_TEXT ("visit_sequence - ")
                         ACE_TEXT ("Bad element type\n")),
                        -1);
    }

  AST_Decl::NodeType nt = bt->node_type ();

  // If our base type is an anonymous sequence, we must create a name
  // and generate a class declaration for it as well.
  if (nt == AST_Decl::NT_sequence)
    {
      // Temporarily make the context's tdef node 0 so the nested call
      // to create_name will not get confused and give our anonymous
      // sequence element type the same name as we have.
      be_typedef *tmp = this->ctx_->tdef ();
      this->ctx_->tdef (nullptr);

      if (bt->accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_sequence_cs::")
                             ACE_TEXT ("visit_sequence - ")
                             ACE_TEXT ("codegen for anonymous ")
                             ACE_TEXT ("base type failed\n")),
                            -1);
        }

      // Restore the tdef value.
      this->ctx_->tdef (tmp);
    }

  if (be_global->alt_mapping () && node->unbounded ())
    {
      // We are just a typedef and don't need any stub source
      // code generation.
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2;

  TAO_INSERT_COMMENT (os);

  os->gen_ifdef_macro (node->flat_name ());

  // for unbounded sequences, we have a different set of constructors
  if (node->unbounded ())
    {
      *os << be_nl_2
          << node->name () << "::" << node->local_name () << " ("
          << be_idt << be_idt_nl
          << "::CORBA::ULong max)" << be_uidt_nl
          << ": " << be_idt;

      int status =
        node->gen_base_class_name (os,
                                   "",
                                   this->ctx_->scope ()->decl ());

      // Pass it to the base constructor.
      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_sequence_cs::")
                             ACE_TEXT ("visit_sequence - ")
                             ACE_TEXT ("codegen for base ")
                             ACE_TEXT ("sequence class failed\n")),
                           -1);
        }


      *os << " (max)" << be_uidt << be_uidt_nl
          << "{}";
    }

  /// If we are using std::vector, we can't implement this
  /// constructor.
  if (!be_global->alt_mapping () || !node->unbounded ())
    {
      // Constructor with the buffer
      *os << be_nl_2
          << node->name () << "::" << node->local_name () << " ("
          << be_idt << be_idt_nl;

      if (node->unbounded ())
        {
          // Unbounded seq takes this extra parameter.
          *os << "::CORBA::ULong max," << be_nl;
        }

      *os << "::CORBA::ULong length," << be_nl;

      // generate the base type for the buffer
      be_visitor_context ctx (*this->ctx_);
      be_visitor_sequence_buffer_type bt_visitor (&ctx);

      if (bt->accept (&bt_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_sequence_cs::")
                             ACE_TEXT ("visit_sequence - ")
                             ACE_TEXT ("base type visit failed\n")),
                            -1);
        }

      *os << " * buffer," << be_nl
          << "::CORBA::Boolean release)" << be_uidt
          << be_uidt_nl
          << "  : " << be_idt << be_idt;

      // Pass it to the base constructor.
      if (node->gen_base_class_name (os,
                                     "",
                                     this->ctx_->scope ()->decl ()) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_sequence_cs::")
                             ACE_TEXT ("visit_sequence - ")
                             ACE_TEXT ("codegen for base ")
                             ACE_TEXT ("sequence class\n")),
                            -1);
        }

      *os << be_nl << "(";

      if (node->unbounded ())
        {
          *os << "max, ";
        }

      *os << "length, buffer, release)" << be_uidt << be_uidt_nl
          << "{}";
  }

  if (be_global->alt_mapping () && node->unbounded ())
    {
      *os << be_nl_2
          << "::CORBA::ULong" << be_nl
          << node->name () << "::length () const" << be_nl
          << "{" << be_idt_nl
          << "return this->size ();" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "void" << be_nl
          << node->name () << "::length ( ::CORBA::ULong length)"
          << be_nl
          << "{" << be_idt_nl
          << "this->resize (length);" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "::CORBA::ULong" << be_nl
          << node->name () << "::maximum () const" << be_nl
          << "{" << be_idt_nl
          << "return this->capacity ();" << be_uidt_nl
          << "}";
    }

  if (be_global->any_support ()
      && !node->anonymous ()
      && (!node->is_local ()
          || be_global->gen_local_iface_anyops ()))
    {
      *os << be_nl_2
          << "void "
          << node->name () << "::_tao_any_destructor ("
          << be_idt << be_idt_nl
          << "void * _tao_void_pointer)" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << node->local_name () << " * _tao_tmp_pointer ="
          << be_idt_nl
          << "static_cast<" << node->local_name ()
          << " *> (_tao_void_pointer);" << be_uidt_nl
          << "delete _tao_tmp_pointer;" << be_uidt_nl
          << "}";
    }

  os->gen_endif ();

  node->cli_stub_gen (true);
  return 0;
}
