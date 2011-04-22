#include "plasm_impl.hpp"
namespace ecto
{
  plasm::plasm() : impl_(new impl) { }

  void plasm::connect(module::ptr from, const std::string& out_name, module::ptr to, const std::string& in_name)
  {
    if(from->outputs.count(out_name) == 0)
    {
      throw std::logic_error("The specified output does not exist: " + out_name);
    }
    if(to->inputs.count(in_name) == 0)
    {
      throw std::logic_error("The specified input does not exist: "+in_name);
    }

    //this throws on bad connection.
    from->connect(out_name, to, in_name);

    //only add to graph if it was actually connected.
    impl_->modules_.add_edge(from, out_name, to, in_name);
  }

  void plasm::mark_dirty(const module::ptr& m)
  {
    // Access the property accessor type for this graph
    impl_->modules_.mark_dirty(m);

  }
  void plasm::go(const module::ptr& m)
  {
    impl_->modules_.go(m);
  }

  void plasm::viz(std::ostream& out) const
  {
    boost::write_graphviz(out, impl_->modules_.graph_, ModuleGraph::label_writer(impl_->modules_));
  }

  std::string plasm::viz() const
  {
    std::stringstream ss;
    viz(ss);
    return ss.str();
  }

  plasm::vertex_map_t plasm::getVertices()
  {
    return impl_->modules_.getVertices();
  }
  plasm::edge_list_t plasm::getEdges()
  {
    return impl_->modules_.getEdges();
  }

}