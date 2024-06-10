#ifndef NODE_HPP
#define NODE_HPP

class NodeBase
{
protected:
  NodeBase();

  void AppendSubnode(NodeBase *node);

private:
  NodeBase *supnode_;
  NodeBase *previous_sibnode_;
  NodeBase *next_sibnode_;
  NodeBase *first_subnode_;
  NodeBase *last_subnode_;
};

template<typename T>
class Node : public NodeBase
{
public:
  void AppendSubnode(T *node)
  {
    //NodeBase::AppendSubnode(static_cast<>);
  }
};

#endif //NODE_HPP
