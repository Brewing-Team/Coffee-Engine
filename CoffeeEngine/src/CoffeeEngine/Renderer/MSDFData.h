#pragma once

#include "msdf-atlas-gen/GlyphGeometry.h"
#include <vector>

#undef INFINITE
#include <msdf-atlas-gen/msdf-atlas-gen.h>

namespace Coffee {
    
    struct MSDFData
    {
        std::vector<msdf_atlas::GlyphGeometry> Glyphs;
        msdf_atlas::FontGeometry FontGeometry;
    };

}