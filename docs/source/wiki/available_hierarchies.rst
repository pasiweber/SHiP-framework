Available Hierarchies
----------------------
Once a similarity tree is built, it can be transformed into a **clustering hierarchy**. These hierarchies generalize well-known clustering paradigms such as $k$-means and $k$-median, enabling fine-grained control over how cluster centroids and merges are computed.

We support all possible $(k,z)$-hierarchies, allowing flexibility in choosing the most suitable hierarchy for a given dataset.

  - $z = 0$ → $k$-center (actually in theory: $z = ∞$, but in this implementation we use 0 for $∞$)
  - $z = 1$ → $k$-median
  - $z = 2$ → $k$-means
  - ...
