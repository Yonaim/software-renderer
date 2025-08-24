
## Dependency

This project has minimal dependencies for implementing software rasterizer.

- Management of window: SDL2 (cross-platform)
- Image format 'PNG': ??

### 메모
현재 SDL2는 시스템에 설치된 것을 찾음. (개발용)
추후 CMake의 FetchContent 이용 예정


## Math Conventions

This project uses row-vector math with row-major matrix storage, and a right-handed coordinate system:

* Vector type: Row vectors (1×n)
* Transform application: Right multiplication — transformations are applied left-to-right in code

  $$
  v' = v \cdot M_1 \cdot M_2
  $$

  Here, `M1` is applied first, then `M2`.
* Matrix storage: Row-major order in memory
* Coordinate system: Right-handed — +X right, +Y up, +Z forward


