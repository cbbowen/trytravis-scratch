
# Stable_out_adjacency_list

Declared in `<graph/Stable_adjacency_list.hpp>`:
```c++
class Stable_out_adjacency_list;
```

Satisfies the [`Out_edge_graph`](Out_edge_graph.md) concept using a classical adjacency list data structure.

## Member functions

In addition to the members required by the [`Out_edge_graph`](Out_edge_graph.md) concept, `Stable_out_adjacency_list` provides functions to facilitate mutation.

| Member functions | | |
|------------------|-|-|
| `insert_vert()` | `Vert` | constructs a new vertex |
| `insert_edge(Vert s, Vert t)` | `Edge` | constructs a new edge with tail `s` and head `t` |
