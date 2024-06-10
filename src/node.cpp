#include "inc/node.hpp"

NodeBase::NodeBase()
: supnode_{nullptr}
, previous_sibnode_{nullptr}
, next_sibnode_{nullptr}
, first_subnode_{nullptr}
, last_subnode_{nullptr}
{
}

void NodeBase::AppendSubnode(NodeBase *node)
{
  if (NodeBase *supnode{node->supnode_}; supnode)
  {
    //...
  }


}

