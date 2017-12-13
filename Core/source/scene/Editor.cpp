#include "geometry/GL4_Entity.hpp"
#include "geometry/GL4_PolyFunc.hpp"
#include "geometry/polyform/Polyform_Box.hpp"
#include "geometry/polyform/Polyform_Rubiks.hpp"
#include "scene/Editor.hpp"

void Editor::init(GL4_PolyFunc* polyFunc, Polyform_Rubiks* polyRubiks){
    Polyform_Rubiks_Meta polyRubiksMeta;
    polyRubiks->exportMeta(&polyRubiksMeta);

    Editor::xBounds[0] = polyRubiksMeta.xCount / 2;
    Editor::xBounds[1] = polyRubiksMeta.xCount - Editor::xBounds[0];
    Editor::xBounds[0] *= -1;
    Editor::yBounds[0] = polyRubiksMeta.yCount / 2;
    Editor::yBounds[1] = polyRubiksMeta.yCount - Editor::yBounds[0];
    Editor::yBounds[0] *= -1;
    Editor::zBounds[0] = polyRubiksMeta.zCount / 2;
    Editor::zBounds[1] = polyRubiksMeta.zCount - Editor::zBounds[0];
    Editor::zBounds[0] *= -1;

    vertexFeed vFeed;
    polyFunc->dump(&vFeed);
    if(vFeed.pos.empty()){
        std::cerr << "PolyFunc cannot be empty" << std::endl;
        return;
    }
    Editor::vFeed = vFeed;

    Editor::cursor3D->init();
    Editor::cursor3D->feedPos(vFeed.pos.data(), vFeed.pos.size());
    Editor::cursor3D->relMatrix = polyFunc->relMatrix;

    return;
}

void Editor::create(int x, int y, int z){
    if(x < Editor::xBounds[0]) x = Editor::xBounds[0];
    else if(x > Editor::xBounds[1]) x = Editor::xBounds[1];
    if(y < Editor::yBounds[0]) y = Editor::yBounds[0];
    else if(y > Editor::yBounds[1]) y = Editor::yBounds[1];
    if(z < Editor::zBounds[0]) z = Editor::zBounds[0];
    else if(z > Editor::zBounds[1]) z = Editor::zBounds[1];

    Polyform_Rubiks_Meta polyRubiksMeta;
    polyRubiks->exportMeta(&polyRubiksMeta);

    unsigned int absVertex = (x - xBounds[0]) + 
                          ((y - yBounds[0]) * polyRubiksMeta.xCount) + 
                          ((z - zBounds[0]) * polyRubiksMeta.xCount * polyRubiksMeta.yCount);

    std::array<float, 3> cursorPos = {
        Editor::vFeed.pos[absVertex * Editor::vFeed.perVertex],
        Editor::vFeed.pos[absVertex * Editor::vFeed.perVertex + 1],
        Editor::vFeed.pos[absVertex * Editor::vFeed.perVertex + 2]
    };

    Editor::cursor3D->feedPos(cursorPos.data(), 3);
}