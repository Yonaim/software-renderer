
### Math Conventions

This project uses row-vector math with row-major matrix storage, and a right-handed coordinate system:

* Vector type: Row vectors (1×n)
* Transform application: Right multiplication — transformations are applied left-to-right in code

  $$
  v' = v \cdot M_1 \cdot M_2
  $$

  Here, `M1` is applied first, then `M2`.
* Matrix storage: Row-major order in memory
* Coordinate system: Right-handed — +X right, +Y up, +Z forward


