#ifndef GEOMETRY_H
    #include "Geometry.hpp"
    #define GEOMETRY_H
#endif

#ifndef ERROR_CODE_H
    #include "scene/ErrorCode.hpp"
#endif

#ifndef GL4_VERTEX_FACTORY_H
    #include "graphics/factory/GL4_Vertex_Factory.hpp"
#endif

#ifndef GL4_MESH_H
    #include "graphics/geometry/GL4_Mesh.hpp"
#endif

#ifndef POLY_CLONE_H
    class GL4_PolyAngles {
    public:
        void create();
        std::vector<float> vertices;
    };

#define POLY_CLONE_H
#endif