#pragma once

#include "object.h"

namespace aer
{

class Group : public Node
{
    using Children = std::vector< ref_ptr<Node> >;
    Children children;
public:
    void add( ref_ptr<Node> node )
    {
        children.push_back( node );
    }

protected:
    void _traverse( Visitor& visitor )
    {
        for( auto& child : children ) child->traverse( visitor );
    }
};

} // namespace aer