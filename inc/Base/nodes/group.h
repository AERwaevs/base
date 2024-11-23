#pragma once

#include "node.h"

namespace aer {

struct Group : public Node
{
    explicit Group( std::size_t num_children ) : children( num_children ) {};
            ~Group() = default;

    using Children = std::vector<ref_ptr<Node>>;
    Children children;

    void add( ref_ptr<Node> child ){ children.push_back( child ); };

    template< typename Self, typename Visitor > constexpr
    void traverse( this Self&& self, Visitor& visitor )
    {
        for( auto& child : self.children ){ child->accept( visitor ); }
    }
};

} // namespace aer