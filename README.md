## Commands

`make` - compile
`make run` - compile and run
`make clean` - removed compiled

## Example

```cpp
#include "tree.hpp"

int main() {
    auto tree = TREE(float, 1.105);
    tree->add(2.01)->parent()->add(3.01);

    TREE_FOREACH(tree, node, i, {
        cout << node->value() << " at " << i << endl;
    });
    /* Output:
        2.01 at 0
        3.01 at 1
    */

    return 0;
}

```
