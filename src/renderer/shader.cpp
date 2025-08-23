#include "renderer.h"

namespace renderer
{
    namespace shader
    {
        VSOut defaultVS(const VSIn &)
        {
            VSOut out;
            return out;
        }

        FSOut defaultFS(const FSIn &)
        {
            FSOut out;
            return out;
        }

    } // namespace shader
} // namespace renderer
